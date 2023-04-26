//
// Created by jianxff on 2023/4/19.
//

#ifndef PASCC_COMPILER_H
#define PASCC_COMPILER_H

#include "ast.h"
extern FILE* yyin;
const std::string stdout_ = "";
const std::string stdin_ = "";

void yyinput(const char* in);

class Compiler {
 public:
   enum class CODE_STYLE {
     NONE,
     LLVM,
     GOOGLE,
     CHROMIUM,
     MOZILLA,
     WEBKIT,
   };

  int Compile(std::string in, std::string out = stdout_, CODE_STYLE st = CODE_STYLE::GOOGLE);
  int Compile(ast::AST* in, std::string out = stdout_, CODE_STYLE st = CODE_STYLE::GOOGLE);
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
  constexpr static const char* CLANG_FORMAT = "./clang-format -style=%s -i %s";
#endif

  std::vector<std::string> temp_files_;
  void CodeFormat(std::string file_name, CODE_STYLE st);
  void CodePrint(std::string file_name,FILE* dst);

};

#endif // PASCC_COMPILER_H
