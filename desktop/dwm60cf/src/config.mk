# dwm60cf version
VERSION ?= git

# customize below to fit your system

# optional xinerama support
WITH_XINERAMA = -DXINERAMA

# optional per window keyboard layout support
WITH_PWKL = -DPWKL

# optional windows title support
WITH_WINTITLE = -DWINTITLE

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

FT2INC = /usr/include/freetype2

# flags
CPPFLAGS = -I $(FT2INC) -D_DEFAULT_SOURCE -DVERSION=\"$(VERSION)\" \
	   $(WITH_XINERAMA) $(WITH_PWKL) $(WITH_WINTITLE)
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wformat
LDFLAGS = -L$(X11LIB) -L$(XFTLIB) $(if $(WITH_XINERAMA),-L$(XINERAMALIB))
LDFLAGS = -lc -lX11 -lXft $(if $(WITH_XINERAMA),-lXinerama)

# compiler and linker
CC = cc

# End of file.
