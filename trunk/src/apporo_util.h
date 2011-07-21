#ifndef APPORO_UTIL_H
#define APPORO_UTIL_H

#include <iostream>
#include <string>
#include "apporo_query.h"
#include "apporo_strdistance.h"

namespace apporo {
  namespace util {
    std::string getFirstColumn(std::string entry, char boundary = '\t');
    void showResult(std::vector < std::pair <double, std::string> > &result, apporo::query::NgramQuery *nq = NULL, apporo::strdistance::StringDistance *strdist = NULL, int result_num = 0);

    struct pair_first_low_order {
      template <class T, class K> bool operator()(const std::pair <T, K> &x, const std::pair <T, K> &y) const {
	return x.first > y.first;
      }
    };

    struct pair_first_up_order {
      template <class T, class K> bool operator()(const std::pair <T, K> &x, const std::pair <T, K> &y) const {
	return x.first < y.first;
      }
    };

    struct pair_second_low_order {
      template <class T, class K> bool operator()(const std::pair <T, K> &x, const std::pair <T, K> &y) const {
	return x.second > y.second;
      }
    };

    struct pair_second_up_order {
      template <class T, class K> bool operator()(const std::pair <T, K> &x, const std::pair <T, K> &y) const {
	return x.second < y.second;
      }
    };

    struct sa_range_low_order {
      template <class T, class K> bool operator()(const std::pair <T, K> &x, const std::pair <T, K> &y) const {
	return (((x.second).second - (x.second).first) > ((y.second).second - (y.second).first));
      }
    };

    struct sa_range_up_order {
      template <class T, class K> bool operator()(const std::pair <T, K> &x, const std::pair <T, K> &y) const {
	return (((x.second).second - (x.second).first) < ((y.second).second - (y.second).first));
      }
    };
  }
}

#endif
