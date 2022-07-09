#!/bin/sh
BATT=/sys/class/power_supply/BAT0
[ -d $BATT ] || exit 1

CAP=$(cat $BATT/capacity)
STAT=$(cat $BATT/status)

case $STAT in
Charging)	echo "batt:C$CAP" ;;
Discharging)	echo "batt:D$CAP" ;;
*)		echo "batt:U$CAP" ;;
esac
