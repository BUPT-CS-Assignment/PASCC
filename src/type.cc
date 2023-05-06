#include "type.h"
#include "log.h"

using std::vector;
using std::string;
using namespace pascals;

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
  if (is_same(m_Type, TYPE_INT)) {
    ret.set((int)(m_INT - other.m_INT));
    return ret;
  } else if (is_same(m_Type, TYPE_REAL)) {
    ret.set((float)(m_REAL - other.m_REAL));
    return ret;
  } else if(is_same(m_Type, TYPE_CHAR)){
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
  if (is_same(m_Type, TYPE_INT)) {
    ret.set((int)(m_INT * other.m_INT));
    return ret;
  } else if (is_same(m_Type, TYPE_REAL)) {
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
  if (is_same(m_Type, TYPE_INT)) {
    ret.set((int)(m_INT / other.m_INT));
    return ret;
  } else if (is_same(m_Type, TYPE_REAL)) {
    ret.set((float)(m_REAL / other.m_REAL));
    return ret;
  } else {
    throw std::runtime_error("ConstValue : operator* : type not supported");
  }
}


namespace pascals {
ArrayType::ArrayBound& ArrayType::ArrayBound::operator=(const ArrayBound& b2) {
  type_ = b2.type_;
  lb_ = b2.lb_;
  ub_ = b2.ub_;
  return *this;
}

bool ArrayType::ArrayBound::operator==(const pascals::ArrayType::ArrayBound &b2) const {
  return type_.get() == b2.type_.get() &&
         lb_ == b2.lb_ &&
         ub_ == b2.ub_;
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

pArrayType ArrayType::Visit(std::vector<pType>& v_types) {
  if(v_types.size() == 0 ) return std::make_shared<ArrayType>(*this);
  if(v_types.size() > dims()) return std::make_shared<ArrayType>(TYPE_ERROR);
  for(int i = 0; i < v_types.size(); i++) {
    bool same_type = is_same(bounds_[i].type_, v_types[i]);
    if(!same_type) return std::make_shared<ArrayType>(TYPE_ERROR);
  }
  return Visit(v_types.size());
}

pArrayType ArrayType::Visit(unsigned int v_layer) {
  if(v_layer == 0 ) return std::make_shared<ArrayType>(*this);
  if(v_layer > dims()) return std::make_shared<ArrayType>(TYPE_ERROR);
  // temp array
  vector<ArrayBound> bs;
  for(int i = v_layer; i < bounds_.size(); i++) {
    bs.emplace_back(bounds_[i]);
  }
  return std::make_shared<ArrayType>(base_type_, bs);
}

bool ArrayType::operator==(const ArrayType &a2) const{
  if(!is_same(base_type_, a2.base_type_)) return false;
  int dims = bounds_.size();
  if(dims != a2.bounds_.size()) return false;
  for(int i = 0; i < dims; i++) {
    if(!(bounds_[i] == a2.bounds_[i])) return false;
  }
  return true;
}

pType RecordType::Visit(std::vector<std::string>& names) {
  pType vtype = std::make_shared<RecordType>(*this);
  int loop = 0, len = names.size();
  while(loop < len){
    // cast assert
    if(vtype->template_type() != TYPE::RECORD) return nullptr;
    // find element
    auto t = vtype->DynamicCast<RecordType>()->Find(names[loop]);
    if(t == nullptr) return nullptr;
    // next
    vtype.reset();
    vtype = t;
    loop++;
  }
  return vtype;
}

bool TypeTemplate::StringLike() {
  if(template_type_ == TYPE::ARRAY) {
    auto array = DynamicCast<ArrayType>();
    return array->StringLike();
  }
  return false;
}

bool ArrayType::StringLike(int access_layer) {
  if (base_type_ != TYPE_CHAR) return false;
  if (bounds_.size() != access_layer + 1) return false;
  return true;
}


bool RecordType::add(std::string name, pType type) {
  if(types_map_.find(name) != types_map_.end()) return false;
  types_map_[name] = type;
  return true;
}

pType RecordType::Find(std::string name) {
  auto ptr = types_map_.find(name);
  if(ptr != types_map_.end()) return (*ptr).second;
  else return nullptr;
}


/* **************** global initialize **************** */
pBasicType TYPE_INT;
pBasicType TYPE_REAL;
pBasicType TYPE_BOOL;
pBasicType TYPE_CHAR;
pBasicType TYPE_NONE;
pBasicType TYPE_ERROR;
pBasicType TYPE_STRINGLIKE;




OperationMap operation_map;

void TypeInit() {
//  log_info("initializing pascal_type ...");
  TYPE_ERROR = nullptr;

  TYPE_BOOL = std::make_shared<BasicType>(BasicType::BASIC_TYPE::BOOL);
  TYPE_CHAR = std::make_shared<BasicType>(BasicType::BASIC_TYPE::CHAR);
  TYPE_INT = std::make_shared<BasicType>(BasicType::BASIC_TYPE::INT);
  TYPE_REAL = std::make_shared<BasicType>(BasicType::BASIC_TYPE::REAL);
  TYPE_NONE = std::make_shared<BasicType>(BasicType::BASIC_TYPE::NONE);
  TYPE_STRINGLIKE = std::make_shared<BasicType>(BasicType::BASIC_TYPE::CHAR);

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
  operation_map[Operation(TYPE_INT, NULL, "-")] = TYPE_INT;
  operation_map[Operation(TYPE_INT, NULL, "+")] = TYPE_INT;
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