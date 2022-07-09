.POSIX:

include config.mk

SRC = st.c x.c
OBJ = $(SRC:.c=.o)

all: st084cf

.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -c $<

st.o: config.h st.h win.h normalMode.h normalMode.c utils.h
x.o: arg.h config.h st.h win.h

${OBJ}: config.h config.mk

st084cf: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install: st084cf
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f st084cf ${DESTDIR}${PREFIX}/bin/
	chmod 755 ${DESTDIR}${PREFIX}/bin/st084cf
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < st.1 > ${DESTDIR}${MANPREFIX}/man1/st084cf.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/st084cf.1
	tic -sx st.info
	@echo Please see the README file regarding the terminfo entry of st.

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/st084cf
	rm -f ${DESTDIR}${MANPREFIX}/man1/st084cf.1

clean:
	rm -f st084cf ${OBJ}

.PHONY: all install uninstall clean
