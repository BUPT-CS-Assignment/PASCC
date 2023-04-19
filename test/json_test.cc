//
// Created by jianxff on 2023/4/18.
//

#include "compiler.h"
#include "argparse.hpp"
using std::string;

int main(int argc, char** argv){
  ArgumentParser parser;
  // add optional argument 'input file'
  parser.add_argument<string>("-i","--input").help("input json files")
        .default_("../scripts/json/max.json");
  // add optional argument 'output file'
  parser.add_argument<string>("-o","--output").default_("stdout").help("output c files")
        .default_("output.c");

  // argument parse
  parser.parse_args(argc, argv);
  // get argument value
  string input = parser.get_value<string>("i");
  string output = parser.get_value<string>("o");
  const char* out_str = (output == "stdout" ? nullptr : output.c_str());

  // load ast
  ast::AST ast;
  ast.LoadFromJson(input);

  // compile
  Compiler compiler;
  compiler.Compile(&ast,out_str);

  return 0;
}