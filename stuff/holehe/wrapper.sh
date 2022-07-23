#!/bin/sh

_name=holehe
_pyver=$(/usr/bin/python3 -c 'import sys;print(f"{sys.version_info[0]}.{sys.version_info[1]}")')

export PATH=$PATH:/opt/$_name/bin
export PYTHONPATH=/opt/$_name:/opt/$_name/usr/lib/python$_pyver/site-packages

/usr/bin/python3 -c 'from holehe import core; core.main()' $@

# End of file.
