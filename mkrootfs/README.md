ABOUT
=====

This directory provides a Makefile for building customized root
filesystem for chroot installation.


USAGE
=====

By default, **make** builds only core packages (see PACKAGES variable).
This can be redefined from command-line:

```sh
sudo make rootfs PACKAGES="..."
```

In addition, **make** uses `/etc/pkgmk.conf` and `/etc/pkgman.conf`
configuration files by default. These files (if you haven't edited
them in your zeppe-lin installation) contain reasonable defaults and
can be used to create a root filesystem with your current built system
packages.

If you changed configuration files mentioned above, for example changed
`CFLAGS` in `/etc/pkgmk.conf` from `-march=x86_64` to `-march=native`,
but want to build rootfs for generic x86_64, then make a copy of
`pkgmk.conf` with your settings and tell **make** to use it:

```sh
sudo make rootfs PKGMKCONF="/path/to/custom/pkgmk.conf"
```

Also, use the `PKGMANCONF` variable if you want to use your custom
`pkgman.conf` location.

By default, **make** builds the root filesystem in the
`/tmp/rootfs-${DATE}-${ARCH}` directory, and saves the build/install
logs in `/tmp/rootfs-${DATE}-${ARCH}.log`. This can be changed by
`ROOTFS` and `LOGFILE` variables.

So, let's create the root filesystem:

```sh
sudo make rootfs
```

In the end, we'll see the report of installed packages and the message
that "Install transaction done".

The next step is to check the rootfs for missing libraries of installed
packages:

```sh
sudo make revdep
```

Now, let's prepare the rootfs tarball:

```sh
sudo make tarball
```

By default **make** creates an `XZ` archive in the same directory as
the root filesystem, ie. `/tmp/rootfs-${DATE}-${ARCH}.tar.xz`.
This can be changed by `TARBALL` variable.

So, having created a tarball, you must sign it with a PGP key:

```sh
make sign
```

This command calls `gpg(1)` with your default signing key. It doesn't
need to be called via `sudo(8)` unless the root owns the signing keys.

`make sign` will create the file
`/tmp/rootfs-${DATE}-${ARCH}.tar.xz.sig`.

That's it, the rootfs tarball and signature are ready.
Type "make" or "make help" to view all available options.


Customization
-------------

If you want to change the default variables (like `ROOTFS`, `TARBALL`,
`LOGFILE`, `PACKAGES`, `PKGMKCONF` and `PKGMANCONF`), there are at
least two ways:

1. You can just call make with arguments (`make ROOTFS=/tmp/rootfs`).

2. Copy file `config.def.mk` to `config.mk` and modify it. **make**
   first reads file `config.def.mk`, then `config.mk` (if any), and
   overrides the variables defined in `config.def.mk`.


Prepare official release
========================

First, you need to make a rootfs as described above. Next, create a
tag in [pkgsrc](https://github.com/zeppe-lin/pkgsrc) repository. For
example:

```sh
cd /usr/src/pkgsrc
TAG=$(date +"%Y-%m-%d")
git tag -a $TAG -m $TAG
git push
git push --tags
```

Note, that the `tag` format is `YEAR-MONTH-DAY`.

Secondly, go to `pkgsrc` repository on the github and
[draft a new release](https://github.com/zeppe-lin/pkgsrc/releases/new).

Select **Choose a tag** and select the tag you previously created.

Set **Release title** the same as the tag.

To describe this release, prepare the git log for the rootfs image:

```sh
git log --oneline $PREV_TAG..$TAG core/ | xclip -selection clipboard
```
Note, that `$PREV_TAG` is the previous existing tag, before you created
the actual one. To see all tags type:

```sh
git tag -l
```

Next, paste the clipboard content into the `describe` section in github.

Attach previously created rootfs and signature.

Publish the release.

**REMEMBER** to update the rootfs URL in the
[handbook](https://github.com/zeppe-lin/handbook).


<!-- vim:ft=markdown:cc=72
End of file. -->
