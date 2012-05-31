#include <string>
#include "apporo_query.h"
#include "apporo_strdistance.h"
#include "apporo_search.h"

using namespace std;
using namespace apporo::storage;
using namespace apporo::strdistance;
using namespace apporo::util;
using namespace apporo::search;
using namespace apporo::query;

int main(int argc, char** argv) {
  int res = 0;
  try {
    string query = "";
    int ngram_length = 2;
    bool is_pre = false;
    bool is_suf = false;
    bool is_utf8 = false;
    double dist_threshold = 0.0;
    string index_path = "";
    string dist_func = "dice";
    int entry_buf_len = 1024;
    string engine = "tsubomi";
    int result_num = 10;
    char param = '\0';
    int bucket_size = result_num * 200;
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        if (argv[i][1] == '\0') { continue; }
        switch (argv[i][1]) {
        case 'p': is_pre = true;       break;
        case 's': is_suf = true;       break;
        case 'u': is_utf8 = true; break;
          //case 'h': is_help  = true;       break;
        default: param = argv[i][1]; break;
        }
      }
      else if (param) {
        switch (param) {
        case 'i': index_path = argv[i]; break;
        case 'q': query = argv[i]; break;
        case 'n': ngram_length = atoi(argv[i]); break;
        case 't': dist_threshold = atof(argv[i]) ;break;
        case 'd': dist_func = argv[i] ;break;
        case 'e': engine = argv[i] ;break;
        case 'b': entry_buf_len = atoi(argv[i]) ;break;
        case 'r': result_num = atoi(argv[i]) ;break;
        case 'B': bucket_size = atoi(argv[i]) ;break;
        }
        param = '\0';
      }
    }
    
    if ((isFileExist(index_path)) && (query != "")) {
      // N-gram query を生成
      //cout << query << ":" << ngram_length << ":" << is_pre << ":" << is_suf << ":" << is_utf8 << ":" << dist_threshold << endl;
      NgramQuery *nq = new NgramQuery(query, ngram_length, is_pre, is_suf, is_utf8, dist_threshold); //engine に非依存
      //cout << dist_func << ":" <<  nq->ngram_length << ":" << nq->chars_num << ":" << dist_threshold << endl;
      // 閾値がある場合は枝刈り用のパラメタも生成
      StringDistance *strdist = new StringDistance(dist_func, nq->ngram_length, nq->chars_num, dist_threshold); //engine に非依存
      //cout << engine << ":" << index_path << ":" << entry_buf_len << endl;
      // 閾値がない場合はインスタンスができるだけ
      //StringDistance *strdist_no_threshold = new StringDistance(dist_func, ngram_length); //engine に非依存
      
      // 検索用のインスタンスを生成
      NgramSearch *ngram_searcher = new NgramSearch(engine, index_path, entry_buf_len); //engine に依存
      
      //int entry_num_par_page = 10;//Resultを作ったらつかえるな
      
      // 検索用のインスタンスで検索開始
      // Step1. N-gram query の token ごとに 集計済みDoc ID を取得
      // Step1-1. N-gram query の token ごとに SuffixArray 上の範囲を取得
      vector < pair<string, sa_range> > range_vector = ngram_searcher->getRange(nq); //engine に依存
      // Step1-2. 範囲から Doc ID を取得して集計
      vector <sa_index> index_vec(ngram_searcher->tdb->entry_num, 0);
      vector < pair <sa_index, int> > id_freq_vec = ngram_searcher->getIDMap(index_vec, range_vector, nq, strdist, bucket_size); //engine に依存
      //cout << result_num << ":" << bucket_size << endl;
      // Step2. Doc ID ごとにクエリとの類似度を測り、結果を出力する
      
      if (id_freq_vec.size() > 0) {
        vector < pair <double, string> > result = ngram_searcher->rerankAndGetResult(id_freq_vec, index_vec, nq, strdist, result_num, bucket_size); //engine に依存
        if (!result.empty()) {
          // 結果を出力する
          showResult(result, nq, strdist, result_num); //engine に非依存
        }
        else {
          showMessage("Not Found.");
          res = 0;
        }
      }
      else {
        showMessage("Not Found.");
        res = 0;
      }
      delete ngram_searcher;
      delete strdist;
      delete nq;
    }
    else {
      if (query != "") {
        showMessage("Can't find your index file. You must set filepath of indexed file.");
        res = 0;
      }
      else {
        showMessage("Query parameter is empty. You must input query.");
        res = 0;
      }
    }
  } catch (const char *err) {
    cerr << err << endl;
    res = 1;
  }
  return res;
}
