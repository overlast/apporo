#ifndef APPORO_H
#define APPORO_Ha

#include <vector>
#include <string>
#include "apporo_config.h"

namespace apporo {
  class Apporo {
  public:
    int ngram_length;
    bool is_pre;
    bool is_suf;
    bool is_utf8;
    double dist_threshold;
    std::string index_path;
    std::string dist_func;
    int entry_buf_len;
    std::string engine;
    int result_num;
    int bucket_size;
    bool is_surface;
    bool is_kana;
    bool is_roman;
    bool is_mecab;
    bool is_juman;
    bool is_kytea;

    Apporo(std::string &file);
    ~Apporo();
    std::string config_file_path;
    apporo::config::ConfigFile *config;
    void prepare();
    std::vector <std::string> approximate_match(std::string &query);
  };
}

#endif
