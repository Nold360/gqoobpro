SUBDIRS = src

pcfiles = libqoob.pc

all-local: $(pcfiles)

%.pc: %.pc
	cp $< $@

if FREEBSD
pkgconfigdir = $(prefix)/libdata/pkgconfig
else
pkgconfigdir = $(libdir)/pkgconfig
endif

pkgconfig_DATA = $(pcfiles)

deb-install:
	mkdir -p $(top_srcdir)/debian/tmp/etc/udev/rules.d
	mkdir -p $(top_srcdir)/debian/tmp/etc/modprobe.d
	install -c --mode=644 $(top_srcdir)/data/86-qoobpro.rules $(top_srcdir)/debian/tmp/etc/udev/rules.d/
	install -c --mode=644 $(top_srcdir)/data/qoobpro.conf $(top_srcdir)/debian/tmp/etc/modprobe.d/

EXTRA_DIST = libqoob.pc.in autogen.sh data/86-qoobpro.rules \
	data/qoobpro.conf TODO README COPYING NEWS ChangeLog ReleeaseNotes \
        packaging/debian/changelog packaging/debian/compat \
	packaging/debian/control packaging/debian/copyright \
	packaging/debian/rules packaging/debian/libqoob0.install \
	packaging/debian/libqoob0.postinst packaging/debian/libqoob0.postrm \
	packaging/debian/libqoob-dev.install
CLEANFILES = $(pcfiles) $(pcfiles_uninstalled)
