//
// Created by jianxff on 2023/3/26.
//

#include "ast.h"
#include "type.h"
using namespace ast;

#define INPUT_PATH "../scripts/json/input"


int main(){
  pascal_type::TypeInit();

  AST ast_json;
  ast_json.LoadFromJson(INPUT_PATH "/max.json");
  ast_json.Print();
  return 0;
}