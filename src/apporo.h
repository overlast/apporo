#ifndef APPORO_H
#define APPORO_H

#include "apporo_util.h"
#include "apporo_search.h"

void app_search_by_query(const char* sufarr_file_name, int ngram_num, int result_num, const char* function_name, const char* ranking_mode, const char* input_string, int input_string_len);
void app_search_by_stream(const char* sufarr_file_name, int ngram_num, int result_num, const char* function_name, const char* ranking_mode, FILE* target_file_p);
void app_search_by_file(const char* sufarr_file_name, int ngram_num, int result_num, const char* function_name, const char* ranking_mode, const char* input_file_name);
void app_print_usage(void);

#endif
