//
// Created by jianxff on 2023/4/25.
//
#include "pstdlib.h"

using symbol_table::SymbolTable;
using pascal_symbol::FunctionSymbol;
using pascal_type::TYPE_INT;
using pascal_type::TYPE_CHAR;
using pascal_type::TYPE_REAL;
using pascal_type::TYPE_BOOL;
using pascal_type::TYPE_NONE;

namespace pstdlib{

PStdLibs::PStdLibs() {
  lib_map_["abs"] = {"abs(x) (x < 0 ? -x : x)", false};
  lib_map_["sqr"] = {"sqr(x) (x * x)", false};
  lib_map_["odd"] = {"odd(x) (x % 2 == 1)", false};
  lib_map_["chr"] = {"chr(x) ((char)x)", false};
  lib_map_["ord"] = {"ord(x) ((int)x)", false};
  lib_map_["succ"] = {"succ(x) (x + 1)", false};
  lib_map_["pred"] = {"pred(x) (x - 1)", false};
  lib_map_["round"] = {"round(x) ((int)(x + 0.5))", false};
  lib_map_["trunc"] = {"trunc(x) ((int)x)", false};
  lib_map_["sin"] = {"sin(x) (sin((float)x))", false};
  lib_map_["cos"] = {"cos(x) (cos((float)x))", false};
  lib_map_["exp"] = {"exp(x) (exp((float)x))", false};
  lib_map_["ln"] = {"ln(x) (log((float)x))", false};
  lib_map_["sqrt"] = {"sqrt(x) (sqrt((float)x))", false};
  lib_map_["arctan"] = {"arctan(x) (atan((float)x))", false};
  lib_map_["eof"] = {"eof() feof(stdin)", false};
  lib_map_["eoln"] = {"bool eoln() {\\\nint c = getchar();\\\nungetc(c,stdin);\\\nreturn c == '\\n' || c == EOF;\\\n}", false};
}



void PStdLibs::Preset(symbol_table::SymbolTable *st) {
  using Parameter = FunctionSymbol::Parameter;
  using ParamType = FunctionSymbol::ParamType;
  using ParamMode = FunctionSymbol::PARAM_MODE;

  struct param_struct{
    std::string name;
    pascal_type::BasicType* return_type;
    pascal_type::BasicType* param_type;
  };

  std::vector<param_struct> params = {
      {"abs", TYPE_INT, TYPE_INT},
      {"sqr", TYPE_INT, TYPE_INT},
      {"odd", TYPE_BOOL, TYPE_INT},
      {"chr", TYPE_CHAR, TYPE_INT},
      {"ord", TYPE_INT, TYPE_CHAR},
      {"succ", TYPE_CHAR, TYPE_CHAR},
      {"pred", TYPE_CHAR, TYPE_CHAR},
      {"round", TYPE_INT, TYPE_REAL},
      {"trunc", TYPE_INT, TYPE_REAL},
      {"sin", TYPE_REAL, TYPE_REAL},
      {"cos", TYPE_REAL, TYPE_REAL},
      {"exp", TYPE_REAL, TYPE_REAL},
      {"ln", TYPE_REAL, TYPE_REAL},
      {"sqrt", TYPE_REAL, TYPE_REAL},
      {"arctan", TYPE_REAL, TYPE_REAL},
      {"eof", TYPE_BOOL, nullptr},
      {"eoln", TYPE_BOOL, nullptr},
      {"read", TYPE_NONE, nullptr},
      {"readln", TYPE_NONE, nullptr},
      {"write", TYPE_NONE, nullptr},
      {"writeln", TYPE_NONE, nullptr}
  };

  for(auto& p : params){
    std::vector<Parameter> param_list;
    if(p.param_type != nullptr)
      param_list.emplace_back(Parameter("x",ParamType(p.param_type, ParamMode::VALUE)));
    st->Insert(p.name, new FunctionSymbol(p.name, p.return_type, 0, param_list));
  }



}

bool PStdLibs::Call(std::string lib_name) {
  if(lib_map_.find(lib_name) != lib_map_.end()){
    lib_map_[lib_name].called_ = true;
    return true;
  }
  return false;
}

bool PStdLibs::UnCall(std::string lib_name) {
  if(lib_map_.find(lib_name) != lib_map_.end()){
    lib_map_[lib_name].called_ = false;
    return true;
  }
  return false;
}

void PStdLibs::Format(FILE* dst){
  // format cstdlib
  fprintf(dst, "#include <stdio.h>\n");
  fprintf(dst, "#include <string.h>\n");
  if(CheckMathLib()) {
    fprintf(dst, "#include <math.h>\n\n");
  }
  // format defines
  for(auto& lib : lib_map_){
    if(lib.second.called_){
      fprintf(dst, "#define %s\n", lib.second.def_.c_str());
    }
  }
}

bool PStdLibs::CheckMathLib() {
  return lib_map_["sin"].called_ || lib_map_["cos"].called_
      || lib_map_["exp"].called_ || lib_map_["ln"].called_
      || lib_map_["sqrt"].called_ || lib_map_["arctan"].called_;
}



} // namespace pstdlib