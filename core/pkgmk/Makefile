# pkgmk version
VERSION = 5.41

# paths
PREFIX  = /usr/local
BINDIR  = $(PREFIX)/sbin
MANDIR  = $(PREFIX)/share/man

all: pkgmk pkgmk.8 pkgmk.conf.5 Pkgfile.5

%: %.pod
	pod2man --nourls -r $(VERSION) -c ' ' -n $(basename $@) \
		-s $(subst .,,$(suffix $@)) $< > $@

%: %.in
	sed -e "s/@VERSION@/$(VERSION)/g" $< > $@

install: all
	install -m 755 -D pkgmk              $(DESTDIR)$(BINDIR)/pkgmk
	install -m 644 -D pkgmk.8            $(DESTDIR)$(MANDIR)/man8/pkgmk.8
	install -m 644 -D pkgmk.conf.5       $(DESTDIR)$(MANDIR)/man5/pkgmk.conf.5
	install -m 644 -D Pkgfile.5          $(DESTDIR)$(MANDIR)/man5/Pkgfile.5

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/pkgmk
	rm -f $(DESTDIR)$(MANDIR)/man8/pkgmk.8
	rm -f $(DESTDIR)$(MANDIR)/man5/pkgmk.conf.5
	rm -f $(DESTDIR)$(MANDIR)/man5/Pkgfile.5

clean:
	rm -f pkgmk pkgmk.8 pkgmk.conf.5 Pkgfile.5

.PHONY: all install uninstall clean
