#ifndef PASCC_AST_H
#define PASCC_AST_H

#include <iostream>
#include <vector>
#include "json.hpp"
#include "symbol_table.h"

namespace ast{

#define OUT(format, ...)\
  do {\
    fprintf(stdout, format, ##__VA_ARGS__);\
  } while (0);

#define ERR(format, ...)\
  do {\
    fprintf(stderr, format, ##__VA_ARGS__);\
  } while (0);
  
#define FILE(file, format, ...)\
  do {\
    fprintf(file, format, ##__VA_ARGS__);\
  } while (0);


class Node;

class AST {
 public:
  AST();
  ~AST();

  Node* root();
  void set_root(Node* root);
  symbol_table::TableSet* symbol_table() { return symbol_table_;}
  void LoadFromJson(std::string file_name);

 private:
  Node* root_;
  symbol_table::TableSet* symbol_table_;
};

class Node {
 public:
  Node();
  ~Node();

  static Node* NewNodeFromStr(std::string str);

  void set_parent(Node* parent);
  void append_child(Node* child);
  Node* parent();
  std::vector<Node*> child_list();

  // type cast
  template <typename T> T* StaticCast() {
    return dynamic_cast<T*>(this);
  }
  
  template <typename T> T* DynamicCast() {
    return dynamic_cast<T*>(this);
  }

  void TransCodeAt(int);
  void LoadFromJson(const nlohmann::json&, symbol_table::TableSet*);

  virtual void TransCode(){};
 protected:
  // entry

  Node* parent_;
  std::vector<Node*> child_list_;
 private:
  
};

// Leaf Node including id, num, letter ...
class LeafNode : public Node {
 public:
  enum class LEAF_TYPE {
     IDENTIFIER,
     CONST_VALUE,
   };

  LeafNode() {}
  LeafNode(LEAF_TYPE t) : leaf_type_(t) {}
  LeafNode(int val) : leaf_type_(LEAF_TYPE::CONST_VALUE), value_(val), ts_(nullptr), entry_(nullptr) {}
  LeafNode(float val) : leaf_type_(LEAF_TYPE::CONST_VALUE), value_(val), ts_(nullptr), entry_(nullptr)  {}
  LeafNode(char val) : leaf_type_(LEAF_TYPE::CONST_VALUE), value_(val), ts_(nullptr) , entry_(nullptr) {}
  LeafNode(bool val) : leaf_type_(LEAF_TYPE::CONST_VALUE), value_(val), ts_(nullptr), entry_(nullptr)  {}
  LeafNode(std::string id) : leaf_type_(LEAF_TYPE::IDENTIFIER), name_(id), ts_(nullptr), entry_(nullptr)  {}
  LeafNode(std::string id, symbol_table::TableSet* ts)
      : leaf_type_(LEAF_TYPE::IDENTIFIER), name_(id), ts_(ts), entry_(nullptr) {};

  LEAF_TYPE leaf_type() {return leaf_type_;}
  const pascal_symbol::ConstValue* value() { return &value_; }
  const std::string id() {
    return name_;
  }
  void set_entry(pascal_symbol::ObjectSymbol* entry) { entry_ = entry; }

  template <typename T> T value() {
    return value_.get<T>();
  }

  pascal_symbol::ObjectSymbol* entry() {
    if(entry_ == nullptr) SearchEntry();
    return entry_;
  }

  template <typename T> T* entry_cast() {
    if(entry_ == nullptr) SearchEntry();
    return static_cast<T*>(entry_);
  }

  void TransCode() override;

private:
  LEAF_TYPE leaf_type_;
  std::string name_ = "";
  pascal_symbol::ConstValue value_;

  void SearchEntry(){
    entry_ = ts_->SearchEntry<pascal_symbol::ObjectSymbol>(name_);
  }

  symbol_table::TableSet* ts_;
  pascal_symbol::ObjectSymbol* entry_;

};

class ProgramNode : public Node {
 //program → program_head | program_body
 public:
  void TransCode() override;
 private:
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
  void TransCode() override;
  std::vector<Node*> get_id_list();
  std::vector<Node*>* IdList() { return &child_list_; }
 private:
  GrammarType grammar_type_;
};


class ConstDeclarationsNode : public Node {
 public:
  enum class GrammarType {
    EPSILON,             //const_declarations → EPSILON
    CONST_DECLARATION    //const_declarations → const_declaration
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class ConstDeclarationNode : public Node {
 public:
  enum class GrammarType {
    VALUE,        //ConstDeclaration → id = const_var
    DECLARATION   //ConstDeclaration →ConstDeclaration; id = const_var
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class ConstVariableNode : public Node {
  // const_variable → +id | -id | id | +num | -num | num | 'letter'
 public:
  void TransCode() override;
 private:
};

class VariableDeclarationsNode : public Node {
 public:
  enum class GrammarType {
    EPSILON,          //variable_declarations→EPSILON
    VAR_DECLARATION   //variable_declarations→var VariableDeclaration
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class VariableDeclarationNode : public Node {
 public:
  enum class GrammarType {
    SINGLE_DECL,  //VariableDeclaration→idlist:type
    MULTIPLE_DECL //VariableDeclaration→VariableDeclaration;idlist:type
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class TypeDeclarationsNode : public Node {
  public:
    enum class GrammarType {
      EPSILON,         //TypeDeclarations→EPSILON
      TYPE_DECLARATION //TypeDeclarations→type TypeDeclaration
  };
  void TransCode() override;
  private:
    GrammarType grammar_type_;
};

class TypeDeclarationNode : public Node {
 public:
  enum class GrammarType {
    SINGLE_DECL,  //TypeDeclaration→TypeDeclaration;id=type
    MULTIPLE_DECL //id = type
  };
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
  void TransCode() override;
  Node* get_basic_type();
  GrammarType GetGrammarType() { return grammar_type_; }
  void PeriodsTransCode();
 private:
  GrammarType grammar_type_;
};

class BasicTypeNode : public Node {
 //BasicType →integer|real|boolean|char
 public:
  BasicTypeNode() {}
  BasicTypeNode(pascal_type::BasicType* type) : type_(type) {}

  void TransCode() override;
 private:
  pascal_type::BasicType* type_;
};

class RecordBodyNode : public Node {
 public:
  enum class GrammarType {
    EPSILON,
    VAR_DECLARATION
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class PeriodsNode : public Node {
 public:
  enum class GrammarType {
    SINGLE_DECL,   //Periods→Period
    MULTIPLE_DECL  //Periods→Periods,Period
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class PeriodNode : public Node {
 //Period → const_var ... const var
 public:
  void TransCode() override;
 private:
  int len_;
};

class SubprogramDeclarationsNode : public Node {
 public:
  enum class GrammarType {
    EPSILON,            //subprogram_declarations → EPSILON
    SUBPROGRAM_DECL     //subprogram_declarations → subprogram_declarations subprogram subprogram_declaration ;
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class SubprogramDeclarationNode : public Node {
  //subprogram_declaration -> subprogram_head subprogram_body
 public:
  void TransCode() override;
 private:
};

class SubprogramBodyNode : public Node {
   // subprogram_body → const_declarations
   //                type_declarations
   //                var_declarations
   //                compound_statement
 public:
   void TransCode() override;
 private:
};

class SubprogramHeadNode : public Node {
 public:
  enum class GrammarType {
    PROCEDURE,    // subprogram_head → procedure id formal_parameter
    FUNCTION      // subprogram_head → function id formal_parameter : basic_type
  };
  void TransCode() override;

 private:
  GrammarType grammar_type_;
};

class FormalParamNode : public Node {
 public:
  enum class GrammarType {
    EPSILON,      //formal_parameter → EPSILON 
    PARAM_LISTS,  //formal_parameter → ( parameter_lists )
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class ParamListsNode : public Node {
 public:
  enum class GrammarType {
    SINGLE_PARAM_LIST,      //param_lists→ param_list
    MULTIPLE_PARAM_LIST,    //param_lists→ param_lists ; param_list
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class ParamListNode : public Node {
 public:
  enum class GrammarType {
    VAR_PARAM,              //parameter_list → var_parameter
    VALUE_PARAM,            //parameter_list → value_parameter 
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
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
  enum class GrammarType {
    STATEMENT,                  //statement_list → statement 
    STATEMENT_LIST_STATEMENT,   //statement_list → statement_list ; statement
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
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
    WHILE_STATEMENT,      //statement → while expression do s
    REPEAT_STATEMENT      //statement → repeat statement_list until expresssion
  };
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
  void TransCode() override;
 private:
  GrammarType grammar_type_;

};

class VariableNode : public Node {
  //variable → id id_varpart 
 public:
  void TransCode() override;
 private:

};

class IDVarPartsNode : public Node {
 public:
  enum class GrammarType {
    EPSILON,      //id_varparts → EPSILON 
    MULTIPLE_IDv, //id_varparts → id_varparts id_varpart 
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class IDVarPartNode : public Node {
 public:
  enum class GrammarType {
    _ID,          //id_varpart → .id 
    EXP_LIST,     //id_varpart → [ expression_list ]
  };
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
  enum class GrammarType {
    SINGLE_CON,   //constlist → const_variable
    MULTIPLE_CON, //constlist → constlist , const_variable
  };
  std::vector<Node*>* Consts() { return &child_list_; }
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class UpdownNode : public Node {
 //updown → to | downto
 public:
  UpdownNode(bool is_increase = true) : is_increase_(is_increase) {}
  bool IsIncrease() { return is_increase_; }
  void set_increase(bool inc) { is_increase_ = inc;}
  void TransCode() override;

 private:
  bool is_increase_;
};

class ProcedureCallNode : public Node {
 public:
  enum class GrammarType {
    ID,             //procedure_call → id
    ID_EXP_LIST,    //procedure_call → id ( expression_list )
  };
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
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};


class ExpressionNode : public Node {
 public:
  enum class GrammarType {
    S_EXP,              //expression → simple_expression 
    S_EXP_ADDOP_TERM,   //expression → simple_expression relop simple_expression 
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};


class SimpleExpressionNode : public Node {
 public:
  enum class GrammarType {
    TERM,               //simple_expression → term
    POSI_TERM,          //simple_expression → +term
    NEGI_TERM,          //simple_expression → -term
    S_EXP_ADDOP_TERM,   //simple_expression → simple_expression addop term 
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};


class TermNode : public Node {
 public:
  enum class GrammarType {
    FACTOR,               //term → factor
    TERM_MULOP_FACTOR,    //term → term mulop factor
  };
  void TransCode() override;
 private:
  GrammarType grammar_type_;
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
  void TransCode() override;
 private:
  GrammarType grammar_type_;
};

class UnsignConstVarNode : public Node {
  // unsigned_const_variable → num | 'letter'
 public:
  void TransCode() override;
  private:
  
};

} // namespace ast




#endif