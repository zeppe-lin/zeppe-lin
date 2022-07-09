.POSIX:

include config.mk

all: dwm60cf

.c.o:
	${CC} -c ${CFLAGS} ${CPPFLAGS} $<

dwm60cf: dwm60cf.o
	${CC} -o $@ ${LDFLAGS} dwm60cf.o

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp dwm60cf ${DESTDIR}${PREFIX}/bin/
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm60cf
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < dwm60cf.1 > \
		${DESTDIR}${MANPREFIX}/man1/dwm60cf.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm60cf.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm60cf
	rm -f ${DESTDIR}${MANPREFIX}/man1/dwm60cf.1

clean:
	rm -f dwm60cf dwm60cf.o

.PHONY: all install uninstall clean
