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

  Compiler(){DirAssert();};
  ~Compiler(){};

  int Compile(std::string in, std::string out = stdout_, CODE_STYLE st = CODE_STYLE::GOOGLE);
  int Compile(ast::AST* in, std::string out = stdout_, CODE_STYLE st = CODE_STYLE::GOOGLE);
  void CodeExecute(std::string in, std::string out = stdout_);
  void SetGCCFmt(const char* fmt){gcc_fmt = std::string(fmt);}
  const char* tmp_file(int pos);
  void Clear();

 private:
  constexpr static const char* format_tmp_file_ = ".tmp/runtime.pascc.$TIME.c";
  std::vector<std::string> temp_files_;
  std::string gcc_fmt = "gcc $IN -o $OUT";

  void DirAssert();
  void CodeFormat(std::string file, CODE_STYLE st);
  void CodePrint(std::string file,FILE* dst);

};

#endif // PASCC_COMPILER_H
