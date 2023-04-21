//
// Created by jianxff on 2023/4/18.
//

#include "compiler.h"
#include "argparse.hpp"
using std::string;

int main(int argc, char** argv){
  ArgumentParser parser;
  // add optional argument 'input json file' to set input json file
  parser.add_argument<string>("-i","--input").nArgs('?').help("input json file")
        .default_("../scripts/json/max.json");
  // add optional argument 'output file' to set output c file
  parser.add_argument<string>("-o","--output").nArgs('?').help("output c file")
        .default_("");
  // add optional argument 'test' to run test and set test output
  parser.add_argument<string>("-t","--test").nArgs('?').help("test output c file")
        .default_("");
  // add optional argument 'remove cache' to auto-clear cache files
  parser.add_argument<bool>("-r","--reserve").help("reserve cache files");

  // argument parse
  parser.parse_args(argc, argv);
  // get argument value
  // get input source file
  string input = parser.get_value<string>("i");
  // get output destination
  string out = parser.get_value<string>("o");
  const char* output = (out.length() == 0 ? Compiler::stdout_ : out.c_str());

  Compiler compiler;
  int p;

  ///// JSON TEST /////
  ast::AST ast;
  ast.LoadFromJson(input);  // load ast
  p = compiler.Compile(&ast,output);

  // optional test execute
  if(parser.is_call("t")) {
    string test_out = parser.get_value<string>("t");
    const char* test = (test_out.length() == 0 ? Compiler::stdout_ : test_out.c_str());
    compiler.CodeExecute(compiler.tmp_file(p),test);
  }

  // optional reserve cache
  if(!parser.is_call("r")){
    compiler.Clear();
  }

  return 0;
}