#ifndef PASCC_PARSER_H
#define PASCC_PARSER_H

#include <iostream>
#include <string>
#include <stack>
#include <memory>
#include "compiler.h"
#include "type.h"
extern int yydebug;

template<typename T>
using sptr = std::shared_ptr<T>;
using namespace pascals::ast;

struct Token {
  ConstValue value;
  int length;
  int line_num;
  int column_num;
};

struct IdListAttr {
  sptr<std::vector<std::pair<std::string, int>>> list_ref;
  pIdListNode id_list_node;
};

struct ValueAttr {
  ConstValue value;
  pascals::pBasicType type_ptr;
  pLeafNode const_variable_node;
  bool is_right = true;
};

struct StandardTypeAttr {
  pascals::pBasicType type_ptr;
  pBasicTypeNode standard_type_node;
};

struct TypeAttr {
  enum MainType { BASIC, ARRAY, RECORD } main_type;
  pascals::pType type_ptr;
  pTypeNode base_type_node;
  pTypeNode type_node;
  sptr<std::vector<pascals::ArrayType::ArrayBound>> bounds;
  sptr<std::unordered_map<std::string, pascals::pType>> record_info;
};

struct PeriodsAttr {
  sptr<std::vector<pascals::ArrayType::ArrayBound>> bounds;
  pPeriodsNode periods_node;
};

struct PeriodAttr {
  sptr<pascals::ArrayType::ArrayBound> bound;
  pPeriodNode period_node;
};

struct RecordAttr {
  sptr<pascals::RecordType::RecordInfo> record_info;
  pRecordBodyNode record_body_node;
};

struct VariableDeclarationAttr {
  sptr<pascals::RecordType::RecordInfo> record_info;
  pVariableDeclarationNode variable_declaration_node;
};

struct FormalParameterAttr {
  sptr<std::vector<pascals::FunctionSymbol::Parameter>> parameters;
  pFormalParamNode formal_parameter_node;
};

struct ParameterListsAttr {
  sptr<std::vector<pascals::FunctionSymbol::Parameter>> parameters;
  pParamListsNode param_lists_node;
};
struct ParameterListAttr {
  sptr<std::vector<pascals::FunctionSymbol::Parameter>> parameters;
  pParamListNode param_list_node;
};
struct VarParameterAttr {
  sptr<std::vector<pascals::FunctionSymbol::Parameter>> parameters;
  pVarParamNode var_parameter_node;
};

struct ValueParameterAttr {
  sptr<std::vector<pascals::FunctionSymbol::Parameter>> parameters;
  pValueParamNode value_parameter_node;
};

struct VariableAttr {
  pascals::pType type_ptr;
  pVariableNode variable_node;
  std::string* name;
  bool is_lvalue;
};

struct VariableListAttr {
  sptr<std::vector<pascals::pBasicType>> basic_types;
  pVariableListNode variable_list_node;
};

struct ExpressionAttr {
  pascals::pType type_ptr;
  pExpressionNode expression_node;
  int length;
  bool is_lvalue;
};

struct StrExpressionAttr {
  pascals::pType type_ptr;
  pStrExpressionNode str_expression_node;
  int length;
  bool is_lvalue;
};

struct SimpleExpressionAttr {
  pascals::pType type_ptr;
  pSimpleExpressionNode simple_expression_node;
  bool is_lvalue;
};

struct TermAttr {
  pascals::pType type_ptr;
  pTermNode term_node;
  bool is_lvalue;
};

struct FactorAttr {
  pascals::pType type_ptr;
  pFactorNode factor_node;
  bool is_lvalue;
};

struct UnsignedConstantVarAttr {
  pascals::pType type_ptr;
  pUnsignConstVarNode unsigned_constant_var_node;
};

struct VarParts {
  bool flag;
  sptr<std::vector<pascals::pType>> subscript;
  std::string name;
};

struct IDVarpartsAttr {
  sptr<std::vector<VarParts>> var_parts;
  pIDVarPartsNode id_varparts_node;
  pascals::pType AccessCheck(pascals::pType base_type);
};

struct IDVarpartAttr {
  VarParts* var_part;
  pIDVarPartNode id_varpart_node;
};

struct ExpressionListAttr {
  sptr<std::vector<pascals::pType>> type_ptr_list;
  pExpressionListNode expression_list_node;
  sptr<std::vector<bool>> is_lvalue_list;
};

struct CaseBodyAttr {
  pascals::pType type_ptr;
  pCaseBodyNode case_body_node;
};

struct BranchListAttr {
  pascals::pType type_ptr;
  pBranchListNode branch_list_node;
};

struct BranchAttr {
  pascals::pType type_ptr;
  pBranchNode branch_node;
};

struct ConstListAttr {
  pascals::pType type_ptr;
  pConstListNode const_list_node;
};

extern std::string buf;

// #define YYSTYPE Token
// #define YYSTYPE

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

struct YYSTYPE
{
    Token token_info;
    ValueAttr value_node_info;
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

    pProgramNode program_node;
    pProgramHeadNode program_head_node;
    pProgramBodyNode program_body_node;
    pConstDeclarationsNode const_declarations_node;
    pConstDeclarationNode const_declaration_node;
    pTypeDeclarationsNode type_declarations_node;
    pTypeDeclarationNode type_declaration_node;
    pBasicTypeNode basic_type_node;
    pVariableDeclarationsNode variable_declarations_node;
    pSubprogramDeclarationsNode subprogram_declarations_node;
    pSubprogramDeclarationNode subprogram_declaration_node;
    pSubprogramHeadNode subprogram_head_node;
    pSubprogramBodyNode subprogram_body_node;
    pCompoundStatementNode compound_statement_node;
    pStatementListNode statement_list_node;
    pStatementNode statement_node;
    pElseNode else_node;
    pUpdownNode updown_node;
    pProcedureCallNode procedure_call_node;

};
typedef struct YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif
#endif
