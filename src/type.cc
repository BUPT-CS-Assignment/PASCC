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
  } else if (m_Type == TYPE_STRINGLIKE) {
    ret.set(m_STRING + other.m_STRING);
    return ret;
  } else if(m_Type == TYPE_CHAR){
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
ArrayType::ArrayBound& ArrayType::ArrayBound::operator=(const ArrayBound& b2) {
  type_ = b2.type_;
  lb_ = b2.lb_;
  ub_ = b2.ub_;
  return *this;
}

ArrayType::ArrayType(const ArrayType& other) {
  base_type_ = other.base_type_;
  for(auto& b : other.bounds_) {
    bounds_.emplace_back(b);
  }
}

ArrayType& ArrayType::operator=(const ArrayType& other) {
  base_type_ = other.base_type_;
  for(auto& b : other.bounds_) {
    bounds_.emplace_back(b);
  }
  return *this;
}

ArrayType ArrayType::Visit(std::vector<TypeTemplate *> v_types) {
  if(v_types.size() == 0 ) return *this;
  if(v_types.size() > dims()) return ArrayType(TYPE_ERROR);
  for(int i = 0; i < v_types.size(); i++) {
    if(bounds_[i].type_ != v_types[i]) return ArrayType(TYPE_ERROR);
  }
  return Visit(v_types.size());
}

ArrayType ArrayType::Visit(unsigned int v_layer) {
  if(v_layer == 0 ) return *this;
  if(v_layer > dims()) return ArrayType(TYPE_ERROR);
  // temp array
  vector<ArrayBound> bs;
  for(int i = dims() - v_layer; i < bounds_.size(); i++) {
    bs.emplace_back(bounds_[i]);
  }
  return ArrayType(base_type_, bs);
}

bool ArrayType::operator==(const pascal_type::ArrayType &a2) const{
  if(base_type_ != a2.base_type_) return false;
  int dims = bounds_.size();
  if(dims != a2.bounds_.size()) return false;
  for(int i = 0; i < dims; i++) {
    if(!(bounds_[i] == a2.bounds_[i])) return false;
  }
  return true;
}

TypeTemplate* RecordType::Visit(std::vector<std::string> names) {
  if(names.size() == 0) return this;
  int loop = names.size();
  RecordType* cur_record = this;
  while(loop){
    TypeTemplate* t = cur_record->Find(names[loop - 1]);
    if(t == nullptr) return nullptr;

    if(t->template_type() == TYPE::RECORD) {
      cur_record = t->DynamicCast<RecordType>();
    } else {
      if(loop > 1) return nullptr;
      else return t;
    }
    loop--;
  }
  return nullptr;
}

bool TypeTemplate::StringLike() {
  if(template_type_ == TYPE::ARRAY) {
    auto array = DynamicCast<ArrayType>();
    return array->StringLike();
  }
  return false;
}

bool ArrayType::StringLike(int access_layer) {
  if (base_type_ != pascal_type::TYPE_CHAR) return false;
  if (bounds_.size() != access_layer + 1) return false;
  return true;
}


bool RecordType::add(std::string name, TypeTemplate* type) {
  if(types_map_.find(name) != types_map_.end()) return false;
  types_map_[name] = type;
  return true;
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
BasicType* TYPE_NONE;
BasicType* TYPE_ERROR;
BasicType* TYPE_STRINGLIKE;




OperationMap operation_map;

void TypeInit() {
//  log_info("initializing pascal_type ...");
  TYPE_ERROR = nullptr;

  TYPE_BOOL = new BasicType(BasicType::BASIC_TYPE::BOOL);
  TYPE_CHAR = new BasicType(BasicType::BASIC_TYPE::CHAR);
  TYPE_INT = new BasicType(BasicType::BASIC_TYPE::INT);
  TYPE_REAL = new BasicType(BasicType::BASIC_TYPE::REAL);
  TYPE_NONE = new BasicType(BasicType::BASIC_TYPE::NONE);
  TYPE_STRINGLIKE = new BasicType(BasicType::BASIC_TYPE::CHAR);

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