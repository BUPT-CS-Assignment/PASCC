#ifndef PASCC_AST_H
#define PASCC_AST_H

#include <iostream>
#include <vector>
#include "json.hpp"
#include "symbol_table.h"

namespace ast{
/* **************** just for test **************** */
// static FILE* OUTPUT_DST = stdout;
static FILE* OUTPUT_DST = fopen("../scripts/json/output/max.c", "w");

#define PRINT(format, ...)\
  do {\
    fprintf(OUTPUT_DST, format, ##__VA_ARGS__);\
  } while (0);

/* **************** just for test **************** */

class Node {
 public:
  Node();
  ~Node();

  static Node* Create(std::string node_name, int sub_type = 0, int other_type = 0);
  static Node* Create(nlohmann::json& json_node);

  void set_parent(Node* parent);
  void append_child(Node* child);
  Node* parent();
  std::vector<Node*> child_list();

  // type cast
  template <typename T> T* StaticCast() { return dynamic_cast<T*>(this); }
  template <typename T> T* DynamicCast() { return dynamic_cast<T*>(this); }

  void TransCodeAt(int);
  void LoadFromJson(const nlohmann::json&);

  virtual void TransCode(){
      for(auto child : child_list_) child->TransCode();
  };

 protected:
  Node* parent_;
  std::vector<Node*> child_list_;
 private:
  
};


class AST {
 public:
//  AST() {}
//  ~AST() {}

  Node* root();
  void set_root(Node* root);
  symbol_table::TableSet* symbol_table() { return symbol_table_;}
  void LoadFromJson(std::string file_name);

  void Print() {
    if(root_ != nullptr) root_->TransCode();
    fclose(OUTPUT_DST);
  }

private:
  Node* root_;
  symbol_table::TableSet* symbol_table_;

};





// Leaf Node including id, num, letter ...
class LeafNode : public Node {
 public:
  enum class LEAF_TYPE {
     IDENTIFIER,
     CONST_VALUE,
     OPERATION,
   };

  LeafNode() {}
  LeafNode(LEAF_TYPE t) : leaf_type_(t) {}
  // const value
  LeafNode(int val) : leaf_type_(LEAF_TYPE::CONST_VALUE), name_(""), value_(val) {}
  LeafNode(float val) : leaf_type_(LEAF_TYPE::CONST_VALUE), name_(""), value_(val) {}
  LeafNode(char val) : leaf_type_(LEAF_TYPE::CONST_VALUE), name_(""), value_(val) {}
  LeafNode(bool val) : leaf_type_(LEAF_TYPE::CONST_VALUE), name_(""), value_(val) {}
  // identifier or operation
  LeafNode(std::string id, bool is_op = false) : name_(id) { leaf_type_ = (is_op ? LEAF_TYPE::OPERATION : LEAF_TYPE::IDENTIFIER);}

  LEAF_TYPE leaf_type() {return leaf_type_;}
  const pascal_symbol::ConstValue* value() { return &value_; }
  const std::string id() { return is_ref_ ? "(*" + name_ + ")" : name_;}
  const std::string origin_id() { return name_;}

  void set_id(std::string name) { leaf_type_ = LEAF_TYPE::IDENTIFIER; name_ = name;}
  void set_op(std::string op_str) { leaf_type_ = LEAF_TYPE::OPERATION; name_ = op_str;}
  void set_value(pascal_symbol::ConstValue value) { leaf_type_ = LEAF_TYPE::CONST_VALUE; value_ = value; }
  void set_ref(bool ref) { is_ref_ = ref; }

  template <typename T> T value() {
    return value_.get<T>();
  }

  bool AnalyzeReference(symbol_table::TableSet* ts, pascal_symbol::FunctionSymbol* fn);
  void TransCode() override;

private:
  LEAF_TYPE leaf_type_;
  std::string name_;
  pascal_symbol::ConstValue value_;
  bool is_ref_ = false;

};

class ProgramNode : public Node {
 //program → program_head  program_body
 public:
};

class ProgramHeadNode : public Node {
 //programhead → program_id (idlists)
 public:
  void TransCode() override;
 private:
};

class ProgramBodyNode : public Node {
  // program_body → const_declarations 
  //                type_declarations
  //                var_declarations
  //                subprogram_declarations
  //                compound_statement
 public:
  void TransCode() override;
 private:

};

class IdListNode : public Node {
 public:
  enum class GrammarType {
    SINGLE_ID,      //idlists → id
    MULTIPLE_ID     //idlists → idlist,id 
  };
  IdListNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
  std::vector<LeafNode*> Lists();
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
    VALUE,        //ConstDeclaration → id = const_var
    DECLARATION   //ConstDeclaration →ConstDeclaration; id = const_var
  };
  ConstDeclarationNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
 private:
  GrammarType grammar_type_;
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
    SINGLE_DECL,      //VariableDeclaration → idlist : xx
    MULTIPLE_DECL     //VariableDeclaration → VariableDeclaration ; idlist : xx
  };
  enum class ListType {
    TYPE,
    ID
  };
  VariableDeclarationNode(GrammarType gt, ListType lt) : grammar_type_(gt), list_type_(lt) {}
  void TransCode() override;
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
    SINGLE_DECL,  //id = type
    MULTIPLE_DECL //TypeDeclaration→TypeDeclaration ; id = type
  };

  TypeDeclarationNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
 private:
  GrammarType grammar_type_;

};

class TypeNode : public Node {
 public:
  enum class GrammarType {
    BASIC_TYPE,
    ARRAY,
    RECORD_TYPE
  };

  TypeNode(GrammarType gt) : grammar_type_(gt) {}

  GrammarType grammar_type() { return grammar_type_; }
  void set_base_type_node(TypeNode* node) { base_type_node_ = node; }
  TypeNode* base_type() { return base_type_node_; }
  void PeriodsTransCode();

  void TransCode() override;
 private:
  GrammarType grammar_type_;
  TypeNode* base_type_node_;
};

class BasicTypeNode : public Node {
 //BasicType →integer|real|boolean|char
 public:
  BasicTypeNode() {}
  BasicTypeNode(pascal_type::BasicType* type) : type_(type) {}

  std::string TypeName(bool ref = false) {
    std::string type_name = type_->type_name() + (ref ? "*" : "");
    return type_name;
  }
  void set_type(pascal_type::BasicType* type) { type_ = type; }
  void TransCode() override;
 private:
  pascal_type::BasicType* type_;
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
 //Period → const_var ... const var
 public:
  void TransCode() override;
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
  //subprogram_declaration -> subprogram_head subprogram_body
 public:
   void TransCode() override;
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
    PROCEDURE,    // subprogram_head → procedure id formal_parameter
    FUNCTION      // subprogram_head → function id formal_parameter : basic_type
  };

  SubprogramHeadNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;

 private:
  GrammarType grammar_type_;
};

class FormalParamNode : public Node {
 public:
//  enum class GrammarType {
//    EPSILON,      //formal_parameter → EPSILON
//    PARAM_LISTS,  //formal_parameter → ( parameter_lists )
//  };
  void TransCode() override;
};

class ParamListsNode : public Node {
 public:
  enum class GrammarType {
    SINGLE_PARAM_LIST,      //param_lists→ param_list
    MULTIPLE_PARAM_LIST,    //param_lists→ param_lists ; param_list
  };

  ParamListsNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
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
 //VarParam → var ValueParam
 public:
  void TransCode() override;
 private:
};

class ValueParamNode : public Node {
 //ValueParam → idlist : basic_type
 public:
  void TransCode() override;
  void TransCode(bool ref);
 private:

};

class CompoundStatementNode : public Node {
 //CompoundStatement → begin StatementList end
 public:
  void TransCode() override;
 private:

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
    EPSILON,              //statement → EPSILON
    VAR_ASSIGN_OP_EXP,    //statement → variable assignop expression
    FUNC_ASSIGN_OP_EXP,   //statement → func_id assignop expression
    PROCEDURE_CALL,       //statement → procedure_call 
    COMPOUND_STATEMENT,   //statement → compound_statement
    IF_STATEMENT,         //statement → if expression then statement else_part
    FOR_STATEMENT,        //statement → for id assignop expression downto expression do statement
    READ_STATEMENT,       //statement → read ( variable_list )
    WRITE_STATEMENT,      //statement → write ( expression_list )
    CASE_STATEMET,        //statement → case expression of case_body end
    WHILE_STATEMENT,      //statement → while expression do statement
    REPEAT_STATEMENT      //statement → repeat statement_list until expression
  };

  StatementNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class VariableListNode : public Node {
 public:
  enum class GrammarType {
     VARIABLE,                  //variable_list → variable  
     VARIABLE_LIST_VARIABLE,    //variable_list → variable_list , variable
  };

  VariableListNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
 private:
  GrammarType grammar_type_;

};

class VariableNode : public Node {
  //variable → id id_varparts
 public:
};

class IDVarPartsNode : public Node {
 public:
//  enum class GrammarType {
//    EPSILON,      //id_varparts → EPSILON
//    MULTIPLE_IDv, //id_varparts → id_varparts id_varpart
//  };
};

class IDVarPartNode : public Node {
 public:
  enum class GrammarType {
    _ID,          //id_varpart → .id 
    EXP_LIST,     //id_varpart → [ expression_list ]
  };

  IDVarPartNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class BranchListNode : public Node {
public:
  enum class GrammarType {
    SINGLE_BRAN,   //branchlist → branch
    MULTIPLE_BRAN, //branchlist → branchlist branch
  };

  BranchListNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
private:
  GrammarType grammar_type_;
};

class CaseBodyNode : public Node {
 public:
  enum class GrammarType {
    BRANCH_LIST,   //case_body → branch_list 
    EPSILON,       //case_body → EPSILON
  };

  CaseBodyNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
  BranchListNode* GetBranchList() {
    return child_list_[0]->DynamicCast<BranchListNode>();
  }
 private:
  GrammarType grammar_type_;
};



class BranchNode : public Node {
 //branch → const_list : statement
 public:
  void TransCode() override;
 private:
};

class ConstListNode : public Node {
 public:
//  enum class GrammarType {
//    SINGLE_CON,   //constlist → const_variable
//    MULTIPLE_CON, //constlist → constlist , const_variable
//  };
  // ConstListNode(GrammarType gt) : grammar_type_(gt) {}
  std::vector<Node*>* Consts() { return &child_list_; }
};

class UpdownNode : public Node {
 //updown → to | downto
 public:
  UpdownNode(bool is_increase = true) : is_increase_(is_increase) {}
  bool IsIncrease() { return is_increase_; }
  void set_increase(bool inc) { is_increase_ = inc;}

 private:
  bool is_increase_;
};

class ProcedureCallNode : public Node {
 public:
  enum class GrammarType {
    ID,             //procedure_call → id
    ID_EXP_LIST,    //procedure_call → id ( expression_list )
  };

  ProcedureCallNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};


class ElseNode : public Node {
 public:
  enum class GrammarType {
    EPSILON,          //else_part → EPSILON
    ELSE_STATEMENT,   //else_part → else statement 
  };

  ElseNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
  StatementNode* GetStatement() {
    return child_list_[0]->DynamicCast<StatementNode>();
  }
 private:
  GrammarType grammar_type_;
};

class ExpressionListNode: public Node {
 public:
  enum class GrammarType {
    EXP,              //expression_list → expression
    EXP_LIST_EXP,     //expression_list → expression_list , expression
  };

  ExpressionListNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};


class ExpressionNode : public Node {
 public:
//  enum class GrammarType {
//    S_EXP,              //expression → simple_expression
//    S_EXP_ADDOP_TERM,   //expression → simple_expression relop simple_expression
//  };
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
};


class FactorNode : public Node {
 public:
  enum class GrammarType {
    UCONST_VAR,     //factor → unsigned_const_variable  
    VARIABLE,       //factor → variable
    EXP,            //factor → ( expression )  
    ID_EXP_LIST,    //factor → id ( expression_list ) 
    NOT,            //factor → not factor 
  };

  FactorNode(GrammarType gt) : grammar_type_(gt) {}
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class UnsignConstVarNode : public Node {
  // unsigned_const_variable → num | 'letter'
 public:
};


} // namespace ast


#endif