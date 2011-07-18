#!/bin/sh

ABSDIR=$(cd $(dirname $0) && pwd)

perl -MDevel::PPPort -e 'Devel::PPPort::WriteFile()'
make clean
perl Makefile.PL
make
