CC  = /usr/bin/g++
PREFIX = /usr/local/Cellar/apporo/0.0.1-01/ 
BIN = /usr/local/Cellar/apporo/0.0.1-01/bin/
LIB = /usr/local/Cellar/apporo/0.0.1-01/lib/
INC = /usr/local/Cellar/apporo/0.0.1-01/include/
SRC = apporo_config.cc apporo_config_test.cc apporo_index.cc apporo_query.cc apporo_resultset.cc apporo_search.cc apporo_searcher.cc apporo_strdistance.cc apporo_tsubomi_db.cc apporo_utf8.cc apporo_util.cc apporo_clang.cc
HDR = apporo_config.h apporo_query.h apporo_resultset.h apporo_search.h apporo_strdistance.h apporo_tsubomi_db.h apporo_utf8.h apporo_util.h apporo_clang.h
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
	$(CC) -O2 -Wall -fPIC -g -c apporo_clang.cc

ifeq ($(UNAME), Darwin)
	$(CC) -O2 -Wall -g -dynamiclib -Wl,-dylib_install_name,libapporo.1.0.dylib -o libapporo.so.1.0.dylib apporo_tsubomi_db.o apporo_clang.o apporo.o apporo_config.o apporo_utf8.o apporo_util.o apporo_query.o apporo_strdistance.o apporo_search.o apporo_index.o -ltsubomi
else
	$(CC) -O2 -Wall -g -shared -Wl,-soname,libapporo.so.1 -o libapporo.so.1.0 apporo_tsubomi_db.o apporo_clang.o apporo.o apporo_config.o apporo_utf8.o apporo_util.o apporo_query.o apporo_strdistance.o apporo_search.o apporo_index.o -ltsubomi
endif

	ar rcs libapporo.a apporo_clang.o apporo_tsubomi_db.o apporo.o apporo_config.o apporo_utf8.o apporo_util.o apporo_query.o apporo_strdistance.o apporo_search.o apporo_index.o

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
	if [ ! -d $(PREFIX) ]; then \
		echo ";; mkdir -p $(PREFIX)"; mkdir -p $(PREFIX); \
	fi
	if [ ! -d $(LIB) ]; then \
		echo ";; mkdir $(LIB)"; mkdir $(LIB); \
	fi
	if [ ! -d $(BIN) ]; then \
		echo ";; mkdir $(BIN)"; mkdir $(BIN); \
	fi
	if [ ! -d $(INC) ]; then \
		echo ";; mkdir $(INC)"; mkdir $(INC); \
	fi
ifeq ($(UNAME), Linux)
	cp libapporo.so.1.0 $(LIB)
	cp libapporo.a $(LIB)
	/sbin/ldconfig -n $(LIB)
	ln -fs $(LIB)libapporo.so.1 $(LIB)libapporo.so
else
ifeq ($(UNAME), Darwin)
	cp libapporo.1.0.dylib $(LIB)
	cp libapporo.a $(LIB)
	ln -fs $(LIB)libapporo.1.0.dylib $(LIB)libappro.1.dylib
	ln -fs $(LIB)libapporo.1.dylib $(LIB)libapporo.dylib
else
	cp libapporo.so.1.0 $(LIB)
	cp libapporo.a $(LIB)
	ln -fs $(LIB)libapporo.so.1.0 $(LIB)libapporo.so.1
	/sbin/ldconfig -m $(LIB)
	ln -fs $(LIB)libapporo.so.1 $(LIB)libapporo.so
endif
endif
	cp apporo $(BIN)
	cp apporo_indexer $(BIN)
	cp apporo_searcher $(BIN)
	cp apporo.h $(INC)
	cp apporo_ma_interface.h $(INC)
	cp apporo_search.h $(INC)
	cp apporo_utf8.h $(INC)
	cp apporo_clang.h $(INC)
	cp apporo_ma_mecab.h $(INC)
	cp apporo_storage.h $(INC)
	cp apporo_util.h $(INC)
	cp apporo_config.h $(INC)
	cp apporo_query.h $(INC)
	cp apporo_strdistance.h $(INC)
	cp apporo_ma.h $(INC)
	cp apporo_resultset.h $(INC)
	cp apporo_tsubomi_db.h $(INC)

uninstall:
	- rm -f $(LIB)libapporo.so
	- rm -f $(LIB)libapporo.so.1
	- rm -f $(LIB)libapporo.so.1.0
	- rm -f $(LIB)libapporo.a
	- rm -f $(BIN)apporo
	- rm -f $(BIN)apporo_indexer
	- rm -f $(BIN)apporo_searcher
	- rm -f $(INC)apporo.h
	- rm -f $(INC)apporo_ma_interface.h
	- rm -f $(INC)apporo_search.h
	- rm -f $(INC)apporo_utf8.h
	- rm -f $(INC)apporo_clang.h
	- rm -f $(INC)apporo_ma_mecab.h
	- rm -f $(INC)apporo_storage.h
	- rm -f $(INC)apporo_util.h
	- rm -f $(INC)apporo_config.h
	- rm -f $(INC)apporo_query.h
	- rm -f $(INC)apporo_strdistance.h
	- rm -f $(INC)apporo_ma.h
	- rm -f $(INC)apporo_resultset.h
	- rm -f $(INC)apporo_tsubomi_db.h

