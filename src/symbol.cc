//
// Created by jianxff on 2023/3/27.
//

#include "symbol.h"
#include "type.h"
#include <cstdio>
using namespace pascal_type;
using std::string;
using std::vector;

namespace pascal_symbol {

FunctionSymbol::FunctionSymbol(std::string name, pascal_type::BasicType *return_type, int decl_line,
                               const std::vector<Parameter> &params)
    : ObjectSymbol(name, return_type, decl_line), params_(params) {
  for(int i = 0; i < params.size(); i++) {
    param_map_[params[i].first] = i;
  }
  symbol_type_ = SYMBOL_TYPE::FUNCTION;
}

FunctionSymbol::FunctionSymbol(std::string name, pascal_type::BasicType *return_type, int decl_line)
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

FunctionSymbol::ParamType* FunctionSymbol::operator[](std::string name) {
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

bool FunctionSymbol::AssertParams(const vector<TypeTemplate *> &params_in,
                                  const vector<bool> value_type_in) {
  if (params_in.size() != params_.size() || params_in.size() != value_type_in.size()) {
    return false;
  }

  for (int i = 0; i < params_in.size(); i++) {
    // assert parameter type
    if (params_in[i] != params_[i].second.first ||
        (value_type_in[i] == false &&
         params_[i].second.second == PARAM_MODE::REFERENCE)) {
      //    std::cout << params_in[i] << params_[i].second.first << std::endl;
      return false;
    }
  }
  return true;
}
}