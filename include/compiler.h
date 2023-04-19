//
// Created by jianxff on 2023/4/19.
//

#ifndef PASCC_COMPILER_H
#define PASCC_COMPILER_H

#include "ast.h"

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

  Compiler(){};
  ~Compiler(){};

  void Compile(const char* in, const char* out = nullptr, CODE_STYLE st = CODE_STYLE::GOOGLE);
  void Compile(ast::AST* in, const char* out = nullptr, CODE_STYLE st = CODE_STYLE::GOOGLE);

 private:
  const char* format_tmp_file = ".pascc-format.tmp";
  void CodeFormat(const char* file, CODE_STYLE st);
  void CodePrint(const char* file,FILE* dst);
};

#endif // PASCC_COMPILER_H
