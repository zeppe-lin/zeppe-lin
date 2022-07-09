# dwmblocks version
VERSION ?= git

# customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

X11INC = /usr/local/include
X11LIB = /usr/local/lib

# includes and libs
INCS = -I${X11INC}
LIBS = -L${X11LIB} -lX11

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L \
	   -DVERSION=\"${VERSION}\" ${INCS}
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wformat
LDFLAGS = ${LIBS}

# compiler and linker
CC = cc

# End of file.
