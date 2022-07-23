#!/bin/sh -e
export PATH=$PATH:/opt/socialscan/bin
export PYTHONPATH=/opt/socialscan
/usr/bin/python3 /opt/socialscan/bin/socialscan $@
