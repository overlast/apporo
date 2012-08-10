#ifndef APPORO_CLANG_H
#define APPORO_CLANG_H

#ifdef __cplusplus
extern "C" {
#endif

  void* app_c_new(char*);
  void app_c_delete(void*);
  char** app_c_retrieve(void*, char*);

#ifdef __cplusplus
}
#endif

#endif
