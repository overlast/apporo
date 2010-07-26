#ifndef APPORO_SEARCH_H
#define APPORO_SEARCH_H

#define DEBUG2 1

int app_surface_ngram_match(const char* sufarr_file_name, int ngram_num, int result_num, const char* ranking_mode, const char* input_string, char* result_buf, int result_buf_len);

#endif
