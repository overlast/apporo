#!/usr/bin/env perl

use strict;
use warnings;

my $filepath = $ARGV[0];
my $suffix_arrsy_lib = $ARGV[1];
my $in;
open ($in, "< $filepath");

if (!(defined $suffix_arrsy_lib) || (($suffix_arrsy_lib ne 'sufary') && ($suffix_arrsy_lib ne 'tsubomi'))) {
    die "you must set 'sufary' or 'tsubomi' to select the suffix array library\n";
}

my $width_mode = "N";
if ($suffix_arrsy_lib eq 'tsubomi') {
    $width_mode = "V";
}

my $point_count = 0;
print pack($width_mode, $point_count);

while (<$in>) {
    my $line = $_;
    $point_count += length($line);
    print pack($width_mode, $point_count);
}

close($in);
