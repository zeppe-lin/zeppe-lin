#!/bin/sh

_pyver=$(/usr/bin/python3 -c 'import sys;print(f"{sys.version_info[0]}.{sys.version_info[1]}")')
export PATH=$PATH:/opt/karma/bin
export PYTHONPATH=/opt/karma:/opt/karma/usr/lib/python${_pyver}/site-packages
/usr/bin/python3 /opt/karma/usr/bin/karma $@

# End of file.
