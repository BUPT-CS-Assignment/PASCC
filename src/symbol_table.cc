//
// Created by jianxff on 2023/3/27.
//

#include "symbol_table.h"
using namespace symbol_table;
using namespace pascal_type;
using namespace pascal_symbol;
using json = nlohmann::json;
using std::vector;
using std::string;

namespace symbol_table {

TableSet* TableSet::CreateNext(std::string tag) {
  TableSet* table_set = new TableSet(tag, this);
  next_table_sets_.insert(std::make_pair(tag, table_set));
  return table_set;
}

void TableSet::LoadFromJson(nlohmann::json & input_json) {
  // parse type define
  json type_json = input_json["type_def"];
  for (auto item : type_json) {
    if (item["type_id"] == "array") {
      auto new_array = new ArrayType(item["type_info"], this);
      def_types_.Insert(item["type_name"], new_array);

    } else if (item["type_id"] == "basic") {
      // TODO
    }
  }

  // parse symbol
  json sym_json = input_json["symbol"];

  for (auto item : sym_json) {

    if (item["type_id"] == "basic") {
      BasicType* tp = SearchEntry<BasicType>(item["type"]);
      auto new_basic = new ObjectSymbol(item["type_info"], tp, 0);
      symbols_.Insert(item["name"], new_basic);

    } else if (item["type_id"] == "array") {
      ArrayType* tp = SearchEntry<ArrayType>(item["type"]);
      auto new_array = new ObjectSymbol(item["type_info"], tp, 0);
      symbols_.Insert(item["name"], new_array);

    } else if (item["type_id"] == "function") {
      vector<FunctionSymbol::Parameter> params;
      auto tp = SearchEntry<BasicType>(item["type"]);
      for(auto p : item["params"]) {
        BasicType* tp = SearchEntry<BasicType>(p["type_name"]);
        int passing = p["passing"].get<int>();
        params.emplace_back(FunctionSymbol::Parameter(tp, (FunctionSymbol::PARAM_PASSING)passing));
      }
      auto new_function = new FunctionSymbol(item["type_info"], tp, 0, params);
      symbols_.Insert(item["name"], new_function);

    } else if (item["type_id"] == "const") {
      BasicType* tp = SearchEntry<BasicType>(item["type"]);
      ConstSymbol* new_const = nullptr;
      if(item["type_name"] == "int") {
        new_const = new ConstSymbol(item["type_info"], tp, 0, item["value"].get<int>());
      } else if (item["type_name"] == "char") {
        new_const = new ConstSymbol(item["type_info"], tp, 0, item["value"].get<char>());
      } else if (item["type_name"] == "real") {
        new_const = new ConstSymbol(item["type_info"], tp, 0, item["value"].get<float>());
      }
      symbols_.Insert(item["name"], new_const);
    }
  }

}

} // namespace symbol_table