//
// Created by jianxff on 2023/3/27.
//

#include "symbol.h"

#include <cstdio>

#include "type.h"
using std::string;
using std::vector;

namespace pascals {

FunctionSymbol::FunctionSymbol(const FunctionSymbol &f)
    : ObjectSymbol(f.name_, f.type_, f.decl_line_) {
  params_.assign(f.params_.begin(), f.params_.end());
  for (auto it = f.param_map_.begin(); it != f.param_map_.end(); ++it) {
    param_map_[it->first] = it->second;
  }
  symbol_type_ = SYMBOL_TYPE::FUNCTION;
}

FunctionSymbol::FunctionSymbol(std::string name, BasicType *return_type,
                               int decl_line,
                               const std::vector<Parameter> &params)
    : ObjectSymbol(name, return_type, decl_line), params_(params) {
  for (int i = 0; i < params.size(); i++) {
    param_map_[params[i].first] = i;
  }
  symbol_type_ = SYMBOL_TYPE::FUNCTION;
}

FunctionSymbol::FunctionSymbol(std::string name, BasicType *return_type,
                               int decl_line)
    : ObjectSymbol(name, return_type, decl_line) {
  symbol_type_ = SYMBOL_TYPE::FUNCTION;
}

bool FunctionSymbol::InsertParam(Parameter &p) {
  auto ptr = param_map_.find(p.first);
  if (ptr == param_map_.end()) {
    params_.emplace_back(p);
    param_map_[p.first] = params_.size() - 1;
    return true;
  } else {
    return false;
  }
}

FunctionSymbol::ParamType *FunctionSymbol::operator[](std::string name) {
  auto ptr = param_map_.find(name);
  if (ptr == param_map_.end()) {
    return nullptr;
  } else {
    return &(params_[ptr->second].second);
  }
}

bool FunctionSymbol::IsReference(std::string name) {
  auto ptr = (*this)[name];
  if (ptr == nullptr) {
    return false;
  } else {
    return ptr->second == PARAM_MODE::REFERENCE;
  }
}

std::string FunctionSymbol::ParamName() {
   std::string res = "";
   for(int i = 0; i < params_.size(); i++){
       res += type_name(params_[i].second.first);
       if(params_[i].second.second == PARAM_MODE::REFERENCE) res += "&";
       if(i != params_.size() - 1) res += ", ";
   }
   return res;
}

bool FunctionSymbol::AssertParams(const vector<TypeTemplate *> &params_in,
                                  const vector<bool> value_type_in) {
  if (params_in.size() != params_.size() ||
      params_in.size() != value_type_in.size()) {
    return false;
  }

  for (int i = 0; i < params_in.size(); i++) {
    // assert parameter type
    if (params_in[i] != params_[i].second.first ||
        (value_type_in[i] == false &&
         params_[i].second.second == PARAM_MODE::REFERENCE)) {
      return false;
    }
  }
  return true;
}

bool FunctionSymbol::AssertParams() { return params_.size() == 0; }

std::vector<bool> FunctionSymbol::ParamRefVec() {
  std::vector<bool> res;
  for (auto &p : params_) {
    res.push_back(p.second.second == PARAM_MODE::REFERENCE);
  }
  return res;
}

}  // namespace pascals