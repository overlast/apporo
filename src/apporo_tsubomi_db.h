#ifndef APPORO_TSUBOMI_DB_H
#define APPORO_TSUBOMI_DB_H

#include "tsubomi_basic_searcher.h"
#include "tsubomi_indexer.h"
#include "tsubomi_mmap.h"
#include <string>

namespace apporo {
  namespace strage {
    using namespace std;
    
    typedef int                           sa_index;
    typedef std::pair<sa_index, sa_index> sa_range;

    const int utf8_char_size[] = {
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
      3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
    };

    class TsubomiDBSearch : public tsubomi::basic_searcher {
    public:
      TsubomiDBSearch(string &filename);
      ~TsubomiDBSearch();
      sa_index getDID(sa_index num);
      sa_index binaryDIDSearch(sa_index offset, sa_index begin, sa_index end);
    protected:
      tsubomi::mmap_reader<sa_index> mr_did_;
    };

    class TsubomiDBWrite : public tsubomi::writer {
    public:
      TsubomiDBWrite(FILE *fout);
      ~TsubomiDBWrite();
    };
    
    class TsubomiDBIndex : public tsubomi::indexer  {
    public:
      TsubomiDBIndex(string &filename);
      ~TsubomiDBIndex();
      void makeDIDIndex(string &boundary);
      void mkary_make(std::vector<sa_index> &sa, const char *seps, bool is_utf8);
    };
  }
}

#endif
