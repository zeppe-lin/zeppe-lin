include config.mk

all: start-stop-daemon start-stop-daemon.8

.cpp.o:
	${CXX} -c ${CFLAGS} ${CPPFLAGS} $<

patch: crux-patch.diff
	patch -Np1 -i crux-patch.diff

patch-revert: crux-patch.diff
	git checkout start-stop-daemon.c start-stop-daemon.8.pod

start-stop-daemon: start-stop-daemon.o
	${CXX} -o $@ ${LDFLAGS} start-stop-daemon.o

start-stop-daemon.8: start-stop-daemon.8.pod
	pod2man --nourls -r ${VERSION} -c ' ' -n start-stop-daemon \
		-s 8 start-stop-daemon.8.pod > $@

install: all
	mkdir -p ${DESTDIR}${PREFIX}/sbin
	cp -f start-stop-daemon ${DESTDIR}${PREFIX}/sbin/
	chmod 755 ${DESTDIR}${PREFIX}/sbin/start-stop-daemon
	mkdir -p ${DESTDIR}${MANPREFIX}/man8
	cp start-stop-daemon.8 ${DESTDIR}${MANPREFIX}/man8/
	chmod 644 ${DESTDIR}${MANPREFIX}/man8/start-stop-daemon.8

uninstall:
	rm -f ${DESTDIR}${PREFIX}/sbin/start-stop-daemon
	rm -f ${DESTDIR}${MANPREFIX}/man8/start-stop-daemon.8

clean:
	rm -f start-stop-daemon start-stop-daemon.8 \
		start-stop-daemon.o

.PHONY: all install uninstall clean
