#include <string>
#include <map>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "apporo_strdistance.h"
#include "apporo_utf8.h"

using namespace std;
using namespace apporo;
using namespace apporo::strdistance;

StringDistance::StringDistance(string &dist_func_, int ngram_length_, int query_chars_num_, double dist_threshold_)
  : dist_func(dist_func_), ngram_length(ngram_length_), query_chars_num(query_chars_num_), dist_threshold(dist_threshold_) { 
  if ((dist_threshold_ > 0.0) && (query_chars_num > 0)) {
    search_threshold = getSearchThreshold(dist_func, query_chars_num, ngram_length, dist_threshold);
  }
  return;
 }
StringDistance::~StringDistance() { return; }

vector < pair <int, int> > StringDistance::getSearchThreshold(string &dist_func, int chars_num, int ngram_length, double dist_threshold) {
  vector < pair <int, int> > vec;
  for (int i = 0; i < 256; i++) {
    int min = 0;
    int max = 0;
    if (i > 0) {
      if ("edit" == dist_func) {
	min = (chars_num + ngram_length + 1) - (int)((double)(chars_num + ngram_length + 1) * (1.0 - dist_threshold) * (double)ngram_length);
	max = (chars_num + ngram_length + 1) - (int)((double)(chars_num + ngram_length + 1) * (1.0 - dist_threshold) + (double)ngram_length);
      }
      else if ("dice" == dist_func) {
	min = (int)(dist_threshold / (2.0 - dist_threshold) * (double)(chars_num + ngram_length + 1));
	max = (int)(0.5 * dist_threshold * ((double)(chars_num + ngram_length + 1) + (double)i));
      }
      else if ("jaccard" == dist_func) {
	min = (int)(dist_threshold * (double)(chars_num + ngram_length + 1));
	max = (int)(dist_threshold * ((double)(chars_num + ngram_length + 1) + (double)i) / (1.0 + dist_threshold));
      }
      else if ("cosine" == dist_func) {
	min = (int)(dist_threshold * dist_threshold * (double)(chars_num + ngram_length + 1));
	max = (int)(dist_threshold * sqrt((double)(chars_num + ngram_length + 1) * (double)i));
      }
      else if ("overlap" == dist_func) {
	min = 0;
	max = (int)(dist_threshold * (double)std::min(chars_num + ngram_length + 1, i));
      }
      if (min < 0) { min = 0; }
      if (max < 0) { max = 0; }
      //cout << "score threshold:"<< min << "-" << max << endl;
    }
    pair <int, int> threshold(min, max);
    vec.push_back(threshold);
  }
  return vec;
}

vector < vector <int> > StringDistance::initMatrix(int num) {
  vector < vector <int> > matrix(num + 1);
  for ( vector < vector <int> >::iterator i = matrix.begin(); i != matrix.end(); i++ ) {
    (*i).resize(num + 1, 0);
  }
  int counter = 0;
  for (vector <int>::iterator i = matrix[0].begin(); i != matrix[0].end(); i++ ) {
    (*i) = counter;
    counter++;
  }
  counter = 0;
  for ( vector < vector <int> >::iterator i = matrix.begin(); i != matrix.end(); i++ ) {
    (*i)[0] = counter;
    counter++;
  }
  return matrix;
}


//探索打ち切れる
double StringDistance::getEditDist(string &str1, string &str2) {
  double res = 0.0;
  int str1_len = str1.size();
  int str2_len = str2.size();
  int matrix_width = str1_len;
  if (str2_len > matrix_width) {
    matrix_width = str2_len;
  }
  vector < vector <int> > matrix = initMatrix(matrix_width);
  for (int i = 0; i < str1_len; i++) {
    for (int j = 0; j < str2_len; j++) {
      if (str1[i] == str2[j]) {
	matrix[i + 1][j + 1] = matrix[i][j];
      }
      else {
	int min = matrix[i][j]; // left+up
	if (matrix[i][j + 1] < min) { min = matrix[i][j + 1];  }//left
	else if (matrix[i + 1][j] < min) { min = matrix[i + 1][j]; }//up
	matrix[i + 1][j + 1] = min + 1;
      }
    }
  }
  //cout << matrix[str1_len][str2_len] << endl;
  res =  1.0 - ((double)matrix[str1_len][str2_len] / (double)matrix_width);
  return res;
}

template <typename T>
double StringDistance::getUTF8BPAEditDist(string &str1, vector <int> &vec1, string &str2) {
  double res = 0.0;
  vector <int> vec2 = apporo::utf8::getUTF8Width(str2);
  //cout << vec2.size() << endl;
  //cout << vec1.size() << endl;
  int vec1_len = vec1.size();
  int vec2_len = vec2.size();

  string s1;
  string s2;
  vector <int> v1;
  vector <int> v2;
  int bit_len = sizeof(T) * 8;
  if (vec1_len >= vec2_len) { s1 = str1;  s2 = str2; v1 = vec1; v2 = vec2; }
  else { s1 = str2;  s2 = str1; v1 = vec2; v2 = vec1; }

  int m = v1.size();
  int n = v2.size();
  //cout << vec2.size() << endl;
  //cout << vec1.size() << endl;  

  if (m > bit_len) { m = bit_len; }
  if (n > bit_len) { n = bit_len; }
  //cout << "s1:" << s1 << " s2:" << s2 << endl;  
  //cout << "m:"<< m << ":n:" << n << endl;

  const T one = 1;
  T VP = 0;
  T VN = 0;
  int err = m;
  map <string, T> B; // when ASCII, string have to replace with char.
  for (int i = 0, focus = 0; i < m; focus += v1[i], i++) {
    //if (focus + v1[i] >= (int)s1.size()) {break;}
    //cout << v1[i] << ":" << s1.substr(focus, v1[i]) << ":" << s1 << ":"<< i << ":" << m<< endl;
    if (B.find(s1.substr(focus, v1[i])) != B.end()) {
      B[s1.substr(focus, v1[i])] |= one << i;
    }
    else {
      T tmp = 0;
      tmp |= one << i;
      B[s1.substr(focus, v1[i])] = tmp;
    }
    VP |= one << i;
  }
  
  for (int j = 0, focus = 0; j < n; focus += v2[j], j++) {
    T X;
    //cout << v2[j] << ":" << s2.substr(focus, v2[j]) << endl;
    //if (focus + v2[j] >= (int)s2.size()) {break;}
    if (B.find(s2.substr(focus, v2[j])) != B.end()){ X = B[s2.substr(focus, v2[j])] | VN; }
    else { X = 0 + VN; }
    T D0 = ((VP + (X & VP)) ^ VP) | X;
    T HN = VP & D0;
    T HP = VN | ~(VP | D0);
    X = HP << one;
    VN = X & D0;
    VP = (HN << one) | ~(X | D0);
    if (HP & (one << (m - 1))) { err++; }
    else if (HN & (one << (m - 1))) { err--; }
  }

  //cout << "err:" << err << " m:"<< m << endl;
  res = 1.0 - ((double)err / (double)m);
  
  return res;
}

template <typename T>
double StringDistance::getBPAEditDist(string &str1, string &str2) {
  double res = 0.0;
  string s1;
  string s2;
  int bit_len = sizeof(T) * 8;

  if (str1.size() >= str2.size()) { s1 = str1;  s2 = str2; }
  else { s1 = str2;  s2 = str1; }

  int m = s1.size();
  int n = s2.size();
  
  if (m > bit_len) { m = bit_len * 8; }
  if (n > bit_len) { n = bit_len * 8; }
  //cout << "s1:" << s1 << " s2:" << s2 << endl;  
  //cout << "m:"<< m << ":n:" << n << endl;

  const T one = 1;
  T VP = 0;
  T VN = 0;
  int err = m;

  /*
  T B[256] = {0};
  for (int i = 0; i < m; i++) {
    B[s1[i]] |= (one << i);
    VP |= (one << i);
  }
  */
  
  map <char, T> B; // when UTF8, char have to replace with string.
  for (int i = 0; i < m; i++) {
    if (B.find(s1[i]) != B.end()) {
      B[s1[i]] |= one << i;
    }
    else {
      T tmp = 0;
      tmp |= one << i;
      B[s1[i]] = tmp;
    }
    VP |= one << i;
  }
  

  for (int j = 0; j < n; j++) {
    
    T X = 0;
    if (B.find(s2[j]) != B.end()){ X = B[s2[j]] | VN; }
    else { X |=  VN; }
    
    /*
    T X = B[(int)s2[j]] | VN;
    */

    T D0 = ((VP + (X & VP)) ^ VP) | X;
    T HN = VP & D0;
    T HP = VN | ~(VP | D0);
    X = HP << one;
    VN = X & D0;
    VP = (HN << one) | ~(X | D0);
    if (HP & (one << (m - 1))) { err++; }
    else if (HN & (one << (m - 1))) { err--; }
  }

  //cout << "err:" << err << " m:"<< m << endl;
  res = 1.0 - ((double)err / (double)m);
  return res;
}


double StringDistance::getUTF8EditDist(string &str1, vector <int> &vec1, string &str2) {
  
  double res = 0.0;
  vector <int> vec2 = apporo::utf8::getUTF8Width(str2);
  int vec1_len = vec1.size();
  int vec2_len = vec2.size();
  int matrix_width = vec1_len;
  if (vec2_len > matrix_width) {
    matrix_width = vec2_len;
  }
  vector < vector <int> > matrix = initMatrix(matrix_width);
  for (int i = 0, focus1 = 0; i < vec1_len; focus1 += vec1[i], i++) {
    for (int j = 0, focus2 = 0; j < vec2_len; focus2 += vec2[j], j++) {
      if ((vec1[i] == vec2[j]) && (str1.substr(focus1, vec1[i]) == str2.substr(focus2, vec2[j]))) {
	matrix[i + 1][j + 1] = matrix[i][j];
      }
      else {
	int min = matrix[i][j]; // left+up
	if (matrix[i][j + 1] < min) { min = matrix[i][j + 1];  }//left
	else if (matrix[i + 1][j] < min) { min = matrix[i + 1][j]; }//up
	matrix[i + 1][j + 1] = min + 1;
	  }
    }
  }
  res =  1.0 - ((double)matrix[vec1_len][vec2_len] / (double)matrix_width);
  return res;
}

double StringDistance::getStringDistance(string &str_dist, string &str1, string &str2, vector <int> &str1_utf8_width) {
  double dist = 0.0;
  if (!str1_utf8_width.empty()) {
    if ((str1.size() <= 64) && (str2.size() <= 64)) {
      dist = getUTF8BPAEditDist<unsigned long long>(str1, str1_utf8_width, str2);
    }
    else {
      dist = getUTF8EditDist(str1, str1_utf8_width, str2);
    }
  }
  else {
    if ((str1.size() <= 64) && (str1.size() <= 64)) {
      dist = getBPAEditDist<unsigned long long>(str1, str2);
    }
    else {
	dist = getEditDist(str1, str2);
    }
  }
  return dist;
}
