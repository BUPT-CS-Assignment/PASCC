#include "ast.h"
#include <fstream>

using std::vector;
using std::string;
using namespace symbol_table;
using json = nlohmann::json;

namespace ast {

Node::Node() : parent_(nullptr) {

}

Node::~Node() {

}

Node* Node::parent(){
  return parent_;
}

std::vector<Node*> Node::child_list() {
  return child_list_;
}

void Node::set_parent(Node* parent) {
  parent_ = parent;
}

void Node::append_child(Node* child) {
  child_list_.emplace_back(child);
}

void Node::TransCodeAt(int pos) {
  return child_list_[pos]->TransCode();
}


//////////////////////////////
void LeafNode::TransCode() {
  // TODO
}

void ProgramNode::TransCode(){
  TransCodeAt(0);
  TransCodeAt(1);
}

void ProgramHeadNode::TransCode(){
  OUT("// ")
  TransCodeAt(0);
  OUT("\n")
}


void ProgramBodyNode::TransCode(){
  TransCodeAt(0);   // const decl
  TransCodeAt(1);   // type decl // TODO
  TransCodeAt(2);   // var decl
  TransCodeAt(3);   // sub prog decl
  OUT("int main(int argc, char** argv) {\n")
  OUT("if (argc >= 1) freopen(argv[1], \"r\", stdin);\n")
  OUT("if (argc >= 2) freopen(argv[2], \"w\", stdout);\n")
  TransCodeAt(4);   // comp stat
  OUT("}\n")
}

void IdListNode::TransCode() {
  TransCodeAt(0);
  if (grammar_type_ == GrammarType::MULTIPLE_ID) {
    OUT(", ")
    TransCodeAt(1);
  }
}


void ConstDeclarationsNode::TransCode() {
  if (grammar_type_ == GrammarType::CONST_DECLARATION) {
    TransCodeAt(0);
  }
}

void ConstDeclarationNode::TransCode(){
  if (grammar_type_ == GrammarType::DECLARATION) {
    TransCodeAt(0);
    OUT(";\nconst ")
    TransCodeAt(1);
    OUT(" = ")
    TransCodeAt(2);
    OUT(";\n")
  } else if (grammar_type_ == GrammarType::VALUE) {
    OUT(";\nconst ")
    TransCodeAt(0);
    OUT(" = ")
    TransCodeAt(1);
    OUT(";\n")
  }
}

void ConstVariableNode::TransCode() {
  for(auto child : child_list_) {
    child->TransCode();
  }
}

void VariableDeclarationsNode::TransCode() {
  if (grammar_type_ == GrammarType::VAR_DECLARATION) {
    TransCodeAt(0);
  }
}

void VariableDeclarationNode::TransCode() {
  for (int idx = 0; idx < child_list_.size(); idx += 2) {
    auto id_list_node = child_list_[idx]->DynamicCast<IdListNode>();
    auto id_list = *id_list_node->IdList();
    auto type_node = child_list_[idx + 1]->DynamicCast<TypeNode>();
    auto basic_type = type_node->get_basic_type();
    for (auto id_node : id_list) {
      basic_type->TransCode();
      OUT(" ")
      id_node->TransCode();
      if (type_node->GetGrammarType() == TypeNode::GrammarType::ARRAY) {
        type_node->PeriodsTransCode();
      }
      OUT(";\n")
    }
  }
}

void TypeDeclarationsNode::TransCode() {
  if(grammar_type_ == GrammarType::TYPE_DECLARATION) {
    TransCodeAt(0);
  }
}

void TypeDeclarationNode::TransCode() {
  for (int idx = 0; idx < child_list_.size(); idx += 2) {
    child_list_[idx]->TransCode();
    auto type_node = child_list_[idx + 1]->DynamicCast<TypeNode>();
    auto basic_type = type_node->get_basic_type();
    OUT("typedef ")
    basic_type->TransCode();
    OUT(" ")
    type_node->TransCode();
    OUT(";\n")
  }
}

void TypeNode::TransCode() {
  switch (grammar_type_) {
    case GrammarType::BASIC_TYPE:
      TransCodeAt(0);
      break;
    case GrammarType::ARRAY: // upper funciton will solve this
      break;
    case GrammarType::RECORD_TYPE:
      OUT("struct {\n")
      TransCodeAt(0);
      OUT("}")
      break;
  }
}

void TypeNode::PeriodsTransCode() {
  if (grammar_type_ == GrammarType::ARRAY) {
    child_list_[0]->TransCode();
    child_list_[1]->DynamicCast<TypeNode>()->PeriodsTransCode();
  }
}

Node* TypeNode::get_basic_type() {
  if (grammar_type_ != GrammarType::ARRAY) {
    return this;
  } else {
    return child_list_[1]->DynamicCast<TypeNode>()->get_basic_type();
  }
}

void BasicTypeNode::TransCode() {
  TransCodeAt(0);
}

void RecordBodyNode::TransCode() {
  if (grammar_type_ == GrammarType::VAR_DECLARATION) {
    TransCodeAt(0);
  }
}

void PeriodsNode::TransCode() {
  for (auto child : child_list_) {
    child->TransCode();
  }
}

void PeriodNode::TransCode() {
  OUT("[%d]", len_)
}

void SubprogramDeclarationsNode::TransCode() {
  for (auto child : child_list_) {
    child->TransCode();
  }
}

void SubprogramDeclarationNode::TransCode() {

}

void SubprogramHeadNode::TransCode() {

}

void FormalParamNode::TransCode() {

}

void ParamListsNode::TransCode() {

}

void ParamListNode::TransCode() {

}

void VarParamNode::TransCode() {

}

void ValueParamNode::TransCode() {

}

void CompoundStatementNode::TransCode() {
  OUT("{\n")
  TransCodeAt(0);
  OUT("}\n")
}

void StatementListNode::TransCode() {
  for (auto child : child_list_) {
    child->TransCode();
  }
}

void StatementNode::TransCode() {
  switch (grammar_type_) {
    case GrammarType::EPSILON:
      return;
    case GrammarType::VAR_ASSIGN_OP_EXP:
      TransCodeAt(0);
      OUT(" = ")
      TransCodeAt(1);
      OUT(";\n")
      break;
    case GrammarType::FUNC_ASSIGN_OP_EXP:
      TransCodeAt(0);
      OUT(" = ")
      TransCodeAt(1);
      OUT(";\n");      
      break;
    case GrammarType::PROCEDURE_CALL:
      TransCodeAt(0);
      break;
    case GrammarType::COMPOUND_STATEMENT:
      TransCodeAt(0);
      break;
    case GrammarType::IF_STATEMENT:
      OUT("if (")
      TransCodeAt(0);
      OUT(") {\n")
      TransCodeAt(1);
      OUT("}\n")
      TransCodeAt(2);
      break;
    case GrammarType::FOR_STATEMENT: {
      auto updown_node = child_list_[2]->DynamicCast<UpdownNode>();
      bool increase = updown_node->IsIncrease();
      OUT("for (")
      TransCodeAt(0);
      OUT("=")
      TransCodeAt(1);
      OUT(";")
      TransCodeAt(0);
      OUT(increase ?  "<=" : ">=")
      TransCodeAt(3);
      OUT(";")
      TransCodeAt(0);
      OUT(increase ? "++" : "--")
      OUT(") {\n")
      for (int i = 4; i < child_list_.size(); i++)
      TransCodeAt(i);
      OUT("}\n")
      break;
    }
    case GrammarType::READ_STATEMENT:
    // TODO
      break;
    case GrammarType::WRITE_STATEMENT:
    // TODO
      break;
    case GrammarType::CASE_STATEMET:
      OUT("switch (")
      TransCodeAt(0);
      OUT(") {\n")
      TransCodeAt(1);
      OUT("}\n")
      break;
    case GrammarType::WHILE_STATEMENT:
      OUT("while (")
      TransCodeAt(0);
      OUT(") {\n")
      TransCodeAt(1);
      OUT("}\n")
      break;
    case GrammarType::REPEAT_STATEMENT:
      OUT("do {\n")
      TransCodeAt(0);
      OUT("} while (")
      TransCodeAt(1);
      OUT(");\n")
      break;
  }
}

void VariableListNode::TransCode() {
  TransCodeAt(0);
  if(grammar_type_ == GrammarType::VARIABLE_LIST_VARIABLE) {
    OUT(",")
    TransCodeAt(1);
  }
}

void VariableNode::TransCode(){
  for(auto& child : child_list_) {
    child->TransCode();
  }
}

void IDVarPartsNode::TransCode() {
  for(auto& child : child_list_) {
    child->TransCode();
  }
}

void IDVarPartNode::TransCode() {
  if (grammar_type_ == GrammarType::_ID) {
    OUT(".")
    TransCodeAt(0);
  } else if (grammar_type_ == GrammarType::EXP_LIST) {
    OUT("[")
    TransCodeAt(0);
    OUT("]")
  }
}


void BranchListNode::TransCode() {
  for (auto child : child_list_)
    child->TransCode();
}

void CaseBodyNode::TransCode() {
  switch (grammar_type_) {
  case GrammarType::EPSILON:
    return;
  case GrammarType::BRANCH_LIST:
    GetBranchList()->TransCode();
  }
}

void BranchNode::TransCode() {
  for (int id = 0; id < child_list_.size(); id += 2) {
    auto const_list = child_list_[id]->DynamicCast<ConstListNode>();
    auto statement = child_list_[id + 1];
    auto const_vars = const_list->Consts();
    for (auto const_var : *const_vars) {
      OUT("case ")
      const_var->TransCode();
      OUT(":\n")
      statement->TransCode();
    }
  }
}

void ConstListNode::TransCode() {

}

void UpdownNode::TransCode() {

}


void ProcedureCallNode::TransCode() {
  TransCodeAt(0);
  if (grammar_type_ == GrammarType::ID) {
    OUT("();")
  } else if(grammar_type_ == GrammarType::ID_EXP_LIST) {
    OUT("(")
    TransCodeAt(1);
    OUT(");")
  }
}

void ElseNode::TransCode() {
  switch (grammar_type_) {
    case GrammarType::EPSILON:
      return;
    case GrammarType::ELSE_STATEMENT:
      OUT("else {\n")
      GetStatement()->TransCode();
      OUT("}\n")
  }
}

void ExpressionListNode::TransCode() {
  TransCodeAt(0);
  OUT(",")
  TransCodeAt(1);
}

void ExpressionNode::TransCode() {
  for(auto& child : child_list_){
    child->TransCode();
  }
}

void SimpleExpressionNode::TransCode() {
  for (auto& child : child_list_) {
    child->TransCode();
  }
}

void TermNode::TransCode() {
  for (auto& child : child_list_) {
    child->TransCode();
  }
}

void FactorNode::TransCode() {

  switch (grammar_type_) {
    case GrammarType::UCONST_VAR :
    case GrammarType::VARIABLE :
      TransCodeAt(0);
      break;

    case GrammarType::EXP : 
      OUT("(")
      TransCodeAt(0);
      OUT(")")
      break;
      
    case GrammarType::ID_EXP_LIST : 
      TransCodeAt(0);
      OUT("(")
      TransCodeAt(1);
      OUT(")")
      break;

    case GrammarType::NOT : 
      OUT("!")
      TransCodeAt(0);
      break;
      
    default :
      break;
  }
}

void UnsignConstVarNode::TransCode() {
  TransCodeAt(0);
}


/////////////////////////////

void AST::LoadFromJson(std::string file_name) {
  std::ifstream ifs(file_name);

  if (!ifs.is_open()) {
    std::cout << "open file failed" << std::endl;
    return;
  }

  json ast_json;
  ifs >> ast_json;
  ifs.close();

  root_ = new ProgramNode();
  symbol_table_ = new TableSet("root", nullptr);


  //
  root_->LoadFromJson(ast_json, symbol_table_);
}


void Node::LoadFromJson(const nlohmann::json & node_json, const symbol_table::TableSet * node_table) {



}







}  // namespace ast