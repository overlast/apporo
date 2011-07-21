#include "apporo_search.h"
#include "apporo_profile.h"

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

/*
//vector < pair <sa_index, int> > NgramSearch::getIDMap(map <sa_index, sa_index> &id_index_map, vector < pair<string, sa_range> > &range_vector, NgramQuery *nq, StringDistance *strdist) {
vector < pair < pair <sa_index, int>, sa_index > > NgramSearch::getIDMap(vector < pair<string, sa_range> > &range_vector, NgramQuery *nq, StringDistance *strdist) {
  int ngram_que_count = range_vector.size();
  int ngram_que_limit = ngram_que_count;
  //vector < pair <sa_index, int> > dummy_id_freq_vec;
  //vector < pair <sa_index, int> > &id_freq_vec = dummy_id_freq_vec;
  
  vector < pair <pair <sa_index, int>, sa_index > > dummy_id_freq_ind_vec;
  vector < pair <pair <sa_index, int>, sa_index > > &id_freq_ind_vec = dummy_id_freq_ind_vec;
  int id_freq_vec_ind_count = id_freq_ind_vec.size();
  vector < pair <sa_index, sa_index> > id_index_vec;
  if (nq->dist_threshold > 0.0) {
    ngram_que_limit -= strdist->search_threshold[nq->min_hit_num].first;
  }
  

  for (vector < pair<string, sa_range> >::iterator it = range_vector.begin(); it != range_vector.end(); it++) {
    pair <string, sa_range> item = (*it);
    
    double t1 = getUsageSec();
    for (sa_index i = (item.second).first; i <= (item.second).second; i++) {
      sa_index id = tdb->getDID(i);
      id_index_vec.push_back(pair<sa_index, sa_index>(id, i));
    }
    double t2 = getUsageSec();
    sort(id_index_vec.begin(), id_index_vec.end(), pair_first_up_order());

    printTime(t1,t2);
  
    int count = 0;
    sa_index prev_id = id_index_vec[0].first;
    sa_index prev_po = id_index_vec[0].second;
    for (vector < pair< sa_index, sa_index> >::iterator idit = id_index_vec.begin(); idit != id_index_vec.end(); idit++) {
      if (prev_id != (*idit).first) {
	id_freq_ind_vec.push_back(pair< pair<sa_index, int>, sa_index>(pair<sa_index, int>(prev_id, count), prev_po));
	count = 1;
	prev_id = (*idit).first;
	prev_po = (*idit).second;
      }
      else { count++; }
    }
    if (count > 0) {
      id_freq_ind_vec.push_back(pair< pair<sa_index, int>, sa_index>(pair<sa_index, int>(prev_id, count), prev_po));
      count = 0;
    }
    //cout << id_freq_ind_vec.size() <<endl;
    */    
    /*
    if (nq->dist_threshold > 0.0) {
      //cout << ngram_que_limit << endl;
      if (ngram_que_limit <= 0) {
	sort(id_freq_vec.begin(), id_freq_vec.end(), pair_first_up_order());
	vector < pair <sa_index, int> > tmp_id_freq_vec;
	count = 0;
	prev_id = id_freq_vec[0].first;
	
	for (vector < pair <sa_index, int> >::iterator idmit = id_freq_vec.begin(); idmit != id_freq_vec.end(); idmit++) {
	  if (prev_id != (*idmit).first) {
	    if (count + ngram_que_count < strdist->search_threshold[nq->min_hit_num].first) {}
	    else { tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count)); }
	    count = (*idmit).second;
	    prev_id = (*idmit).first;
	  }
	  else { count += (*idmit).second; }
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
    */
/*
  }
*/
  /*
  if (nq->dist_threshold > 0.0) {
    sort(id_freq_vec.begin(), id_freq_vec.end(), pair_first_up_order());
    vector < pair <sa_index, int> > tmp_id_freq_vec;
    int count = 0;
    sa_index prev_id = id_freq_vec[0].first;
    for (vector < pair <sa_index, int> >::iterator idfit = id_freq_vec.begin(); idfit != id_freq_vec.end(); idfit++) {
      if (prev_id != (*idfit).first) {
	
	//if (strdist->dist_func == "dist") {
	//  bool is_next = false;
	 // if (strdist->search_threshold[nq->min_hit_num].second > 0)  {
	  //  if (count < strdist->search_threshold[nq->min_hit_num].second) { is_next = true; }
	 // }
	  if (!is_next) {
	
	tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
	
	//  }
	//}
	count = (*idfit).second;
	prev_id = (*idfit).first;
      }
      else { count += (*idfit).second; }
      //cout << (*i).first << ":" << (*i).second << endl;
    }
    if (count > 0) {
      
      //bool is_next = false;
      //if (strdist->search_threshold[nq->min_hit_num].second > 0)  {
      //if (count < strdist->search_threshold[nq->min_hit_num].second) { is_next = true; }
      //}
      //if (!is_next) {
      
      tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
      
      //}
      
      count = 0;
    }
    id_freq_vec = tmp_id_freq_vec;
  }
  else {
  
    sort(id_freq_ind_vec.begin(), id_freq_ind_vec.end(), pair_first_pair_first_up_order());
    vector < pair < pair <sa_index, int>, sa_index> > tmp_id_freq_ind_vec;
    int count = 0;
    sa_index prev_id = (id_freq_ind_vec[0].first).first;
    sa_index prev_po = id_freq_ind_vec[0].second;
    for (vector < pair < pair <sa_index, int>, sa_index> >::iterator idfit = id_freq_ind_vec.begin(); idfit != id_freq_ind_vec.end(); idfit++) {
      if (prev_id != ((*idfit).first).first) {
	tmp_id_freq_ind_vec.push_back(pair < pair <sa_index, int>, sa_index>(pair<sa_index, int>(prev_id, count), prev_po));
	//cout << prev_id <<":"<< count <<":"<<  prev_po <<":"<< endl;
	count = ((*idfit).first).second;
	prev_id = ((*idfit).first).first;
	prev_po = (*idfit).second;
      }
      else { count += ((*idfit).first).second; }
      //cout << (*i).first << ":" << (*i).second << endl;
    }    
    if (count > 0) {
      tmp_id_freq_ind_vec.push_back(pair < pair <sa_index, int>, sa_index>(pair<sa_index, int>(prev_id, count), prev_po));
      //tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
      count = 0;
    }
    id_freq_ind_vec = tmp_id_freq_ind_vec;
    
// }
    
  //cout << id_freq_vec.size() << endl;
  
//  sort(id_index_vec.begin(), id_index_vec.end(), pair_first_up_order());
 // sa_index tmp_sa_index = id_index_vec[0].first;
 // sa_index tmp_id = id_index_vec[0].second;
  //for (vector < pair <sa_index, sa_index> >::iterator it = id_index_vec.begin(); it != id_index_vec.end(); it++) {
   // if (tmp_sa_index != (*it).first) {
    //  id_index_map[tmp_sa_index] = tmp_id;
     // tmp_sa_index = (*it).first;
    //  tmp_id = (*it).second;
   // }
 // }
  

  sort(id_freq_ind_vec.begin(), id_freq_ind_vec.end(), pair_first_pair_second_low_order());
  return id_freq_ind_vec;
}
*/


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
    //vector < pair <sa_index, sa_index> > &id_freq_bucket;
    int count = 0;
    sa_index prev_id = -1;
    //string query_str = item.first;
    //sa_range p = item.second;
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

    
    // for pthread
    //for (vector < pair <sa_index, int> >::iterator idfit = id_freq_bucket.begin(); idfit != id_freq_bucket.end(); idfit++) {
    //  id_freq_vec.push_back(*idfit);
    //}
    
    if (nq->dist_threshold > 0.0) {
      //cout << ngram_que_limit << endl;
      if (ngram_que_limit <= 0) {
	sort(id_freq_vec.begin(), id_freq_vec.end(), pair_first_up_order());
	vector < pair <sa_index, int> > tmp_id_freq_vec;
	count = 0;
	prev_id = id_freq_vec[0].first;
	
	for (vector < pair <sa_index, int> >::iterator idmit = id_freq_vec.begin(); idmit != id_freq_vec.end(); idmit++) {
	  if (prev_id != (*idmit).first) {
	    if (count + ngram_que_count < strdist->search_threshold[nq->min_hit_num].first) {}
	    else { tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count)); }
	    count = (*idmit).second;
	    prev_id = (*idmit).first;
	  }
	  else { count += (*idmit).second; }
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
    for (vector < pair <sa_index, int> >::iterator idfit = id_freq_vec.begin(); idfit != id_freq_vec.end(); idfit++) {
      if (prev_id != (*idfit).first) {
	
	//if (strdist->dist_func == "dist") {
	// bool is_next = false;
	// if (strdist->search_threshold[nq->min_hit_num].second > 0)  {
	//   if (count < strdist->search_threshold[nq->min_hit_num].second) { is_next = true; }
	//  }
	//  if (!is_next) {

	tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
	//  }
	//}
	count = (*idfit).second;
	prev_id = (*idfit).first;
      }
      else { count += (*idfit).second; }
      //cout << (*i).first << ":" << (*i).second << endl;
    }
    if (count > 0) {
      //bool is_next = false;
      //if (strdist->search_threshold[nq->min_hit_num].second > 0)  {
      //	if (count < strdist->search_threshold[nq->min_hit_num].second) { is_next = true; }
      //}
      //if (!is_next) {
      
      tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
      
      //}
      
      count = 0;
    }
    id_freq_vec = tmp_id_freq_vec;
  }
  else {
    sort(id_freq_vec.begin(), id_freq_vec.end(), pair_first_up_order());
    vector < pair <sa_index, int> > tmp_id_freq_vec;
    int count = 0;
    sa_index prev_id = id_freq_vec[0].first;
    for (vector < pair <sa_index, int> >::iterator idfit = id_freq_vec.begin(); idfit != id_freq_vec.end(); idfit++) {
      if (prev_id != (*idfit).first) {
	tmp_id_freq_vec.push_back(pair<sa_index, int>(prev_id, count));
	count = (*idfit).second;
	prev_id = (*idfit).first;
      }
      else { count += (*idfit).second; }
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
  //vector < pair <double, string> > NgramSearch::rerankAndGetResult(vector < pair< pair <sa_index, int>, sa_index> > &id_freq_ind_vec, NgramQuery *nq, StringDistance *strdist, int result_num, int bucket_size) {
  int push_count = 0;
  vector < pair <double, string> > result;
  //for (vector < pair < pair <sa_index, int>, sa_index > >::iterator it = id_freq_ind_vec.begin(); it != id_freq_ind_vec.end(); it++) {
  for (vector <pair <sa_index, int> >::iterator it = id_freq_vec.begin(); it != id_freq_vec.end(); it++) {
    if ((result_num > 0) && (push_count == bucket_size)) { break; } // どうする
    //pair < pair <sa_index, int>, sa_index> item = *it;
    pair <sa_index, int> item = *it;
    //cout << item.first << '/' << item.second << endl;
    char tmp_buf[this->entry_buf_len];
    tmp_buf[0] = '\0';
    sa_index pos;
    this->tdb->get_line(id_index_map[item.first], tmp_buf, entry_buf_len, &pos);
    //this->tdb->get_line(item.second, tmp_buf, entry_buf_len, &pos);
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

