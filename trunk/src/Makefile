CC  = /usr/bin/g++
BIN = /usr/local/bin/
LIB = /usr/local/lib/
INC = /usr/local/include/
SRC = apporo_config.cc apporo_config_test.cc apporo_index.cc apporo_query.cc apporo_resultset.cc apporo_search.cc apporo_searcher.cc apporo_strdistance.cc apporo_tsubomi_db.cc apporo_utf8.cc
HDR = apporo_config.h apporo_query.h apporo_resultset.h apporo_search.h apporo_strdistance.h apporo_tsubomi_db.h apporo_utf8.h apporo_util.h
UNAME = $(shell uname)

tools: apporo_indexer apporo_searcher apporo
apporo: apporo_config_test.cc libapporo.so.1.0

apporo_indexer: $(HDR) apporo_tsubomi_db.cc apporo_index.cc
	$(CC) -O2 -Wall -g apporo_tsubomi_db.cc apporo_index.cc -o apporo_indexer -ltsubomi
apporo_searcher: $(HDR) apporo_searcher.cc apporo_utf8.cc apporo_util.cc apporo_tsubomi_db.cc apporo_query.cc apporo_strdistance.cc apporo_search.cc
	$(CC) -O2 -Wall -g apporo_searcher.cc apporo_utf8.cc apporo_util.cc apporo_tsubomi_db.cc apporo_query.cc apporo_strdistance.cc apporo_search.cc -o apporo_searcher -ltsubomi

libapporo.so.1.0: $(SRC) $(HDR)
	$(CC) -O2 -Wall -fPIC -g -c apporo.cc
	$(CC) -O2 -Wall -fPIC -g -c apporo_config.cc
	$(CC) -O2 -Wall -fPIC -g -c apporo_utf8.cc
	$(CC) -O2 -Wall -fPIC -g -c apporo_util.cc
	$(CC) -O2 -Wall -fPIC -g -c apporo_tsubomi_db.cc
	$(CC) -O2 -Wall -fPIC -g -c apporo_query.cc
	$(CC) -O2 -Wall -fPIC -g -c apporo_strdistance.cc
	$(CC) -O2 -Wall -fPIC -g -c apporo_search.cc
	$(CC) -O2 -Wall -fPIC -g -c apporo_index.cc
	$(CC) -O2 -Wall -g -dynamiclib -Wl,-dylib_install_name,libapporo.so.1 -o libapporo.so.1.0 apporo_tsubomi_db.o apporo.o apporo_config.o apporo_utf8.o apporo_util.o apporo_query.o apporo_strdistance.o apporo_search.o apporo_index.o -ltsubomi
	$(CC) -O2 -Wall -g -dynamiclib -Wl,-dylib_install_name,libapporo.so.1 -o libapporo.so.1.0 apporo_tsubomi_db.o apporo.o apporo_config.o apporo_utf8.o apporo_util.o apporo_query.o apporo_strdistance.o apporo_search.o apporo_index.o -ltsubomi
	ar rcs libapporo.a apporo_tsubomi_db.o apporo.o apporo_config.o apporo_utf8.o apporo_util.o apporo_query.o apporo_strdistance.o apporo_search.o apporo_index.o

clean:
	- rm apporo
	- rm apporo.o
	- rm apporo_config.o
	- rm apporo_utf8.o
	- rm apporo_util.o
	- rm apporo_tsubomi_db.o
	- rm apporo_query.o
	- rm apporo_strdistance.o
	- rm apporo_search.o
	- rm apporo_index.o
	- rm -f apporo_indexer
	- rm -f apporo_searcher
	- rm -f libapporo.so.1.0
	- rm -f libapporo.a

install:
	cp libapporo.so.1.0 $(LIB)
	cp libapporo.a $(LIB)
ifeq ($(UNAME), Linux)
#	/sbin/ldconfig -n $(LIB)
	ln -fs $(LIB)libapporo.so.1 $(LIB)libapporo.so
else
	ln -fs $(LIB)libapporo.so.1.0 $(LIB)libapporo.so.1
#	/sbin/ldconfig -m $(LIB)
	ln -fs $(LIB)libapporo.so.1 $(LIB)libapporo.so
endif
	cp apporo $(BIN)
	cp apporo_indexer $(BIN)
	cp apporo_searcher $(BIN)
	cp apporo_*.h $(INC)
	cp apporo.h $(INC)

uninstall:
	- rm -f $(LIB)libapporo.so
	- rm -f $(LIB)libapporo.so.1
	- rm -f $(LIB)libapporo.so.1.0
	- rm -f $(LIB)libapporo.a
	- rm -f $(BIN)apporo
	- rm -f $(BIN)apporo_indexer
	- rm -f $(BIN)apporo_searcher
	- rm -f $(INC)apporo_*.h
	- rm -f $(INC)apporo.h

