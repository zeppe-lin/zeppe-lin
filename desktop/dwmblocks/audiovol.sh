#!/bin/sh
[ -x /usr/bin/amixer ] || exit 1

if amixer get Master | grep -q off; then
	echo "off"
else
	amixer get Master | tail -1 | sed 's/.*\[\([0-9]*\)%\].*/\1/'
fi

# End of file.
