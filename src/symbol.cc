//
// Created by jianxff on 2023/3/27.
//

#include "symbol.h"
#include "type.h"
using namespace pascal_type;
using std::string;
using std::vector;

namespace pascal_symbol {

bool FunctionSymbol::AssertParams(const vector<BasicType*>& params_in){
  if (params_in.size() != params_.size()) {
    return false;
  }

  for (int i = 0; i < params_.size(); i++) {
    // assert parameter type
    if(params_in[i] != params_[i].first) {
      return false;
    }
  }
  return true;
}



}