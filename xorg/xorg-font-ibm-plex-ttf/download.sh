#!/bin/sh
. ./Pkgfile
wget https://github.com/IBM/plex/releases/download/v$version/TrueType.zip \
	-O $name-$version.zip
