#include "apporo_search.h"

using namespace std;
using namespace apporo;
using namespace apporo::storage;
using namespace apporo::strdistance;
using namespace apporo::util;
using namespace apporo::search;
using namespace apporo::query;

DeleteID::DeleteID(int num){ threshold = num; return; }
DeleteID::~DeleteID() { return; }

NgramSearch::NgramSearch (string &engine_, string &index_path_, int entry_buf_len_) 
  : engine(engine_), index_path(index_path_), entry_buf_len(entry_buf_len_) {
  if (engine == "tsubomi") {
    tdb = new TsubomiDBSearch(index_path);
  }
  return;
}
NgramSearch::~NgramSearch () {
  delete tdb;
  return;
}

vector < pair<string, sa_range> > NgramSearch::getRange(NgramQuery *nq) {
  vector < pair<string, sa_range> > ngram_vector;
  for (set <string>::iterator bi = nq->tokens.begin(); bi != nq->tokens.end(); ++bi) {
    string query_str = *bi;
    sa_range p = this->tdb->search(query_str.c_str());
    //cout << query_str  << ":" << p.first << ":" << p.second << endl;
    if (p.second >= 0) { // if result is found
      pair <string, sa_range> ngram_freq(query_str, p);
      ngram_vector.push_back(ngram_freq);
      //ngram_que_count++;
    }
  }
  sort(ngram_vector.begin(), ngram_vector.end(), sa_range_up_order());
  return ngram_vector;
}

vector < pair <sa_index, int> > NgramSearch::getIDMap(vector <sa_index> &index_vec, vector < pair<string, sa_range> > &range_vector, NgramQuery *nq, StringDistance *strdist, int bucket_size) {
  int ngram_que_count = range_vector.size();
  int ngram_que_limit = ngram_que_count;
  vector < pair <sa_index, int> > id_freq_vec;
  int id_freq_vec_count = id_freq_vec.size();
  int count = 0;
  vector <sa_index> tmp_id_bucket;
  vector <sa_index> id_count(index_vec.size(), 0);
  if (nq->dist_threshold > 0.0) {
    ngram_que_limit -= strdist->search_threshold[nq->min_hit_num].first;
    for (vector < pair<string, sa_range> >::iterator bi = range_vector.begin(), ei = range_vector.end(); bi != ei; ++bi) {
      pair <string, sa_range> item = (*bi);
      if ((item.second).first < 0) { continue; }
      for (sa_index i = (item.second).first, e = (item.second).second; i <= e; i++) {
        sa_index id = tdb->getDID(i);
        index_vec[id] = i;
        id_count[id]++;
        count++;
      }
      
      if (ngram_que_limit <= 0) {
        if (count > 0) {
          for (size_t i = 0; i < index_vec.size(); i++) {
            if ((id_count[i] != 0) && (id_count[i] + ngram_que_count < strdist->search_threshold[nq->min_hit_num].first)) {
              id_count[i] = 0;
              count--;
            }
            else {
              if (id_count[i] != 0) { }
            }
          }
        }
        int tmp_id_freq_vec_count = count;
        if (id_freq_vec_count >= tmp_id_freq_vec_count) { break; }
        else { id_freq_vec_count = tmp_id_freq_vec_count; }
      }
      else {
        id_freq_vec_count = count;
      }
      ngram_que_count--;
      ngram_que_limit--;
    }
    
    if (count > 0) {
      for (size_t i = 0; i < index_vec.size(); i++) {
        if (id_count[i] > 0) {
          id_freq_vec.push_back(pair<sa_index, int>(i, id_count[i]));
        }
      }
    }
    
    //cout << "size: " << id_freq_vec.size() << endl;
    vector< pair <sa_index, int> >::iterator id_freq_vec_end_it;
    vector< pair <sa_index, int> >::iterator id_delete_it = remove_if(id_freq_vec.begin(), id_freq_vec_end_it, DeleteID(strdist->search_threshold[nq->min_hit_num].second));
    if (id_delete_it != id_freq_vec_end_it) {
      //cout << "kesu" << endl;
      id_freq_vec.erase(id_delete_it, id_freq_vec_end_it);
    }
  }
  else {
    count = 0;
    for (vector < pair<string, sa_range> >::iterator bi = range_vector.begin(), ei = range_vector.end(); bi != ei; ++bi) {
      pair <string, sa_range> item = (*bi);
      for (sa_index i = (item.second).first, e = (item.second).second; i <= e; i++) {
        sa_index id = tdb->getDID(i);
        index_vec[id] = i;
        id_count[id]++;
        count++;
      }
    }
    if (count > 0) {
      for (size_t i = 0; i < index_vec.size(); i++) {
        if (id_count[i] > 0) {
          id_freq_vec.push_back(pair<sa_index, int>(i, id_count[i]));
        }
      }
      count = 0;
    }
  }
  
  if (id_freq_vec.size() > 0) {
    if ((0 != bucket_size) && ((int)id_freq_vec.size() > bucket_size)) {
      partial_sort(id_freq_vec.begin(), id_freq_vec.begin() + bucket_size, id_freq_vec.end(), pair_second_low_order());
    }
    else {
      sort(id_freq_vec.begin(), id_freq_vec.end(), pair_second_low_order());
    }
  }
  return id_freq_vec;
}

vector < pair <double, string> > NgramSearch::rerankAndGetResult(vector < pair <sa_index, int> > &id_freq_vec, vector <sa_index> &index_vec, NgramQuery *nq, StringDistance *strdist, int result_num, int bucket_size) {
  int push_count = 0;
  vector < pair <double, string> > result;
  for (vector <pair <sa_index, int> >::iterator bi = id_freq_vec.begin(), ei = id_freq_vec.end(); bi != ei; ++bi) {
    if ((result_num > 0) && (push_count == bucket_size)) { break; } // どうする
    pair <sa_index, int> item = *bi;
    char tmp_buf[this->entry_buf_len];
    tmp_buf[0] = '\0';
    sa_index pos;
    this->tdb->get_line(index_vec[item.first], tmp_buf, entry_buf_len, &pos);
    string entry_str = getFirstColumn(tmp_buf, '\t');
    
    double dist = strdist->getStringDistance(strdist->dist_func, nq->query, entry_str, nq->utf8_boundary);
    if (dist > 0.0) {
      if ((nq != NULL) && (nq->dist_threshold > 0.0)) {
        if (dist < nq->dist_threshold) { continue;}
      }
      result.push_back(pair<double, string>(dist, (string)(tmp_buf[0] != '\0' ? tmp_buf : "")));
      push_count++; // 足して良い時と悪い時がある
    }
  }
  if (push_count > 0) {
    if ((0 != result_num) && (push_count > result_num)) {
      partial_sort(result.begin(), result.begin() + result_num, result.end(), pair_first_low_order());
    }
    else { sort(result.begin(), result.end(), pair_first_low_order()); }
  }
  return result;
}
