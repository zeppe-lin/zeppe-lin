.POSIX:

include config.mk

all: xkbswitch

.c.o:
	${CC} -c ${CPPFLAGS} ${CFLAGS} $<

xkbswitch: xkbswitch.o
	${CC} -o $@ ${LDFLAGS} xkbswitch.o

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f xkbswitch ${DESTDIR}${PREFIX}/bin/
	chmod 755 ${DESTDIR}${PREFIX}/bin/xkbswitch
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < xkbswitch.1 > \
		${DESTDIR}${MANPREFIX}/man1/xkbswitch.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/xkbswitch.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/xkbswitch
	rm -f ${DESTDIR}${MANPREFIX}/man1/xkbswitch.1

clean:
	rm -f xkbswitch xkbswitch.o

.PHONY: all install uninstall clean
