#include "apporo_ma_mecab.h"

using namespace std;
using namespace apporo;
using namespace apporo::ma;

MAMeCab::MAMeCab() { 
  tagger = MeCab::createTagger("");
  return;
};
MAMeCab::~MAMeCab() { return; };

string MAMeCab::getMAResult(string &str) {
  const char *result = tagger->parse(str.c_str());
  string res = result;
  return res;
};

string MAMeCab::getRoman(string &str) {
  string res = "roman dayo";
  return res;
};

vector <string> split(const string &str, char delim){
  vector<string> res;
  size_t current = 0, found;
  while((found = str.find_first_of(delim, current)) != string::npos){
    res.push_back(string(str, current, found - current));
    current = found + 1;
  }
  res.push_back(string(str, current, str.size() - current));
  return res;
}

string join(vector <string> vec) {
  string str;
  size_t vec_size = vec.size();
  for (size_t i = 0; i < vec_size; i++) {
    str += vec[i];
  }
  return str;
}

string MAMeCab::getYomi(string &str) {
  const MeCab::Node* node = tagger->parseToNode(str.c_str());
  vector <string> buf;
  for (; node; node = node->next) {
    if (node->stat == MECAB_BOS_NODE) {}
    else if (node->stat == MECAB_EOS_NODE) { }
    else {
      string f = node->feature;
      vector <string> features = split(f, ',');
      if (features.size() > 8) {
	buf.push_back(features[7]);
      }
    }
  }
  string res = join(buf);
  return res;
};

/*
#define CHECK(eval) if (! eval) { \
    const char *e = tagger ? tagger->what() : MeCab::getTaggerError(); \
    std::cerr << "Exception:" << e << std::endl; \
   delete tagger; \
   return -1;}

int main (int argc, char **argv) {
  string input = "太郎は次郎が持っている本を花子に.渡した。";
  MAMeCab *ma = new MAMeCab();
  cout << ma->getMAResult(input) << endl;
  cout << ma->getYomi(input) << endl;
  delete ma;

  MeCab::Tagger *tagger = MeCab::createTagger(argc, argv);
  CHECK(tagger);

  const char *result = tagger->parse(input);
  CHECK(result);
  std::cout << "INPUT: " << input << std::endl;
  std::cout << "RESULT: " << result << std::endl;

  result = tagger->parseNBest(3, input);
  CHECK(result);
  std::cout << "NBEST: " << std::endl << result;
  
  CHECK(tagger->parseNBestInit(input));
  for (int i = 0; i < 3; ++i) {
    std::cout << i << ":" << std::endl << tagger->next();
  }

  const MeCab::Node* node = tagger->parseToNode(input);
  CHECK(node);
  for (; node; node = node->next) {
    std::cout.write(node->surface, node->length);
  }

  node = tagger->parseToNode(input);
  CHECK(node);

  for (; node; node = node->next) {
    std::cout << node->id << ' ';
    if (node->stat == MECAB_BOS_NODE)
      std::cout << "BOS";
    else if (node->stat == MECAB_EOS_NODE)
      std::cout << "EOS";
    else      std::cout.write (node->surface, node->length);

    std::cout << ' ' << node->feature
	      << ' ' << (int)(node->surface - input)
	      << ' ' << (int)(node->surface - input + node->length)
	      << ' ' << node->rcAttr
	      << ' ' << node->lcAttr
	      << ' ' << node->posid
	      << ' ' << (int)node->char_type
	      << ' ' << (int)node->stat      << ' ' << (int)node->isbest
	      << ' ' << node->alpha
	      << ' ' << node->beta
	      << ' ' << node->prob
	      << ' ' << node->cost << std::endl;
  }

  delete tagger;

  return 0;
}

*/

