#include <iostream>
#include <string>
#include "apporo_tsubomi_db.h"

using namespace std;
using namespace apporo;
using namespace apporo::storage;

void show_usage(void) {
  cout << "----" << endl;
  cout << "mk_didindex - indexer for making did index" << endl;
  cout << "./mk_didindex -i FILE_PATH -s BOUNDARY_CHAR " << endl;
  cout << endl;
  cout << "Options" << endl;
  cout << "-i FILE_PATH : path of a file that you want to index" << endl;
  cout << "-b BOUNDARY_CHAR : boundary char of a each entry" << endl;
  cout << "----" << endl;
  return;
}

int main (int argc, char** argv) {
  int res = 0;
  try {
    string index_path;
    string boundary = "\n";
    char param = '\0';
    bool is_did = false;
    bool is_utf8 = false;
    bool is_help         = false;
    bool is_progress     = true;
    bool is_makeonly     = false;
    bool is_sortonly     = false;
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        if (argv[i][1] == '\0') { continue; }
        switch (argv[i][1]) {
        case 'd': is_did = true;  break;
        case 'u': is_utf8 = true; break;
        case 'p': is_progress = true;       break;
        case 'm': is_makeonly = true;       break;
        case 's': is_sortonly = true;       break;
        case 'h': is_help     = true;       break;
        case 'b':
          switch (argv[i][2]) {
          case 'l': boundary = "\n"; break;
          case 'c': boundary = "\t\n"; break;
          case 'w': boundary = " \t\n"; break;
          case 't': boundary = "\t"; break;
          }
        default: param = argv[i][1]; break;
        }
      }
      else if (param) {
        switch (param) {
        case 'o': cout << "ooo" << cout; ;break;
        case 'i': index_path = argv[i]; ;break;
        }
        param = '\0';
      }
      else {
        show_usage();
      }
    }

    if (index_path[0] == '\0' || is_help) {
      show_usage();
    }
    else {
      TsubomiDBIndex tdb(index_path);
      if (is_did) {
        tdb.makeDIDIndex(boundary);
      }
      else {
        // make aryfile and write
        bool is_make = true;
        if (is_sortonly) { is_make = false; }
        bool is_sort = true;
        if (is_makeonly) { is_sort = false; }
        tdb.mkary(boundary.c_str(), is_utf8, is_progress, is_make, is_sort);
      }
    }
  }
  catch (const char *err) {
    cerr << err << endl;
    res = 1;
  }
  return res;
}
