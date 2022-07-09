.POSIX:

VERSION ?= git

PREFIX  = /usr
BINDIR  = ${PREFIX}/sbin
DATADIR = ${PREFIX}/share
MANDIR  = ${PREFIX}/share/man

all: mkinitramfs mkinitramfs.8 mkinitramfs.config.5 mkinitramfs.cmdline.7 mkinitramfs.hooks.7

%: %.in
	sed "s/@VERSION@/$(VERSION)/" $^ > $@

%: %.pod
	pod2man --nourls -r $(VERSION) -c ' ' \
		-n $(basename $@) -s $(subst .,,$(suffix $@)) $< > $@

install: all
	install -d ${DESTDIR}${DATADIR}/mkinitramfs
	cp -R hooks device-helper init          ${DESTDIR}${DATADIR}/mkinitramfs/
	install -m 755 -D mkinitramfs           ${DESTDIR}${BINDIR}/mkinitramfs
	install -m 644 -D mkinitramfs.8         ${DESTDIR}${MANDIR}/man8/mkinitramfs.8
	install -m 644 -D mkinitramfs.cmdline.7 ${DESTDIR}${MANDIR}/man7/mkinitramfs.cmdline.7
	install -m 644 -D mkinitramfs.hooks.7   ${DESTDIR}${MANDIR}/man7/mkinitramfs.hooks.7
	install -m 644 -D mkinitramfs.config.5  ${DESTDIR}${MANDIR}/man5/mkinitramfs.config.5

uninstall:
	rm -f  ${DESTDIR}${BINDIR}/mkinitramfs
	rm -rf ${DESTDIR}${DATADIR}/mkinitramfs
	rm -f  ${DESTDIR}${MANDIR}/man8/mkinitramfs.8
	rm -f  ${DESTDIR}${MANDIR}/man7/mkinitramfs.cmdline.7
	rm -f  ${DESTDIR}${MANDIR}/man7/mkinitramfs.hooks.7
	rm -f  ${DESTDIR}${MANDIR}/man5/mkinitramfs.config.5

clean:
	rm -f mkinitramfs mkinitramfs.8 mkinitramfs.cmdline.7 mkinitramfs.hooks.7 mkinitramfs.config.5

.PHONY: all install uninstall clean
