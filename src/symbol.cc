//
// Created by jianxff on 2023/3/27.
//

#include "symbol.h"
#include "type.h"
#include <cstdio>
using std::string;
using std::vector;

namespace pascals {

FunctionSymbol::FunctionSymbol(std::string name, pBasicType return_type, int decl_line,
                               const std::vector<Parameter> &params)
    : ObjectSymbol(name, return_type, decl_line), params_(params) {
  for(int i = 0; i < params.size(); i++) {
    param_map_[params[i].first] = i;
  }
  symbol_type_ = SYMBOL_TYPE::FUNCTION;
}

FunctionSymbol::FunctionSymbol(std::string name, pBasicType return_type, int decl_line)
    : ObjectSymbol(name, return_type, decl_line) {symbol_type_ = SYMBOL_TYPE::FUNCTION;}

bool FunctionSymbol::InsertParam(Parameter &p) {
  auto ptr = param_map_.find(p.first);
  if(ptr == param_map_.end()) {
    params_.emplace_back(p);
    param_map_[p.first] = params_.size() - 1;
    return true;
  } else {
    return false;
  }
}

const FunctionSymbol::ParamType* FunctionSymbol::operator[](std::string name) {
  auto ptr = param_map_.find(name);
  if(ptr == param_map_.end()) {
    return nullptr;
  } else {
    return &(params_[ptr->second].second);
  }
}

bool FunctionSymbol::IsReference(std::string name) {
  auto ptr = (*this)[name];
  if(ptr == nullptr) {
    return false;
  } else {
    return ptr->second == PARAM_MODE::REFERENCE;
  }
}

bool FunctionSymbol::AssertParams(const vector<pType>& params_in,
                                  const vector<bool>& value_type_in) {
  if (params_in.size() != params_.size() || params_in.size() != value_type_in.size()) {
    return false;
  }

  for (int i = 0; i < params_in.size(); i++) {
    // assert parameter type
    if (!is_same(params_in[i], params_[i].second.first) ||
        (!value_type_in[i] &&
         params_[i].second.second == PARAM_MODE::REFERENCE)) {
      //    std::cout << params_in[i] << params_[i].second.first << std::endl;
      return false;
    }
  }
  return true;
}

} // namespace pascals