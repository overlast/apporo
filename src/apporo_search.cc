#include "apporo_search.h"

using namespace std;
using namespace apporo;
using namespace apporo::strage;
using namespace apporo::strdistance;
using namespace apporo::util;
using namespace apporo::search;
using namespace apporo::query;

NgramSearch::NgramSearch (string &engine_, string &index_path_, int entry_buf_len_) 
  : engine(engine_), index_path(index_path_), entry_buf_len(entry_buf_len_) {
  if (engine == "tsubomi") {
    tdb = new TsubomiDBSearch(index_path);
  }
  return;
}
NgramSearch::~NgramSearch () { return; }

vector < pair<string, sa_range> > NgramSearch::getRange(NgramQuery *nq) {
  vector < pair<string, sa_range> > ngram_vector;
  for (set <string>::iterator it = nq->tokens.begin(); it != nq->tokens.end(); it++) {
    string query_str = *it;
    sa_range p = this->tdb->search(query_str.c_str());
    //cout << query_str  << ":" << p.first << ":" << p.second << endl;
    if (p.first < 0) { continue; }
    pair <string, sa_range> ngram_freq(query_str, p);
    ngram_vector.push_back(ngram_freq);
    //ngram_que_count++;
  }
  sort(ngram_vector.begin(), ngram_vector.end(), sa_range_up_order());
  return ngram_vector;
}

vector < pair <sa_index, int> > NgramSearch::getIDMap(tr1::unordered_map <sa_index, sa_index> &id_index_map, vector < pair<string, sa_range> > &range_vector, NgramQuery *nq, StringDistance *strdist) {
  int ngram_que_count = range_vector.size();
  int ngram_que_limit = ngram_que_count;
  vector < pair <sa_index, int> > id_freq_vec;
  tr1::unordered_map <sa_index, int> id_map;
  int id_map_count = id_map.size();
  if (nq->dist_threshold > 0.0) {
    ngram_que_limit -= strdist->search_threshold[nq->min_hit_num].first;
  }
  for (vector < pair<string, sa_range> >::iterator it = range_vector.begin(); it != range_vector.end(); it++) {
    pair <string, sa_range> item = (*it);
    //string query_str = item.first;
    //sa_range p = item.second;
    for (sa_index i = (item.second).first; i <= (item.second).second; i++) {
      sa_index id = 1;//tdb->getDID(i);
      if ((nq->dist_threshold > 0.0) && (ngram_que_limit <= 0)) {
	if (id_map.find(id) != id_map.end()) {
	  if (id_map[id] + ngram_que_count < strdist->search_threshold[nq->min_hit_num].first) {
	    id_map.erase(id);
	    continue;
	  }
	}
	else if (ngram_que_count < strdist->search_threshold[nq->min_hit_num].first) { continue; }
      }
      id_map[id]++;
      if (id_index_map.find(id) != id_index_map.end()) {}
      else { id_index_map[id] = i; }
    }
    ngram_que_count--;
    ngram_que_limit--;
    if (nq->dist_threshold > 0.0) {
      if ((ngram_que_limit <= 0) && (id_map_count == (int)id_map.size())) { break; }
      else { id_map_count = id_map.size(); }
    }
  }

  for ( tr1::unordered_map <sa_index, int>::iterator i = id_map.begin(); i != id_map.end(); i++ ) {
    if ((nq->dist_threshold > 0.0) && (strdist->search_threshold[nq->min_hit_num].second > 0))  {
      if ((*i).second < strdist->search_threshold[nq->min_hit_num].second) {continue; }
    }
    id_freq_vec.push_back(*i);
    //cout << (*i).first << ":" << (*i).second << endl;
  }
  //cout << id_freq_vec.size() << endl;
  sort(id_freq_vec.begin(), id_freq_vec.end(), pair_second_low_order());
  return id_freq_vec;
}

vector < pair <double, string> > NgramSearch::rerankAndGetResult(vector < pair <sa_index, int> > &id_freq_vec, tr1::unordered_map <sa_index, sa_index> &id_index_map, NgramQuery *nq, StringDistance *strdist, int result_num, int bucket_size) {
  int push_count = 0;
  vector < pair <double, string> > result;
  for (vector < pair <sa_index, int> >::iterator it = id_freq_vec.begin(); it != id_freq_vec.end(); it++) {
    if ((result_num > 0) && (push_count == bucket_size)) { break; } // どうする
    pair <sa_index, int> item = *it;
    //cout << item.first << '/' << item.second << endl;
    char tmp_buf[this->entry_buf_len];
    tmp_buf[0] = '\0';
    sa_index pos;
    this->tdb->get_line(id_index_map[item.first], tmp_buf, entry_buf_len, &pos);
    //cout << "id:" << item.first << ":" << id_index_map[item.first] << "pos:"<< pos <<endl;
    //cout << tmp_buf << endl;
    string entry_str = getFirstColumn(tmp_buf, '\t');
    //cout << entry_str << endl;
    double dist = strdist->getStringDistance(strdist->dist_func, nq->query, entry_str, nq->utf8_boundary);
    if (dist > 0.0) {
      result.push_back(pair<double, string>(dist, (string)(tmp_buf[0] != '\0' ? tmp_buf : "")));
      push_count++; // 足して良い時と悪い時がある
    }
  }
  sort(result.begin(), result.end(), pair_first_low_order());
  return result;
}

