#include "ast.h"

#include "log.h"

using std::string;
using std::vector;
using json = nlohmann::json;
using namespace symbol_table;
using namespace pascal_symbol;

namespace ast {
/* **************** standard output **************** */

/* **************** ast output **************** */
void AST::Format(const char* file_name) {
  FILE* dst = fopen(file_name, "w");
  if (dst == nullptr) {
    log_fatal("failed to open file %s", file_name);
    abort();
  }

  if (root_ != nullptr) root_->Format(dst);
  log_info("ast format success");
  fclose(dst);
}

//////////////////////////////

void LeafNode::Format(FILE* dst) {
  if (leaf_type_ != LEAF_TYPE::CONST_VALUE) {
    PRINT(" %s ", name_.c_str())
  } else {
    BasicType* tp = value_.type();
    if (tp == pascal_type::TYPE_INT) {
      PRINT(" %d ", value_.get<int>())
    } else if (tp == pascal_type::TYPE_REAL) {
      PRINT(" %.2f ", value_.get<float>())
      // fprintf(dst, "%.2f", value_.get<float>());
    } else if (tp == pascal_type::TYPE_BOOL) {
      PRINT(" %s ", value_.get<bool>() ? "true" : "false")
    } else if (tp == pascal_type::TYPE_CHAR) {
      PRINT(" '%c' ", value_.get<char>())
    }
  }
}

bool LeafNode::AnalyzeReference(symbol_table::TableSet* ts,
                                pascal_symbol::FunctionSymbol* fn) {
  if (fn == nullptr || leaf_type_ != LEAF_TYPE::IDENTIFIER) return false;

  // search table and judge if is current layer
  bool local = false;
  ObjectSymbol* entry =
      ts->SearchEntry<pascal_symbol::ObjectSymbol>(name_, &local);
  if (entry != nullptr && local) {
    FunctionSymbol::ParamType* pt = (*fn)[name_];
    // search param-passing mode
    if (pt != nullptr && pt->second == FunctionSymbol::PARAM_MODE::REFERENCE) {
      is_ref_ = true;
    }
  }
  return is_ref_;
}
/////////////////////////////////////////////

void ProgramHeadNode::Format(FILE* dst) {
  PRINT("#include <stdio.h>\n")
  PRINT("//")
  FormatAt(0, dst);
  PRINT("\n")
}

void ProgramBodyNode::Format(FILE* dst) {
  FormatAt(0, dst);  // const decl
  FormatAt(1, dst);  // type decl
  FormatAt(2, dst);  // var decl
  FormatAt(3, dst);  // sub prog decl
  PRINT("int main(int argc, char** argv) {\n")
  PRINT("if (argc >= 1) freopen(argv[1], \"r\", stdin);\n")
  PRINT("if (argc >= 2) freopen(argv[2], \"w\", stdout);\n")
  FormatAt(4, dst);  // comp stat
  PRINT("}\n")
}

void IdListNode::Format(FILE* dst) {
  FormatAt(0, dst);
  if (grammar_type_ == GrammarType::MULTIPLE_ID) {
    PRINT(", ")
    FormatAt(1, dst);
  }
}

vector<LeafNode*> IdListNode::Lists() {
  vector<LeafNode*> lists;
  IdListNode* cur_node = this;
  GrammarType gtype = grammar_type_;

  while (gtype == GrammarType::MULTIPLE_ID) {
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

// void ConstDeclarationsNode::TransCode() {
//   if (grammar_type_ == GrammarType::CONST_DECLARATION) {
//     TransCodeAt(0);
//   }
// }

void ConstDeclarationNode::Format(FILE* dst) {
  if (grammar_type_ == GrammarType::DECLARATION) {
    FormatAt(0, dst);
    PRINT("const ")
    FormatAt(1, dst);
    PRINT(" = ")
    FormatAt(2, dst);
    PRINT(";\n");
  } else if (grammar_type_ == GrammarType::VALUE) {
    PRINT("const ")
    FormatAt(0, dst);
    PRINT(" = ")
    FormatAt(1, dst);
    PRINT(";\n");
  }
}

void VariableDeclarationNode::Format(FILE* dst) {
  if (grammar_type_ == GrammarType::MULTIPLE_DECL) {
    FormatAt(0, dst);
  }
  // analyze current layer
  size_t last = child_list_.size() - 1;
  Node* tnode = child_list_[last];
  vector<LeafNode*> idlist =
      child_list_[last - 1]->DynamicCast<IdListNode>()->Lists();

  if (list_type_ == ListType::ID) {
    tnode->Format(dst);
    PRINT(" ");
    for (auto id : idlist) id->Format(dst);
  } else {
    // ListType::TYPE
    // TypeNode* tn = tnode->DynamicCast<TypeNode>()->base_type();
    TypeNode* tn = tnode->DynamicCast<TypeNode>();
    tn->base_type()->Format(dst);
    PRINT(" ");
    // idlist
    bool array_flag = (tn->grammar_type() == TypeNode::GrammarType::ARRAY);
    for (int i = 0; i < idlist.size(); i++) {
      idlist[i]->Format(dst);
      if (array_flag) tn->PeriodsFormat(dst);
      if (i < idlist.size() - 1) {
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

// void TypeDeclarationsNode::TransCode() {
//   if(grammar_type_ == GrammarType::TYPE_DECLARATION) {
//     TransCodeAt(0);
//   }
// }

void TypeDeclarationNode::Format(FILE* dst) {
  if (grammar_type_ == GrammarType::MULTIPLE_DECL) {
    FormatAt(0, dst);
  }
  // analyze current layer
  TypeNode* tnode = (*child_list_.rbegin())->DynamicCast<TypeNode>();
  LeafNode* idnode = (*(child_list_.rbegin() + 1))->DynamicCast<LeafNode>();

  TypeNode* base_type = tnode->base_type();
  PRINT("typedef ")
  base_type->Format(dst);
  if (tnode->grammar_type() == TypeNode::GrammarType::ARRAY) {
    tnode->PeriodsFormat(dst);
  }
  PRINT(" ")
  idnode->Format(dst);
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

void TypeNode::Format(FILE* dst) {
  switch (grammar_type_) {
    case GrammarType::BASIC_TYPE:
      FormatAt(0, dst);
      break;
    case GrammarType::ARRAY:  // upper funciton will solve this
      break;
    case GrammarType::RECORD_TYPE:
      PRINT("struct {\n")
      FormatAt(0, dst);
      PRINT("}")
      break;
  }
}

void TypeNode::PeriodsFormat(FILE* dst) {
  if (grammar_type_ == GrammarType::ARRAY) {
    child_list_[0]->Format(dst);
    child_list_[1]->DynamicCast<TypeNode>()->PeriodsFormat(dst);
  }
}

void BasicTypeNode::Format(FILE* dst) {
  PRINT("%s", type_->type_name().c_str())
}

void PeriodNode::Format(FILE* dst) { PRINT("[%d]", len_) }

void SubprogramDeclarationNode::Format(FILE* dst) {
  FormatAt(0, dst);
  PRINT("{\n")
  FormatAt(1, dst);
  PRINT("}\n")
}

void SubprogramHeadNode::Format(FILE* dst) {
  if (grammar_type_ == GrammarType::PROCEDURE) {
    PRINT("void ")
  } else {
    FormatAt(2, dst);  // basic_type
  }
  FormatAt(0, dst);
  FormatAt(1, dst);
}

void FormalParamNode::Format(FILE* dst) {
  PRINT("(")
  for (auto child : child_list_) {
    child->Format(dst);
  }
  //  if (grammar_type_ == GrammarType::PARAM_LISTS) {
  //    TransCodeAt(0);
  //  }
  PRINT(")")
}

void ParamListsNode::Format(FILE* dst) {
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

void VarParamNode::Format(FILE* dst) {
  ValueParamNode* vp = child_list_[0]->DynamicCast<ValueParamNode>();
  vp->Format(true, dst);
}

void ValueParamNode::Format(FILE* dst) { this->Format(false, dst); }

void ValueParamNode::Format(bool ref, FILE* dst) {
  // get idlists
  IdListNode* idnode = child_list_[0]->DynamicCast<IdListNode>();
  // get type
  BasicTypeNode* tnode = child_list_[1]->DynamicCast<BasicTypeNode>();
  vector<LeafNode*> idlist = idnode->Lists();
  string tname = tnode->TypeName(ref);

  for (int i = 0; i < idlist.size(); i++) {
    PRINT("%s %s", tname.c_str(), idlist[i]->id().c_str());
    if (i < idlist.size() - 1) {
      PRINT(", ")
    }
  }
}

void CompoundStatementNode::Format(FILE* dst) {
  PRINT("{\n")
  FormatAt(0, dst);
  PRINT("}\n")
}

void StatementNode::Format(FILE* dst) {
  switch (grammar_type_) {
    case GrammarType::EPSILON:
      return;
    case GrammarType::VAR_ASSIGN_OP_EXP:
      FormatAt(0, dst);
      PRINT(" = ")
      FormatAt(1, dst);
      PRINT(";\n")
      break;
    case GrammarType::FUNC_ASSIGN_OP_EXP:
      PRINT("return ")
      FormatAt(1, dst);
      PRINT(";\n");
      break;
    case GrammarType::PROCEDURE_CALL:
    case GrammarType::COMPOUND_STATEMENT:
      FormatAt(0, dst);
      break;
    case GrammarType::IF_STATEMENT:
      PRINT("if (")
      FormatAt(0, dst);
      PRINT(") {\n")
      FormatAt(1, dst);
      PRINT("}\n")
      FormatAt(2, dst);
      break;
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
      for (int i = 4; i < child_list_.size(); i++) FormatAt(i, dst);
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
      FormatAt(0, dst);
      PRINT(") {\n")
      FormatAt(1, dst);
      PRINT("}\n")
      break;
    case GrammarType::WHILE_STATEMENT:
      PRINT("while (")
      FormatAt(0, dst);
      PRINT(") {\n")
      FormatAt(1, dst);
      PRINT("}\n")
      break;
    case GrammarType::REPEAT_STATEMENT:
      PRINT("do {\n")
      FormatAt(0, dst);
      PRINT("} while (")
      FormatAt(1, dst);
      PRINT(");\n")
      break;
  }
}

void VariableListNode::Format(FILE* dst) {
  FormatAt(0, dst);
  if (grammar_type_ == GrammarType::VARIABLE_LIST_VARIABLE) {
    PRINT(",")
    FormatAt(1, dst);
  }
}

void IDVarPartNode::Format(FILE* dst) {
  if (grammar_type_ == GrammarType::_ID) {
    PRINT(".")
    FormatAt(0, dst);
  } else if (grammar_type_ == GrammarType::EXP_LIST) {
    PRINT("[")
    FormatAt(0, dst);
    PRINT("]")
  }
}

void BranchNode::Format(FILE* dst) {
  for (int id = 0; id < child_list_.size(); id += 2) {
    auto const_list = child_list_[id]->DynamicCast<ConstListNode>();
    auto statement = child_list_[id + 1];
    auto const_vars = const_list->Consts();
    for (auto const_var : *const_vars) {
      PRINT("case ")
      const_var->Format(dst);
      PRINT(":\n")
      statement->Format(dst);
    }
  }
}

void ProcedureCallNode::Format(FILE* dst) {
  FormatAt(0, dst);
  if (grammar_type_ == GrammarType::ID) {
    PRINT("();")
  } else if (grammar_type_ == GrammarType::ID_EXP_LIST) {
    PRINT("(")
    FormatAt(1, dst);
    PRINT(");")
  }
}

void ElseNode::Format(FILE* dst) {
  switch (grammar_type_) {
    case GrammarType::EPSILON:
      return;
    case GrammarType::ELSE_STATEMENT:
      PRINT("else {\n")
      GetStatement()->Format(dst);
      PRINT("}\n")
  }
}

void ExpressionListNode::Format(FILE* dst) {
  for (int i = 0; i < child_list_.size(); i++) {
    child_list_[i]->Format(dst);
    if (i < child_list_.size() - 1) {
      PRINT(", ")
    }
  }
}

void FactorNode::Format(FILE* dst) {
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

/////////////////////////////

}  // namespace ast