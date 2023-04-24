#ifndef PARSER_HPP
#define PARSER_HPP
#include <string.h>

#include <iostream>
#include <string>
#include <stack>

#include "compiler.h"
#include "type.h"

// using namespace std;
struct Value {
  int m_INT;
  float m_REAL;
  char m_CHAR;
  bool m_BOOLEAN;
  char m_OTHERS[64];
};

struct Token {
  struct Value value;
  int length;
  int line_num;
  int column_num;
};

struct IdListAttr {
  std::vector<std::pair<std::string, int>>* list_ref;
  ast::IdListNode* id_list_node;
};

struct ValueAttr {
  pascal_type::BasicType* type_ptr;
  Value value;
  ast::LeafNode* const_variable_node;
};

struct StandardTypeAttr {
  pascal_type::BasicType* type_ptr;
  ast::BasicTypeNode* standard_type_node;
};

struct TypeAttr {
  enum MainType { BASIC, ARRAY, RECORD } main_type;
  pascal_type::TypeTemplate* type_ptr;
  pascal_type::TypeTemplate* array_type_ptr;
  ast::TypeNode* type_node;
  std::vector<std::pair<int, int>>* bounds;
  std::unordered_map<std::string, pascal_type::TypeTemplate*>* record_info;
};

struct PeriodsAttr {
  std::vector<std::pair<int, int>>* bounds;
  ast::PeriodsNode* periods_node;
};

struct PeriodAttr {
  std::pair<int, int>* bound;
  ast::PeriodNode* period_node;
};

struct RecordAttr {
  std::unordered_map<std::string, pascal_type::TypeTemplate*>* record_info;
  ast::RecordBodyNode* record_body_node;
};

struct VariableDeclarationAttr {
  std::unordered_map<std::string, pascal_type::TypeTemplate*>* record_info;
  ast::VariableDeclarationNode* variable_declaration_node;
};

struct FormalParameterAttr {
  std::vector<std::pair<std::string, std::pair<pascal_type::BasicType*,pascal_symbol::FunctionSymbol::PARAM_MODE>>>* parameters;
  ast::FormalParamNode* formal_parameter_node;
};

struct ParameterListsAttr {
  std::vector<std::pair<std::string, std::pair<pascal_type::BasicType*,pascal_symbol::FunctionSymbol::PARAM_MODE>>>* parameters;
  ast::ParamListsNode* param_lists_node;
};
struct ParameterListAttr {
  std::vector<std::pair<std::string, std::pair<pascal_type::BasicType*,pascal_symbol::FunctionSymbol::PARAM_MODE>>>* parameters;
  ast::ParamListNode* param_list_node;
};
struct VarParameterAttr {
  std::vector<std::pair<std::string, std::pair<pascal_type::BasicType*,pascal_symbol::FunctionSymbol::PARAM_MODE>>>* parameters;
  ast::VarParamNode* var_parameter_node;
};

struct ValueParameterAttr {
  std::vector<std::pair<std::string, std::pair<pascal_type::BasicType*,pascal_symbol::FunctionSymbol::PARAM_MODE>>>* parameters;
  ast::ValueParamNode* value_parameter_node;
};

struct VariableAttr {
  pascal_type::TypeTemplate* type_ptr;
  ast::VariableNode* variable_node;
  std::string* name;
};

struct ExpressionAttr {
  pascal_type::TypeTemplate* type_ptr;
  ast::ExpressionNode* expression_node;
};

struct SimpleExpressionAttr {
  pascal_type::TypeTemplate* type_ptr;
  ast::SimpleExpressionNode* simple_expression_node;
};

struct TermAttr {
  pascal_type::TypeTemplate* type_ptr;
  ast::TermNode* term_node;
};

struct FactorAttr {
  pascal_type::TypeTemplate* type_ptr;
  ast::FactorNode* factor_node;
};

struct UnsignedConstantVarAttr {
  pascal_type::TypeTemplate* type_ptr;
  ast::UnsignConstVarNode* unsigned_constant_var_node;
};

struct VarParts {
  bool flag;
  std::vector<pascal_type::TypeTemplate*>* subscript;
  // std::string name;
  char name[64];
};

struct IDVarpartsAttr {
  std::vector<VarParts>* var_parts;
  ast::IDVarPartsNode* id_varparts_node;
};

struct IDVarpartAttr {
  VarParts* var_part;
  ast::IDVarPartNode* id_varpart_node;
};

struct ExpressionListAttr {
  std::vector<pascal_type::TypeTemplate*>* type_ptr_list;
  ast::ExpressionListNode* expression_list_node;
};

struct CaseBodyAttr {
  std::vector<pascal_type::TypeTemplate*>* type_ptr_list;
  ast::CaseBodyNode* case_body_node;
};

struct BranchListAttr {
  std::vector<pascal_type::TypeTemplate*>* type_ptr_list;
  ast::BranchListNode* branch_list_node;
};

struct BranchAttr {
  // std::vector<pascal_type::TypeTemplate*>* type_ptr_list;
  pascal_type::TypeTemplate* type_ptr;
  ast::BranchNode* branch_node;
};

struct ConstListAttr {
  // std::vector<pascal_type::TypeTemplate*>* type_ptr_list;
  pascal_type::TypeTemplate* type_ptr;
  ast::ConstListNode* const_list_node;
};

extern std::string buf;

// #define YYSTYPE Token
// #define YYSTYPE
#endif
