#include "apporo_util.h"

using namespace std;
using namespace apporo;
using namespace apporo::util;
using namespace apporo::query;
using namespace apporo::strdistance;

namespace apporo {
namespace util {
string getFirstColumn(string entry, char boundary) {
  string res = "";
  int entry_len = entry.size();
  for (int i = 0; i < entry_len; i++) {
    if (entry[i] == boundary) {
      res = entry.substr(0, i);
      break;
    }
  }
  if (res == "") { res = entry; }
  return res;
}

void showResult(vector < pair <double, string> > &result, NgramQuery *nq, StringDistance *strdist, int result_num) {
  int result_list_len = result.size();
  for (int i = 0; i < result_list_len; i++) {
    if ((result_num > 0) && (i >= result_num)) { break; }
    cout << i << ":" << result[i].first << ":" << result[i].second << endl;
  }
  return;
}

void showMessage(string mes) {
  cout << mes << endl;
  return;
}

bool isFileExist(string path) {
  bool is_exist = false;
  ifstream in_file(path.c_str(), ios::in);
  if (in_file.rdstate() && ifstream::failbit) {
  }
  else {
    is_exist = true;
  }
  return is_exist;
}


}
}
