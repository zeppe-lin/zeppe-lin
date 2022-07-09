.POSIX:

include config.mk

all: xkbswitch

.c.o:
	${CC} -c ${CPPFLAGS} ${CFLAGS} $<

xkbswitch: xkbswitch.o
	${CC} -o $@ ${LDFLAGS} xkbswitch.o

install: all
	install -m 0755 -Dt ${DESTDIR}${PREFIX}/bin/     xkbswitch
	install -m 0644 -Dt ${DESTDIR}${MANPREFIX}/man1/ xkbswitch.1
	sed -i "s/VERSION/${VERSION}/g" \
		${DESTDIR}${MANPREFIX}/man1/xkbswitch.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/xkbswitch
	rm -f ${DESTDIR}${MANPREFIX}/man1/xkbswitch.1

clean:
	rm -f xkbswitch xkbswitch.o

.PHONY: all install uninstall clean
