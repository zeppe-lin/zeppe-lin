#!/bin/sh -e
if [ -x /usr/bin/apulse ]; then
	exec apulse /opt/firefox/firefox "$@"
else
	exec /opt/firefox/firefox "$@"
fi
