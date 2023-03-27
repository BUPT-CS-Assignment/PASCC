#ifndef PASCC_SYMBOL_TABLE_H_
#define PASCC_SYMBOL_TABLE_H_

#include <unordered_map>
#include <string>
#include "type.h"
#include "symbol.h"

namespace symbol_table {

using namespace pascal_type;

template <typename T>
class SymbolTableTemplate {
 public:
  SymbolTableTemplate() {}
  ~SymbolTableTemplate() {}
  bool Insert(std::string name, T *symbol) {
    if (table_.find(name) != table_.end()) {
      return false;
    }
    table_.insert(std::make_pair(name, symbol));
    return true;
  }

  T *Find(std::string name) {
    auto it = table_.find(name);
    if (it != table_.end()) return it->second;
    return nullptr;
  }
 protected:
  std::unordered_map<std::string, T*> table_;
};

class TypeTable : public SymbolTableTemplate<TypeTemplate> {};
class SymbolTable : public SymbolTableTemplate<pascal_symbol::ObjectSymbol> {};

class TableSet {
 public:
  TableSet(std::string tag, TableSet* pre_set) : tag_(tag), prev_table_set_(pre_set) {}
  ~TableSet() {}

  SymbolTable *symbols() { return &symbols_; }
  TypeTable *def_types() { return &def_types_; }
  TableSet *prev_table_set() { return prev_table_set_; }

  std::string tag() { return tag_; }
  std::unordered_map<std::string, TableSet*> *next_table_sets() { return &next_table_sets_; }

  template <typename T> T* SearchEntry(std::string name) {
    if(std::is_same<T,pascal_symbol::ObjectSymbol>::value) {
      auto symbol_entry = symbols_.Find(name);
      if (symbol_entry != nullptr)  return (T*)symbol_entry;
    } else if (std::is_same<T, TypeTemplate>::value ||
               std::is_same<T, ArrayType>::value ||
               std::is_same<T, RecordType>::value) {
      auto type_entry = def_types_.Find(name);
      if (type_entry != nullptr)  return (T*)type_entry;
    }
    if(prev_table_set_ != nullptr)  return prev_table_set_->SearchEntry<T>(name);
    else return nullptr;
  }

  TableSet* CreateNext(std::string tag);
  void LoadFromJson(nlohmann::json&);


 private:
  std::string tag_;
  SymbolTable symbols_;
  TypeTable def_types_;

  TableSet* prev_table_set_;
  std::unordered_map<std::string, TableSet*> next_table_sets_;

};

} // namespace symbol_table

#endif // PASCC_SYMBOL_TABLE_H_