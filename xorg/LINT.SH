#!/bin/sh
#
# xorg/LINT.SH: find missing dependencies for xorg pkgsrc collection.
#

find . -type f -name Pkgfile -print | cut -d/ -f2 | while read p; do
	pkgman  --root= \
		--no-std-config \
		--config-append="pkgsrcdir $PWD" \
		--config-append="pkgsrcdir $PWD/../system" \
		--config-append="pkgsrcdir $PWD/../core" \
		dep --all --recursive $p | \
		awk -v p=$p '/not found/{print p":"$1}'
done

# End of file.
