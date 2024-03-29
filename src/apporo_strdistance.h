#ifndef APPORO_STRDISTANCE_H
#define APPORO_STRDISTANCE_H

#include <string>
#include <vector>
#include "apporo_utf8.h"


namespace apporo {
  namespace strdistance {
    class StringDistance {
    public:
      std::string dist_func;
      int ngram_length;
      int query_chars_num;
      double dist_threshold;
      std::vector < std::pair <int, int> > search_threshold;
      StringDistance(std::string &dist_func, int ngram_length_, int query_chars_num = 0, double dist_threshold = 0.0);
      ~StringDistance();
      std::vector < std::pair <int, int> > getSearchThreshold(std::string &dist_func, int chars_num, int ngram_length, double dist_threshold);
      std::vector < std::vector <int> > initMatrix(int num);
      double getEditDist(std::string &str1, std::string &str2);
      double getUTF8EditDist(std::string &str1, std::string &str2);
      double getUTF8EditDist(std::string &str1, std::vector <int> &vec1, std::string &str2);
      template <typename T> double getBPMEditDist(std::string &str1, std::string &str2);
      template <typename T> double getUTF8BPMEditDist(std::string &str1, std::string &str2);
      template <typename T> double getUTF8BPMEditDist(std::string &str1, std::vector <int> &vec1, std::string &str2);
      double getStringDistance(std::string &str_dist, std::string &str1, std::string &str2, std::vector <int> &str1_utf8_width);
    };
  }
}

#endif
