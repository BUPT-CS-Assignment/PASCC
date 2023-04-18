//
// Created by jianxff on 2023/4/18.
//

#include "ast.h"
#include "log.h"

int main(){

  ast::AST ast;
  ast.LoadFromJson("../scripts/json/max.json");
  ast.Print("max.c");
  return 0;
}