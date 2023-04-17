#include "ast.h"
#include <fstream>

using std::vector;
using std::string;
using namespace symbol_table;
using json = nlohmann::json;
using namespace pascal_symbol;

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
  child->set_parent(this);
  child_list_.emplace_back(child);
}

void Node::TransCodeAt(int pos) {
  return child_list_[pos]->TransCode();
}


//////////////////////////////

void LeafNode::TransCode() {
  if(leaf_type_ == LEAF_TYPE::IDENTIFIER) {
    OUT(" %s ",entry_->name().c_str())
  } else {
    BasicType* tp = value_.type();
    if(tp == pascal_type::TYPE_INT) {
      OUT(" %d ", value_.get<int>())
    } else if (tp == pascal_type::TYPE_REAL) {
      OUT(" %.2f ", value_.get<float>())
    } else if (tp == pascal_type::TYPE_BOOL) {
      OUT(" %s ", value_.get<bool>() ? "true" : "false")
    } else if (tp == pascal_type::TYPE_CHAR) {
      OUT(" %c ", value_.get<char>())
    }
  }
}

bool LeafNode::SearchReference(pascal_symbol::FunctionSymbol *func) {
  if(func == nullptr || leaf_type_ != LEAF_TYPE::IDENTIFIER)
    return false;

  // search table and judge if is current layer
  if(!searched_) SearchEntry();
  if(is_local_) {
    // search param
    FunctionSymbol::ParamType* pt = (*func)[name_];
    // search param-passing mode
    if(pt != nullptr && pt->second == FunctionSymbol::PARAM_MODE::REFERENCE) {
      is_ref_ = true;
    }
  }
  return is_ref_;
}
/////////////////////////////////////////////

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
  TransCodeAt(1);   // type decl
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

vector<LeafNode*> IdListNode::Lists() {
  vector<LeafNode*> lists;
  IdListNode* cur_node = this;
  GrammarType gtype = grammar_type_;
  while(gtype == GrammarType::MULTIPLE_ID) {
    LeafNode* ln = this->child_list_[1]->DynamicCast<LeafNode>();
    lists.insert(lists.begin(), ln);
    // switch to next layer
    cur_node = child_list_[0]->DynamicCast<IdListNode>();
    gtype = cur_node->grammar_type_;
  }

  // insert last
  LeafNode* ln = (*this->child_list_.rbegin())->DynamicCast<LeafNode>();
  lists.insert(lists.begin(), ln);
  return lists;
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

//void ConstVariableNode::TransCode() {
//  for(auto child : child_list_) {
//    child->TransCode();
//  }
//}

//void VariableDeclarationsNode::TransCode() {
//  if (grammar_type_ == GrammarType::VAR_DECLARATION) {
//    TransCodeAt(0);
//  }
//}

void VariableDeclarationNode::TransCode() {
  if(grammar_type_ == GrammarType::MULTIPLE_DECL) {
    TransCodeAt(0);
  }

  // analyze current layer
  Node* tnode = *child_list_.rbegin();
  vector<LeafNode*> idlist = (*(child_list_.rbegin() - 1))->DynamicCast<IdListNode>()->Lists();

  if(list_type_ == ListType::ID){
    tnode->TransCode();
    OUT(" ")
    for(auto id : idlist) id->TransCode();
  } else {
    // ListType::TYPE
    TypeNode* tn = tnode->DynamicCast<TypeNode>()->base_type();
    tn->TransCode();
    OUT(" ")
    // idlist
    bool array_flag = (tn->grammar_type() == TypeNode::GrammarType::ARRAY);
    for(auto id : idlist) {
      id->TransCode();
      if(array_flag) tn->PeriodsTransCode();
      OUT(", ")
    }
  }
  OUT(";\n")

//  for (int idx = 0; idx < child_list_.size(); idx += 2) {
//    auto id_list_node = child_list_[idx]->DynamicCast<IdListNode>();
//    auto id_list = *id_list_node->IdList();
//    auto type_node = child_list_[idx + 1]->DynamicCast<TypeNode>();
//    auto base_type = type_node->base_type();
//    for (auto id_node : id_list) {
//      base_type->TransCode();
//      OUT(" ")
//      id_node->TransCode();
//      if (type_node->grammar_type() == TypeNode::GrammarType::ARRAY) {
//        type_node->PeriodsTransCode();
//      }
//      OUT(";\n")
//    }
//  }
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
    auto base_type = type_node->base_type();
    OUT("typedef ")
    base_type->TransCode();
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


void BasicTypeNode::TransCode() {
  OUT("%s", type_->type_name().c_str())
}

void RecordBodyNode::TransCode() {
  if (grammar_type_ == GrammarType::VAR_DECLARATION) {
    TransCodeAt(0);
  }
}

//void PeriodsNode::TransCode() {
//  for (auto child : child_list_) {
//    child->TransCode();
//  }
//}

void PeriodNode::TransCode() {
  OUT("[%d]", len_)
}

//void SubprogramDeclarationsNode::TransCode() {
//  for (auto child : child_list_) {
//    child->TransCode();
//  }
//}

//void SubprogramDeclarationNode::TransCode() {
//  TransCodeAt(0);
//  TransCodeAt(1);
//}

void SubprogramHeadNode::TransCode() {
  if (grammar_type_ == GrammarType::PROCEDURE) {
    OUT("void ")
  } else {
    TransCodeAt(2);   // basic_type
  }
  TransCodeAt(0);
  TransCodeAt(1);
}

//void SubprogramBodyNode::TransCode() {
//  for(auto child : child_list_) {
//    child->TransCode();
//  }
//}

void FormalParamNode::TransCode() {
  OUT("(")
  if (grammar_type_ == GrammarType::PARAM_LISTS) {
    TransCodeAt(0);
  }
  OUT(")")
}

void ParamListsNode::TransCode() {
  for(int i = 0; i < child_list_.size(); i++) {
    child_list_[i]->TransCode();
    if(i < child_list_.size() - 1) {
      OUT(",")
    }
  }
}

void ParamListNode::TransCode() {
  TransCodeAt(0);
  OUT(", ")
  TransCodeAt(1);
}

void VarParamNode::TransCode() {
  ValueParamNode* vp = child_list_[0]->DynamicCast<ValueParamNode>();
  vp->TransCode(true);
}

void ValueParamNode::TransCode() {
  this->TransCode(false);
}

void ValueParamNode::TransCode(bool ref) {
  // get idlists
  IdListNode* idnode = child_list_[0]->DynamicCast<IdListNode>();
  // get type
  BasicTypeNode *tnode = child_list_[1]->DynamicCast<BasicTypeNode>();
  vector<LeafNode*> idlist = idnode->Lists();
  string tname = tnode->TypeName(ref);

  for(int i = 0; i < idlist.size(); i++) {
    OUT("%s %s", tname.c_str(), idlist[i]->id().c_str());
    if(i != idlist.size() - 1) {
      OUT(", ")
    }
  }
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

//void ConstListNode::TransCode() {
//
//}

//void UpdownNode::TransCode() {
//
//}


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

Node* Node::NewNodeFromStr(std::string str) {
//  if (str == "leaf") {
//    return new LeafNode();
//  } else if (str == "program") {
//    return new ProgramNode();
//  } else if (str == "program_head") {
//    return new ProgramHeadNode();
//  } else if (str == "program_body") {
//    return new ProgramBodyNode();
//  } else if (str == "idlists") {
//    return new IdListNode();
//  } else if (str == "const_decls") {
//    return new ConstDeclarationsNode();
//  } else if (str == "const_decl") {
//    return new ConstDeclarationNode();
//  } else if (str == "const_var") {
//    return new ConstVariableNode();
//  } else if (str == "var_decls") {
//    return new VariableDeclarationsNode();
//  } else if (str == "var_decl") {
//    return new VariableDeclarationNode();
//  } else if (str == "type_decls") {
//    return new TypeDeclarationsNode();
//  } else if (str == "type_decl") {
//    return new TypeDeclarationNode();
//  } else if (str == "type") {
//    return new TypeNode();
//  } else if (str == "basic_type") {
//    return new BasicTypeNode();
//  } else if (str == "record_body") {
//    return new RecordBodyNode();
//  } else if (str == "periods") {
//    return new PeriodsNode();
//  } else if (str == "period") {
//    return new PeriodNode();
//  } else if (str == "subp_decls") {
//    return new SubprogramDeclarationsNode();
//  } else if (str == "subp_decl") {
//    return new SubprogramDeclarationNode();
//  } else if (str == "subp_head") {
//    return new SubprogramHeadNode();
//  } else if (str == "subp_body") {
//    return new SubprogramBodyNode();
//  } else if (str == "formal_param") {
//    return new FormalParamNode();
//  } else if (str == "param_lists") {
//    return new ParamListsNode();
//  } else if (str == "param_list") {
//    return new ParamListNode();
//  } else if (str == "var_param") {
//    return new VarParamNode();
//  } else if (str == "value_param") {
//    return new ValueParamNode();
//  } else if (str == "comp_stat") {
//    return new CompoundStatementNode();
//  } else if (str == "stat_list") {
//    return new StatementListNode();
//  } else if (str == "stat") {
//    return new StatementNode();
//  } else if (str == "var_list") {
//    return new VariableListNode();
//  } else if (str == "var") {
//    return new VariableNode();
//  } else if (str == "id_varparts") {
//    return new IDVarPartsNode();
//  } else if (str == "id_varpart") {
//    return new IDVarPartNode();
//  } else if (str == "branch_list") {
//    return new BranchListNode();
//  } else if (str == "case_body") {
//    return new CaseBodyNode();
//  } else if (str == "branch") {
//    return new BranchNode();
//  } else if (str == "const_list") {
//    return new ConstListNode();
//  } else if (str == "updown") {
//    return new UpdownNode();
//  } else if (str == "proc_call") {
//    return new ProcedureCallNode();
//  } else if (str == "else") {
//    return new ElseNode();
//  } else if (str == "exp_list") {
//    return new ExpressionListNode();
//  } else if (str == "exp") {
//    return new ExpressionNode();
//  } else if (str == "simple_exp") {
//    return new SimpleExpressionNode();
//  } else if (str == "term") {
//    return new TermNode();
//  } else if (str == "factor") {
//    return new FactorNode();
//  } else if (str == "unsign_const_var") {
//    return new UnsignConstVarNode();
//  } else {
//    return nullptr;
//  }
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
  symbol_table_->LoadFromJson(ast_json);
  root_->LoadFromJson(ast_json, symbol_table_);
}


void Node::LoadFromJson(const nlohmann::json & node_json, symbol_table::TableSet * node_table) {
  for(auto item : node_json["children"]) {
    string tp_str = item["type"];
    Node* child = Node::NewNodeFromStr(tp_str);
     if (tp_str == "leaf") {
      string leaf_type = item["leaf_type"].get<string>();
      LeafNode* lf = child->StaticCast<LeafNode>();

      if (leaf_type == "id") {
        lf->set_id(item["name"].get<string>());
        lf->set_tableset(node_table);
      } else if (leaf_type == "int") {
        lf->set_value(ConstValue(item["value"].get<int>()));
      } else if (leaf_type == "real") {
        lf->set_value(ConstValue(item["value"].get<float>()));
      } else if (leaf_type == "char") {
        lf->set_value(ConstValue(item["value"].get<char>()));
      } else if (leaf_type == "bool") {
        lf->set_value(ConstValue(item["value"].get<bool>()));
      }

    } else if (tp_str == "subp_decl") {
      TableSet* tb = new TableSet(item["name"], node_table);
      tb->LoadFromJson(item);
      child->LoadFromJson(item, tb);

    } else {
      child->LoadFromJson(item, node_table);
    }

    child->set_parent(this);
    this->append_child(child);
  }
}

}  // namespace ast