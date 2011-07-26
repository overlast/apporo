#include "apporo_search.h"
#include "apporo_profile.h"

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
    if (p.first < 0) { continue; }
    pair <string, sa_range> ngram_freq(query_str, p);
    ngram_vector.push_back(ngram_freq);
    //ngram_que_count++;
  }
  sort(ngram_vector.begin(), ngram_vector.end(), sa_range_up_order());
  return ngram_vector;
}


//vector < pair <sa_index, int> > NgramSearch::getIDMap(map <sa_index, sa_index> &id_index_map, vector < pair<string, sa_range> > &range_vector, NgramQuery *nq, StringDistance *strdist) {
vector < pair <sa_index, int> > NgramSearch::getIDMap(vector <sa_index> &index_vec, vector < pair<string, sa_range> > &range_vector, NgramQuery *nq, StringDistance *strdist) {
  int ngram_que_count = range_vector.size();
  int ngram_que_limit = ngram_que_count;
  vector < pair <sa_index, int> > dummy_id_freq_vec;
  vector < pair <sa_index, int> > &id_freq_vec = dummy_id_freq_vec;
  int id_freq_vec_count = id_freq_vec.size();
  int count = 0;
  vector <sa_index> tmp_id_bucket;
  vector <sa_index> &id_bucket = tmp_id_bucket;
  if (nq->dist_threshold > 0.0) {
    ngram_que_limit -= strdist->search_threshold[nq->min_hit_num].first;
    for (vector < pair<string, sa_range> >::iterator bi = range_vector.begin(), ei = range_vector.end(); bi != ei; ++bi) {
      pair <string, sa_range> item = (*bi);
      for (sa_index i = (item.second).first, e = (item.second).second; i <= e; i++) {
	sa_index id = tdb->getDID(i);
	id_bucket.push_back(id);
	index_vec[id] = i;
      }
      
      if (ngram_que_limit <= 0) {
	vector <sa_index> null_id_bucket;
	sort(id_bucket.begin(), id_bucket.end());
	count = 0;
	sa_index *prev_id_p = &id_bucket[0];
	for (vector < sa_index >::iterator bit = id_bucket.begin(), eit = id_bucket.end(); bit != eit; ++bit) {
	  if (*prev_id_p != *bit) {
	    id_freq_vec.push_back(pair<sa_index, int>(*prev_id_p, count));
	    count = 1;
	    prev_id_p = &(*bit);
	  }
	  else { count++; }
	}
	if (count > 0) {
	  id_freq_vec.push_back(pair<sa_index, int>(*prev_id_p, count));
	  count = 0;
	}
	
	id_bucket = null_id_bucket;

	sort(id_freq_vec.begin(), id_freq_vec.end(), pair_first_up_order());
	vector < pair <sa_index, int> > tmp_id_freq_vec;
	count = 0;
	prev_id_p = &(id_freq_vec[0].first);
	for (vector < pair <sa_index, int> >::iterator bit = id_freq_vec.begin(), eit = id_freq_vec.end(); bit != eit; ++bit) {
	  if (*prev_id_p != (*bit).first) {
	    if (count + ngram_que_count < strdist->search_threshold[nq->min_hit_num].first) {}
	    else { tmp_id_freq_vec.push_back(pair<sa_index, int>(*prev_id_p, count)); }
	    count = (*bit).second;
	    prev_id_p = &((*bit).first);
	  }
	  else { count += (*bit).second; }
	}
	if (count > 0) {
	  if (count + ngram_que_count < strdist->search_threshold[nq->min_hit_num].first) {}
	  else { tmp_id_freq_vec.push_back(pair<sa_index, int>(*prev_id_p, count)); }
	  count = 0;
	}
	id_freq_vec = tmp_id_freq_vec;
	int tmp_id_freq_vec_count = (int)id_freq_vec.size();
	//cout << id_freq_vec_count << ":" << tmp_id_freq_vec_count << endl;
	if (id_freq_vec_count >= tmp_id_freq_vec_count) { break; }
	else { id_freq_vec_count = tmp_id_freq_vec_count; }
      }
      else {
	id_freq_vec_count = (int)id_freq_vec.size();
      }
	
      ngram_que_count--;
      ngram_que_limit--;
    }

    if (ngram_que_limit < 0) {
    }
    else {
      sort(id_bucket.begin(), id_bucket.end());
      count = 0;
      sa_index *prev_id_p = &(id_bucket[0]);
      for (vector <sa_index>::iterator bi = id_bucket.begin(), ei = id_bucket.end(); bi != ei; ++bi) {
	if (*prev_id_p != (*bi)) {
	  id_freq_vec.push_back(pair<sa_index, int>(*prev_id_p, count));
	  count = 1;
	  prev_id_p = &(*bi);
	}
	else { count++; }
      }
      if (count > 0) {
	id_freq_vec.push_back(pair<sa_index, int>(*prev_id_p, count));
	count = 0;
      }
    }
    vector< pair <sa_index, int> >::iterator id_freq_vec_end_it;
    vector< pair <sa_index, int> >::iterator id_delete_it = remove_if(id_freq_vec.begin(), id_freq_vec_end_it, DeleteID(strdist->search_threshold[nq->min_hit_num].second));
    if (id_delete_it != id_freq_vec_end_it) {
      id_freq_vec.erase(id_delete_it, id_freq_vec_end_it);
    }
  }
  else {
    for (vector < pair<string, sa_range> >::iterator bi = range_vector.begin(), ei = range_vector.end(); bi != ei; ++bi) {
      pair <string, sa_range> item = (*bi);
      for (sa_index i = (item.second).first, e = (item.second).second; i <= e; i++) {
	sa_index id = tdb->getDID(i);
	id_bucket.push_back(id);
	index_vec[id] = i;
      }
    }
    sort(id_bucket.begin(), id_bucket.end());
    count = 0;
    sa_index *prev_id_p = &id_bucket[0];
    for (vector<sa_index>::iterator bi = id_bucket.begin(), ei = id_bucket.end(); bi != ei; ++bi) {
      if (*prev_id_p != (*bi)) {
	id_freq_vec.push_back(pair<sa_index, int>(*prev_id_p, count));
	count = 1;
	prev_id_p = &(*bi);
      }
      else { count++; }
    }
    if (count > 0) {
      id_freq_vec.push_back(pair<sa_index, int>(*prev_id_p, count));
      count = 0;
    }
  }
  sort(id_freq_vec.begin(), id_freq_vec.end(), pair_second_low_order());
  return id_freq_vec;
}

vector < pair <double, string> > NgramSearch::rerankAndGetResult(vector < pair <sa_index, int> > &id_freq_vec, vector <sa_index> &index_vec, NgramQuery *nq, StringDistance *strdist, int result_num, int bucket_size) {
  int push_count = 0;
  vector < pair <double, string> > result;
  for (vector <pair <sa_index, int> >::iterator bi = id_freq_vec.begin(), ei = id_freq_vec.end(); bi != ei; ++bi) {
    if ((result_num > 0) && (push_count == bucket_size)) { break; } // どうする
    pair <sa_index, int> item = *bi;
    //cout << item.first << '/' << item.second << endl;
    char tmp_buf[this->entry_buf_len];
    tmp_buf[0] = '\0';
    sa_index pos;
    this->tdb->get_line(index_vec[item.first], tmp_buf, entry_buf_len, &pos);
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
