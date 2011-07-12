#!/bin/sh

#g++ -c -o apporo_tsubomi_db.o ./apporo_tsubomi_db.cpp
#g++ -c -o apporo_strdistance.o ./apporo_strdistance.cpp
#g++ -c -o apporo_query.o ./apporo_query.cpp
#g++ -c -o apporo_search.o ./apporo_search.cpp
#g++ -c -o apporo_index.o ./apporo_index.cpp
#g++ -c -o apporo_util.o ./apporo_util.cpp
#g++ -c -o apporo_utf8.o ./apporo_utf8.cpp

echo "compile apporo_indexer"
g++ -O2 -g apporo_tsubomi_db.cpp apporo_index.cpp -o apporo_indexer -ltsubomi
echo "compile apporo_searcher"
g++ -O2 -g apporo_searcher.cpp apporo_utf8.cpp apporo_util.cpp apporo_tsubomi_db.cpp apporo_query.cpp apporo_strdistance.cpp apporo_search.cpp -o apporo_searcher -ltsubomi

