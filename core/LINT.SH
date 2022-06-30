#!/bin/sh
#
# core/LINT.SH: find missing dependencies for core pkgsrc collection.
#

find . -type f -name Pkgfile -print | cut -d/ -f2 | while read p; do
	pkgman  --root= \
		--no-std-config \
		--config-set="pkgsrcdir $PWD" \
		dep --all --recursive $p | \
		awk -v p=$p '/not found/{print p":"$1}'
done

# End of file.
