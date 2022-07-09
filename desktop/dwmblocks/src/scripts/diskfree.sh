#!/bin/sh
df -h | awk '/\/boot|root|pkgmk|raid/ {
	if ($1 == "pkgmk") {
		printf "%s%s ", $1, $4;
	} else if ($1 == "/dev/mapper/raid") {
		printf "raid%s ", $4;
	} else {
		printf "%s%s ", $6, $4;
	}
}' | sed 's/ *$//g'
