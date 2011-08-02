#ifndef APPORO_MA_MECAB_H
#define APPORO_MA_MECAB_H

#import <string>
#import "apporo_ma_interface.h"

namespace apporo {
  namespace ma {
    class MAMeCab : public MAInterface {
    public:
      MAMeCab();
      ~MAMeCab();
      std::string getMAResult(std::string &str);
      std::string getRoman(std::string &str);
      std::string getYomi(std::string &str);
    };
  }
}

#endif
