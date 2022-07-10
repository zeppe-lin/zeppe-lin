# start-stop-daemon version
VERSION ?= git

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -DVERSION=\"${VERSION}\"
CFLAGS = -std=c99 -Wall -Wextra -Wformat

# End of file.
