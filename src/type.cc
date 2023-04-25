#include "type.h"
#include "log.h"

using std::vector;
using std::string;
using namespace pascal_type;

ConstValue::ConstValue(const ConstValue& other) {
  m_Type = other.m_Type;
  m_INT = other.m_INT;
  m_REAL = other.m_REAL;
  m_CHAR = other.m_CHAR;
  m_BOOLEAN = other.m_BOOLEAN;
  m_STRING = other.m_STRING;
}

ConstValue& ConstValue::operator=(const ConstValue& other) {
  m_Type = other.m_Type;
  m_INT = other.m_INT;
  m_REAL = other.m_REAL;
  m_CHAR = other.m_CHAR;
  m_BOOLEAN = other.m_BOOLEAN;
  m_STRING = other.m_STRING;
  return *this;
}

// operation +
ConstValue ConstValue::operator+(const ConstValue& other) {
  if(m_Type != other.m_Type) throw std::runtime_error("ConstValue : operator+ : type not match");
  ConstValue ret;
  if (m_Type == TYPE_INT) {
    ret.set((int)(m_INT + other.m_INT));
    return ret;
  } else if (m_Type == TYPE_REAL) {
    ret.set((float)(m_REAL + other.m_REAL));
    return ret;
  } else if (m_Type == pascal_type::TYPE_STRING) {
    ret.set(m_STRING + other.m_STRING);
    return ret;
  } else if(m_Type == pascal_type::TYPE_CHAR){
    ret.set((char)((int)m_CHAR + (int)other.m_CHAR));
    return ret;
  } else {
    throw std::runtime_error("ConstValue : operator+ : type not supported");
  }
}

// operation -
ConstValue ConstValue::operator-(const ConstValue& other) {
  if(m_Type != other.m_Type) throw std::runtime_error("ConstValue : operator- : type not match");
  ConstValue ret;
  if (m_Type == pascal_type::TYPE_INT) {
    ret.set((int)(m_INT - other.m_INT));
    return ret;
  } else if (m_Type == pascal_type::TYPE_REAL) {
    ret.set((float)(m_REAL - other.m_REAL));
    return ret;
  } else if(m_Type == pascal_type::TYPE_CHAR){
    ret.set((char)((int)m_CHAR - (int)other.m_CHAR));
    return ret;
  } else {
    throw std::runtime_error("ConstValue : operator- : type not supported");
  }
}

// operation *
ConstValue ConstValue::operator*(const ConstValue& other) {
  if(m_Type != other.m_Type) throw std::runtime_error("ConstValue : operator* : type not match");
  ConstValue ret;
  if (m_Type == pascal_type::TYPE_INT) {
    ret.set((int)(m_INT * other.m_INT));
    return ret;
  } else if (m_Type == pascal_type::TYPE_REAL) {
    ret.set((float)(m_REAL * other.m_REAL));
    return ret;
  } else {
    throw std::runtime_error("ConstValue : operator* : type not supported");
  }
}

// operation /
ConstValue ConstValue::operator/(const ConstValue& other) {
  if(m_Type != other.m_Type) throw std::runtime_error("ConstValue : operator* : type not match");
  ConstValue ret;
  if (m_Type == pascal_type::TYPE_INT) {
    ret.set((int)(m_INT / other.m_INT));
    return ret;
  } else if (m_Type == pascal_type::TYPE_REAL) {
    ret.set((float)(m_REAL / other.m_REAL));
    return ret;
  } else {
    throw std::runtime_error("ConstValue : operator* : type not supported");
  }
}



namespace pascal_type {

bool TypeTemplate::ComputeType(TypeTemplate* type1, TypeTemplate* type2, std::string op,
                               TypeTemplate** result_type) {
  if (operation_map.find(Operation(type1, type2, op)) != operation_map.end()) {
    if (result_type != nullptr) {
      *result_type = operation_map[Operation(type1, type2, op)];
    }
    return true;
  } else return false;
}

bool TypeTemplate::TypeEqual(TypeTemplate* type1, TypeTemplate* type2) {
  if (type1 == type2) {
    return true;
  } else if (type1->template_type_ == TYPE::ARRAY && type2->template_type_ == TYPE::ARRAY) {
    auto array1 = type1->DynamicCast<ArrayType>();
    auto array2 = type2->DynamicCast<ArrayType>();
    auto array1_bounds = array1->bounds();
    auto array2_bounds = array2->bounds();
    if(array1_bounds->size() != array2_bounds->size()) {
      return false;
    }
    for(int i = 0; i < array1_bounds->size(); i++) {
      if((*array1_bounds)[i].first != (*array2_bounds)[i].first ||
         (*array1_bounds)[i].second != (*array2_bounds)[i].second) {
        return false;
      }
    }
    return TypeEqual(array1->type(), array2->type());
  } else return false;
}

bool TypeTemplate::StringLike() {
  if(template_type_ == TYPE::ARRAY) {
    auto array = DynamicCast<ArrayType>();
    return array->StringLike();
  }
  return false;
}

bool ArrayType::StringLike(int access_layer) {
  if (type_ != pascal_type::TYPE_CHAR) return false;
  if (bounds_.size() != access_layer + 1) return false;
  return true;
}

bool ArrayType::AccessArray(vector<TypeTemplate*> index_types, TypeTemplate **type) {
  if (index_types.size() != bound_types_.size()) return false;
  for (int i = 0; i < index_types.size(); i++) {
    if (index_types[i] != bound_types_[i]) return false;
  }
  if(type != nullptr) *type = type_;
  return true;
}


void RecordType::InsertType(std::string name, TypeTemplate* type) {
  types_map_.insert(std::make_pair(name, type));
  types_num_++;
}

TypeTemplate* RecordType::Find(std::string name) {
  auto ptr = types_map_.find(name);
  if(ptr != types_map_.end()) return (*ptr).second;
  else return nullptr;
}


/* **************** global initialize **************** */
BasicType* TYPE_INT;
BasicType* TYPE_REAL;
BasicType* TYPE_BOOL;
BasicType* TYPE_CHAR;
BasicType* TYPE_STRING;
BasicType* TYPE_ERROR;

OperationMap operation_map;

void TypeInit() {
  log_info("initializing pascal_type ...");

  TYPE_BOOL = new BasicType(BasicType::BASIC_TYPE::BOOL);
  TYPE_CHAR = new BasicType(BasicType::BASIC_TYPE::LETTER);
  TYPE_INT = new BasicType(BasicType::BASIC_TYPE::INT);
  TYPE_REAL = new BasicType(BasicType::BASIC_TYPE::REAL);

  //bool
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "and")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "or")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, NULL, "not")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "<")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_BOOL, TYPE_BOOL, ">=")] = TYPE_BOOL;
  // int
  operation_map[Operation(TYPE_INT, TYPE_INT, "+")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "-")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "*")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "/")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, TYPE_INT, "mod")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "div")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, TYPE_INT, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, "<")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_INT, ">=")] = TYPE_BOOL;
  //real
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "+")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "-")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "*")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "/")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "<")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, ">=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_REAL, "<=")] = TYPE_BOOL;
  //char
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, "<")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_CHAR, TYPE_CHAR, ">=")] = TYPE_BOOL;
  // int real
  operation_map[Operation(TYPE_REAL, TYPE_INT, "+")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "-")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "*")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "/")] = TYPE_REAL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, ">=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_REAL, TYPE_INT, "<")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "+")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "-")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "*")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "/")] = TYPE_REAL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "<>")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, ">")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, ">=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "<=")] = TYPE_BOOL;
  operation_map[Operation(TYPE_INT, TYPE_REAL, "<")] = TYPE_BOOL;
}
int _ = (TypeInit(), 0);

} // namespace pascal_type