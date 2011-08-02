#ifndef APPORO_MA_INTERFACE_H
#define APPORO_MA_INTERFACE_H

#import <string>

namespace apporo {
  namespace ma {
    class MAInterface {
    public:
      MAInterface() {};
      ~MAInterface() {};
      virtual std::string getMAResult(std::string &str);
      virtual std::string getRoman(std::string &str);
      virtual std::string getYomi(std::string &str);
    };
    
  }
}

#endif
