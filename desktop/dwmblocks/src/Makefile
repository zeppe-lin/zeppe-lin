.POSIX:

include config.mk

all: dwmblocks #dwmblocks.1

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

dwmblocks: dwmblocks.o
	$(CC) -o $@ $(LDFLAGS) dwmblocks.o

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwmblocks ${DESTDIR}${PREFIX}/bin/
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwmblocks
	mkdir -p ${DESTDIR}${PREFIX}/share/dwmblocks
	cp -f scripts/*.sh ${DESTDIR}${PREFIX}/share/dwmblocks/
	chmod 755 ${DESTDIR}${PREFIX}/share/dwmblocks/*.sh
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < dwmblocks.1 > \
		${DESTDIR}${MANPREFIX}/man1/dwmblocks.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwmblocks.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dwmblocks
	rm -r $(DESTDIR)$(PREFIX)/share/dwmblocks
	rm -f $(DESTDIR)$(MANPREFIX)/man1/dwmblocks.1

clean:
	rm -f dwmblocks dwmblocks.o

.PHONY: all install uninstall clean
