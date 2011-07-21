#ifndef APPORO_SEARCH_H
#define APPORO_SEARCH_H

#include <iostream>
#include <algorithm>
#include <set>
#include <string>
#include <vector>
#include <map>
#include "apporo_tsubomi_db.h"
#include "apporo_query.h"
#include "apporo_strdistance.h"
#include "apporo_util.h"

namespace apporo {
  namespace search {
    class NgramSearch {
    public:
      NgramSearch (std::string &engine, std::string &index_path, int entry_buf_len);
      ~NgramSearch ();
      std::string engine;
      std::string index_path;
      int entry_buf_len;
      apporo::strage::TsubomiDBSearch *tdb;
      std::vector < std::pair <std::string, apporo::strage::sa_range> > getRange(apporo::query::NgramQuery *nq);
      std::vector < std::pair <apporo::strage::sa_index, int> > getIDMap(std::map <apporo::strage::sa_index, apporo::strage::sa_index> &id_index_map, std::vector < std::pair<std::string, apporo::strage::sa_range> > &range_vector, apporo::query::NgramQuery *nq, apporo::strdistance::StringDistance *strdist);
      //std::vector < std::pair < std::pair <apporo::strage::sa_index, int>, apporo::strage::sa_index > > getIDMap(std::vector < std::pair<std::string, apporo::strage::sa_range> > &range_vector, apporo::query::NgramQuery *nq, apporo::strdistance::StringDistance *strdist);
      std::vector < std::pair <double, std::string> > matchApproximately(std::string engine, std::string index_path);
      std::vector < std::pair <double, std::string> > rerankAndGetResult(std::vector <std::pair <apporo::strage::sa_index, int> > &id_freq_vec, std::map <apporo::strage::sa_index, apporo::strage::sa_index> &id_index_map, apporo::query::NgramQuery *nq, apporo::strdistance::StringDistance *strdist, int result_num = 0, int bucket_size = 2000);
      //std::vector < std::pair <double, std::string> > rerankAndGetResult(std::vector < std::pair < std::pair <apporo::strage::sa_index, int>, apporo::strage::sa_index > > &id_freq_ind_vec, apporo::query::NgramQuery *nq, apporo::strdistance::StringDistance *strdist, int result_num = 0, int bucket_size = 2000);
    };
  }
}

#endif
