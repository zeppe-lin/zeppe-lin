# dwm60cf version
VERSION ?= git

# customize below to fit your system

# optional xinerama support (comment to disable)
XINERAMAFLAGS = -DXINERAMA
XINERAMALIBS = -lXinerama

# optional per window keyboard layout support (comment to disable)
PWKL = -DPWKL

# optional windows title support (comment to disable)
WINTITLE = -DWINTITLE

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = /usr/local/include
X11LIB = /usr/local/lib

FT2INC = /usr/local/include/freetype2
FT2LIB = -lfontconfig -lXft

# includes and libs
INCS = -I${X11INC} -I${FT2INC}
LIBS = -L${X11LIB} -lX11 ${FT2LIB} ${XINERAMALIBS}

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L \
	   -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS} ${PWKL} ${WINTITLE} \
	   ${INCS}
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wformat
LDFLAGS = ${LIBS}

# compiler and linker
CC = cc

# End of file.
