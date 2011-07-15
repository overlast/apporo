#include "apporo_util.h"

using namespace std;
using namespace apporo;
using namespace apporo::util;
using namespace apporo::query;
using namespace apporo::strdistance;

namespace apporo {
  namespace util {
    string getFirstColumn(string entry, char boundary) {
      string res;
      int entry_len = entry.size();
      for (int i = 0; i < entry_len; i++) {
	if (entry[i] == boundary) {
	  res = entry.substr(0, i);
	}
      }
      return res;
    }
    
    void showResult(vector < pair <double, string> > &result, NgramQuery *nq, StringDistance *strdist, int result_num) {
      int result_list_len = result.size();
      for (int i = 0; i < result_list_len; i++) {
	if ((result_num > 0) && (i >= result_num)) { break; }
	if ((nq != NULL) && (nq->dist_threshold > 0.0)) {
	  if (result[i].first < nq->dist_threshold) { continue;}
	}
	cout << i << ":" << result[i].first << ":" << result[i].second << endl;
      }
      return;
    }
  }
}
