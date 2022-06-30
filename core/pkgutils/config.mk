# pkgutils version if undefined
VERSION ?= git

# paths
PREFIX  = /usr/local
BINDIR  = $(PREFIX)/sbin
MANDIR  = $(PREFIX)/share/man

# flags
CXXFLAGS += -Wall -Wextra -pedantic
CPPFLAGS += -D_POSIX_SOURCE -D_GNU_SOURCE -DVERSION=\"$(VERSION)\" \
            -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DNDEBUG
LDFLAGS += -static $(shell pkg-config --libs --static libarchive)

# End of file.
