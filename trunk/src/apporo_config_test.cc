#include <string>
#include <iostream>
#include <vector>

#include "apporo.h"
#include "apporo_config.h"

using namespace std;
using namespace apporo;
using namespace apporo::config;

int main(int argc, char** argv) {
  int res = 0;
  try {
    string config_file_path(argv[1]);
    string query(argv[2]);
    Apporo *apporo = new Apporo(config_file_path);
    vector <string> result = apporo->approximate_match(query);
    for (int i = 0; i < (int)result.size(); i++) {
      cout << result[i] << endl;
    }
    delete apporo;
  } catch (const char *err) {
    cerr << err << endl;
    res = 1;
  }
  return res;
}
