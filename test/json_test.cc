//
// Created by jianxff on 2023/4/18.
//

#include "ast.h"
#include "argparse.hpp"
using std::string;

int main(int argc, char** argv){
  ArgumentParser parser;
  // add optional argument 'input file'
  parser.add_argument<string>("-i","--input").help("input json files")
        .default_<string>("../scripts/json/max.json");
  // add optional argument 'output file'
  parser.add_argument<string>("-o","--output").help("output c files")
        .default_<string>("output.c");

  // argument parse
  parser.parse_args(argc, argv);
  // get argument value
  string input = parser.get_value<string>("i");
  string output = parser.get_value<string>("o");

  // load ast
  ast::AST ast;
  ast.LoadFromJson(input);
  ast.Print(output.c_str());
  return 0;
}