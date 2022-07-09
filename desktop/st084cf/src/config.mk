# st084cf version
VERSION ?= git

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

X11INC = /usr/local/include
X11LIB = /usr/local/lib

FT2INC = /usr/local/include/freetype2
FT2LIB = -lfontconfig -lXft

# includes and libs
INCS = -I${X11INC} -I${FT2INC}
LIBS = -L${X11LIB} -lX11 -lm -lrt -lutil ${FT2LIB} -lfreetype

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -D_XOPEN_SOURCE=600 -D_BSD_SOURCE \
	   ${INCS}
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wformat
LDFLAGS = ${LIBS}

# compiler and linker
CC = cc

# End of file.
