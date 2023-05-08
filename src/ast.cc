#include "ast.h"
#include <ctime>

#include "log.h"

using std::string;
using std::vector;
using std::stack;
using json = nlohmann::json;

namespace pascals {
namespace ast {
/* **************** standard output **************** */

/* **************** ast output **************** */
void AST::Format(FILE *dst) {
  if (root_ != nullptr) {
    libs_.Format(dst);
    root_->Format(dst);
  }
  log_info("ast format success");
  fclose(dst);
}

//////////////////////////////

void LeafNode::Format(FILE *dst) {
  BasicType *tp = value_.type();
  if (tp == TYPE_INT) {
    PRINT("%d", value_.get<int>())
  } else if (tp == TYPE_REAL) {
    PRINT("%.2f", value_.get<float>())
  } else if (tp == TYPE_BOOL) {
    PRINT("%d", value_.get<bool>())
  } else if (tp == TYPE_CHAR) {
    PRINT("'%c'", value_.get<char>())
  } else if (tp == TYPE_STRINGLIKE) {
    PRINT("%s", value_.get<string>().c_str())
  }
}

bool LeafNode::AnalyzeReference(TableSet *ts, FunctionSymbol *fn) {
  if (fn == nullptr)
    return false;
  // search table and judge if is current layer
  bool local = false;
  string id = value_.get<string>();
  auto *entry = ts->SearchEntry<ObjectSymbol>(id, &local);
  if (entry != nullptr && local) {
    FunctionSymbol::ParamType *pt = (*fn)[id];
    // search param-passing mode
    if (pt != nullptr && pt->second == FunctionSymbol::PARAM_MODE::REFERENCE) {
      is_ref_ = true;
    }
  }
  return is_ref_;
}
/////////////////////////////////////////////

void ProgramHeadNode::Format(FILE *dst) {
  time_t now = time(nullptr);
  tm *local = localtime(&now);
  PRINT("/// [")
  FormatAt(0, dst);
  PRINT("] created on %d/%d/%d\n", 1900 + local->tm_year, 1 + local->tm_mon,
        local->tm_mday)
}

void ProgramBodyNode::Format(FILE *dst) {
  FormatAt(0, dst); // const decl
  FormatAt(1, dst); // type decl
  FormatAt(2, dst); // var decl
  FormatAt(3, dst); // sub prog decl
  PRINT("int main() {\n")
  FormatAt(4, dst); // comp stat
  PRINT("}\n")
}

void IdListNode::Format(FILE *dst) {
  FormatAt(0, dst);
  if (grammar_type_ == GrammarType::MULTIPLE_ID) {
    PRINT(", ")
    FormatAt(1, dst);
  }
}

vector<LeafNode *> IdListNode::Lists() {
  vector<LeafNode *> lists;
  IdListNode *cur_node = this;
  GrammarType gtype = grammar_type_;

  while (gtype == GrammarType::MULTIPLE_ID) {
    LeafNode *ln = cur_node->child_list_[1]->DynamicCast<LeafNode>();
    lists.insert(lists.begin(), ln);
    // switch to next layer
    cur_node = cur_node->child_list_[0]->DynamicCast<IdListNode>();
    gtype = cur_node->grammar_type_;
  }

  // insert last
  LeafNode *ln = (*cur_node->child_list_.rbegin())->DynamicCast<LeafNode>();
  lists.insert(lists.begin(), ln);
  return lists;
}

// void ConstDeclarationsNode::TransCode() {
//   if (grammar_type_ == GrammarType::CONST_DECLARATION) {
//     TransCodeAt(0);
//   }
// }
void ConstDeclarationNode::print_type(FILE *dst) {
  if (type_ == TYPE_INT) {
    PRINT("int ")
  } else if (type_ == TYPE_REAL) {
    PRINT("float ")
  } else if (type_ == TYPE_CHAR) {
    PRINT("char ")
  }
}
void ConstDeclarationNode::Format(FILE *dst) {
  if (grammar_type_ == GrammarType::DECLARATION) {
    FormatAt(0, dst);
    PRINT("const ")
    print_type(dst);
    FormatAt(1, dst);
    PRINT(" = ")
    FormatAt(2, dst);
    PRINT(";\n");
  } else if (grammar_type_ == GrammarType::VALUE) {
    PRINT("const ")
    print_type(dst);
    FormatAt(0, dst);
    PRINT(" = ")
    FormatAt(1, dst);
    PRINT(";\n");
  }
}

void VariableDeclarationNode::Format(FILE *dst) {
  if (grammar_type_ == GrammarType::MULTIPLE_DECL) {
    FormatAt(0, dst);
  }
  // analyze current layer
  size_t last = child_list_.size() - 1;
  Node *tnode = child_list_[last];
  vector<LeafNode *> idlist = child_list_[last - 1]->DynamicCast<IdListNode>()->Lists();

  if (list_type_ == ListType::ID) {
    tnode->Format(dst);
    PRINT(" ");
    // idlist
    for (int i = 0; i < idlist.size(); i++) {
      idlist[i]->Format(dst);
      if (i < idlist.size() - 1) {
        PRINT(", ")
      }
    }
  } else {
    // TypeNode* tn = tnode->DynamicCast<TypeNode>()->base_type();
    TypeNode *tn = tnode->DynamicCast<TypeNode>();
    tn->base_type()->Format(dst);
    PRINT(" ");
    // idlist
    bool array_flag = (tn->grammar_type() == TypeNode::GrammarType::ARRAY);
    for (int i = 0; i < idlist.size(); i++) {
      idlist[i]->Format(dst);
      if (array_flag)
        tn->PeriodsFormat(dst);
      if (i < idlist.size() - 1) {
        PRINT(", ")
      }
    }
  }
  PRINT(";\n")
}

// void TypeDeclarationsNode::TransCode() {
//   if(grammar_type_ == GrammarType::TYPE_DECLARATION) {
//     TransCodeAt(0);
//   }
// }

void TypeDeclarationNode::Format(FILE *dst) {
  if (grammar_type_ == GrammarType::MULTIPLE_DECL) {
    FormatAt(0, dst);
  }
  // analyze current layer
  TypeNode *tnode = (*child_list_.rbegin())->DynamicCast<TypeNode>();
  LeafNode *idnode = (*(child_list_.rbegin() + 1))->DynamicCast<LeafNode>();

  TypeNode *base_type = tnode->base_type();
  PRINT("typedef ")
  base_type->Format(dst);
  PRINT(" ")
  idnode->Format(dst);
  if (tnode->grammar_type() == TypeNode::GrammarType::ARRAY) {
    tnode->PeriodsFormat(dst);
  }
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

void TypeNode::Format(FILE *dst) {
  switch (grammar_type_) {
  case GrammarType::BASIC_TYPE:
    FormatAt(0, dst);
    break;
  case GrammarType::ARRAY: // upper funciton will solve this
    break;
  case GrammarType::RECORD_TYPE:
    PRINT("struct {\n")
    FormatAt(0, dst);
    PRINT("}")
    break;
  }
}

void TypeNode::PeriodsFormat(FILE *dst) {
  if (grammar_type_ == GrammarType::ARRAY) {
    child_list_[0]->Format(dst);
    child_list_[1]->DynamicCast<TypeNode>()->PeriodsFormat(dst);
  }
}

void BasicTypeNode::Format(FILE *dst) {
  PRINT("%s", type_->type_name().c_str())
}

void PeriodNode::Format(FILE *dst) { PRINT("[%d]", len_) }

void SubprogramDeclarationNode::Format(FILE *dst) {
  FormatAt(0, dst);
  PRINT("{\n")
  FormatAt(1, dst);
  PRINT("}\n")
}

void SubprogramHeadNode::Format(FILE *dst) {
  if (grammar_type_ == GrammarType::PROCEDURE) {
    PRINT("void ")
  } else {
    FormatAt(2, dst); // basic_type
    PRINT(" ")
  }
  FormatAt(0, dst);
  FormatAt(1, dst);
}

void FormalParamNode::Format(FILE *dst) {
  PRINT("(")
  for (auto child : child_list_) {
    child->Format(dst);
  }
  //  if (grammar_type_ == GrammarType::PARAM_LISTS) {
  //    TransCodeAt(0);
  //  }
  PRINT(")")
}

void ParamListsNode::Format(FILE *dst) {
  for (int i = 0; i < child_list_.size(); i++) {
    child_list_[i]->Format(dst);
    if (i < child_list_.size() - 1) {
      PRINT(",")
    }
  }
}

// void ParamListNode::TransCode() {
//   TransCodeAt(0);
//   PRINT(", ")
//   TransCodeAt(1);
// }

void VarParamNode::Format(FILE *dst) {
  ValueParamNode *vp = child_list_[0]->DynamicCast<ValueParamNode>();
  vp->Format(true, dst);
}

void ValueParamNode::Format(FILE *dst) { this->Format(false, dst); }

void ValueParamNode::Format(bool ref, FILE *dst) {
  // get idlists
  IdListNode *idnode = child_list_[0]->DynamicCast<IdListNode>();
  // get type
  BasicTypeNode *tnode = child_list_[1]->DynamicCast<BasicTypeNode>();
  vector<LeafNode *> idlist = idnode->Lists();
  string tname = tnode->TypeName(ref);

  for (int i = 0; i < idlist.size(); i++) {
    PRINT("%s %s", tname.c_str(), idlist[i]->id_ref().c_str());
    if (i < idlist.size() - 1) {
      PRINT(", ")
    }
  }
}

void StatementNode::Format(FILE *dst) {
  switch (grammar_type_) {
  case GrammarType::EPSILON:
    return;
  case GrammarType::VAR_ASSIGN_OP_EXP: {
    auto exp = child_list_[1]->DynamicCast<ExpressionNode>();
    switch (exp->target_type()) {
    case ExpressionNode::TargetType::EXPRESSION: { // other expression
      FormatAt(0, dst);
      PRINT(" = ");
      FormatAt(1, dst);
      break;
    }
    case ExpressionNode::TargetType::VAR_ARRAY: { // array-list
      PRINT("memcpy(");
      FormatAt(0, dst);
      PRINT(", ");
      FormatAt(1, dst);
      PRINT(", sizeof(");
      FormatAt(0, dst);
      PRINT("))")
      break;
    }
    case ExpressionNode::TargetType::CONST_STRING: { // const-string
      PRINT("strcpy(");
      FormatAt(0, dst);
      PRINT(", ");
      FormatAt(1, dst);
      PRINT(")")
      break;
    }
    }
    PRINT(";\n")
    break;
  }
  case GrammarType::FUNC_ASSIGN_OP_EXP: {
    PRINT("return ")
    FormatAt(1, dst);
    PRINT(";\n");
    break;
  }
  case GrammarType::PROCEDURE_CALL:
  case GrammarType::COMPOUND_STATEMENT: {
    FormatAt(0, dst);
    PRINT("\n")
    break;
  }
  case GrammarType::IF_STATEMENT: {
    PRINT("if (")
    FormatAt(0, dst);
    PRINT(") {\n")
    FormatAt(1, dst);
    PRINT("}\n")
    FormatAt(2, dst);
    break;
  }
  case GrammarType::FOR_STATEMENT: {
    auto updown_node = child_list_[2]->DynamicCast<UpdownNode>();
    bool increase = updown_node->IsIncrease();
    PRINT("for (")
    FormatAt(0, dst);
    PRINT("=")
    FormatAt(1, dst);
    PRINT(";")
    FormatAt(0, dst);
    PRINT(increase ? "<=" : ">=")
    FormatAt(3, dst);
    PRINT(";")
    FormatAt(0, dst);
    PRINT(increase ? "++" : "--")
    PRINT(") {\n")
    for (int i = 4; i < child_list_.size(); i++)
      FormatAt(i, dst);
    PRINT("}\n")
    break;
  }
  case GrammarType::READ_STATEMENT:
  case GrammarType::READLN_STATEMENT: {
    // TODO readln(...)
    auto *vlnodes = child_list_[0]->DynamicCast<VariableListNode>();
    if (grammar_type_ == GrammarType::READLN_STATEMENT) {
      PRINT("scanf(\"%s\\n\", ", vlnodes->FormatString().c_str())
    } else {
      PRINT("scanf(\"%s\", ", vlnodes->FormatString().c_str())
    }
    vlnodes->Format(true, dst);
    PRINT(");\n")
    break;
  }
  case GrammarType::WRITE_STATEMENT:
  case GrammarType::WRITELN_STATEMENT: {
    auto *elnode = child_list_[0]->DynamicCast<ExpressionListNode>();
    if (grammar_type_ == GrammarType::WRITELN_STATEMENT) {
      PRINT("printf(\"%s\\n\"", elnode->FormatString().c_str())
    } else {
      PRINT("printf(\"%s\"", elnode->FormatString().c_str())
    }
    PRINT(", ")
    elnode->Format(dst);
    PRINT(");\n")
    break;
  }
  case GrammarType::CASE_STATEMET: {
    PRINT("switch (")
    FormatAt(0, dst);
    PRINT(") {\n")
    FormatAt(1, dst);
    PRINT("}\n")
    break;
  }
  case GrammarType::WHILE_STATEMENT: {
    PRINT("while (")
    FormatAt(0, dst);
    PRINT(") {\n")
    FormatAt(1, dst);
    PRINT("}\n")
    break;
  }
  case GrammarType::REPEAT_STATEMENT: {
    PRINT("do {\n")
    FormatAt(0, dst);
    PRINT("} while (!(")
    FormatAt(1, dst);
    PRINT("));\n")
    break;
  }
  }
}

string VariableListNode::FormatString() {
  string format = "";
  for (int i = 0; i < basic_types.size(); i++) {
    BasicType *type = basic_types[i];
    string chfmt = (type == TYPE_INT || type == TYPE_BOOL) ? "%d"
                   : type == TYPE_STRINGLIKE               ? "%s"
                   : type == TYPE_REAL                     ? "%.2f"
                   : type == TYPE_CHAR
                       ? "%c"
                       : throw std::runtime_error(
                             "ExpressionListNode: FormatString() : error type");
    format += chfmt + " ";
  }
  return format;
}

void VariableListNode::Format(FILE *dst) { Format(false, dst); }

void VariableListNode::Format(bool ref, FILE *dst) {
  if (grammar_type_ == GrammarType::VARIABLE) {
    child_list_[0]->DynamicCast<VariableNode>()->Format(ref, dst);
  }
  if (grammar_type_ == GrammarType::VARIABLE_LIST_VARIABLE) {
    child_list_[0]->DynamicCast<VariableListNode>()->Format(ref, dst);
    PRINT(",")
    child_list_[1]->DynamicCast<VariableNode>()->Format(ref, dst);
  }
}

bool VariableListNode::set_types(
    std::vector<TypeTemplate *> *type_list) {
  if (!type_list)
    return true;
  for (auto i : *type_list) {
    if (is_basic(i) || i == TYPE_STRINGLIKE) {
      basic_types.push_back(dynamic_cast<BasicType *>(i));
    } else if (i != TYPE_ERROR && i->StringLike()) {
      basic_types.push_back(TYPE_STRINGLIKE);
    } else {
      return false;
    }
  }
  return true;
}

void VariableNode::Format(bool ref, FILE *dst) {
  if (ref)
    PRINT("&")
  Node::Format(dst);
}

void IDVarPartNode::Format(FILE *dst) {
  if (grammar_type_ == GrammarType::_ID) {
    PRINT(".")
    FormatAt(0, dst);
  } else if (grammar_type_ == GrammarType::EXP_LIST) {
    PRINT("[")
    FormatAt(0, dst);
    if (array_lb_ > 0)
      PRINT(" - %d", array_lb_)
    else if (array_lb_ < 0)
      PRINT(" + %d", -array_lb_)
    PRINT("]")
  }
}

void BranchNode::Format(FILE *dst) {
  for (int id = 0; id < child_list_.size(); id += 2) {
    auto const_list = child_list_[id]->DynamicCast<ConstListNode>();
    auto statement = child_list_[id + 1]->DynamicCast<StatementNode>();
    const_list->Format_Constlist(dst, statement);
  }
}

void ProcedureCallNode::Format(FILE *dst) {
  FormatAt(0, dst);
  if (grammar_type_ == GrammarType::ID) {
    PRINT("();")
  } else if (grammar_type_ == GrammarType::ID_EXP_LIST) {
    PRINT("(")
    FormatAt(1, dst);
    PRINT(");")
  }
}

void ElseNode::Format(FILE *dst) {
  switch (grammar_type_) {
  case GrammarType::EPSILON:
    return;
  case GrammarType::ELSE_STATEMENT:
    PRINT("else {\n")
    GetStatement()->Format(dst);
    PRINT("}\n")
  }
}

string ExpressionListNode::FormatString() {
  string format = "";
  for (int i = 0; i < basic_types.size(); i++) {
    BasicType *type = basic_types[i];
    string chfmt = (type == TYPE_INT || type == TYPE_BOOL) ? "%d"
                   : type == TYPE_STRINGLIKE               ? "%s"
                   : type == TYPE_REAL                     ? "%.2f"
                   : type == TYPE_CHAR
                       ? "%c"
                       : throw std::runtime_error(
                             "ExpressionListNode: FormatString() : error type");
    format += chfmt;
  }
  return format;
}

void ExpressionListNode::Format(FILE *dst) {
  for (int i = 0; i < child_list_.size(); i++) {
    child_list_[i]->Format(dst);
    if (i < child_list_.size() - 1) {
      PRINT(", ")
    }
  }
}

bool ExpressionListNode::set_types(
    std::vector<TypeTemplate *> *type_list) {
  if (!type_list)
    return true;
  for (auto i : *type_list) {
    if (is_basic(i) || i == TYPE_STRINGLIKE) {
      basic_types.push_back(dynamic_cast<BasicType *>(i));
    } else if (i != TYPE_ERROR && i->StringLike()) {
      basic_types.push_back(TYPE_STRINGLIKE);
    } else {
      return false;
    }
  }
  return true;
}

void ExpressionListNode::set_ref(std::stack<bool> *ref) {
  if (grammar_type_ == GrammarType::EXP) {
    if (ref->top())
      child_list_[0]->DynamicCast<ExpressionNode>()->set_is_ref();
  } else {
    if (ref->top())
      child_list_[1]->DynamicCast<ExpressionNode>()->set_is_ref();
    ref->pop();
    child_list_[0]->DynamicCast<ExpressionListNode>()->set_ref(ref);
  }
}

void ExpressionNode::Format(FILE *dst) {
  if (is_ref_)
    PRINT("&")
  Node::Format(dst);
}

void StrExpressionNode::Format(FILE *dst) {
  if (child_list_.size() == 2) {
    FormatAt(0, dst);
  }
  PRINT("\"")
  FormatAt(-1, dst);
  PRINT("\"")
}

void TermNode::Format(FILE *dst) {
  if (op_div)
    PRINT("(float)")
  for (auto child : child_list_) {
    child->Format(dst);
  }
}

void FactorNode::Format(FILE *dst) {
  switch (grammar_type_) {
  case GrammarType::UCONST_VAR:
  case GrammarType::VARIABLE:
    FormatAt(0, dst);
    break;

  case GrammarType::EXP:
    PRINT("(")
    FormatAt(0, dst);
    PRINT(")")
    break;

  case GrammarType::ID_EXP_LIST:
    FormatAt(0, dst);
    PRINT("(")
    FormatAt(1, dst);
    PRINT(")")
    break;

  case GrammarType::NOT:
    PRINT("!")
    FormatAt(0, dst);
    break;

  default:
    break;
  }
}

void ConstListNode::Format_Constlist(FILE *dst, StatementNode *statement) {
  if (child_list_.size() == 1) {
    PRINT("case ")
    child_list_[0]->Format(dst);
    PRINT(":\n")
    statement->Format(dst);
    PRINT("break;\n")
  } else {
    child_list_[0]->DynamicCast<ConstListNode>()->Format_Constlist(dst,
                                                                   statement);
    PRINT("case ")
    child_list_[1]->Format(dst);
    PRINT(":\n")
    statement->Format(dst);
    PRINT("break;\n")
  }
}

/////////////////////////////

} // namespace ast
}// namespace pascals