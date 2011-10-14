#ifndef APPORO_MA_MECAB_H
#define APPORO_MA_MECAB_H

#include "apporo_ma_interface.h"
#include <mecab.h>
#include <string>
#include <vector>
#include <iostream>

namespace apporo {
  namespace ma {
    class MAMeCab : public MAInterface {
    public:
      MAMeCab();
      ~MAMeCab();
      MeCab::Tagger *tagger;
      std::string getMAResult(std::string &str);
      std::string getRoman(std::string &str);
      std::string getYomi(std::string &str);
    };
  }
}

#endif
