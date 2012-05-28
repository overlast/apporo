#include "apporo_clang.h"
#include "apporo.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace apporo;

void* app_c_new(char* file_path_) {
  string file_path(file_path_);
  return (void*) new Apporo(file_path);
}

void app_c_delete(void* p) {
  delete (Apporo*)p;
}

char** app_c_retrieve(void* p, char* query_) {
  string query(query_);
  vector <string> vec =  ((Apporo*)p)->retrieve(query);
  int vec_size = vec.size();
  char** res = (char**)malloc(sizeof(char*) * vec_size);
  int count = 0;
  for (vector <string>::iterator bi = vec.begin(), ei = vec.end(); bi != ei; ++bi) {
    int str_len = (*bi).size();
    res[count] = (char*)calloc(sizeof(char*), str_len + 1);
    for (int i = 0; i < str_len; i++) {
      res[count][i] = (*bi)[i];
    }
    count++;
  }
  return res;
}
