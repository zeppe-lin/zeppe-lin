#!/bin/sh
mem=$(free -m | awk 'NR == 2 { print int($3*100/$2) }')
cpu=$(awk '/cpu /{ print int(($2+$4)*100/($2+$4+$5)) }' /proc/stat)
echo c${cpu}m${mem}
