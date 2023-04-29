//
// Created by jianxff on 2023/4/29.
//
#include "type.h"
using std::string;
using std::vector;

namespace pascal_type{

BasicType* compute(BasicType* t1, BasicType* t2, string op){
  if(t1 == nullptr || t2 == nullptr) return TYPE_ERROR;
  auto res = operation_map.find(Operation(t1,t2,op));
  if(res != operation_map.end()) {
    return (*res).second;
  } else {
    return TYPE_ERROR;
  }
}

bool is_basic(TypeTemplate* t){
  if(t == nullptr) return false;
  if(t->template_type() != TypeTemplate::TYPE::BASIC) return false;
  return t == TYPE_INT || t == TYPE_REAL || t == TYPE_BOOL || t == TYPE_CHAR;
}

bool is_same(TypeTemplate* t1, TypeTemplate* t2){
  if(t1 == t2) return true;
  if(t1 == nullptr || t2 == nullptr) return false;
  if(t1->template_type() != t2->template_type()) return false;
  if(t1->template_type() == TypeTemplate::TYPE::ARRAY) {
    auto a1 = t1->DynamicCast<ArrayType>(),
         a2 = t2->DynamicCast<ArrayType>();
    return (*a1) == (*a2);
  }
  return false;
}

bool is_same(ArrayType* t1, int vdim1, ArrayType* t2, int vdim2){
  if(t1 == nullptr || t2 == nullptr) return false;
  if(t1->base_type() != t2->base_type()) return false;
  ArrayType temp_t1 = t1->Visit(vdim1);
  ArrayType temp_t2 = t2->Visit(vdim2);
  return temp_t1 == temp_t2;
}

bool is_same(ArrayType* t1, int vdim, BasicType* t2){
  if(t1 == nullptr || t2 == nullptr) return false;
  ArrayType temp_t1 = t1->Visit(vdim);
  ArrayType temp_t2 = ArrayType(t2);
  return temp_t1 == temp_t2;
}

bool is_same(RecordType* t1, vector<string> n1, RecordType* t2, vector<string> n2){
  TypeTemplate* temp_t1 = t1->Visit(n1);
  TypeTemplate* temp_t2 = t2->Visit(n2);
  return is_same(temp_t1, temp_t2);
}

bool is_same(RecordType* t1, vector<string> n1, TypeTemplate* t2) {
  TypeTemplate *temp_t1 = t1->Visit(n1);
  return is_same(temp_t1, t2);
}

}// namespace pascal_type