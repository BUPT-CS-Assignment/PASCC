//
// Created by jianxff on 2023/4/17.
//
#include "ast.h"
#include "log.h"
#include <fstream>

using std::vector;
using std::string;
using namespace symbol_table;
using json = nlohmann::json;
using namespace pascal_symbol;

namespace ast {
/////////////////////////////
Node* Node::Create(std::string node_name, int sub_type, int other_type) {
  if (node_name == "leaf") {
    return new LeafNode();
  } else if (node_name == "program") {
    return new ProgramNode();
  } else if (node_name == "program_head") {
    return new ProgramHeadNode();
  } else if (node_name == "program_body") {
    return new ProgramBodyNode();
  } else if (node_name == "idlists") {
    return new IdListNode((IdListNode::GrammarType)sub_type);
  } else if (node_name == "const_decls") {
    return new ConstDeclarationsNode();
  } else if (node_name == "const_decl") {
    return new ConstDeclarationNode((ConstDeclarationNode::GrammarType)sub_type);
  } else if (node_name == "const_var") {
    return new ConstVariableNode();
  } else if (node_name == "var_decls") {
    return new VariableDeclarationsNode();
  } else if (node_name == "var_decl") {
    return new VariableDeclarationNode((VariableDeclarationNode::GrammarType)sub_type,
                                       (VariableDeclarationNode::ListType)other_type);
  } else if (node_name == "type_decls") {
    return new TypeDeclarationsNode();
  } else if (node_name == "type_decl") {
    return new TypeDeclarationNode((TypeDeclarationNode::GrammarType)sub_type);
  } else if (node_name == "type") {
    return new TypeNode((TypeNode::GrammarType)sub_type);
  } else if (node_name == "basic_type") {
    return new BasicTypeNode();
  } else if (node_name == "record_body") {
    return new RecordBodyNode();
  } else if (node_name == "periods") {
    return new PeriodsNode();
  } else if (node_name == "period") {
    return new PeriodNode();
  } else if (node_name == "subp_decls") {
    return new SubprogramDeclarationsNode();
  } else if (node_name == "subp_decl") {
    return new SubprogramDeclarationNode();
  } else if (node_name == "subp_head") {
    return new SubprogramHeadNode((SubprogramHeadNode::GrammarType)sub_type);
  } else if (node_name == "subp_body") {
    return new SubprogramBodyNode();
  } else if (node_name == "formal_param") {
    return new FormalParamNode();
  } else if (node_name == "param_lists") {
    return new ParamListsNode((ParamListsNode::GrammarType)sub_type);
  } else if (node_name == "param_list") {
    return new ParamListNode();
  } else if (node_name == "var_param") {
    return new VarParamNode();
  } else if (node_name == "value_param") {
    return new ValueParamNode();
  } else if (node_name == "comp_stmt") {
    return new CompoundStatementNode();
  } else if (node_name == "stmt_list") {
    return new StatementListNode();
  } else if (node_name == "stmt") {
    return new StatementNode((StatementNode::GrammarType)sub_type);
  } else if (node_name == "var_list") {
    return new VariableListNode((VariableListNode::GrammarType)sub_type);
  } else if (node_name == "var") {
    return new VariableNode();
  } else if (node_name == "id_varparts") {
    return new IDVarPartsNode();
  } else if (node_name == "id_varpart") {
    return new IDVarPartNode((IDVarPartNode::GrammarType)sub_type);
  } else if (node_name == "branch_list") {
    return new BranchListNode((BranchListNode::GrammarType)sub_type);
  } else if (node_name == "case_body") {
    return new CaseBodyNode((CaseBodyNode::GrammarType)sub_type);
  } else if (node_name == "branch") {
    return new BranchNode();
  } else if (node_name == "const_list") {
    return new ConstListNode();
  } else if (node_name == "updown") {
    return new UpdownNode();
  } else if (node_name == "proc_call") {
    return new ProcedureCallNode((ProcedureCallNode::GrammarType)sub_type);
  } else if (node_name == "else") {
    return new ElseNode((ElseNode::GrammarType)sub_type);
  } else if (node_name == "exp_list") {
    return new ExpressionListNode((ExpressionListNode::GrammarType)sub_type);
  } else if (node_name == "exp") {
    return new ExpressionNode();
  } else if (node_name == "simple_exp") {
    return new SimpleExpressionNode();
  } else if (node_name == "term") {
    return new TermNode();
  } else if (node_name == "factor") {
    return new FactorNode((FactorNode::GrammarType)sub_type);
  } else if (node_name == "uconst_var") {
    return new UnsignConstVarNode();
  } else {
    return nullptr;
  }
}

Node* Node::Create(nlohmann::json &json_node) {
  string type_name = json_node.at("type");
  int sub_type = 0, other_type = 0;
  if(json_node.contains("sub_type")) sub_type = json_node.at("sub_type");
  if(json_node.contains("other_type")) other_type = json_node.at("other_type");
  return Create(type_name, sub_type, other_type);
}

void AST::LoadFromJson(std::string file_name) {
  std::ifstream ifs(file_name);

  if (!ifs.is_open()) {
    log_error("failed to open file : %s", file_name.c_str());
    exit(-1);
  }

  json ast_json;
  ifs >> ast_json;
  ifs.close();

  log_info("successly load ast from json : %s", file_name.c_str());

  root_ = new ProgramNode();
  root_->LoadFromJson(ast_json);
}


void Node::LoadFromJson(const nlohmann::json & node_json) {
  if(!node_json.contains("children")) return;
  for(auto item : node_json.at("children")) {

    string type_name = item.at("type");
    int sub_type = 0, other_type = 0;
    if(item.contains("sub_type")) sub_type = item.at("sub_type");
    if(item.contains("other_type")) other_type = item.at("other_type");

    Node* node = Node::Create(type_name, sub_type, other_type);
    if(type_name == "leaf") {
      LeafNode* lf = node->StaticCast<LeafNode>();

      if(sub_type == (int)LeafNode::LEAF_TYPE::IDENTIFIER) {
        lf->set_id(item.at("value"));
        if(item.contains("ref"))
          lf->set_ref(item.at("ref"));

      } else if(sub_type == (int)LeafNode::LEAF_TYPE::OPERATION) {
        lf->set_op(item.at("value"));

      } else {
        string value_type = item.at("value_type");
        if(value_type == "integer") {
          lf->set_value(ConstValue(item.at("value").get<int>()));
        } else if(value_type == "real") {
          lf->set_value(ConstValue(item.at("value").get<float>()));
        } else if(value_type == "char") {
          lf->set_value(ConstValue(item.at("value").get<char>()));
        } else if(value_type == "bool") {
          lf->set_value(ConstValue(item.at("value").get<bool>()));
        }
      }
    }
    else if(type_name == "basic_type") {
      BasicTypeNode* bt = node->StaticCast<BasicTypeNode>();
      string value_type = item.at("value");
      if(value_type == "integer") {
        bt->set_type(TYPE_INT);
      } else if(value_type == "real") {
        bt->set_type(TYPE_REAL);
      } else if(value_type == "char") {
        bt->set_type(TYPE_CHAR);
      } else if(value_type == "bool") {
        bt->set_type(TYPE_BOOL);
      }
    }else if(type_name == "type") {
      TypeNode* tn = node->StaticCast<TypeNode>();
      if(!item.contains("base_type")) {
        tn->set_base_type_node(tn);
      } else {
        Node* base_node = Node::Create(item.at("base_type"));
        tn->set_base_type_node(base_node->DynamicCast<TypeNode>());
      }
    }

    node->LoadFromJson(item);
    node->set_parent(this);

    this->append_child(node);
  }
}

} // namespace ast