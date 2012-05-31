#include <vector>
#include <string>
#include <sstream>
#include "apporo.h"
#include "apporo_util.h"
#include "apporo_config.h"
#include "apporo_query.h"
#include "apporo_strdistance.h"
#include "apporo_search.h"
#include "apporo_tsubomi_db.h"

using namespace std;
using namespace apporo;
using namespace apporo::util;
using namespace apporo::config;
using namespace apporo::query;
using namespace apporo::strdistance;
using namespace apporo::search;
using namespace apporo::storage;

Apporo::Apporo(string &config_file_path_)
  : config_file_path(config_file_path_) {
  config = new ConfigFile(config_file_path);
  this->prepare();
  return;
}

Apporo::~Apporo() { 
  delete config;
  return;
}

void Apporo::prepare() {
  ngram_length = 2;
  is_pre = false;
  is_suf = false;
  is_utf8 = false;
  dist_threshold = 0.0;
  index_path = "";
  dist_func = "dice";
  entry_buf_len = 1024;
  engine = "tsubomi";
  result_num = 10;
  bucket_size = result_num * 200;
  is_surface = true;
  is_kana = false;
  is_roman = false;
  is_mecab = false;
  is_juman = false;
  is_kytea = false;
  
  if (config->isThere("ngram_length")) { ngram_length = config->getValue<int>("ngram_length"); }
  if (config->isThere("is_pre")) { is_pre = config->getValue<bool>("is_pre"); }
  if (config->isThere("is_suf")) { is_suf = config->getValue<bool>("is_suf"); }
  if (config->isThere("is_utf8")) { is_utf8 = config->getValue<bool>("is_utf8"); }
  if (config->isThere("dist_threshold")) { dist_threshold = config->getValue<double>("dist_threshold"); }
  if (config->isThere("index_path")) { index_path = config->getValue<string>("index_path"); }
  if (config->isThere("dist_func")) { dist_func = config->getValue<string>("dist_func"); }
  if (config->isThere("entry_buf_len")) { entry_buf_len = config->getValue<int>("entry_buf_len"); }
  if (config->isThere("engine")) { engine = config->getValue<string>("engine"); }
  if (config->isThere("result_num")) { result_num = config->getValue<int>("result_num"); }
  if (config->isThere("bucket_size")) { bucket_size = config->getValue<int>("bucket_size"); }
  if (config->isThere("is_surface")) { is_surface = config->getValue<bool>("is_surface"); }
  if (config->isThere("is_kana")) { is_kana = config->getValue<bool>("is_kana"); }
  if (config->isThere("is_roman")) { is_roman = config->getValue<bool>("is_roman"); }
  if (config->isThere("is_mecab")) { is_mecab = config->getValue<bool>("is_mecab"); }
  if (config->isThere("is_juman")) { is_juman = config->getValue<bool>("is_juman"); }
  if (config->isThere("is_kytea")) { is_kytea = config->getValue<bool>("is_kytea"); }
  
  if (isFileExist(index_path)) {}
  else {
    showMessage("Can't find your index file. You must set filepath of index config file.");
    exit(1);
  }

  if (is_mecab) {
  }
  
  return;
}

vector <string> Apporo::retrieve(string &query) {
  vector <string> res;
  if ((query.empty()) || (query == "")) {
  }
  else {
    NgramQuery *nq;
    if (is_kana) {
      string ma_query = query;
      nq = new NgramQuery(ma_query, ngram_length, is_pre, is_suf, is_utf8, dist_threshold);
    }
    else {
      nq = new NgramQuery(query, ngram_length, is_pre, is_suf, is_utf8, dist_threshold);
    }
    StringDistance *strdist = new StringDistance(dist_func, nq->ngram_length, nq->chars_num, dist_threshold);
    NgramSearch *ngram_searcher = new NgramSearch(engine, index_path, entry_buf_len);
    vector < pair<string, sa_range> > range_vector = ngram_searcher->getRange(nq);
    vector <sa_index> index_vec(ngram_searcher->tdb->entry_num, 0);
    vector < pair <sa_index, int> > id_freq_vec = ngram_searcher->getIDMap(index_vec, range_vector, nq, strdist, bucket_size);
    vector < pair <double, string> > result = ngram_searcher->rerankAndGetResult(id_freq_vec, index_vec, nq, strdist, result_num, bucket_size);
    
    if (!result.empty()) {
      for (int i = 0; i < (int)result.size(); i++) {
        std::stringstream ss;
        string tmp;
        if ((result_num > 0) && (i >= result_num)) { break; }
        ss << (result[i]).first;
        ss >> tmp;
        tmp += "\t" + (result[i]).second;
        res.push_back(tmp);
      }
    }
    delete ngram_searcher;
    delete strdist;
    delete nq;
  }
  return res;
}

