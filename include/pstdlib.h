//
// Created by jianxff on 2023/4/24.
//
#include <iostream>
#include <unordered_map>
#include <vector>

#include "symbol_table.h"

#ifndef PASCC_PSTDLIB_H
#define PASCC_PSTDLIB_H

namespace pascals {

/**
 * @brief standard library for pascal-s
 */
class PStdLibs {
 public:
  PStdLibs();
  ~PStdLibs(){};
  // set call
  bool Call(std::string lib_name);
  bool UnCall(std::string lib_name);

  // pre-insert to table
  void Preset(SymbolTable* st);

  // static formater
  void Format(FILE* dst);

 private:
  bool CheckMathLib();
  struct Lib {
    std::string def_;
    bool called_;
  };
  std::unordered_map<std::string, Lib> lib_map_;
};

}  // namespace pascals

#endif  // PASCC_PAS_STDLIB_H
