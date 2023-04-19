//
// Created by jianxff on 2023/4/19.
//

#include "compiler.h"
#include "log.h"
#include <fstream>
using std::string;

void Compiler::Compile(const char *in, const char *out, Compiler::CODE_STYLE st) {
  // TODO: compile from input pascal-s file
}

void Compiler::Compile(ast::AST *in, const char *out, Compiler::CODE_STYLE st) {
  if(in == nullptr)
    return;
  // filename check
  string outs;
  if(out != nullptr) {
    outs = string(out);
    size_t len = outs.length();
    if(len < 2 || (outs[len - 2] != '.' && outs[len - 1] != 'c'))
      outs += ".c";
    log_info("compiler: result will format to %s", outs.c_str());
  }
  // temp-file create
  time_t stamp = time(nullptr);
  string tmpfile = std::to_string(stamp) + string(format_tmp_file);
  const char* tfp = tmpfile.c_str();

  // ast-format
  in->Format(tfp);
  // code-format
  CodeFormat(tfp,st);
  // print
  CodePrint(tfp, (out == nullptr) ? stdout : (fopen(outs.c_str(),"w")));
  // remove
  string cmd = "del " + tmpfile;
  system(cmd.c_str());
}

void Compiler::CodePrint(const char* file,FILE* dst) {
  std::filebuf fbuf;
  if(fbuf.open(file, std::ios::in) == nullptr)
    throw std::runtime_error("failed to open temp file");

  if(dst == nullptr)
    throw std::runtime_error("no output file stream");

  std::istream ss(&fbuf);
  std::string strbuf;
  while(!ss.eof()) {
    std::getline(ss,strbuf);
    fprintf(dst,"%s\n",strbuf.c_str());
  }
  fbuf.close();
}

void Compiler::CodeFormat(const char* file, Compiler::CODE_STYLE st) {
  if(st == CODE_STYLE::NONE) return;
  string style;
  switch (st) {
    case CODE_STYLE::LLVM:      style="llvm";break;
    case CODE_STYLE::GOOGLE:    style="google";break;
    case CODE_STYLE::CHROMIUM:  style="chromium";break;
    case CODE_STYLE::MOZILLA:   style="mozilla";break;
    case CODE_STYLE::WEBKIT:    style="webkit";break;
    default:return;
  }
  string cmd = "clang-format.exe -style=" + style + " -i " + string(file);
  system(cmd.c_str());
}
