#!/usr/bin/perl

use strict;
use warnings;
use diagnostics;
use autodie;
use feature 'say';
use Cwd;
use File::Basename;

my %obsolete = map { $_ => 1 } qw(
	autoconf-2.13 gtk-engines hcxtools3
	ktsuss librsvg-compat libxcrypt2.4 mdk3 newsboat213 pango-compat
	pm-utils python python-cairo python-gobject-compact smtp-user-enum
	tnscmd10g transset-df picom shared-mime-info
	python-gobject-compat gtk-engine-murrine
	);

my %pkgsrc = map { $_ => 1 } qw(
	filesystem rc torify-all-traffic
	dwm60cf st084cf xkbswitch dwmblocks
	pkgmk pkgman revdep rejmerge pkgutils pkgmaint
	trinity-gtk-theme trinity-icon-theme trinity-extra-theme
	);

my %pkgdb = map { chomp; $_, 1} qx(pkgman printf "%n\n");
my %feeds;

open my $fh, getcwd() . '/RSS';
while (<$fh>) {
	next if /^===/ or /^$/ or /^#/;
	next unless /^(?:.*?)\s+"~\[.*?\]\s+(?<title>.*?)"(?:\s+(?:.*?)\s+)?$/;

	$feeds{$_}++ for (split /,\s*/, $+{title});
}
close $fh;

print <<EOF;

The following packages have no RSS feeds
========================================

EOF

for (sort keys %pkgdb) {
	if (!exists $feeds{$_}) {
		if ($obsolete{$_}) {
			say "[x] \e[9m$_\e[0m (obsolete)";
		} elsif ($pkgsrc{$_}) {
			say "[v] $_ (internal)";
		} else {
			say "[ ] $_";
		}
	}
}

print <<EOF;

The following feeds are redundant
=================================

EOF

for (sort keys %feeds) {
	say unless exists $pkgdb{$_};
}

# End of file.
