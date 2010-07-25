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

my $regexp_utf8_len = '(?:[\x00-\x7F]|'. # 1byte
    '[\xC2-\xDF][\x80-\xBF]{1}|'. # 2byte
    '[\xE0-\xEF][\x80-\xBF]{2}|'. # 3byte
    '[\xF0-\xF7][\x80-\xBF]{3})'; # 4byte

my $point_count = 0;

while (<$in>) {
    my $line = $_;
    my $focus_point = 0;
    my $is_tab = 0;
    
    while ($line =~ m|($regexp_utf8_len)|og) {
        my $char = $1;
        if ($char =~ /[\t]/) {
            $is_tab = 1;
        }
        elsif ($char =~ /[\n]/) {
            print pack($width_mode, $point_count + $focus_point);
            last;
        }
        
        unless ($is_tab) {
            print pack($width_mode, $point_count + $focus_point);
        }
        $focus_point += length($char);
    }
    $point_count += length($line);
}

close($in);
