#include "apporo_query.h"
#include "apporo_utf8.h"

using namespace std;
using namespace apporo;
using namespace apporo::utf8;
using namespace apporo::query;

NgramQuery::NgramQuery (string &query_, int ngram_length_, bool is_pre_, bool is_suf_, bool is_utf8_, double dist_threshold_)
  : query(query_), ngram_length(ngram_length_), is_pre(is_pre_), is_suf(is_suf_), is_utf8(is_utf8_), dist_threshold(dist_threshold_) {
  if (is_utf8) {
    utf8_boundary = getUTF8Width(query);
    chars_num = utf8_boundary.size();
  }
  else { chars_num = query.size(); }
  if (dist_threshold > 0.0) {
    ngram_length = this->tuneNgramLength(ngram_length, dist_threshold);
  }
  this->generateNgramTokens();
  return;
}
NgramQuery::~NgramQuery () { return; }

int NgramQuery::tuneNgramLength(int ngram_length, double dist_threshold) {
  int length = 0;
  double missnum = (double)this->chars_num * (1.0 - dist_threshold);
  length = (int)((double)this->chars_num / (missnum + 1.0));
  if (ngram_length > length) { length = ngram_length; };
  //cout << "change ngram:" << length << endl;
  return length;
}

// シフト幅を返す関数を返すようにするとエレガント
int NgramQuery::generateNgramTokens() {
  int chars_num = this->chars_num;
  set <string> rset;
  int len = this->ngram_length;
  if (len > chars_num) { len = chars_num; }
  if (!is_utf8) {
    if (this->is_pre) {
      for (int i = 1; i < len; i++) {
	string tmp = "\n" + this->query.substr(0, i);
	rset.insert(tmp);
      }
    }
    if (this->is_suf) {
      for (int i = 1; i < len; i++) {
	string tmp = this->query.substr(chars_num - i, i) + "\t";
	rset.insert(tmp);
      }
    }
    for (int i = 0; i <= (chars_num - len); i++) {
      rset.insert(this->query.substr(i, len));
    }
  }
  else {
    if (this->is_pre) {
      for (int i = 1; i < len; i++) {
	int width = 0;
	for (int j = 0; j < i; j++) {
	  width += this->utf8_boundary[j];
	}
	string tmp = "\n" + this->query.substr(0, width);
	rset.insert(tmp);
	//cout << ":" << tmp << ":" <<endl;
      }
    }
    if (this->is_suf) {
      for (int i = 1; i < len; i++) {
	int width = 0;
	for (int j = chars_num - 1; j > (chars_num - 1 - i); j--) {
	  width += this->utf8_boundary[j];
	}
	string tmp = this->query.substr(this->query.size() - width, width) + "\t";
	rset.insert(tmp);
	//cout << tmp << endl;
      }
    }
    for (int i = 0, focus = 0; i < (chars_num - len + 1);i++) {
      int width = 0;
      for (int j = 0; j < len; j++) {
	width += this->utf8_boundary[i + j];
      }
      rset.insert(this->query.substr(focus, width));
      //cout << this->query.substr(focus, width) << endl;
      focus += this->utf8_boundary[i];

    }
  }
  this->tokens = rset;
  return chars_num;
}
