#!/bin/sh
. $PWD/Pkgfile

_name=${name/-bin/}
curl -LO https://github.com/OJ/$_name/releases/download/v$version/$_name-linux-amd64.7z
mv $_name-linux-amd64.7z $name-$version.7z
