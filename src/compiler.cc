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
  yyparse(&ast);
  // ast format
  return Compile(&ast,out,st);
}

int Compiler::Compile(ast::AST *in, string out, Compiler::CODE_STYLE st) {
  if(in == nullptr || !in->root())
    return -1;
  // filename check
  size_t len = out.length();
  if(len > 0){
    if(len < 2 || (out[len - 2] != '.' && out[len - 1] != 'c'))
      out += ".c";
    log_info("compiler: result will format to %s", out.c_str());
  }

  // temp-file create
  time_t stamp = time(nullptr);
  string tmpfile = string(format_tmp_file_);
  tmpfile.replace(tmpfile.rfind("$TIME"),5,std::to_string(stamp));
  temp_files_.push_back(tmpfile);

  // ast-format
  in->Format(tmpfile.c_str());
  // code-format
  CodeFormat(tmpfile,st);
  // print
  CodePrint(tmpfile, (out.length() == 0) ? stdout : (fopen(out.c_str(),"w")));
  return temp_files_.size() -1;
}


void Compiler::CodePrint(string file,FILE* dst) {
  if(file == stdin_) return;

  std::filebuf fbuf;
  if(fbuf.open(file.c_str(), std::ios::in) == nullptr)
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


void Compiler::CodeFormat(string file, Compiler::CODE_STYLE st) {
  if(file == stdin_ || st == CODE_STYLE::NONE) return;
  string style;
  switch (st) {
    case CODE_STYLE::LLVM:      style="llvm";break;
    case CODE_STYLE::GOOGLE:    style="google";break;
    case CODE_STYLE::CHROMIUM:  style="chromium";break;
    case CODE_STYLE::MOZILLA:   style="mozilla";break;
    case CODE_STYLE::WEBKIT:    style="webkit";break;
    default:return;
  }

#ifdef WIN32
  string formatter = "clang-format.exe";
#else
  string formatter = "clang-format";
#endif

  string cmd = formatter + " -style=" + style + " -i " + file;
  system(cmd.c_str());
}

void Compiler::Clear() {
#ifdef WIN32
  string rm = " del ",
         exist = " if exist ",
         end = " ";
#else
  string rm = " rm ",
         exist = " if [ -f ",
         end = " ] ";
#endif
  log_info("compiler: clean cache files");
  for(auto& f : temp_files_) {
    f.replace(0,5,"");
    string cmd = "cd .tmp && " + exist + f + end + rm + f;
    system(cmd.c_str());
    f += ".exe";
    cmd = "cd .tmp && " + exist + f + end + rm + f;
    system(cmd.c_str());
  }
  temp_files_.clear();
}



void Compiler::CodeExecute(string in, string out) {
  if(in == stdin_)
    return;

#ifdef WIN32
  string file_out = in + ".exe";
#else
  string file_out = in + ".out";
#endif

  // compile
  string cmd = gcc_fmt;
  cmd.replace(cmd.find("$IN"), 3, in);
  cmd.replace(cmd.find("$OUT"),4,file_out);
  log_info("exec: compile command : %s",cmd.c_str());
  system(cmd.c_str());

  // execute
  cmd = "cd .tmp && " + file_out.replace(0,5,"");
  if(out != stdout_){
    cmd += " > " + out;
  }
  log_info("exec: exec command :  %s",cmd.c_str());
  system(cmd.c_str());

}


const char* Compiler::tmp_file(int pos) {
  if(temp_files_.size() == 0) return nullptr;
  if(pos < 0) pos = temp_files_.size() + pos;
  if(pos < 0 || pos >= temp_files_.size()) return nullptr;
  return temp_files_[pos].c_str();
}

void Compiler::DirAssert() {
  system("IF NOT EXIST \".tmp\" MD \".tmp\"");
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
    throw std::runtime_error("failed to open input file");
}
