#include "type.h"
#include "symbol_table.h"

using std::vector;
using std::string;
using namespace symbol_table;

namespace pascal_type {

ArrayType::ArrayType(nlohmann::json& json, void* table) : TypeTemplate(TYPE::ARRAY) {
  TableSet* cur_table = (TableSet*)table;
  std::string type_str = json["base_type"].get<std::string>();
  type_ = cur_table->SearchEntry<TypeTemplate>(type_str);

  for (auto& bound : json["bounds"]) {
    string type_str = bound["type"].get<string>();
    bound_types_.emplace_back(cur_table->SearchEntry<TypeTemplate>(type_str));

    if(type_str == "int"){
        bounds_.push_back(std::make_pair(bound["lb"].get<int>(), bound["ub"].get<int>()));
    } else if (type_str == "char") {
        bounds_.push_back(std::make_pair((int)bound["lb"].get<string>()[0], (int)bound["ub"].get<string>()[0]));
    }
  }

}


bool ArrayType::AccessArray(vector<TypeTemplate*> index_types, TypeTemplate **type) {
  if (index_types.size() != bound_types_.size()) {
    return false;
  }
  for (int i = 0; i < index_types.size(); i++) {
    if (index_types[i] != bound_types_[i]) {
      return false;
    }
  }
  if(type != nullptr)
    *type = type_;

  return true;
}

void RecordType::InsertType(std::string name, TypeTemplate* type) {
  types_map_.insert(std::make_pair(name, type));
  types_num_++;
}

TypeTemplate* RecordType::Find(std::string name) {
  auto ptr = types_map_.find(name);
  if(ptr != types_map_.end()) {
    return (*ptr).second;
  } else {
    return nullptr;
  }
}

bool FunctionSymbol::AssertParams(const vector<BasicType*>& params_in){
  if (params_in.size() != params_.size()) {
    return false;
  }

  for (int i = 0; i < params_.size(); i++) {
    // assert parameter type
    if(params_in[i] != params_[i].first) {
      return false;
    }
  }
  return true;
}

} // namespace pascal_type