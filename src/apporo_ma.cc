#include "apporo_ma.h"
#include "apporo_ma_mecab.h"

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

string MA::getMAName(void) {
  return ma->getParserName();
}
