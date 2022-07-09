.POSIX:

include config.mk

all: dwm60cf

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

dwm60cf: dwm60cf.o
	$(CC) -o $@ $(LDFLAGS) $^

install: all
	install -m 0755 -Dt $(DESTDIR)$(PREFIX)/bin/     dwm60cf
	install -m 0644 -Dt $(DESTDIR)$(MANPREFIX)/man1/ dwm60cf.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dwm60cf
	rm -f $(DESTDIR)$(MANPREFIX)/man1/dwm60cf.1

clean:
	rm -f dwm60cf dwm60cf.o

.PHONY: all install uninstall clean
