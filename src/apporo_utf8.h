#ifndef APPORO_UTF8_H
#define APPORO_UTF8_H

#include <string>
#include <vector>

namespace apporo {
  namespace utf8 {
    class UTF8String {
    public:
      UTF8String() {};
      ~UTF8String() {};
    };
    std::vector <int> getUTF8Width(std::string &str);
  }
}

#endif
