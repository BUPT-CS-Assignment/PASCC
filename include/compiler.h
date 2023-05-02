//
// Created by jianxff on 2023/4/19.
//

#ifndef PASCC_COMPILER_H
#define PASCC_COMPILER_H

#include "ast.h"
#include <set>
extern FILE* yyin;
const std::string stdout_ = "";
const std::string stdin_ = "";

void yyinput(const char* in);
void yydebug_(int level);

class Compiler {
 public:
  Compiler() : cur_dir_(".") {}
  Compiler(std::string dir);
  static std::set<std::string> CODE_STYLES;

  int Compile(std::string in, std::string out = stdout_, std::string st = "google");
  int Compile(ast::AST* in, std::string out, std::string st);
  void CodeExecute(std::string file_name, std::string args = "");
  void Remove(std::string file_name);

 private:
#ifdef WIN32
  constexpr static const char* EXECUTE_FORMAT = "gcc \"%s.c\" -o \"%s.exe\" && \"%s.exe\" ";
  constexpr static const char* CLANG_FORMAT = "%s\\clang-format.exe -style=%s -i \"%s\"";
#else
  constexpr static const char* EXECUTE_FORMAT = "gcc %s.c -o %s.out && ./%s.out ";
  constexpr static const char* CLANG_FORMAT = "%s/clang-format -style=%s -i %s";
#endif

  std::string cur_dir_;
  bool FilenameAssert(std::string& file_name);
  void CodeFormat(std::string file_name, std::string st);

};

#endif // PASCC_COMPILER_H
