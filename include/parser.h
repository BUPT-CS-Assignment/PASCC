#ifndef PASCC_PARSER_H
#define PASCC_PARSER_H

#include <iostream>
#include <stack>
#include <string>

#include "compiler.h"
#include "type.h"
extern int yydebug;

struct Token {
  pascals::ConstValue value;
  int length;
  int line_num;
  int column_num;
};

struct IdListAttr {
  std::vector<std::pair<std::string, int>>* list_ref;
  pascals::ast::IdListNode* id_list_node;
};

struct ValueAttr {
  pascals::BasicType* type_ptr;
  pascals::ConstValue value;
  pascals::ast::LeafNode* const_variable_node;
  bool is_right = true;
};

struct StandardTypeAttr {
  pascals::BasicType* type_ptr;
  pascals::ast::BasicTypeNode* standard_type_node;
};

struct TypeAttr {
  enum MainType { BASIC, ARRAY, RECORD } main_type;
  pascals::TypeTemplate* type_ptr;
  pascals::ast::TypeNode* base_type_node;
  pascals::ast::TypeNode* type_node;
  std::vector<pascals::ArrayType::ArrayBound>* bounds;
  std::unordered_map<std::string, pascals::TypeTemplate*>* record_info;
};

struct PeriodsAttr {
  std::vector<pascals::ArrayType::ArrayBound>* bounds;
  // std::vector<std::pair<int, int>>* bounds;
  pascals::ast::PeriodsNode* periods_node;
};

struct PeriodAttr {
  pascals::ArrayType::ArrayBound* bound;
  // std::pair<int, int>* bound;
  pascals::ast::PeriodNode* period_node;
};

struct RecordAttr {
  std::unordered_map<std::string, pascals::TypeTemplate*>* record_info;
  pascals::ast::RecordBodyNode* record_body_node;
};

struct VariableDeclarationAttr {
  std::unordered_map<std::string, pascals::TypeTemplate*>* record_info;
  pascals::ast::VariableDeclarationNode* variable_declaration_node;
  std::unordered_map<std::string, std::pair<int, int>>* pos_info;
};

struct FormalParameterAttr {
  std::vector<pascals::FunctionSymbol::Parameter>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  pascals::ast::FormalParamNode* formal_parameter_node;
};

struct ParameterListsAttr {
  std::vector<pascals::FunctionSymbol::Parameter>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  pascals::ast::ParamListsNode* param_lists_node;
};
struct ParameterListAttr {
  std::vector<pascals::FunctionSymbol::Parameter>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  pascals::ast::ParamListNode* param_list_node;
};
struct VarParameterAttr {
  std::vector<pascals::FunctionSymbol::Parameter>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  pascals::ast::VarParamNode* var_parameter_node;
};

struct ValueParameterAttr {
  std::vector<pascals::FunctionSymbol::Parameter>* parameters;
  std::vector<std::pair<int, int>>* pos_info;
  pascals::ast::ValueParamNode* value_parameter_node;
};

struct VariableAttr {
  pascals::TypeTemplate* type_ptr;
  pascals::ast::VariableNode* variable_node;
  std::string* name;
  bool is_lvalue;
};

struct VariableListAttr {
  std::vector<pascals::TypeTemplate*>* type_ptr_list;
  pascals::ast::VariableListNode* variable_list_node;
};

struct ExpressionAttr {
  pascals::TypeTemplate* type_ptr;
  pascals::ast::ExpressionNode* expression_node;
  int length;
  bool is_lvalue;
};

struct StrExpressionAttr {
  pascals::TypeTemplate* type_ptr;
  pascals::ast::StrExpressionNode* str_expression_node;
  int length;
  bool is_lvalue;
};

struct SimpleExpressionAttr {
  pascals::TypeTemplate* type_ptr;
  pascals::ast::SimpleExpressionNode* simple_expression_node;
  bool is_lvalue;
};

struct TermAttr {
  pascals::TypeTemplate* type_ptr;
  pascals::ast::TermNode* term_node;
  bool is_lvalue;
};

struct FactorAttr {
  pascals::TypeTemplate* type_ptr;
  pascals::ast::FactorNode* factor_node;
  bool is_lvalue;
};

struct UnsignedConstantVarAttr {
  pascals::TypeTemplate* type_ptr;
  pascals::ast::UnsignConstVarNode* unsigned_constant_var_node;
};

struct VarParts {
  bool flag;
  std::vector<pascals::TypeTemplate*>* subscript;
  std::string name;
};

struct IDVarpartsAttr {
  std::vector<VarParts>* var_parts;
  pascals::ast::IDVarPartsNode* id_varparts_node;
  pascals::TypeTemplate* AccessCheck(pascals::TypeTemplate* base_type);
};

struct IDVarpartAttr {
  VarParts* var_part;
  pascals::ast::IDVarPartNode* id_varpart_node;
};

struct ExpressionListAttr {
  std::vector<pascals::TypeTemplate*>* type_ptr_list;
  pascals::ast::ExpressionListNode* expression_list_node;
  std::vector<bool>* is_lvalue_list;
};

struct CaseBodyAttr {
  pascals::TypeTemplate* type_ptr;
  pascals::ast::CaseBodyNode* case_body_node;
};

struct BranchListAttr {
  pascals::TypeTemplate* type_ptr;
  pascals::ast::BranchListNode* branch_list_node;
};

struct BranchAttr {
  // std::vector<pascals::TypeTemplate*>* type_ptr_list;
  pascals::TypeTemplate* type_ptr;
  pascals::ast::BranchNode* branch_node;
};

struct ConstListAttr {
  // std::vector<pascals::TypeTemplate*>* type_ptr_list;
  pascals::TypeTemplate* type_ptr;
  pascals::ast::ConstListNode* const_list_node;
};

extern std::string buf;

// #define YYSTYPE Token
// #define YYSTYPE

#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED

struct YYSTYPE {
  Token token_info;
  ValueAttr value_node_info;
  union {
    IdListAttr id_list_node_info;
    TypeAttr type_node_info;
    StandardTypeAttr standared_type_node_info;
    PeriodsAttr periods_node_info;
    PeriodAttr period_node_info;
    RecordAttr record_node_info;
    FormalParameterAttr formal_parameter_node_info;
    ParameterListsAttr parameter_lists_node_info;
    ParameterListAttr parameter_list_node_info;
    VarParameterAttr var_parameter_node_info;
    ValueParameterAttr value_parameter_node_info;

    VariableDeclarationAttr variable_declaration_node_info;
    VariableAttr variable_node_info;
    VariableListAttr variable_list_node_info;
    ExpressionAttr expression_node_info;
    SimpleExpressionAttr simple_expression_node_info;
    StrExpressionAttr str_expression_node_info;
    TermAttr term_node_info;
    FactorAttr factor_node_info;
    UnsignedConstantVarAttr unsigned_constant_var_node_info;
    IDVarpartsAttr id_varparts_node_info;
    IDVarpartAttr id_varpart_node_info;
    ExpressionListAttr expression_list_node_info;
    CaseBodyAttr case_body_node_info;
    BranchListAttr branch_list_node_info;
    BranchAttr branch_node_info;
    ConstListAttr const_list_node_info;

    pascals::ast::ProgramNode* program_node;
    pascals::ast::ProgramHeadNode* program_head_node;
    pascals::ast::ProgramBodyNode* program_body_node;
    pascals::ast::ConstDeclarationsNode* const_declarations_node;
    pascals::ast::ConstDeclarationNode* const_declaration_node;
    pascals::ast::TypeDeclarationsNode* type_declarations_node;
    pascals::ast::TypeDeclarationNode* type_declaration_node;
    pascals::ast::BasicTypeNode* basic_type_node;
    pascals::ast::VariableDeclarationsNode* variable_declarations_node;
    pascals::ast::SubprogramDeclarationsNode* subprogram_declarations_node;
    pascals::ast::SubprogramDeclarationNode* subprogram_declaration_node;
    pascals::ast::SubprogramHeadNode* subprogram_head_node;
    pascals::ast::SubprogramBodyNode* subprogram_body_node;
    pascals::ast::CompoundStatementNode* compound_statement_node;
    pascals::ast::StatementListNode* statement_list_node;
    pascals::ast::StatementNode* statement_node;
    pascals::ast::ElseNode* else_node;
    pascals::ast::UpdownNode* updown_node;
    pascals::ast::ProcedureCallNode* procedure_call_node;
  };
};
typedef struct YYSTYPE YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1
#endif
#endif
