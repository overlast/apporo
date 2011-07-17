#include <string>
#include <fstream>
#include <iostream>
#include "apporo_config.h"

using namespace std;
using namespace apporo;
using namespace apporo::config;

ConfigFile::ConfigFile(std::string &file_path_) 
  : file_path(file_path_) {
  this->readConfig();
  return;
}
ConfigFile::~ConfigFile() { return; };

void ConfigFile::readConfig() {
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
  return;
};

bool ConfigFile::isThere(string key) {
  bool res = false;
  if (conf.find(key) != conf.end()) { res = true; }
  return res;
}


