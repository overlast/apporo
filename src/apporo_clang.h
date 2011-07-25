#ifndef APPORO_CLANG_H
#define APPORO_CLANG_H

#ifdef __cplusplus
extern "C" {
#endif

  void* apporo_c_new(char*);
  void apporo_c_delete(void*);
  char** apporo_c_approximate_match(void*, char*);

#ifdef __cplusplus
}
#endif

#endif
