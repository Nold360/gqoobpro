/*
 * Copyright (C) 2009 Joni Valtanen <jvaltane@kapsi.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include <qoob.h>

#include "qoob-flasher-util.h"

void 
qoob_flasher_util_parse_options (qoob_flasher_t *flasher, 
                                 int *argc, 
                                 char ***argv)
{
  while (1) {
    static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},
      {"verbose",no_argument, 0, 'v'},
      {"slot-list",no_argument, 0, 's'},
      {"elf", no_argument, 0, 'l'},
      {"dol", no_argument, 0, 'd'},
      {"qoob", no_argument, 0, 'q'},
      {"write",required_argument, 0, 'w'},
      {"read", required_argument, 0, 'r'},
      {"force-erase", required_argument, 0, 'f'},
      {"erase", required_argument, 0, 'e'},
      {0, 0, 0, 0}
    };

    int index = 0;
     
    char c = getopt_long (*argc, *argv, "hvsldqw:r:f:e:", long_options, &index);
     
    if (c == -1)
      break;

    switch (c) {
    case 0:
      break;
    case 'h':
      flasher->help = QOOB_TRUE;
      break;
    case 'v':
      flasher->verbose++;
      break;
    case 's':
      flasher->list = QOOB_TRUE;
      break;
    case 'l':
      qoob_sync_file_format_set (&flasher->qoob, QOOB_BINARY_TYPE_ELF);
      break;
    case 'd':
      qoob_sync_file_format_set (&flasher->qoob, QOOB_BINARY_TYPE_DOL);
      break;
    case 'q':
      qoob_sync_file_format_set (&flasher->qoob, QOOB_BINARY_TYPE_GCB);
      break;
    case 'w':

      flasher->command = FLASHER_COMMAND_WRITE;
      flasher->slot_num = (int)strtol (optarg, NULL, 10);
      break;
    case 'r':
      flasher->command = FLASHER_COMMAND_READ;
      flasher->slot_num = (int)strtol (optarg, NULL, 10);
      break;
    case 'e':
      flasher->command = FLASHER_COMMAND_ERASE;
      flasher->slot_num = (int)strtol (optarg, NULL, 10);
      break;
    case 'f':
      flasher->command = FLASHER_COMMAND_FORCE_ERASE;
      flasher->slot_num = (int)strtol (optarg, NULL, 10);
      break;
    case '?':
      break;
    default:
      return;
      break;
    }
  }

  if (optind < (*argc)) {
    flasher->file =  strdup ((char *)(*argv)[optind]);
  }
}

int 
qoob_flasher_util_test_options (qoob_flasher_t *flasher)
{
  if (flasher->help == QOOB_TRUE) {
    qoob_sync_usb_clear (&flasher->qoob);
    qoop_flasher_util_print_help_and_exit (0);
  }

  if (flasher->command == FLASHER_COMMAND_READ ||
      flasher->command == FLASHER_COMMAND_WRITE) {
    qoob_error_t ret;
    binary_type_t type;

    if (flasher->slot_num >= QOOB_PRO_SLOTS || 
        flasher->slot_num < 0 || 
        flasher->file == NULL) {
      return 1;
    }

    ret = qoob_sync_file_format_get (&(flasher->qoob), &type);    
    if ((flasher->command == FLASHER_COMMAND_WRITE) && 
        (ret != QOOB_ERROR_OK)) {
      return 1;
    }
    if ((flasher->command == FLASHER_COMMAND_WRITE) &&
        (type == QOOB_BINARY_TYPE_VOID)) {
      return 1;
    }
  }

  if (flasher->command == FLASHER_COMMAND_ERASE ||
      flasher->command == FLASHER_COMMAND_FORCE_ERASE) {
    if (flasher->slot_num >= QOOB_PRO_SLOTS || 
        flasher->slot_num < 0) {
      return 1;
    }
  }
 
  return 0;
}

void
qoop_flasher_util_print_help (void) 
{
  printf ("Usage: qoob-flasher [OPTION]... [FILE]\n");
  printf ("Util to work with Qoop Pro Gamecube mod-chip.\n\n");

  printf ("  -h, --help               display this help and exits\n");
  printf ("  -v, --verbose            gives more information what happens\n");
  printf ("  -s, --slot-list          prints slot list after write or erase\n");
  printf ("  -w, --write=SLOT         writes given file to flash. Use with -l, -d or -q\n");
  printf ("  -r, --read=SLOT          reads given slot as gcb to given file\n");
  printf ("  -e, --erase=SLOT         erase application in flash\n");
  printf ("  -f, --force-erase=SLOT   erase one slot\n");
  printf ("  -l, --elf                Set ELF file format to write\n");
  printf ("  -d, --dol                Set DOL file format to write\n");
  printf ("  -q, --qoob               Set GCB or Config file format to write.\n");
  printf ("\n");


  printf ("Examples:\n\n");

  printf (" Show what is flashed.\n");
  printf ("  qoob-flasher\n\n");

  printf (" Read qoob-bios from flash\n");
  printf ("  qoob-flasher -r0 /tmp/qoob-bios.gcb\n\n");

  printf (" Erase qoob-bios from flash\n");
  printf ("  qoob-flasher -e0\n\n");

  printf (" Write qoob-bios to flash\n");
  printf ("  qoob-flasher -q -w0 /tmp/qoob-bios.gcb\n\n");

  printf ("See also the man page.\n\n");
}

void
qoop_flasher_util_print_help_and_exit (int e)
{
  qoop_flasher_util_print_help ();
  exit (e);
}

/* Emacs indentatation information
   Local Variables:
   indent-tabs-mode:nil
   tab-width:2
   c-set-offset:2
   c-basic-offset:2
   End:
*/
// vim: filetype=c:expandtab:shiftwidth=2:tabstop=2:softtabstop=2
