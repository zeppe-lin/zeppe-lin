.POSIX:

include config.mk

all: dwmblocks #dwmblocks.1

.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

#%: %.pod
#	pod2man -c ' ' -n dwmblocks -r $(VERSION) $^ > $@

dwmblocks: dwmblocks.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

install: all
	install -m 0755 -Dt $(DESTDIR)$(PREFIX)/bin/     dwmblocks
	#install -m 0644 -Dt $(DESTDIR)$(MANPREFIX)/man1/ dwmblocks.1
	install -m 0755 -Dt $(DESTDIR)$(DATAPREFIX)/dwmblocks scripts/*.sh

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dwmblocks
	#rm -f $(DESTDIR)$(MANPREFIX)/man1/dwmblocks.1
	rm -r $(DESTDIR)$(DATAPREFIX)/dwmblocks

clean:
	rm -f dwmblocks dwmblocks.o #xkbswitch.1

.PHONY: all install uninstall clean
