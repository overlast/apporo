#include "apporo_clang.h"
#include "apporo.h"

using namespace std;
using namespace apporo;

void* apporo_c_new(char* file_path) {
  return (void*) new Apporo(file_path);
}

void apporo_c_delete(void* p) {
  delete (Apporo*)p;
}

char** apporo_c_approximate_match(void* p, char* query) {
  return ((Apporo*)p)->approximate_match(query);
}
