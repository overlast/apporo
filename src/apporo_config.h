#ifndef APPORO_CONFIG_H
#define APPORO_CONFIG_H

#include <string>
#include <tr1/unordered_map>
#include <sstream>


namespace apporo {
  namespace config {
    class ConfigFile {
    public:
      std::string file_path;
      std::tr1::unordered_map <std::string, std::string> conf;
      ConfigFile(std::string &file_path_);
      ~ConfigFile();
      void readConfig();
      bool isThere(std::string key);
      
      template <typename T> T getValue(std::string key) {
	T res;
	std::stringstream ss;
	std::string tmp = conf[key];
	ss << tmp;
	ss >> res;
	return res;
      }
    };
  }
}


#endif
