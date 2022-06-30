#!/bin/bash
[ -x /usr/bin/amixer ] || exit 1

if amixer get Master | grep -q off; then
	printf "%b" "\ufa80"
else
	printf "%b %i" \
		"\ufa7d" \
		"$(amixer get Master | tail -1 | sed 's/.*\[\([0-9]*\)%\].*/\1/')"
fi

# End of file.
