#include "symbol_table.h"
#include "type.h"
#include "symbol.h"

using std::vector;
using std::string;
using namespace symbol_table;
using namespace pascal_symbol;

namespace pascal_type {

bool TypeTemplate::ComputeType(TypeTemplate* type1, TypeTemplate* type2, std::string op,
                               TypeTemplate** result_type = nullptr) {
  if (operation_map.find(Operation(type1, type2, op)) != operation_map.end()) {
    if (result_type != nullptr) {
      *result_type = operation_map[Operation(type1, type2, op)];
    }
    return true;
  } else {
    return false;
  }
}

bool TypeTemplate::operator==(const TypeTemplate& type) const {
  if
}

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



void TypeInit() {
  BOOL = new BasicType();
  CHAR = new BasicType();
  INT = new BasicType();
  REAL = new BasicType();
  operation_map[Operation(BOOL, BOOL, "and")] = BOOL;
  operation_map[Operation(BOOL, BOOL, "or")] = BOOL;

}
} // namespace pascal_type