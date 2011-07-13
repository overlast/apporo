#!/bin/sh

ABSDIR=$(cd $(dirname $0) && pwd)

#g++ -c -o apporo_tsubomi_db.o ./apporo_tsubomi_db.cc
#g++ -c -o apporo_strdistance.o ./apporo_strdistance.cc
#g++ -c -o apporo_query.o ./apporo_query.cc
#g++ -c -o apporo_search.o ./apporo_search.cc
#g++ -c -o apporo_index.o ./apporo_index.cc
#g++ -c -o apporo_util.o ./apporo_util.cc
#g++ -c -o apporo_utf8.o ./apporo_utf8.cc

cd ${ABSDIR}/../src
echo "compile apporo_indexer"
g++ -O2 -Wall -g apporo_tsubomi_db.cc apporo_index.cc -o ${ABSDIR}/../bin/apporo_indexer -ltsubomi
echo "compile apporo_searcher"
g++ -O2 -Wall -g apporo_searcher.cc apporo_utf8.cc apporo_util.cc apporo_tsubomi_db.cc apporo_query.cc apporo_strdistance.cc apporo_search.cc -o ${ABSDIR}/../bin/apporo_searcher -ltsubomi
