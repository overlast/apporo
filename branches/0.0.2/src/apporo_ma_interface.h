#ifndef APPORO_MA_INTERFACE_H
#define APPORO_MA_INTERFACE_H

#include <string>

namespace apporo {
  namespace ma {
    class MAInterface {
    public:
      MAInterface() { return; }
      virtual ~MAInterface() { return; }
      virtual std::string getMAResult(std::string &str)=0;
      virtual std::string getRoman(std::string &str)=0;
      virtual std::string getYomi(std::string &str)=0;
    };
  }
}

#endif
