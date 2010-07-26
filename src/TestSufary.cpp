#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sufary.h>
#include "apporo.h"




void app_search_by_query(const char* sufarr_file_name, int ngram_num, int result_num, const char* function_name, const char* ranking_mode, const char* input_string) {
  return;
}

void app_search_by_file(const char* sufarr_file_name, int ngram_num, int result_num, const char* function_name, const char* ranking_mode, const char* input_string) {
  return;
}

void app_search_by_stream(const char* sufarr_file_name, int ngram_num, int result_num, const char* function_name, const char* ranking_mode, FILE* stdio) {
  return;
}

void app_print_usage(void) {
  printf("[usage]: prog [params] (filepathes)\n");
  printf("[options]\n");
  printf(" -s : index file name (must set -s param or give filepath of suffix array index file).\n");
  printf(" -n : number of N which use to generate N-gram tokens from index (2 is defalt).\n");
  printf(" -r : number of search result (100 is defalt).\n");
  printf(" -f : name of search function (surface_ngram is defalt).\n");
  printf(" -m : mode of search result ranking (ngram(ranking depends on hit number) is defalt).\n");
  printf(" -q : query to search index (must set -q param or give filepath of text-format file).\n");
  printf(" -h : print help message.\n");
  return;
}

int main(int argc, char* argv[]) {
  int ngram_num = 2;
  int result_num = 100;
  const char* sufarr_file_name = NULL;
  const char* input_file_name = NULL;
  const char* input_string = NULL;
  const char* ranking_mode = "ngram";
  const char* function_name = "search_ngram";
  
  while (1 < argc) {// parse comand-line options
    if ('-' != argv[1][0]) {// may be invalid prefix
      app_print_usage();
      exit(0);
    }
    else {
      char op = argv[1][1];
      if ('-' == op) {
        op = argv[1][2]; // parse long option name (ex:"--r 10")
      }
      switch (op) {
      case 'f': // set name of search function
        function_name = argv[2];
        argc--; argv++;
        break;
      case 'h': // show help
        app_print_usage();
        exit(0);
      case 'm': // set mode of search result ranking
        ranking_mode = argv[2];
        argc--; argv++;
        break;
      case 'n': // set ngram_num
        ngram_num = atoi(argv[2]);
        argc--; argv++;
        break;
      case 'q': // set query
        input_string = argv[2];
        argc--; argv++;
        break;
      case 'r': // set result_num
        result_num = atoi(argv[2]);
        argc--; argv++;
        break;
      case 's': // set suffix arrray file path
        sufarr_file_name = argv[2];
        argc--; argv++;
        break;
      default: // invalid
        printf("%c : invalid option", op);
        app_print_usage();
        exit(0);
      }
    }
    argc--; argv++;
  }
  
  if ((NULL == sufarr_file_name) || ("" == sufarr_file_name)) {
    if (1 < argc) {
      sufarr_file_name = argv[1];
      argc--; argv++;
      if (NULL == sufarr_file_name) {
        printf("must set -s param or give filepath of suffix array index file.");
        app_print_usage();
        exit(0);
      }
    }
    else {
      printf("must set -s param or give filepath of suffix array index file.");
      app_print_usage();
      exit(0);
    }
  }
  
  if ((NULL == input_string) || ("" == input_string)){
    if (NULL == input_file_name) {
      if (1 < argc) {
        input_file_name = argv[1];
        argc--; argv++;
        /* // this comment out to use stream input from stdio
           if (NULL == input_file_name) {
           printf(stderr, "must set -q param or give filepath of text-format file.");
           }
        */
      }
      /* // this comment out to use stream input from stdio
         else {
         //printf(stderr, "must set -q param or give filepath of text-format file.");
         }
      */
    }
  }

  if ((NULL != input_string) && (("" != input_string))) {
    app_search_by_query(sufarr_file_name, ngram_num, result_num, function_name, ranking_mode, input_string);
  }
  else if ((NULL != input_file_name) && (("" != input_file_name))) {
    app_search_by_file(sufarr_file_name, ngram_num, result_num, function_name, ranking_mode, input_file_name);
  }
  else {
    app_search_by_stream(sufarr_file_name, ngram_num, result_num, function_name, ranking_mode, stdin);
  }
  
  return 0;
}
