#include "ast.h"
#include "log.h"

using std::vector;
using std::string;
using json = nlohmann::json;
using namespace symbol_table;
using namespace pascal_symbol;

namespace ast {
/* **************** standard output **************** */
FILE* astout = stdout;

/* **************** ast output **************** */
void AST::Print(const char *file_name) {
  if(file_name != nullptr) {
    astout = fopen(file_name, "w");
    if(astout == nullptr) {
      log_fatal("failed to open file %s", file_name);
      abort();
    }
    log_info("out stream for ast redirected : %s", file_name);
  }
  if(root_ != nullptr) root_->TransCode();
  log_info("ast format success");
  fclose(astout);
}

//////////////////////////////

void LeafNode::TransCode() {
  if(leaf_type_ != LEAF_TYPE::CONST_VALUE) {
    PRINT(" %s ", name_.c_str())
  } else {
    BasicType* tp = value_.type();
    if(tp == pascal_type::TYPE_INT) {
      PRINT(" %d ", value_.get<int>())
    } else if (tp == pascal_type::TYPE_REAL) {
      PRINT(" %.2f ", value_.get<float>())
    } else if (tp == pascal_type::TYPE_BOOL) {
      PRINT(" %s ", value_.get<bool>() ? "true" : "false")
    } else if (tp == pascal_type::TYPE_CHAR) {
      PRINT(" '%c' ", value_.get<char>())
    }
  }
}

bool LeafNode::AnalyzeReference(symbol_table::TableSet* ts, pascal_symbol::FunctionSymbol *fn) {
  if(fn == nullptr || leaf_type_ != LEAF_TYPE::IDENTIFIER)
    return false;

  // search table and judge if is current layer
  bool local = false;
  ObjectSymbol* entry = ts->SearchEntry<pascal_symbol::ObjectSymbol>(name_, &local);
  if(entry != nullptr && local) {
    FunctionSymbol::ParamType* pt = (*fn)[name_];
    // search param-passing mode
    if(pt != nullptr && pt->second == FunctionSymbol::PARAM_MODE::REFERENCE) {
      is_ref_ = true;
    }
  }
  return is_ref_;
}
/////////////////////////////////////////////

void ProgramHeadNode::TransCode(){
  PRINT("#include <stdio.h>\n")
  PRINT("//")
  TransCodeAt(0);
  PRINT("\n")
}


void ProgramBodyNode::TransCode(){
  TransCodeAt(0);   // const decl
  TransCodeAt(1);   // type decl
  TransCodeAt(2);   // var decl
  TransCodeAt(3);   // sub prog decl
  PRINT("int main(int argc, char** argv) {\n")
  PRINT("if (argc >= 1) freopen(argv[1], \"r\", stdin);\n")
  PRINT("if (argc >= 2) freopen(argv[2], \"w\", stdout);\n")
  TransCodeAt(4);   // comp stat
  PRINT("}\n")
}

void IdListNode::TransCode() {
  TransCodeAt(0);
  if (grammar_type_ == GrammarType::MULTIPLE_ID) {
    PRINT(", ")
    TransCodeAt(1);
  }
}

vector<LeafNode*> IdListNode::Lists() {
  vector<LeafNode*> lists;
  IdListNode* cur_node = this;
  GrammarType gtype = grammar_type_;


  while(gtype == GrammarType::MULTIPLE_ID) {
    LeafNode* ln = cur_node->child_list_[1]->DynamicCast<LeafNode>();
    lists.insert(lists.begin(), ln);
    // switch to next layer
    cur_node = cur_node->child_list_[0]->DynamicCast<IdListNode>();
    gtype = cur_node->grammar_type_;
  }

  // insert last
  LeafNode* ln = (*cur_node->child_list_.rbegin())->DynamicCast<LeafNode>();
  lists.insert(lists.begin(), ln);
  return lists;
}


//void ConstDeclarationsNode::TransCode() {
//  if (grammar_type_ == GrammarType::CONST_DECLARATION) {
//    TransCodeAt(0);
//  }
//}

void ConstDeclarationNode::TransCode(){
  if (grammar_type_ == GrammarType::DECLARATION) {
    TransCodeAt(0);
    PRINT(";\nconst ")
    TransCodeAt(1);
    PRINT(" = ")
    TransCodeAt(2);
    PRINT(";\n")
  } else if (grammar_type_ == GrammarType::VALUE) {
    PRINT(";\nconst ")
    TransCodeAt(0);
    PRINT(" = ")
    TransCodeAt(1);
    PRINT(";\n")
  }
}

void VariableDeclarationNode::TransCode() {
  if(grammar_type_ == GrammarType::MULTIPLE_DECL) {
    TransCodeAt(0);
  }
  // analyze current layer
  size_t last = child_list_.size() - 1;
  Node* tnode = child_list_[last];
  vector<LeafNode*> idlist = child_list_[last - 1]->DynamicCast<IdListNode>()->Lists();

  if(list_type_ == ListType::ID){
    tnode->TransCode();
    PRINT(" ")
    for(auto id : idlist) id->TransCode();
  } else {

    // ListType::TYPE
    TypeNode* tn = tnode->DynamicCast<TypeNode>()->base_type();
    tn->TransCode();
    PRINT(" ")
    // idlist
    bool array_flag = (tn->grammar_type() == TypeNode::GrammarType::ARRAY);
    for(int i = 0; i < idlist.size(); i++) {
      idlist[i]->TransCode();
      if(array_flag) tn->PeriodsTransCode();
      if(i < idlist.size() - 1){
        PRINT(", ")
      }
    }
  }
  PRINT(";\n")

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

//void TypeDeclarationsNode::TransCode() {
//  if(grammar_type_ == GrammarType::TYPE_DECLARATION) {
//    TransCodeAt(0);
//  }
//}

void TypeDeclarationNode::TransCode() {
  if(grammar_type_ == GrammarType::MULTIPLE_DECL) {
    TransCodeAt(0);
  }
  // analyze current layer
  TypeNode* tnode = (*child_list_.rbegin())->DynamicCast<TypeNode>();
  LeafNode* idnode = (*(child_list_.rbegin() - 1))->DynamicCast<LeafNode>();

  TypeNode* base_type = tnode->base_type();
  PRINT("typedef ")
  base_type->TransCode();
  if(tnode->grammar_type() == TypeNode::GrammarType::ARRAY) {
    tnode->PeriodsTransCode();
  }
  PRINT(" ")
  idnode->TransCode();
  PRINT(";\n")
//  for (int idx = 0; idx < child_list_.size(); idx += 2) {
//    child_list_[idx]->TransCode();
//    auto type_node = child_list_[idx + 1]->DynamicCast<TypeNode>();
//    auto base_type = type_node->base_type();
//    OUT("typedef ")
//    base_type->TransCode();
//    OUT(" ")
//    type_node->TransCode();
//    OUT(";\n")
//  }
}

void TypeNode::TransCode() {
  switch (grammar_type_) {
    case GrammarType::BASIC_TYPE:
      TransCodeAt(0);
      break;
    case GrammarType::ARRAY: // upper funciton will solve this
      break;
    case GrammarType::RECORD_TYPE:
      PRINT("struct {\n")
      TransCodeAt(0);
      PRINT("}")
      break;
  }
}

void TypeNode::PeriodsTransCode() {
  if (grammar_type_ == GrammarType::ARRAY) {
    child_list_[0]->TransCode();
    child_list_[1]->DynamicCast<TypeNode>()->PeriodsTransCode();
  }
}


void BasicTypeNode::TransCode() { PRINT("%s", type_->type_name().c_str())
}

void PeriodNode::TransCode() { PRINT("[%d]", len_)
}

void SubprogramDeclarationNode::TransCode() {
  TransCodeAt(0);
  PRINT("{\n")
  TransCodeAt(1);
  PRINT("}\n")
}

void SubprogramHeadNode::TransCode() {
  if (grammar_type_ == GrammarType::PROCEDURE) {
    PRINT("void ")
  } else {
    TransCodeAt(2);   // basic_type
  }
  TransCodeAt(0);
  TransCodeAt(1);
}

void FormalParamNode::TransCode() {
  PRINT("(")
  for(auto child : child_list_) {
    child->TransCode();
  }
//  if (grammar_type_ == GrammarType::PARAM_LISTS) {
//    TransCodeAt(0);
//  }
  PRINT(")")
}

void ParamListsNode::TransCode() {
  for(int i = 0; i < child_list_.size(); i++) {
    child_list_[i]->TransCode();
    if(i < child_list_.size() - 1) {
      PRINT(",")
    }
  }
}

//void ParamListNode::TransCode() {
//  TransCodeAt(0);
//  PRINT(", ")
//  TransCodeAt(1);
//}

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
    PRINT("%s %s", tname.c_str(), idlist[i]->id().c_str());
    if(i < idlist.size() - 1) {
      PRINT(", ")
    }
  }
}

void CompoundStatementNode::TransCode() {
  PRINT("{\n")
  TransCodeAt(0);
  PRINT("}\n")
}


void StatementNode::TransCode() {
  switch (grammar_type_) {
    case GrammarType::EPSILON:
      return;
    case GrammarType::VAR_ASSIGN_OP_EXP:
      TransCodeAt(0);
      PRINT(" = ")
      TransCodeAt(1);
      PRINT(";\n")
      break;
    case GrammarType::FUNC_ASSIGN_OP_EXP:
      PRINT("return ")
      TransCodeAt(1);
      PRINT(";\n");
      break;
    case GrammarType::PROCEDURE_CALL:
    case GrammarType::COMPOUND_STATEMENT:
      TransCodeAt(0);
      break;
    case GrammarType::IF_STATEMENT:
      PRINT("if (")
      TransCodeAt(0);
      PRINT(") {\n")
      TransCodeAt(1);
      PRINT("}\n")
      TransCodeAt(2);
      break;
    case GrammarType::FOR_STATEMENT: {
      auto updown_node = child_list_[2]->DynamicCast<UpdownNode>();
      bool increase = updown_node->IsIncrease();
      PRINT("for (")
      TransCodeAt(0);
      PRINT("=")
      TransCodeAt(1);
      PRINT(";")
      TransCodeAt(0);
      PRINT(increase ?  "<=" : ">=")
      TransCodeAt(3);
      PRINT(";")
      TransCodeAt(0);
      PRINT(increase ? "++" : "--")
      PRINT(") {\n")
      for (int i = 4; i < child_list_.size(); i++)
      TransCodeAt(i);
      PRINT("}\n")
      break;
    }
    case GrammarType::READ_STATEMENT:
    // TODO
      break;
    case GrammarType::WRITE_STATEMENT:
    // TODO
      break;
    case GrammarType::CASE_STATEMET:
      PRINT("switch (")
      TransCodeAt(0);
      PRINT(") {\n")
      TransCodeAt(1);
      PRINT("}\n")
      break;
    case GrammarType::WHILE_STATEMENT:
      PRINT("while (")
      TransCodeAt(0);
      PRINT(") {\n")
      TransCodeAt(1);
      PRINT("}\n")
      break;
    case GrammarType::REPEAT_STATEMENT:
      PRINT("do {\n")
      TransCodeAt(0);
      PRINT("} while (")
      TransCodeAt(1);
      PRINT(");\n")
      break;
  }
}

void VariableListNode::TransCode() {
  TransCodeAt(0);
  if(grammar_type_ == GrammarType::VARIABLE_LIST_VARIABLE) {
      PRINT(",")
    TransCodeAt(1);
  }
}



void IDVarPartNode::TransCode() {
  if (grammar_type_ == GrammarType::_ID) {
    PRINT(".")
    TransCodeAt(0);
  } else if (grammar_type_ == GrammarType::EXP_LIST) {
    PRINT("[")
    TransCodeAt(0);
    PRINT("]")
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
      PRINT("case ")
      const_var->TransCode();
      PRINT(":\n")
      statement->TransCode();
    }
  }
}



void ProcedureCallNode::TransCode() {
  TransCodeAt(0);
  if (grammar_type_ == GrammarType::ID) {
    PRINT("();")
  } else if(grammar_type_ == GrammarType::ID_EXP_LIST) {
    PRINT("(")
    TransCodeAt(1);
    PRINT(");")
  }
}

void ElseNode::TransCode() {
  switch (grammar_type_) {
    case GrammarType::EPSILON:
      return;
    case GrammarType::ELSE_STATEMENT:
      PRINT("else {\n")
      GetStatement()->TransCode();
      PRINT("}\n")
  }
}

void ExpressionListNode::TransCode() {
  for(int i = 0; i < child_list_.size(); i++) {
    child_list_[i]->TransCode();
    if(i < child_list_.size() - 1) {
      PRINT(", ")
    }
  }
}


void FactorNode::TransCode() {

  switch (grammar_type_) {
    case GrammarType::UCONST_VAR :
    case GrammarType::VARIABLE :
      TransCodeAt(0);
      break;

    case GrammarType::EXP :
      PRINT("(")
      TransCodeAt(0);
      PRINT(")")
      break;
      
    case GrammarType::ID_EXP_LIST : 
      TransCodeAt(0);
      PRINT("(")
      TransCodeAt(1);
      PRINT(")")
      break;

    case GrammarType::NOT :
      PRINT("!")
      TransCodeAt(0);
      break;
      
    default :
      break;
  }
}


/////////////////////////////




}  // namespace ast