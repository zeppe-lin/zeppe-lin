ROOTFS		= /tmp/rootfs-$(shell date +%F)-$(shell uname -m)
TARBALL		= $(ROOTFS).tar.xz
LOGFILE		= $(ROOTFS).log
PACKAGES	= $(shell pkgman --no-std-config \
		  --config-set="pkgsrcdir /usr/src/zeppe-lin/core" printf "%n\n")

PKGMKCONF	= /etc/pkgmk.conf
PKGMANCONF	= /etc/pkgman.conf
