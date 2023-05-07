#ifndef PASCC_AST_H
#define PASCC_AST_H

#include <iostream>
#include <vector>
#include <stack>
#include "json.hpp"
#include "symbol_table.h"
#include "pstdlib.h"
namespace pascals {
namespace ast {

/* **************** standard output **************** */

#define PRINT(format, ...) fprintf(dst, format, ##__VA_ARGS__);

/* **************** basic node **************** */
/**
 * @brief Basic Node class
 */
class Node {
public:
  // constructor
  Node() : parent_(nullptr){};
  virtual ~Node() {
    for (auto child : child_list_) {
      delete child;
    }
  };

  // static creator
  static Node *Create(std::string node_name, int sub_type = 0,
                      int other_type = 0);
  static Node *Create(nlohmann::json &json_node);
  // type cast
  template <typename T> T *StaticCast() { return dynamic_cast<T *>(this); }
  template <typename T> T *DynamicCast() { return dynamic_cast<T *>(this); }

  // getter and setter functions
  //void set_parent(Node *parent) { parent_ = parent; }
  //Node *parent() { return parent_; }
  // append child
  void append_child(Node *child) {
    //child->set_parent(this);
    child_list_.emplace_back(child);
  }

  // generate
  void FormatAt(int pos, FILE *dst) {
    // pos < 0, from tail
    return child_list_[pos < 0 ? pos + child_list_.size() : pos]->Format(dst);
  }
  virtual void Format(FILE *dst) {
    for (auto child : child_list_)
      child->Format(dst);
  }
  // load from json file
  void LoadFromJson(const nlohmann::json &);

protected:
  Node *parent_;                   // parent node pointer
  std::vector<Node *> child_list_; // child pointer list
};

/**
 * @brief AST class
 */
class AST {
public:
  // getter and setter functions
  ~AST() {
    log_debug("~AST(): delete ast");
    if (root_ != nullptr)
      delete root_;
  }
  Node *root() { return root_; }
  PStdLibs *libs() { return &libs_; }
  void set_root(Node *root) { root_ = root; }
  // load from json
  void LoadFromJson(std::string file_name);

  /**
   * @brief print AST
   * @param FILE ptr
   */
  void Format(FILE *dst);

private:
  Node *root_; // root node pointer
  PStdLibs libs_;
};

/* **************** inherited node **************** */
/**
 * Leaf Node including id, num, letter ...
 */
class LeafNode : public Node {
public:
  LeafNode() {}
  // const value
  LeafNode(ConstValue val) : value_(val) {}

  // getter & setter functions
  const std::string id_ref() {
    return is_ref_ ? "(*" + value_.get<std::string>() + ")"
                   : value_.get<std::string>();
  }
  void set_value(ConstValue value) { value_ = value; }
  void set_ref(bool ref) { is_ref_ = ref; }

  // template value getter
  template <typename T> T value() { return value_.get<T>(); }
  BasicType *type() { return value_.type(); }
  // Analyze reference
  bool AnalyzeReference(TableSet *ts,
                        FunctionSymbol *fn);
  void Format(FILE *dst) override;

private:
  ConstValue value_;    // const values
  bool is_ref_ = false; // is referenced
};

class ProgramNode : public Node {
  // program → program_head  program_body
public:
};

class ProgramHeadNode : public Node {
  // programhead → program_id (idlists)
public:
  void Format(FILE *dst) override;

private:
};

class ProgramBodyNode : public Node {
  // program_body → const_declarations
  //                type_declarations
  //                var_declarations
  //                subprogram_declarations
  //                compound_statement
public:
  void Format(FILE *dst) override;

private:
};

class IdListNode : public Node {
public:
  enum class GrammarType {
    SINGLE_ID,  // idlists → id
    MULTIPLE_ID // idlists → idlist,id
  };
  IdListNode(GrammarType gt) : grammar_type_(gt) {}
  void Format(FILE *dst) override;
  std::vector<LeafNode *> Lists();

private:
  GrammarType grammar_type_;
};

class ConstDeclarationsNode : public Node {
public:
  //  enum class GrammarType {
  //    EPSILON,             //const_declarations → EPSILON
  //    CONST_DECLARATION    //const_declarations → const_declaration
  //  };
};

class ConstDeclarationNode : public Node {
public:
  enum class GrammarType {
    VALUE,      // ConstDeclaration → id = const_var
    DECLARATION // ConstDeclaration →ConstDeclaration; id = const_var
  };
  ConstDeclarationNode(GrammarType gt, BasicType *bt)
      : grammar_type_(gt), type_(bt) {}
  void print_type(FILE *dst);
  void Format(FILE *dst) override;

private:
  GrammarType grammar_type_;
  BasicType *type_;
};

class ConstVariableNode : public Node {
  // const_variable → +id | -id | id | +num | -num | num | 'letter'
public:
};

class VariableDeclarationsNode : public Node {
public:
  //  enum class GrammarType {
  //    EPSILON,          //variable_declarations→EPSILON
  //    VAR_DECLARATION   //variable_declarations→var VariableDeclaration
  //  };
};

class VariableDeclarationNode : public Node {
public:
  enum class GrammarType {
    SINGLE_DECL,  // VariableDeclaration → idlist : xx
    MULTIPLE_DECL // VariableDeclaration → VariableDeclaration ; idlist : xx
  };
  enum class ListType { TYPE, ID };
  VariableDeclarationNode(GrammarType gt, ListType lt)
      : grammar_type_(gt), list_type_(lt) {}
  void Format(FILE *dst) override;

private:
  GrammarType grammar_type_;
  ListType list_type_;
};

class TypeDeclarationsNode : public Node {
public:
  //    enum class GrammarType {
  //      EPSILON,         //TypeDeclarations→EPSILON
  //      TYPE_DECLARATION //TypeDeclarations→type TypeDeclaration
  //  };
};

class TypeDeclarationNode : public Node {
public:
  enum class GrammarType {
    SINGLE_DECL,  // id = type
    MULTIPLE_DECL // TypeDeclaration→TypeDeclaration ; id = type
  };

  TypeDeclarationNode(GrammarType gt) : grammar_type_(gt) {}
  void Format(FILE *dst) override;

private:
  GrammarType grammar_type_;
};

class TypeNode : public Node {
public:
  enum class GrammarType { BASIC_TYPE, ARRAY, RECORD_TYPE };

  TypeNode(GrammarType gt) : grammar_type_(gt) {}

  GrammarType grammar_type() { return grammar_type_; }
  void set_base_type_node(TypeNode *node) { base_type_node_ = node; }
  TypeNode *base_type() { return base_type_node_; }

  void PeriodsFormat(FILE *dst);
  void Format(FILE *dst) override;

private:
  GrammarType grammar_type_;
  TypeNode *base_type_node_;
};

class BasicTypeNode : public Node {
  // BasicType →integer|real|boolean|char
public:
  BasicTypeNode() {}
  BasicTypeNode(BasicType *type) : type_(type) {}

  std::string TypeName(bool ref = false) {
    std::string type_name = type_->type_name() + (ref ? "*" : "");
    return type_name;
  }
  void set_type(BasicType *type) { type_ = type;}
  void Format(FILE *dst) override;

private:
  BasicType *type_;
};

class RecordBodyNode : public Node {
  // public:
  //  enum class GrammarType {
  //    EPSILON,
  //    VAR_DECLARATION
  //  };
};

class PeriodsNode : public Node {
  // public:
  //  enum class GrammarType {
  //    SINGLE_DECL,   //Periods→Period
  //    MULTIPLE_DECL  //Periods→Periods,Period
  //  };
};

class PeriodNode : public Node {
  // Period → const_var ... const var
public:
  void Format(FILE *dst) override;
  int len() { return len_; }
  void set_len(int len) { len_ = len; }

private:
  int len_;
};

class SubprogramDeclarationsNode : public Node {
  // public:
  //  enum class GrammarType {
  //    EPSILON,            //subprogram_declarations → EPSILON
  //    SUBPROGRAM_DECL     //subprogram_declarations → subprogram_declarations subprogram subprogram_declaration ;
  //  };
};

class SubprogramDeclarationNode : public Node {
  // subprogram_declaration -> subprogram_head subprogram_body
public:
  void Format(FILE *dst) override;
};

class SubprogramBodyNode : public Node {
  // subprogram_body → const_declarations
  //                type_declarations
  //                var_declarations
  //                compound_statement
public:
};

class SubprogramHeadNode : public Node {
public:
  enum class GrammarType {
    PROCEDURE, // subprogram_head → procedure id formal_parameter
    FUNCTION   // subprogram_head → function id formal_parameter : basic_type
  };

  SubprogramHeadNode(GrammarType gt) : grammar_type_(gt) {}
  void Format(FILE *dst) override;

private:
  GrammarType grammar_type_;
};

class FormalParamNode : public Node {
public:
  //  enum class GrammarType {
  //    EPSILON,      //formal_parameter → EPSILON
  //    PARAM_LISTS,  //formal_parameter → ( parameter_lists )
  //  };
  void Format(FILE *dst) override;
};

class ParamListsNode : public Node {
public:
  enum class GrammarType {
    SINGLE_PARAM_LIST,   // param_lists→ param_list
    MULTIPLE_PARAM_LIST, // param_lists→ param_lists ; param_list
  };

  ParamListsNode(GrammarType gt) : grammar_type_(gt) {}
  void Format(FILE *dst) override;

private:
  GrammarType grammar_type_;
};

class ParamListNode : public Node {
public:
  //  enum class GrammarType {
  //    VAR_PARAM,              //parameter_list → var_parameter
  //    VALUE_PARAM,            //parameter_list → value_parameter
  //  };
  //  ParamListNode(GrammarType gt) : grammar_type_(gt) {}
  //  void TransCode() override;
  // private:
  //  GrammarType grammar_type_;
};

class VarParamNode : public Node {
  // VarParam → var ValueParam
public:
  void Format(FILE *dst) override;

private:
};

class ValueParamNode : public Node {
  // ValueParam → idlist : basic_type
public:
  void Format(FILE *dst) override;
  void Format(bool ref, FILE *dst);

private:
};

class CompoundStatementNode : public Node {
  // CompoundStatement → begin StatementList end
public:
};

class StatementListNode : public Node {
public:
  //  enum class GrammarType {
  //    STATEMENT,                  //statement_list → statement
  //    STATEMENT_LIST_STATEMENT,   //statement_list → statement_list ; statement
  //  };
};

class StatementNode : public Node {
public:
  enum class GrammarType {
    EPSILON,            // statement → EPSILON
    VAR_ASSIGN_OP_EXP,  // statement → variable assignop expression
    FUNC_ASSIGN_OP_EXP, // statement → func_id assignop expression
    PROCEDURE_CALL,     // statement → procedure_call
    COMPOUND_STATEMENT, // statement → compound_statement
    IF_STATEMENT,       // statement → if expression then statement else_part
    FOR_STATEMENT, // statement → for id assignop expression downto expression do statement
    READ_STATEMENT,    // statement → read ( variable_list )
    READLN_STATEMENT,  // statement → readln ( variable_list )
    WRITE_STATEMENT,   // statement → write ( expression_list )
    WRITELN_STATEMENT, // statement → writeln ( expression_list )
    CASE_STATEMET,     // statement → case expression of case_body end
    WHILE_STATEMENT,   // statement → while expression do statement
    REPEAT_STATEMENT   // statement → repeat statement_list until expression
  };

  StatementNode(GrammarType gt) : grammar_type_(gt) {}
  void Format(FILE *dst) override;

private:
  GrammarType grammar_type_;
};

class VariableListNode : public Node {
public:
  enum class GrammarType {
    VARIABLE,               // variable_list → variable
    VARIABLE_LIST_VARIABLE, // variable_list → variable_list , variable
  };

  VariableListNode(GrammarType gt) : grammar_type_(gt) {}
  std::string FormatString();
  void Format(FILE *dst) override;
  void Format(bool ref, FILE *dst);
  bool set_types(std::vector<TypeTemplate *> *type_list);

private:
  // TODO get basic_type ptr lists
  std::vector<BasicType *> basic_types;
  GrammarType grammar_type_;
};

class VariableNode : public Node {
  // variable → id id_varparts
public:
  void Format(bool ref, FILE *dst);
};

class IDVarPartNode : public Node {
public:
  enum class GrammarType {
    _ID,      // id_varpart → .id
    EXP_LIST, // id_varpart → [ expression_list ]
  };

  IDVarPartNode(GrammarType gt) : grammar_type_(gt) {}
  GrammarType grammar_type() { return grammar_type_; }
  void Format(FILE *dst) override;
  void set_array_lb(int lb) { array_lb_ = lb; }

private:
  int array_lb_ = 0;
  GrammarType grammar_type_;
};

class IDVarPartsNode : public Node {
public:
  //  enum class GrammarType {
  //    EPSILON,      //id_varparts → EPSILON
  //    MULTIPLE_IDv, //id_varparts → id_varparts id_varpart
  //  };
  void set_lb(std::vector<ArrayType::ArrayBound> &bound) {
    if (child_list_.size() == 0)
      return;
    child_list_[0]->DynamicCast<IDVarPartsNode>()->set_lb(bound);
    if (child_list_[1]->DynamicCast<IDVarPartNode>()->grammar_type() ==
        IDVarPartNode::GrammarType::EXP_LIST) {
      child_list_[1]->DynamicCast<IDVarPartNode>()->set_array_lb(bound[0].lb_);
      bound.erase(bound.begin());
    }
  }
};

class BranchListNode : public Node {
public:
  //  enum class GrammarType {
  //    SINGLE_BRAN,   //branchlist → branch
  //    MULTIPLE_BRAN, //branchlist → branchlist branch
  //  };
};

class CaseBodyNode : public Node {
public:
  //  enum class GrammarType {
  //    BRANCH_LIST,   //case_body → branch_list
  //    EPSILON,       //case_body → EPSILON
  //  };
};

class BranchNode : public Node {
  // branch → const_list : statement
public:
  void Format(FILE *dst) override;

private:
};

class ConstListNode : public Node {
public:
  //  enum class GrammarType {
  //    SINGLE_CON,   //constlist → const_variable
  //    MULTIPLE_CON, //constlist → constlist , const_variable
  //  };
  // ConstListNode(GrammarType gt) : grammar_type_(gt) {}
  std::vector<Node *> *Consts() { return &child_list_; }
  void Format_Constlist(FILE *dst, StatementNode *statement);
};

class UpdownNode : public Node {
  // updown → to | downto
public:
  UpdownNode(bool is_increase = true) : is_increase_(is_increase) {}
  bool IsIncrease() { return is_increase_; }
  void set_increase(bool inc) { is_increase_ = inc; }

private:
  bool is_increase_;
};

class ProcedureCallNode : public Node {
public:
  enum class GrammarType {
    ID,          // procedure_call → id
    ID_EXP_LIST, // procedure_call → id ( expression_list )
  };

  ProcedureCallNode(GrammarType gt) : grammar_type_(gt) {}
  void Format(FILE *dst) override;

private:
  GrammarType grammar_type_;
};

class ElseNode : public Node {
public:
  enum class GrammarType {
    EPSILON,        // else_part → EPSILON
    ELSE_STATEMENT, // else_part → else statement
  };

  ElseNode(GrammarType gt) : grammar_type_(gt) {}
  void Format(FILE *dst) override;
  StatementNode *GetStatement() {
    return child_list_[0]->DynamicCast<StatementNode>();
  }

private:
  GrammarType grammar_type_;
};

class ExpressionListNode : public Node {
public:
  enum class GrammarType {
    EXP,          // expression_list → expression
    EXP_LIST_EXP, // expression_list → expression_list , expression
  };

  ExpressionListNode(GrammarType gt) : grammar_type_(gt) {}
  std::string FormatString();
  void Format(FILE *dst) override;
  bool set_types(std::vector<TypeTemplate *> *type_list);
  void set_ref(std::stack<bool> *ref);

private:
  // TODO get basic_type ptr lists
  std::vector<BasicType *> basic_types;
  GrammarType grammar_type_;
};

class ExpressionNode : public Node {
  //  enum class GrammarType {
  //    S_STR,              // expression → str_expression
  // str_expression -> str_
  // str_expression -> str_expression + str_
  //    S_EXP,              // expression → simple_expression
  //    S_EXP_ADDOP_TERM,   // expression → simple_expression relop simple_expression
  //  };
public:
  enum class TargetType {
    EXPRESSION,
    VAR_ARRAY,
    CONST_STRING,
  };
  ExpressionNode() : target_type_(TargetType::EXPRESSION) {}
  ExpressionNode(TargetType tg) : target_type_(tg) {}
  void set_is_ref() { is_ref_ = 1; }
  void set_expression_type(TargetType tg) { target_type_ = tg; }
  TargetType target_type() { return target_type_; }
  void Format(FILE *dst) override;

private:
  TargetType target_type_;
  bool is_ref_ = 0;
};

class StrExpressionNode : public Node {
  // str_expression -> str_ ## [leaf]
  // str_expression -> str_expression + str_
public:
  void Format(FILE *dst) override;
};

class SimpleExpressionNode : public Node {
public:
  //  enum class GrammarType {
  //    TERM,               //simple_expression → term
  //    POSI_TERM,          //simple_expression → +term
  //    NEGI_TERM,          //simple_expression → -term
  //    S_EXP_ADDOP_TERM,   //simple_expression → simple_expression addop term
  //  };
};

class TermNode : public Node {
public:
  //  enum class GrammarType {
  //    FACTOR,               //term → factor
  //    TERM_MULOP_FACTOR,    //term → term mulop factor
  //  };
  void set_op_div(bool op) { op_div = op; }
  void Format(FILE *dst) override;

private:
  bool op_div = false;
};

class FactorNode : public Node {
public:
  enum class GrammarType {
    UCONST_VAR,  // factor → unsigned_const_variable
    VARIABLE,    // factor → variable
    EXP,         // factor → ( expression )
    ID_EXP_LIST, // factor → id ( expression_list )
    NOT,         // factor → not factor
  };

  FactorNode(GrammarType gt) : grammar_type_(gt) {}
  void Format(FILE *dst) override;

private:
  GrammarType grammar_type_;
};

class UnsignConstVarNode : public Node {
  // unsigned_const_variable → num | 'letter'
public:
};

} // namespace ast

}// namespace pascals

#endif