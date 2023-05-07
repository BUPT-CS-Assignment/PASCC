//
// Created by jianxff on 2023/4/19.
//

#include "compiler.h"
#include "parser.h"
#include "parser.tab.h"
#include "log.h"
#include <fstream>
using std::string;
using pascals::ast::AST;

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


int Compiler::Compile(string in, string out, string st) {
  yyinput(in.length() == 0 ? nullptr : in.c_str());
  AST ast;

  if(yyparse(&ast) == 0){
    log_info("syntax assert no-error");
  }else{
    log_error("syntax error");
    return -1;
  }
  // ast format
  return Compile(&ast,out,st);
}


int Compiler::Compile(AST *in, string out, string st) {
  if(in == nullptr || in->root() == nullptr)
    return -1;
  // filename check
  FILE* dst = stdout;
  if(out.length() > 0){
    dst = fopen((out + ".c").c_str(),"w");
  }

  if (dst == nullptr) {
    log_fatal("failed to open file %s.c", out.c_str());
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
  log_debug("compiler: code style : %s", st.c_str());

  char cmd_buf[128];
#ifdef WIN32
  sprintf(cmd_buf, CLANG_FORMAT,cur_dir_.c_str(),st.c_str(), file_name.c_str());
#else
  sprintf(cmd_buf, CLANG_FORMAT,st.c_str(), file_name.c_str());
#endif
  system(cmd_buf);
}

void Compiler::Remove(std::string file_name, bool rm_all) {
  log_info("compiler: clean cache files");
  char cmd_buf[128];
  if(file_name.length() == 0) return;
  file_name +=
      (rm_all ? ".*" :
#ifdef  WIN32
                ".exe"
#else
                ".out"
#endif
    );
  const char* fp = file_name.c_str();

#ifdef WIN32
    sprintf(cmd_buf,"if exist \"%s\" del \"%s\"",fp,fp);
#else
    sprintf(cmd_buf,"if [ -f %s ]; then rm %s; fi;",fp,fp);
#endif

    log_debug("compiler: remove command : %s",cmd_buf);
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
  log_debug("compiler: exec command : %s",cmd.c_str());
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
