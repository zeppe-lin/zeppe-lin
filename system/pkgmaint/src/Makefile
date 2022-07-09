VERSION ?= git

PREFIX  = /usr/local
BINDIR  = $(PREFIX)/bin
MANDIR  = $(PREFIX)/share/man
ETCDIR  = /etc

PROGS = $(patsubst %.in,%,$(wildcard *.in))
MANS  = $(patsubst %.pod,%,$(wildcard *.pod))

all: $(PROGS) $(MANS)

%: %.in
	sed -e "s/@VERSION@/$(VERSION)/" $< > $@

%: %.pod
	pod2man --nourls -r $(VERSION) -c ' ' -n $(basename $@) \
		-s $(subst .,,$(suffix $@)) $< > $@

install: all
	install -d $(DESTDIR)$(ETCDIR) $(DESTDIR)$(BINDIR) $(DESTDIR)$(MANDIR)/man1/
	install -m 755 $(PROGS) $(DESTDIR)$(BINDIR)
	install -m 644 $(MANS)  $(DESTDIR)$(MANDIR)/man1/
	install -m 644 finddisowned.whitelist $(DESTDIR)$(ETCDIR)/
	ln -sf pkgmkcflags    $(DESTDIR)$(BINDIR)/pkgmkcxxflags
	ln -sf pkgmkcflags.1  $(DESTDIR)$(MANDIR)/man1/pkgmkcxxflags.1

uninstall:
	cd $(DESTDIR)$(BINDIR) && rm -f $(PROGS)
	cd $(DESTDIR)$(MANDIR)/man1 && rm -f $(MANS)
	rm -f $(DESTDIR)$(ETCDIR)/finddisowned.whitelist
	unlink $(DESTDIR)$(BINDIR)/pkgmkcxxflags
	unlink $(DESTDIR)$(MANDIR)/man1/pkgmkcxxflags.1

clean:
	rm -f $(PROGS) $(MANS)

.PHONY: all install uninstall clean
