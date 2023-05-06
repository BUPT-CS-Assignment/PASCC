//
// Created by jianxff on 2023/4/29.
//
#include "type.h"
using std::string;
using std::vector;

namespace pascals{

pBasicType compute(pType t1, pType t2, string op){
  if(t1 == nullptr || t2 == nullptr) return TYPE_ERROR;
  if(t1->template_type() != TypeTemplate::TYPE::BASIC ||
     t2->template_type() != TypeTemplate::TYPE::BASIC) return TYPE_ERROR;

  pBasicType tb1 = std::dynamic_pointer_cast<BasicType>(t1);
  pBasicType tb2 = std::dynamic_pointer_cast<BasicType>(t2);
  auto res = operation_map.find(Operation(tb1,tb2,op));
  if(res != operation_map.end()) {
    return (*res).second;
  } else {
    return TYPE_ERROR;
  }
}

bool is_basic(pType t){
  if(t == nullptr) return false;
  if(t->template_type() != TypeTemplate::TYPE::BASIC) return false;
  return is_same(t,TYPE_INT) ||
         is_same(t, TYPE_REAL) ||
         is_same(t,TYPE_BOOL) ||
         is_same(t, TYPE_CHAR);
}

bool is_same(pType t1, pType t2){
  if(t1.get() == t2.get()) return true;
  if(t1 == nullptr || t2 == nullptr) return false;
  if(t1->template_type() != t2->template_type()) return false;
  if(t1->template_type() == TypeTemplate::TYPE::ARRAY) {
    auto a1 = t1->DynamicCast<ArrayType>(),
         a2 = t2->DynamicCast<ArrayType>();
    return (*a1) == (*a2);
  }
  return false;
}

bool is_same(pArrayType t1, int vdim1, pArrayType t2, int vdim2){
  if(t1 == nullptr || t2 == nullptr) return false;
  if(t1->base_type() != t2->base_type()) return false;
  pArrayType temp_t1 = t1->Visit(vdim1);
  pArrayType temp_t2 = t2->Visit(vdim2);
  return is_same(temp_t1, temp_t2);
}

bool is_same(pArrayType t1, int vdim, pBasicType t2){
  if(t1 == nullptr || t2 == nullptr) return false;
  pArrayType temp_t1 = t1->Visit(vdim);
  pArrayType temp_t2 = std::make_shared<ArrayType>(t2);
  return is_same(temp_t1, temp_t2);
}

bool is_same(pRecordType t1, vector<string> n1, pRecordType t2, vector<string> n2){
  pType temp_t1 = t1->Visit(n1);
  pType temp_t2 = t2->Visit(n2);
  return is_same(temp_t1, temp_t2);
}

bool is_same(pRecordType t1, vector<string> n1, pType t2) {
  pType temp_t1 = t1->Visit(n1);
  return is_same(temp_t1, t2);
}

}// namespace pascal_type