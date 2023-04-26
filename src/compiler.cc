//
// Created by jianxff on 2023/4/19.
//

#include "compiler.h"
#include "parser.h"
#include "parser.tab.h"
#include "log.h"
#include <fstream>
using std::string;


int Compiler::Compile(string in, string out, Compiler::CODE_STYLE st) {
  yyinput(in.length() == 0 ? nullptr : in.c_str());
  ast::AST ast;
  if(yyparse(&ast) == 0){
    log_info("syntax assert no-error");
  }
  // ast format
  return Compile(&ast,out,st);
}

int Compiler::Compile(ast::AST *in, string out, Compiler::CODE_STYLE st) {
  if(in == nullptr || in->root() == nullptr)
    return -1;
  // filename check
  size_t len = out.length();
  FILE* out_dst = stdout;
  if(len > 0){
    if(len < 2 || (out[len - 2] != '.' && out[len - 1] != 'c'))  out += ".c";
    out_dst = fopen(out.c_str(),"w");
    log_info("compiler: result will format to %s", out.c_str());
  }

  // temp-file create
  string tmp_file_name = string(TMP_FILE_FORMAT) + std::to_string(time(nullptr));
  temp_files_.push_back(tmp_file_name);

  // ast-format
  in->Format(tmp_file_name + ".c");
  // code-format
  CodeFormat(tmp_file_name + ".c", st);
  // print
  CodePrint(tmp_file_name + ".c", out_dst);
  return temp_files_.size() - 1;
}


void Compiler::CodePrint(string file_name,FILE* dst) {
  if(file_name == stdin_) return;

  std::filebuf fbuf;
  if(fbuf.open(file_name.c_str(), std::ios::in) == nullptr)
    throw std::runtime_error("failed to open file " + file_name);

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


void Compiler::CodeFormat(string file_name, Compiler::CODE_STYLE st) {
  if(file_name == stdin_ || st == CODE_STYLE::NONE) return;
  string style;
  switch (st) {
    case CODE_STYLE::LLVM:      style="llvm";break;
    case CODE_STYLE::GOOGLE:    style="google";break;
    case CODE_STYLE::CHROMIUM:  style="chromium";break;
    case CODE_STYLE::MOZILLA:   style="mozilla";break;
    case CODE_STYLE::WEBKIT:    style="webkit";break;
    default:return;
  }

  char cmd_buf[64];
  sprintf(cmd_buf, CLANG_FORMAT,style.c_str(), file_name.c_str());
  system(cmd_buf);
}

void Compiler::Clear() {
  log_info("compiler: clean cache files");
  char cmd_buf[128];
  for(auto& f : temp_files_) {
    const char* fp = f.c_str();
#ifdef WIN32
    sprintf(cmd_buf,"if exist \"%s.*\" del \"%s.*\"",fp,fp);
#else
    sprintf(cmd_buf,"if [ -f %s.* ]; then rm %s.*; fi;",fp,fp);
#endif
    system(cmd_buf);
  }
  temp_files_.clear();
}


void Compiler::CodeExecute(string file_name, string args) {
  if(file_name.length() == 0) return;
  char cmd_buf[128];
  // compile
  const char* fp = file_name.c_str();
  sprintf(cmd_buf,EXECUTE_FORMAT,fp,fp,fp);
  // optional args
  string cmd = string(cmd_buf) + args;
  log_info("exec: exec command : %s",cmd.c_str());
  system(cmd.c_str());
}


const char* Compiler::tmp_file(int pos) {
  if(temp_files_.size() == 0) return nullptr;
  if(pos < 0) pos = temp_files_.size() + pos;
  if(pos < 0 || pos >= temp_files_.size()) return nullptr;
  return temp_files_[pos].c_str();
}


void yyinput(const char* in){
  if(in == nullptr) {
    yyin = stdin;
    log_info("compiler: input from stdin");
    return;
  }

  log_info("compiler: input file : %s",in);
  yyin = fopen(in,"r");
  if(yyin == nullptr)
    throw std::runtime_error("yyinput(): failed to open file " + string(in));
}
