//
// Created by jianxff on 2023/4/18.
//

#include "compiler.h"
#include "parser.h"
#include "parser.tab.h"
#include "argparse.hpp"
using std::string;

int main(int argc, char** argv){
  ArgumentParser parser;
  // add optional argument 'input json file' to set input json file
  parser.add_argument<string>("-i","--input").nArgs('?').help("input json file")
        .default_("test.pas");
  // add optional argument 'output file' to set output c file
  parser.add_argument<string>("-o","--output").nArgs('?').help("output c file")
        .default_("");
  // add optional argument 'test' to run test and set test args
  parser.add_argument<string>("-t","--test").nArgs('?').help("test output c file, optional test args")
        .default_("");
  // add optional argument 'remove cache' to auto-clear cache files
  parser.add_argument<bool>("-r","--reserve").help("reserve cache files");

  // argument parse
  parser.parse_args(argc, argv);
  // get argument value
  // get input source file
  string in = parser.get_value<string>("i");
  // get output destination
  string out = parser.get_value<string>("o");

  Compiler compiler;
  int p = compiler.Compile(in,out);


  // optional test execute
  if(parser.is_call("t")) {
    string test_args = parser.get_value<string>("t");
    compiler.CodeExecute(compiler.tmp_file(p),test_args);
  }

  // optional reserve cache
  if(!parser.is_call("r")){
    compiler.Clear();
  }

  return 0;
}