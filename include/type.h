#ifndef PASCC_TYPE_H_
#define PASCC_TYPE_H_

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <type_traits>

namespace pascal_type {

class TypeTemplate {
 public:
  enum class TYPE {
    BASIC,
    RECORD,
    ARRAY
  };
  TypeTemplate() {}
  TypeTemplate(TYPE template_type) : template_type_(template_type) {}
  ~TypeTemplate() {}
  TYPE template_type() { return template_type_; }
 protected:
  TYPE template_type_;
};

// basic type including INT, REAL, CHAR, BOOL
class BasicType : public TypeTemplate {
 public:
  BasicType() {}
  BasicType(TypeTemplate* type) : TypeTemplate(TYPE::BASIC) {}
  ~BasicType() {}
};

// Array type
class ArrayType : public TypeTemplate {
 public:
  ArrayType() {}
  ArrayType(TypeTemplate* type, std::vector<std::pair<int, int>> bounds)
    : TypeTemplate(TYPE::ARRAY), type_(type), bounds_(bounds) {}
  ~ArrayType() {}
  TypeTemplate* type() { return type_; }
  int dimension() { return bounds_.size(); }
  std::vector<std::pair<int, int>> *bounds() { return &bounds_; }
  bool AccessArray(std::vector<TypeTemplate*> index_types, TypeTemplate *type);
 private:
  TypeTemplate* type_;
  std::vector<std::pair<int, int>> bounds_;
  std::vector<TypeTemplate*> bound_types_;
};

// Record type
class RecordType : public TypeTemplate {
 public:
  RecordType() {}
  RecordType(std::vector<TypeTemplate*>& types)
    : TypeTemplate(TYPE::RECORD), types_(types) {
    types_num_ = types.size();
  }
  ~RecordType() {}
  
  TYPE TypeAt(int pos) {
    return types_[pos]->template_type();
  }

  TypeTemplate* TypePtrAt(int pos) {
    return types_[pos];
  }

 private:
  std::vector<TypeTemplate*> types_;
  int types_num_;
};


// Common object symbol for variables
class ObjectSymbol {
 public:
  ObjectSymbol () {}
  ObjectSymbol (std::string name, TypeTemplate* type, int decl_line, int ref_line)
    : name_(name), type_(type), decl_line_(decl_line), ref_line_(ref_line) {}
  ~ObjectSymbol() {}
  std::string name() { return name_; }
  TypeTemplate* type() { return type_; }
  int decl_line() { return decl_line_; }
  int ref_line() { return ref_line_; }
 protected:
  std::string name_;
  TypeTemplate* type_;
  int decl_line_;
  int ref_line_;
 private:
  /* data */
};

// Object symbol for const variables
class ConstSymbol : public ObjectSymbol {
 public:
  ConstSymbol() {}
  ConstSymbol(std::string name, TypeTemplate* type, int decl_line, int ref_line, int value)
    : ObjectSymbol(name, type, decl_line, ref_line) {
    value_.num = value;
  }
  ConstSymbol(std::string name, TypeTemplate* type, int decl_line, int ref_line, char value)
    : ObjectSymbol(name, type, decl_line, ref_line) {
    value_.letter = value;
  }

  ~ConstSymbol() {}

  // get value by int or char type
  template <typename T> T value() {
    if(std::is_same<T, int>::value) {
      return value_.num;
    } else if (std::is_same<T, char>::value) {
      return value_.letter;
    } else {
//      std::cout << "Error: ConstSymbol::value() type error" << std::endl;
//      std::abort();
      return T();
    }
  }

 private:
  union CONST_VALUE {
    int num;
    char letter;
  } value_;
};


class FunctionSymbol : public ObjectSymbol {
 public:
  enum class PARAM_PASSING {
    BY_VALUE,
    BY_REFERENCE,
  }; // 传参方式

  typedef std::pair<BasicType*, PARAM_PASSING> Parameter; 

	FunctionSymbol() {}
  FunctionSymbol(std::string name, BasicType *return_type,
                 int decl_line, int ref_line,
                 std::vector<Parameter> params)
    : ObjectSymbol(name, return_type, decl_line, ref_line), params_(params) {}

  ~FunctionSymbol() {}
  // get parameters size
  int param_size() { return params_.size(); }

  // get params at specific position
  Parameter* ParamAt(int pos) { return &params_[pos]; }

  // passing parameter assertion
  bool AssertParams(const std::vector<BasicType*>& params);

 private:
  std::vector<Parameter> params_;
};

}; // namespace pascal_type

#endif // PASCC_TYPE_H_