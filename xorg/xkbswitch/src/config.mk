# xkbswitch version
VERSION ?= git

# customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# flags
CPPFLAGS = -I$(X11INC) -D_DEFAULT_SOURCE
CFLAGS = -std=c99 -pedantic -Wall -Wextra
LDFLAGS = -L$(X11LIB)
LDLIBS = -lX11

# compiler and linker
CC = cc

# End of file.
