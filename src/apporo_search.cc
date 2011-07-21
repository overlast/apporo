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

vector < pair <sa_index, int> > NgramSearch::getIDMap(map <sa_index, sa_index> &id_index_map, vector < pair<string, sa_range> > &range_vector, NgramQuery *nq, StringDistance *strdist) {
  int ngram_que_count = range_vector.size();
  int ngram_que_limit = ngram_que_count;
  vector < pair <sa_index, int> > dummy_id_freq_vec;
  vector < pair <sa_index, int> > &id_freq_vec = dummy_id_freq_vec;
  int id_freq_vec_count = id_freq_vec.size();
  vector < pair <sa_index, sa_index> > id_index_vec;
  if (nq->dist_threshold > 0.0) {
    ngram_que_limit -= strdist->search_threshold[nq->min_hit_num].first;
  }
  
  for (vector < pair<string, sa_range> >::iterator it = range_vector.begin(); it != range_vector.end(); it++) {
    pair <string, sa_range> item = (*it);
    vector <sa_index> id_bucket;
    int count = 0;
    sa_index prev_id = -1;
    for (sa_index i = (item.second).first; i <= (item.second).second; i++) {
      sa_index id = tdb->getDID(i);
      id_bucket.push_back(id);
      id_index_vec.push_back(pair<sa_index, sa_index>(id, i));
    }
    sort(id_bucket.begin(), id_bucket.end());
    count = 0;
    prev_id = id_bucket[0];
    for (vector < sa_index >::iterator idit = id_bucket.begin(); idit != id_bucket.end(); idit++) {
      if (prev_id != *idit) {
	id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
	count = 1;
	prev_id = *idit;
      }
      else { count++; }
    }
    if (count > 0) {
      id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
      count = 0;
    }

    if (nq->dist_threshold > 0.0) {
      //cout << ngram_que_limit << endl;
      if (ngram_que_limit <= 0) {
	sort(id_freq_vec.begin(), id_freq_vec.end(), pair_first_up_order());
	vector < pair <sa_index, int> > tmp_id_freq_vec;
	count = 0;
	prev_id = id_freq_vec[0].first;
	
	for (vector < pair <sa_index, int> >::iterator idit = id_freq_vec.begin(); idit != id_freq_vec.end(); idit++) {
	  if (prev_id != (*idit).first) {
	    if (count + ngram_que_count < strdist->search_threshold[nq->min_hit_num].first) {}
	    else { tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count)); }
	    count = (*idit).second;
	    prev_id = (*idit).first;
	  }
	  else { count += (*idit).second; }
	}    
	if (count > 0) {
	  if (count + ngram_que_count < strdist->search_threshold[nq->min_hit_num].first) {}
	  else { tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count)); }
	  count = 0;
	}
	id_freq_vec = tmp_id_freq_vec;
	int tmp_id_freq_vec_count = (int)id_freq_vec.size();
	//cout << tmp_id_freq_vec_count << endl;
	if (id_freq_vec_count <  tmp_id_freq_vec_count) { break; }
	else { id_freq_vec_count = tmp_id_freq_vec_count; }
      }
      ngram_que_count--;
      ngram_que_limit--;
    }
  }

  if (nq->dist_threshold > 0.0) {
    sort(id_freq_vec.begin(), id_freq_vec.end(), pair_first_up_order());
    vector < pair <sa_index, int> > tmp_id_freq_vec;
    int count = 0;
    sa_index prev_id = id_freq_vec[0].first;
    for (vector < pair <sa_index, int> >::iterator idit = id_freq_vec.begin(); idit != id_freq_vec.end(); idit++) {
      if (prev_id != (*idit).first) {
	tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
	count = (*idit).second;
	prev_id = (*idit).first;
      }
      else { count += (*idit).second; }
      //cout << (*i).first << ":" << (*i).second << endl;
    }
    if (count > 0) {
      tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
      count = 0;
    }
    id_freq_vec = tmp_id_freq_vec;
  }
  else {
    sort(id_freq_vec.begin(), id_freq_vec.end(), pair_first_up_order());
    vector < pair <sa_index, int> > tmp_id_freq_vec;
    int count = 0;
    sa_index prev_id = id_freq_vec[0].first;
    for (vector < pair <sa_index, int> >::iterator idit = id_freq_vec.begin(); idit != id_freq_vec.end(); idit++) {
      if (prev_id != (*idit).first) {
	tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
	count = (*idit).second;
	prev_id = (*idit).first;
      }
      else { count += (*idit).second; }
      //cout << (*i).first << ":" << (*i).second << endl;
    }
    if (count > 0) {
      tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
      count = 0;
    }
    id_freq_vec = tmp_id_freq_vec;
  }
  //cout << id_freq_vec.size() << endl;

  sort(id_index_vec.begin(), id_index_vec.end(), pair_first_up_order());
  sa_index tmp_sa_index = id_index_vec[0].first;
  sa_index tmp_id = id_index_vec[0].second;
  for (vector < pair <sa_index, sa_index> >::iterator it = id_index_vec.begin(); it != id_index_vec.end(); it++) {
    if (tmp_sa_index != (*it).first) {
      id_index_map[tmp_sa_index] = tmp_id;
      tmp_sa_index = (*it).first;
      tmp_id = (*it).second;
    }
  }

  sort(id_freq_vec.begin(), id_freq_vec.end(), pair_second_low_order());
  return id_freq_vec;
}

vector < pair <double, string> > NgramSearch::rerankAndGetResult(vector < pair <sa_index, int> > &id_freq_vec, map <sa_index, sa_index> &id_index_map, NgramQuery *nq, StringDistance *strdist, int result_num, int bucket_size) {
  int push_count = 0;
  vector < pair <double, string> > result;
  for (vector <pair <sa_index, int> >::iterator it = id_freq_vec.begin(); it != id_freq_vec.end(); it++) {
    if ((result_num > 0) && (push_count == bucket_size)) { break; } // どうする
    pair <sa_index, int> item = *it;
    //cout << item.first << '/' << item.second << endl;
    char tmp_buf[this->entry_buf_len];
    tmp_buf[0] = '\0';
    sa_index pos;
    this->tdb->get_line(id_index_map[item.first], tmp_buf, entry_buf_len, &pos);
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

