//
// Created by jianxff on 2023/4/19.
//

#include "compiler.h"
#include "parser.h"
#include "parser.tab.h"
#include "log.h"
#include <fstream>
using std::string;

std::set<std::string> Compiler::CODE_STYLES = {
    "llvm",
    "google",
    "chromium",
    "mozilla",
    "webkit",
};

Compiler::Compiler(std::string dir) {
#ifdef WIN32
  size_t pos = dir.rfind("\\");
#else
  size_t pos = dir.rfind("/");
#endif
  if(pos != std::string::npos){
    cur_dir_ = dir.substr(0, pos);
  }else{
    cur_dir_ = dir;
  }
  log_info("compiler: current dir : %s", cur_dir_.c_str());
}

bool Compiler::FilenameAssert(std::string &file) {
  size_t len = file.length();
  if(len == 0 || len >= 64) return false;
  if(len < 2 || (file[len - 2] != '.' && file[len - 1] != 'c'))
    file += ".c";
  return true;
}

int Compiler::Compile(string in, string out, string st) {
  yyinput(in.length() == 0 ? nullptr : in.c_str());
  ast::AST ast;

  if(yyparse(&ast) == 0){
    log_info("syntax assert no-error");
  }else{
    log_error("syntax error");
    return -1;
  }
  // ast format
  return Compile(&ast,out,st);
}


int Compiler::Compile(ast::AST *in, string out, string st) {
  if(in == nullptr || in->root() == nullptr)
    return -1;
  // filename check
  FILE* dst = stdout;
  if(FilenameAssert(out)){
    dst = fopen(out.c_str(),"w");
  }

  if (dst == nullptr) {
    log_fatal("failed to open file %s", out.c_str());
    return -1;
  }

  // ast-format
  in->Format(dst);
  // code-format
  CodeFormat(out,st);
  return 0;
}


void Compiler::CodeFormat(string file_name, string st) {
  if(file_name.length() == 0 || st.length() == 0) return;
  if(CODE_STYLES.find(st) == CODE_STYLES.end()) {
    log_warn("undefined code style: %s, reset to 'google'", st.c_str());
    st = "google";
  }

  char cmd_buf[128];
#ifdef WIN32
  sprintf(cmd_buf, CLANG_FORMAT,cur_dir_.c_str(),st.c_str(), file_name.c_str());
#else
  sprintf(cmd_buf, CLANG_FORMAT,st.c_str(), file_name.c_str());
#endif
  system(cmd_buf);
}

void Compiler::Remove(std::string file_name) {
  log_info("compiler: clean cache files");
  char cmd_buf[128];
  if(file_name.length() == 0) return;

  const char* fp = file_name.c_str();

#ifdef WIN32
    sprintf(cmd_buf,"if exist \"%s.exe\" del \"%s.exe\"",fp,fp);
#else
    sprintf(cmd_buf,"if [ -f %s.out ]; then rm %s.out; fi;",fp,fp);
#endif
    system(cmd_buf);
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

void yydebug_(int level){
  // TODO  set yydebug
  yydebug = (level == 4);
}
