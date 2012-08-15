#!/usr/bin/env perl

use strict;
use warnings;
use utf8;
use Apporo;
use YAML;

my $path = $ARGV[0];
my $query = $ARGV[1];
my $app = Apporo->new($path);
my @arr = $app->retrieve($query);
print Dump \@arr;
