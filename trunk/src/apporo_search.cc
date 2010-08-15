#include "apporo_search.h"

// todo
// 1, return the result as structure or class;




int app_ngram_match(char* result_buf, int result_buf_len, const char* sufarr_file_name, int ngram_num, int result_num, const char* ranking_mode, const char* input_string, int input_string_len) {
  // init
  //// new query object(query string, query token,  N, reranking option, threshold, result_num)
  //// new searcher object (wrapping sufary and tsubomi)
  //// new small result object(size : all doc num)
  //// init final result object(size : result_num)
  // query cleaning
  //// make query object
  //// utf-8 tokenise
  // search
  //// use query object
  //// make searcher object
  //// and search and return small result object(docid, ngramhit_num, dp_score);
  // reranking
  //// use query object
  //// use searcher object
  //// reranking small result object(docid, ngramhit_num, dp_score);
  // threshold
  //// use query object
  //// use searcher object
  //// use small result object
  //// cleaning small result object(docid, ngramhit_num, dp_score);
  // return result
  //// free query object;
  //// free searcher objext;
  //// free small result objext
  //// return final result object or string
  //// use searcher object to get document entity
}
