//
// Created by jianxff on 2023/4/24.
//
#include <iostream>
#include <vector>
#include <unordered_map>

#include "symbol_table.h"

#ifndef PASCC_PSTDLIB_H
#define PASCC_PSTDLIB_H

namespace pascals{
class PStdLibs{
public:
  PStdLibs();
  ~PStdLibs(){};
  void Format(FILE* dst);
  void Preset(SymbolTable* st);
  bool Call(std::string lib_name);
  bool UnCall(std::string lib_name);
private:
  bool CheckMathLib();
  struct Lib{
    std::string def_;
    bool called_;
  };
  std::unordered_map<std::string, Lib> lib_map_;
};

}// namespace pascals

#endif //PASCC_PAS_STDLIB_H
