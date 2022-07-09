VERSION ?= git

PREFIX  = /usr/local
BINDIR  = $(PREFIX)/sbin
MANDIR  = $(PREFIX)/share/man

all: rejmerge rejmerge.8 rejmerge.conf.5

%: %.in
	sed "s/@VERSION@/$(VERSION)/" $^ > $@

%: %.pod
	pod2man --nourls -r $(VERSION) -c ' ' \
		-n $(basename $@) -s $(subst .,,$(suffix $@)) $< > $@

install: all
	install -m 755 -D rejmerge        $(DESTDIR)$(BINDIR)/rejmerge
	install -m 644 -D rejmerge.8      $(DESTDIR)$(MANDIR)/man8/rejmerge.8
	install -m 644 -D rejmerge.conf.5 $(DESTDIR)$(MANDIR)/man5/rejmerge.conf.5

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/rejmerge
	rm -f $(DESTDIR)$(MANDIR)/man8/rejmerge.8
	rm -f $(DESTDIR)$(MANDIR)/man5/rejmerge.conf.5

clean:
	rm -f rejmerge rejmerge.8 rejmerge.conf.5

.PHONY: install uninstall clean
