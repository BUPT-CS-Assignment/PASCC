//
// Created by jianxff on 2023/3/27.
//

#include "symbol_table.h"
using namespace symbol_table;
using namespace pascal_type;
using json = nlohmann::json;
using std::vector;
using std::string;

namespace symbol_table {

void TableSet::LoadFromJson(nlohmann::json & input_json) {
  // parse type define
  json type_json = input_json["type_def"];
  for (auto item : type_json) {
    if (item["type_id"] == "array") {
      TypeSymbolTable *tsb = table_ptr<TypeSymbolTable>();
      auto new_array = new ArrayType(item["type_info"], this);
      tsb->Insert(item["type_name"], new_array);

    } else if (item["type_id"] == "basic") {
      // TODO
    }
  }

  // parse symbol
  json sym_json = input_json["symbol"];

  for (auto item : sym_json) {

    if (item["type_id"] == "basic") {
      BasicSymbolTable *bst = table_ptr<BasicSymbolTable>();
      BasicType* tp = SearchEntry<BasicType>(item["type"]);
      auto new_basic = new ObjectSymbol(item["type_info"], tp, 0);
      bst->Insert(item["name"], new_basic);

    } else if (item["type_id"] == "array") {
      ArraySymbolTable *ast = table_ptr<ArraySymbolTable>();
      ArrayType* tp = SearchEntry<ArrayType>(item["type"]);
      auto new_array = new ObjectSymbol(item["type_info"], tp, 0);
      ast->Insert(item["name"], new_array);

    } else if (item["type_id"] == "function") {
      FunctionSymbolTable *fst = table_ptr<FunctionSymbolTable>();
      vector<FunctionSymbol::Parameter> params;
      auto tp = SearchEntry<BasicType>(item["type"]);
      for(auto p : item["params"]) {
        BasicType* tp = SearchEntry<BasicType>(p["type_name"]);
        int passing = p["passing"].get<int>();
        params.emplace_back(FunctionSymbol::Parameter(tp, (FunctionSymbol::PARAM_PASSING)passing));
      }
      auto new_function = new FunctionSymbol(item["type_info"], tp, 0, params);
      fst->Insert(item["name"], new_function);

    } else if (item["type_id"] == "const") {
      ConstSymbolTable *cst = table_ptr<ConstSymbolTable>();
      BasicType* tp = SearchEntry<BasicType>(item["type"]);
      ConstSymbol* new_const = nullptr;
      if(item["type_name"] == "int") {
        new_const = new ConstSymbol(item["type_info"], tp, 0, item["value"].get<int>());
      } else if (item["type_name"] == "char") {
        new_const = new ConstSymbol(item["type_info"], tp, 0, item["value"].get<char>());
      } else if (item["type_name"] == "real") {
        new_const = new ConstSymbol(item["type_info"], tp, 0, item["value"].get<float>());
      }
      cst->Insert(item["name"], new_const);
    }
  }

}

} // namespace symbol_table