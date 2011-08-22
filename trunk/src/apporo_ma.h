#ifndef APPORO_MA_H
#define APPORO_MA_H

#import <string>
#import "apporo_ma_interface.h"

namespace apporo {
  namespace ma {
    class MA {
    public:
      MA(std::string &ma);
      ~MA();
      std::string parser_name;
      MAInterface *mai;
      std::string getParserName(void);
    };
}

#endif
