#include "type.h"

using std::vector;

namespace pascal_type {

bool ArrayType::AccessArray(vector<TypeTemplate*> index_types, TypeTemplate *type) {
  if (index_types.size() != bound_types_.size()) {
    return false;
  }
  for (int i = 0; i < index_types.size(); i++) {
    if (index_types[i] != bound_types_[i]) {
      return false;
    }
  }
  type = type_;
  return true;
}

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

} // namespace pascal_type