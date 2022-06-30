#!/bin/sh
#
# stuff/LINT.SH: find missing dependencies for stuff pkgsrc collection.
#

find . -name Pkgfile -exec dirname {} \; | sed 's/.\///g' | while read p; do
	pkgman  --root=/dev/null \
		--no-std-config \
		--config-append="pkgsrcdir $PWD" \
		--config-append="pkgsrcdir $PWD/../desktop" \
		--config-append="pkgsrcdir $PWD/../xorg" \
		--config-append="pkgsrcdir $PWD/../system" \
		--config-append="pkgsrcdir $PWD/../core" \
		dep --all --recursive $p | \
		awk -v p=$p '/not found/{print p":"$1}'
done

# End of file.
