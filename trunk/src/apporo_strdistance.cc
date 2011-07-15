#include <string>
#include "apporo_strdistance.h"
#include "apporo_utf8.h"

using namespace std;
using namespace apporo;
using namespace apporo::strdistance;

StringDistance::StringDistance(string &dist_func_, int ngram_length_, int query_chars_num_, double dist_threshold_)
  : dist_func(dist_func_), ngram_length(ngram_length_), query_chars_num(query_chars_num_), dist_threshold(dist_threshold_) { 
  if ((dist_threshold_ > 0.0) && (query_chars_num > 0)) {
    search_threshold = getSearchThreshold(query_chars_num, ngram_length, dist_threshold);
  }
  return;
 }
StringDistance::~StringDistance() { return; }

pair <int, int> StringDistance::getSearchThreshold(int chars_num, int ngram_length, double dist_threshold) {
  pair <int, int> threshold;
  int min = (chars_num + ngram_length + 1) - (int)((double)(chars_num + ngram_length + 1) * (1.0 - dist_threshold) * (double)ngram_length);
  int max = (chars_num + ngram_length + 1) - (int)((double)(chars_num + ngram_length + 1) * (1.0 - dist_threshold) + (double)(ngram_length - 1));
  if (min < 0) { min = 0; }
  if (max < 0) { max = 0; }
  //cout << "score threshold:"<< min << "-" << max << endl;
  threshold = pair <int, int>(min, max);
  return threshold;
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
/*
template <typename T>
double StringDistance::getBPAEditDist(string &str1, string &str2) {
  
  
}
*/
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
  if (str_dist == "edit") {
    if (!str1_utf8_width.empty()) {
      dist = (double)getUTF8EditDist(str1, str1_utf8_width, str2);
    }
    else {
      dist = (double)getEditDist(str1, str2);
    }
  }
  return dist;
}
