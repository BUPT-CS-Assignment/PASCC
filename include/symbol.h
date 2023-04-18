//
// Created by jianxff on 2023/3/27.
//

#ifndef PASCC_SYMBOL_H
#define PASCC_SYMBOL_H

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "type.h"

namespace pascal_symbol {
// Const value
class ConstValue {
public:
  ConstValue() : type_(pascal_type::TYPE_INT) {value_.num_int = 0;}
  ConstValue(int value) : type_(pascal_type::TYPE_INT) {value_.num_int = value;}
  ConstValue(char value) : type_(pascal_type::TYPE_CHAR) {value_.letter = value;}
  ConstValue(float value) : type_(pascal_type::TYPE_REAL) {value_.num_float = value;}
  ConstValue(bool value) : type_(pascal_type::TYPE_BOOL) {value_.bool_val = value;}
  ~ConstValue() {}

  pascal_type::BasicType* type() {return type_;}
  template <typename T> T get() {
    if(std::is_same<T, int>::value) return value_.num_int;
    else if (std::is_same<T, char>::value)  return value_.letter;
    else if (std::is_same<T, float>::value) return value_.num_float;
    else if (std::is_same<T, bool>::value)  return value_.bool_val;
    else  return T();
  }

private:
  union {
    bool bool_val;
    char letter;
    float num_float;
    int num_int;
  } value_;
  pascal_type::BasicType* type_;
};



// Object Symbols for variables
class ObjectSymbol {
public:
  ObjectSymbol () {}
  ObjectSymbol (std::string name, pascal_type::TypeTemplate* type, int decl_line)
      : name_(name), type_(type), decl_line_(decl_line){}
  ~ObjectSymbol() {}

  std::string name() {return name_;}
  pascal_type::TypeTemplate* type() { return type_; }
  int decl_line() { return decl_line_; }
  std::vector<int> ref_lines() { return ref_lines_; }

  void InsertRefLine(int ref_line) { ref_lines_.emplace_back(ref_line); }

protected:
  std::string name_;
  pascal_type::TypeTemplate* type_;
  int decl_line_;
  std::vector<int> ref_lines_;
};




// Object symbol for const variables
class ConstSymbol : public ObjectSymbol {
public:
  ConstSymbol() {}
  ConstSymbol(std::string name, int decl_line, int value)
      : ObjectSymbol(name, pascal_type::TYPE_INT, decl_line), value_(value) {}
  ConstSymbol(std::string name, int decl_line, char value)
      : ObjectSymbol(name, pascal_type::TYPE_CHAR, decl_line), value_(value)  {}
  ConstSymbol(std::string name, int decl_lint, float value)
      : ObjectSymbol(name, pascal_type::TYPE_REAL, decl_lint), value_(value) {}
  ConstSymbol(std::string name, int decl_lint, bool value)
      : ObjectSymbol(name, pascal_type::TYPE_BOOL, decl_lint), value_(value)  {}

  ~ConstSymbol() {}

  // get value by int or char type
  template <typename T> T value() { return value_.get<T>(); }
  // get value type
  pascal_type::BasicType* type() { return value_.type(); }

private:
  ConstValue value_;
};


// function symbols
class FunctionSymbol : public ObjectSymbol {
public:
  enum class PARAM_MODE {
    VALUE,
    REFERENCE,
  };

  typedef std::pair<pascal_type::BasicType*, PARAM_MODE> ParamType;
  typedef std::pair<std::string, ParamType> Parameter;

  FunctionSymbol() {}
  FunctionSymbol(std::string name, pascal_type::BasicType *return_type, int decl_line,
                 const std::vector<Parameter>& params);

  // get parameters size
  int param_size() { return params_.size(); }
  // get params at specific position
  const Parameter* ParamAt(int pos) { return &params_[pos]; }
  bool InsertParam(Parameter&);

  // passing parameter assertion
  bool AssertParams(const std::vector<pascal_type::BasicType*>& params);
  // get param type
  ParamType* operator[](std::string );
  // check ref
  bool IsReference(std::string);

private:
  std::vector<Parameter> params_;
  std::unordered_map<std::string, int> param_map_;
};



}

#endif // PASCC_SYMBOL_H
