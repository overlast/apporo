require 'formula'

# Documentation: https://github.com/mxcl/homebrew/wiki/Formula-Cookbook
# PLEASE REMOVE ALL GENERATED COMMENTS BEFORE SUBMITTING YOUR PULL REQUEST!

class Apporo < Formula
  homepage 'http://code.google.com/p/apporo/'
  url 'http://apporo.googlecode.com/svn/trunk'
  sha1 ''
  version '0.0.1-01'
  head 'http://apporo.googlecode.com/svn/trunk'

  depends_on 'tsubomi' # https://github.com/overlast/private/blob/master/homebrew/tsubomi/tsubomi.rb

  def patches
    # Makefile patch
    DATA
  end

  def install
    cd 'src' do
      system "make"
      system "make install"
    end
  end

  def test
    # This test will fail and we won't accept that! It's enough to just replace
    # "false" with the main program this formula installs, but it'd be nice if you
    # were more thorough. Run the test with `brew test apporo`.
    system "false"
  end
end

__END__
diff --git a/src/Makefile b/./homebrew/Makefile
index 5d17a2a..86ba825 100644
--- a/src/Makefile
+++ b/src/Makefile
@@ -1,7 +1,8 @@
 CC  = /usr/bin/g++
-BIN = /usr/local/bin/
-LIB = /usr/local/lib/
-INC = /usr/local/include/
+PREFIX = /usr/local/Cellar/apporo/0.0.1-01/ 
+BIN = /usr/local/Cellar/apporo/0.0.1-01/bin/
+LIB = /usr/local/Cellar/apporo/0.0.1-01/lib/
+INC = /usr/local/Cellar/apporo/0.0.1-01/include/
 SRC = apporo_config.cc apporo_config_test.cc apporo_index.cc apporo_query.cc apporo_resultset.cc apporo_search.cc apporo_searcher.cc apporo_strdistance.cc apporo_tsubomi_db.cc apporo_utf8.cc apporo_util.cc apporo_clang.cc
 HDR = apporo_config.h apporo_query.h apporo_resultset.h apporo_search.h apporo_strdistance.h apporo_tsubomi_db.h apporo_utf8.h apporo_util.h apporo_clang.h
 UNAME = $(shell uname)
@@ -57,6 +58,18 @@ clean:
 	- rm -f libapporo.a
 
 install:
+	if [ ! -d $(PREFIX) ]; then \
+		echo ";; mkdir -p $(PREFIX)"; mkdir -p $(PREFIX); \
+	fi
+	if [ ! -d $(LIB) ]; then \
+		echo ";; mkdir $(LIB)"; mkdir $(LIB); \
+	fi
+	if [ ! -d $(BIN) ]; then \
+		echo ";; mkdir $(BIN)"; mkdir $(BIN); \
+	fi
+	if [ ! -d $(INC) ]; then \
+		echo ";; mkdir $(INC)"; mkdir $(INC); \
+	fi
 ifeq ($(UNAME), Linux)
 	cp libapporo.so.1.0 $(LIB)
 	cp libapporo.a $(LIB)
@@ -80,7 +93,19 @@ endif
 	cp apporo_indexer $(BIN)
 	cp apporo_searcher $(BIN)
 	cp apporo.h $(INC)
-	cp apporo_*.h $(INC)
+	cp apporo_ma_interface.h $(INC)
+	cp apporo_search.h $(INC)
+	cp apporo_utf8.h $(INC)
+	cp apporo_clang.h $(INC)
+	cp apporo_ma_mecab.h $(INC)
+	cp apporo_storage.h $(INC)
+	cp apporo_util.h $(INC)
+	cp apporo_config.h $(INC)
+	cp apporo_query.h $(INC)
+	cp apporo_strdistance.h $(INC)
+	cp apporo_ma.h $(INC)
+	cp apporo_resultset.h $(INC)
+	cp apporo_tsubomi_db.h $(INC)
 
 uninstall:
 	- rm -f $(LIB)libapporo.so
@@ -91,5 +116,17 @@ uninstall:
 	- rm -f $(BIN)apporo_indexer
 	- rm -f $(BIN)apporo_searcher
 	- rm -f $(INC)apporo.h
-	- rm -f $(INC)apporo_*.h
+	- rm -f $(INC)apporo_ma_interface.h
+	- rm -f $(INC)apporo_search.h
+	- rm -f $(INC)apporo_utf8.h
+	- rm -f $(INC)apporo_clang.h
+	- rm -f $(INC)apporo_ma_mecab.h
+	- rm -f $(INC)apporo_storage.h
+	- rm -f $(INC)apporo_util.h
+	- rm -f $(INC)apporo_config.h
+	- rm -f $(INC)apporo_query.h
+	- rm -f $(INC)apporo_strdistance.h
+	- rm -f $(INC)apporo_ma.h
+	- rm -f $(INC)apporo_resultset.h
+	- rm -f $(INC)apporo_tsubomi_db.h
 
