#ifndef APPORO_QUERY_H
#define APPORO_QUERY_H

#include <string>
#include <vector>
#include <set>

namespace apporo {
  namespace query {
    class NgramQuery {
    public:
      std::string query;
      int ngram_length;
      bool is_pre;
      bool is_suf;
      bool is_utf8;
      bool is_threshold;
      int chars_num;
      int min_hit_num;
      double dist_threshold;
      std::vector <int> utf8_boundary;
      std::set <std::string> tokens;
      NgramQuery (std::string &query_, int ngram_length_, bool is_pre_ = false, bool is_suf_ = false,  bool is_utf8_ = false, double dist_threshold = 0.0);
      ~NgramQuery ();
      int generateNgramTokens();
      int tuneNgramLength(int ngram_length, double dist_threshold);
    };
  }
}

#endif
