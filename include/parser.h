#ifndef PASCC_PARSER_H
#define PASCC_PARSER_H

#include <iostream>
#include <string>
#include <stack>

#include "compiler.h"
#include "type.h"

#define YYPARSE_DEBUG 0

struct Token {
  ConstValue value;
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
  ConstValue value;
  ast::LeafNode* const_variable_node;
};

struct StandardTypeAttr {
  pascal_type::BasicType* type_ptr;
  ast::BasicTypeNode* standard_type_node;
};

struct TypeAttr {
  enum MainType { BASIC, ARRAY, RECORD } main_type;
  pascal_type::TypeTemplate* type_ptr;
  ast::TypeNode* base_type_node;
  ast::TypeNode* type_node;
  std::vector<pascal_type::ArrayType::ArrayBound>* bounds;
  // std::vector<std::pair<int, int>>* bounds;
  std::unordered_map<std::string, pascal_type::TypeTemplate*>* record_info;
};

struct PeriodsAttr {
  std::vector<pascal_type::ArrayType::ArrayBound>* bounds;
  // std::vector<std::pair<int, int>>* bounds;
  ast::PeriodsNode* periods_node;
};

struct PeriodAttr {
  pascal_type::ArrayType::ArrayBound* bound;
  // std::pair<int, int>* bound;
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

struct VariableListAttr {
  std::vector<pascal_type::BasicType*>* basic_types;
  ast::VariableListNode* variable_list_node;
};

struct ExpressionAttr {
  pascal_type::TypeTemplate* type_ptr;
  ast::ExpressionNode* expression_node;
  int length;
};

struct StrExpressionAttr {
  pascal_type::TypeTemplate* type_ptr;
  ast::StrExpressionNode* str_expression_node;
  int length;
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
  std::string name;
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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
struct YYSTYPE
{
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

        ast::ProgramNode *program_node;
        ast::ProgramHeadNode *program_head_node;
        ast::ProgramBodyNode *program_body_node;
        ast::ConstDeclarationsNode *const_declarations_node;
        ast::ConstDeclarationNode *const_declaration_node;
        ast::TypeDeclarationsNode *type_declarations_node;
        ast::TypeDeclarationNode *type_declaration_node;
        ast::BasicTypeNode *basic_type_node;
        ast::VariableDeclarationsNode *variable_declarations_node;
        ast::SubprogramDeclarationsNode *subprogram_declarations_node;
        ast::SubprogramDeclarationNode *subprogram_declaration_node;
        ast::SubprogramHeadNode *subprogram_head_node;
        ast::SubprogramBodyNode *subprogram_body_node;
        ast::CompoundStatementNode *compound_statement_node;
        ast::StatementListNode *statement_list_node;
        ast::StatementNode *statement_node;
        ast::ElseNode *else_node;
        ast::UpdownNode *updown_node;
        ast::ProcedureCallNode *procedure_call_node;
    };
};
typedef struct YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif
#endif
