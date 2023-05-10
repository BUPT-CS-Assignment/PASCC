//
// Created by jianxff on 2023/3/27.
//

#ifndef PASCC_SYMBOL_H
#define PASCC_SYMBOL_H

#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "type.h"

namespace pascals {

/**
 * @brief object symbol
 */
class ObjectSymbol {
 public:
  enum class SYMBOL_TYPE {
    VAR,
    CONST,
    FUNCTION,
  };
  ObjectSymbol() {}
  ObjectSymbol(std::string name, TypeTemplate* type, int decl_line,
               bool is_ref = false)
      : name_(name), type_(type), decl_line_(decl_line), is_ref_(is_ref) {
    symbol_type_ = SYMBOL_TYPE::VAR;
  }
  virtual ~ObjectSymbol() {}

  // getter and setter
  std::string name() { return name_; }
  TypeTemplate* type() { return type_; }
  int decl_line() { return decl_line_; }
  virtual void set_ref(bool r) { is_ref_ = r; }
  virtual bool is_ref() { return is_ref_; }
  SYMBOL_TYPE symbol_type() { return symbol_type_; }

 protected:
  std::string name_;
  TypeTemplate* type_;
  bool is_ref_;
  int decl_line_;
  SYMBOL_TYPE symbol_type_;
};

/**
 * @brief const symbol
 */
class ConstSymbol : public ObjectSymbol {
 public:
  ConstSymbol() {}
  ConstSymbol(std::string name, ConstValue value, int decl_line)
      : ObjectSymbol(name, value.type(), decl_line), value_(value) {
    symbol_type_ = SYMBOL_TYPE::CONST;
  }

  ~ConstSymbol() {}

  // getter and stter
  ConstValue value() { return value_; }
  BasicType* type() { return value_.type(); }
  void set_ref(bool r) override { is_ref_ = false; }
  bool is_ref() override { return false; }

 private:
  ConstValue value_;
};

/**
 * @brief function symbol
 */
class FunctionSymbol : public ObjectSymbol {
 public:
  enum class PARAM_MODE {
    VALUE,
    REFERENCE,
  };

  typedef std::pair<BasicType*, PARAM_MODE> ParamType;
  typedef std::pair<std::string, ParamType> Parameter;

  FunctionSymbol() {}
  FunctionSymbol(const FunctionSymbol& f);
  FunctionSymbol(std::string name, BasicType* return_type, int decl_line,
                 const std::vector<Parameter>& params);
  FunctionSymbol(std::string name, BasicType* return_type, int decl_line);

  // get parameters size
  int param_size() { return params_.size(); }
  // get params at specific position
  const Parameter* ParamAt(int pos) { return &params_[pos]; }
  std::string ParamName();
  bool InsertParam(Parameter&);

  // passing parameter assertion
  bool AssertParams(const std::vector<TypeTemplate*>& params,
                    const std::vector<bool> value_type_in);
  bool AssertParams();
  // get param type
  ParamType* operator[](std::string);
  // check ref
  bool IsReference(std::string);
  void set_ref(bool r) override { is_ref_ = false; }
  bool is_ref() override { return false; }
  std::vector<bool> ParamRefVec();

 private:
  std::vector<Parameter> params_;
  std::unordered_map<std::string, int> param_map_;
};

}  // namespace pascals

#endif  // PASCC_SYMBOL_H
