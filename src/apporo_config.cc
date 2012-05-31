#include <string>
#include <fstream>
#include <iostream>
#include "apporo_util.h"
#include "apporo_config.h"

using namespace std;
using namespace apporo;
using namespace apporo::util;
using namespace apporo::config;

ConfigFile::ConfigFile(std::string &file_path_)
  : file_path(file_path_) {
  this->readConfig();
  return;
}
ConfigFile::~ConfigFile() { return; };

void ConfigFile::readConfig() {
  
  try {
    if (isFileExist(file_path)) {}
    else {
      throw "Can't find your config file. You must set filepath of Apporo's config file.";
    }
    ifstream ifs;
    ifs.open((file_path).c_str());
    string key;
    string val;
    while( !ifs.eof() ) {
      ifs >> key;
      ifs >> val;
      if (conf.find(key) != conf.end()) {}
      else { conf[key] = val; }
      //cout << key << ":" << val << endl;
    }
  }
  catch (const char* str) {
    showMessage(str);
    exit(1);
  }
  return;
};

bool ConfigFile::isThere(string key) {
  bool res = false;
  if (conf.find(key) != conf.end()) { res = true; }
  return res;
}

namespace apporo {
  namespace config {
    template <> bool ConfigFile::getValue<bool>(std::string key) {
      bool res;
      std::string tmp = conf[key];
      if (tmp == "true") {
	res = true;
      }
      else {
	res = false;
      }
      return res;
    }
  }
}
