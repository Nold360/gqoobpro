AC_INIT(libqoob, 0.13, jvaltane@kapsi.fi)
AC_CONFIG_SRCDIR(Makefile.am)
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

AM_MAINTAINER_MODE
AC_PROG_CC
AC_PROG_LIBTOOL

CFLAGS="$CFLAGS -Werror -Wall"

AC_PATH_PROG(PKG_CONFIG, pkg-config)
if test -z "$PKG_CONFIG" ; then
  AC_MSG_ERROR( pkg-config is required )
fi

if test "$(uname)" == "FreeBSD"; then
  PKG_CHECK_MODULES(libusb, [libusb-0.1 >= 0.1], , AC_MSG_ERROR( libusb >= 0.1 is required ))
  AC_SUBST(libusb_CFLAGS)
  AC_SUBST(libusb_LIBS)
  AC_SUBST(PACKAGE_REQUIRES, [libusb-0.1])
  AM_CONDITIONAL([FREEBSD], [true])
else
  AM_CONDITIONAL([FREEBSD], [false])
  PKG_CHECK_MODULES(libusb, [libusb >= 0.1.12], , AC_MSG_ERROR( libusb >= 0.1-12 is required ))
  AC_SUBST(libusb_CFLAGS)
  AC_SUBST(libusb_LIBS)
  AC_SUBST(PACKAGE_REQUIRES, [libusb])
fi

dnl debug
AC_ARG_ENABLE(debug,
[  --enable-debug          turn debugging on],
[case "${enableval}" in
  yes) debug=yes ;;
  no)  debug=no ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
esac],[debug=no])

if test x$debug = xyes; then
   CFLAGS="$CFLAGS -g -O0 -DDEBUG"
fi

libqoob_LIBS="\$(top_srcdir)/src/libqoob.la"
libqoob_CFLAGS="-I\$(top_srcdir)/src"
AC_SUBST(libqoob_CFLAGS)
AC_SUBST(libqoob_LIBS)

case $target in
   *-darwin*) CFLAGS="$CFLAGS -DHAVE_DARWIN" ;;
esac

AC_OUTPUT(
Makefile
src/Makefile
libqoob.pc
)

