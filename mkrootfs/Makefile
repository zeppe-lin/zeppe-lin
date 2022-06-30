include config.def.mk
-include config.mk

help: all
all:
	@echo "Build customized root filesystem for chroot installation."
	@echo
	@echo "make rootfs     - prepare rootfs directory"
	@echo "make revdep     - check rootfs for missing libraries"
	@echo "make tarball    - prepare rootfs' tarball"
	@echo "make sign       - sign prepared tarball"
	@echo "make clean      - remove rootfs directory and logfile"
	@echo "make clean-all  - make clean + remove tarball and signature"

needs_root:
	@if [ "`id -u`" != 0 ]; then echo "This target requires root privileges!" ; exit 1; fi

rootfs: needs_root $(PKGMKCONF)
	@echo prepare rootfs...
	@mkdir -p $(ROOTFS)/var/lib/pkg
	@touch $(ROOTFS)/var/lib/pkg/db $(LOGFILE)
	@pkgman  --root=$(ROOTFS)                      \
		--config="$(PKGMANCONF)"               \
		--config-append="logfile $(LOGFILE)"   \
		--config-append="logmode append"       \
		--config-append="runscripts no"        \
		install --force --deps --group -d      \
		--margs="-cf $(PKGMKCONF)" $(PACKAGES)
	@echo rootfs was created in $(ROOTFS) directory.

revdep: needs_root $(ROOTFS)
	@echo check rootfs for missing libraries...
	@chroot $(ROOTFS) revdep -p
	@echo revdep done.

tarball: needs_root $(ROOTFS)
	@echo prepare tarball...
	(cd $(ROOTFS) ; tar -cJf $(TARBALL) . ; sync)
	@echo $(TARBALL) was created.

sign: $(TARBALL)
	gpg --output $(TARBALL).sig --detach-sig $(TARBALL)
	@echo $(TARBALL).sig was created.

clean: needs_root
	@echo cleaning rootfs and logfile...
	@rm -rf $(ROOTFS) $(LOGFILE)
	@echo clean done.

clean-all: clean
	@echo cleaning tarball and signature...
	@rm -f $(TARBALL) $(TARBALL).sig
	@echo clean-all done.

.PHONY: rootfs revdep tarball sign clean clean-tarball
