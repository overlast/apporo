#include "apporo_ma.h"

using namespace std;
using namespace apporo;
using namespace apporo::ma;

MA::MA(string &parser_name_)
  : parser_name(parser_name_) {
  mai = new MAMeCab();
  return;
}

MA::~MA() {
  delete mai;
  return;
}

string MA::getParserName(void) {
  return parser_name;
}

string MA::getYomi(string &str) {
  string res = mai->getYomi(str);
  return res;
}
