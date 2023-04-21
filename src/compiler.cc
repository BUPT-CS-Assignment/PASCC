//
// Created by jianxff on 2023/4/19.
//

#include "compiler.h"
#include "log.h"
#include <fstream>
using std::string;

int Compiler::Compile(const char *in, const char *out, Compiler::CODE_STYLE st) {
  if(in == nullptr)
    return -1;
  // TODO: compile from input pascal-s file
  ast::AST* ast = nullptr;
  // ast format
  return Compile(ast,out,st);
}

int Compiler::Compile(ast::AST *in, const char *out, Compiler::CODE_STYLE st) {
  if(in == nullptr)
    return -1;
  // filename check
  string outs;
  if(out != stdout_) {
    outs = string(out);
    size_t len = outs.length();
    if(len < 2 || (outs[len - 2] != '.' && outs[len - 1] != 'c'))
      outs += ".c";
    log_info("compiler: result will format to %s", outs.c_str());
  }
  // temp-file create
  time_t stamp = time(nullptr);
  string tmpfile = string(format_tmp_file_);
  tmpfile.replace(tmpfile.rfind("$TIME"),5,std::to_string(stamp));
  temp_files_.push_back(tmpfile);
  const char* tfp = tmpfile.c_str();

  // ast-format
  in->Format(tfp);
  // code-format
  CodeFormat(tfp,st);
  // print
  CodePrint(tfp, (out == stdout_) ? stdout : (fopen(outs.c_str(),"w")));
  return temp_files_.size() -1;
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

#ifdef WIN32
  string formatter = "clang-format.exe";
#else
  string formatter = "clang-format";
#endif

  string cmd = formatter + " -style=" + style + " -i " + string(file);
  system(cmd.c_str());
}

void Compiler::Clear() {
#ifdef WIN32
  string rm = "del ";
#else
  string rm = "rm ";
#endif
  log_info("compiler: clean cache files");
  for(auto& f : temp_files_) {
    f.replace(0,5,"");
    string cmd = "cd .tmp && " + rm + f + " && " + rm + f + ".exe";
    system(cmd.c_str());
  }
  temp_files_.clear();
}



void Compiler::CodeExecute(const char *in, const char *out) {
  if(in == nullptr)
    return;

  string file = string(in);
#ifdef WIN32
  string file_out = file + ".exe";
#else
  string file_out = file + ".out";
#endif

  // compile
  string cmd = gcc_fmt;
  cmd.replace(cmd.find("$IN"),3,file);
  cmd.replace(cmd.find("$OUT"),4,file_out);
  log_info("exec: compile command : %s",cmd.c_str());
  system(cmd.c_str());

  // execute
  cmd = "cd .tmp && " + file_out.replace(0,5,"");
  if(out != stdout_){
    string out_dst = string(out);
    cmd += " > " + out_dst;
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
  system("mkdir .tmp");
}