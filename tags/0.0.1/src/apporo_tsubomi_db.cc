#include "tsubomi_basic_searcher.h"
#include "tsubomi_indexer.h"
#include "tsubomi_mmap.h"
#include "apporo_tsubomi_db.h"

#include <cstdio>
#include <iostream>
#include <string>

namespace apporo {
  namespace storage {
    using namespace std;
    TsubomiDBSearch::TsubomiDBSearch(string &filename) :
      tsubomi::basic_searcher(filename.c_str()), mr_did_(filename + ".did") {
      entry_num = mr_did_.size();
      return;
    }
    TsubomiDBSearch::~TsubomiDBSearch() {
      return;
    }

    sa_index TsubomiDBSearch::binaryDIDSearch(sa_index &offset, sa_index begin, sa_index end) {
      sa_index res = -1;
      while (begin <= end) {
	sa_index pivot = (begin + end) / 2;
	//cout << pivot << ":" << begin << ":" << end << endl;
	int ret = offset - mr_did_[pivot];
	//cout << ret << endl;
	if (ret < 0) { end =  pivot - 1; }
	else if (ret > 0) { begin = pivot + 1; }
	else { res = pivot; break; }
      }
      if (begin > end) { res = begin; }
      //cout << res << endl;
      return res;
    }

    sa_index TsubomiDBSearch::getDID(sa_index &num) {
      sa_index offset = mr_sa_[num];
      //if (id_cache.find(offset) != id_cache.end()) { return id_cache[offset]; }
      sa_index pivot = binaryDIDSearch(offset, 0, mr_did_.size() - 1);
      //id_cache[offset] = pivot;
      return pivot;
    }

    TsubomiDBWrite::TsubomiDBWrite(FILE *fout)
      : tsubomi::writer(fout) {
      return;
    }
    TsubomiDBWrite::~TsubomiDBWrite() {
      return;
    }

    TsubomiDBIndex::TsubomiDBIndex(string &filename)
      : tsubomi::indexer(filename.c_str()) {
      return;
    }
    TsubomiDBIndex::~TsubomiDBIndex() {
      return;
    }

    void TsubomiDBIndex::makeDIDIndex(string &boundary) {
      bool is_progress = true;
      string did_path = (string)filename_ + ".did";
      vector <sa_index> did;
      int N = mr_file_.size();
      tsubomi::progress_bar prg(int(N / 40));
      if (is_progress) { cerr << "+--------------------------------------+" << endl; }
      for (sa_index offset = 0; offset < mr_file_.size(); offset++) {
	if (is_progress) { prg.progress(1); }
	int b_size = boundary.size();
	for (int i = 0; i < b_size; i++) {
	  if (mr_file_[offset] == boundary[i]) {
	    did.push_back(offset);
	    break;
	  }
	}
      }
      // write did index as "filename.did"
      FILE *fout = fopen(did_path.c_str(), "wb");
      if (fout == NULL) {
	throw "error at TsubomiDBIndex::makeDIDIndex(). filename.did cannot open for write.";
      }
      TsubomiDBWrite writer(fout);
      writer.write(did);
      fclose(fout);
      if (is_progress) {  cout << endl << "done!" << endl; }
      return;
    }
    
    void TsubomiDBIndex::mkary_make(vector<sa_index> &sa, const char *seps, bool is_utf8) {
      // read index from file
      if (is_utf8) {
	if (seps[0] == '\t') {
	  bool flg = true;
	  for (sa_index offset = 0; offset < this->mr_file_.size(); offset += utf8_char_size[(unsigned char)(this->mr_file_[offset])]) {
	    if ((flg) && (this->mr_file_[offset] == '\t')) { sa.push_back(offset); flg = false; }
	    else if (this->mr_file_[offset] == '\n') { sa.push_back(offset); flg = true; }
	    else {
	      if (flg) { sa.push_back(offset); }
	    }
	  }
	}
	else {
	  for (sa_index offset = 0;
	       offset < this->mr_file_.size();
	       offset += utf8_char_size[(unsigned char)(this->mr_file_[offset])]) {
	    sa.push_back(offset);
	  }
	}
      } else if (seps[0] == '\0') {
	for (sa_index offset = 0; offset < this->mr_file_.size(); offset++) {
	  sa.push_back(offset);
	}
      } else if (seps[0] == '\t') {
	bool flg = true;
	for (sa_index offset = 0; offset < this->mr_file_.size(); offset++) {
	  if ((flg) && (this->mr_file_[offset] == '\t')) { sa.push_back(offset); flg = false; }
	  else if (this->mr_file_[offset] == '\n') { sa.push_back(offset); flg = true; }
	  else {
	    if (flg) { sa.push_back(offset); }
	  }
	}
      }
      else {
	bool flg = true;
	for (sa_index offset = 0; offset < this->mr_file_.size(); offset++) {
	  if (flg) { sa.push_back(offset); flg = false; }
	  for (int i = 0; seps[i] != '\0'; i++) {
	    if (this->mr_file_[offset] == seps[i]) { flg = true; }
	  }
	}
      }
      return;
    }
  }
}
