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

class Compiler {
 public:
  static std::set<std::string> CODE_STYLES;

  int Compile(std::string in, std::string out = stdout_, std::string st = "google");
  int Compile(ast::AST* in, std::string out, std::string st);
  void CodeExecute(std::string file_name, std::string args = "");
  const char* tmp_file(int pos);
  void Clear();

 private:
  constexpr static const char* TMP_FILE_FORMAT = ".pascc.tf";
#ifdef WIN32
  constexpr static const char* EXECUTE_FORMAT = "gcc \"%s.c\" -o \"%s.exe\" && \"%s.exe\" ";
  constexpr static const char* CLANG_FORMAT = "clang-format.exe -style=%s -i \"%s\"";
#else
  constexpr static const char* EXECUTE_FORMAT = "gcc %s.c -o %s.out && ./%s.out ";
  constexpr static const char* CLANG_FORMAT = "clang-format -style=%s -i %s";
#endif

  std::vector<std::string> temp_files_;
  void CodeFormat(std::string file_name, std::string st);
  void CodePrint(std::string file_name,FILE* dst);

};

#endif // PASCC_COMPILER_H
