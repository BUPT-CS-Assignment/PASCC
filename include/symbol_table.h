#ifndef PASCC_SYMBOL_TABLE_H_
#define PASCC_SYMBOL_TABLE_H_

#include <unordered_map>
#include <string>
#include "type.h"
#include "symbol.h"

namespace symbol_table {

using namespace pascal_type;

// table template
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

  T* operator[](std::string name) {
    return Find(name);
  }

 protected:
  std::unordered_map<std::string, T*> table_;
};

class TypeTable : public SymbolTableTemplate<TypeTemplate> {};
class SymbolTable : public SymbolTableTemplate<pascal_symbol::ObjectSymbol> {};

// table set including symbol table and type table
class TableSet {
 public:
  TableSet(std::string tag, TableSet* pre_set) : tag_(tag), prev_table_set_(pre_set) {}
  ~TableSet() {}

  SymbolTable *symbols() { return &symbols_; }
  TypeTable *def_types() { return &def_types_; }
  TableSet *previous() { return prev_table_set_; }
  std::string tag() { return tag_; }

  template<typename T>
  bool Insert(std::string name,T* symbol){
    pascal_symbol::ObjectSymbol* object_flag = symbols_.Find(name);
    TypeTemplate* type_flag = SearchEntry<TypeTemplate>(name);
    if (object_flag != nullptr || type_flag != nullptr) {
      return false;
    }
    if(std::is_same<T,pascal_symbol::ObjectSymbol>::value||
       std::is_same<T,pascal_symbol::ConstSymbol>::value||
       std::is_same<T,pascal_symbol::FunctionSymbol>::value) {
      symbols_.Insert(name, (pascal_symbol::ObjectSymbol*)symbol);
    } else if (std::is_same<T, TypeTemplate>::value ||
               std::is_same<T, ArrayType>::value ||
               std::is_same<T,BasicType>::value ||
               std::is_same<T, RecordType>::value) {
      def_types_.Insert(name, (TypeTemplate*)symbol);
    }
    return true;
  }

  /**
   * Unified search entry of Symbol (from symbol-table) or Type (form type-table)
   * @tparam T ObjectSymbol || TypeTemplate | ArrayType | RecordType | BasciType
   * @param name name of identifier
   * @param local_zone if is searched from current layer
   * @return T-pointer
   */
  template <typename T> T* SearchEntry(std::string name, bool* local_zone = nullptr) {
    if(local_zone != nullptr) *local_zone = true;
    if(std::is_same<T,pascal_symbol::ObjectSymbol>::value||
       std::is_same<T,pascal_symbol::ConstSymbol>::value||
       std::is_same<T,pascal_symbol::FunctionSymbol>::value) {
      auto symbol_entry = symbols_.Find(name);
      if (symbol_entry != nullptr)  return (T*)symbol_entry;
    } else if (std::is_same<T, TypeTemplate>::value ||
               std::is_same<T, ArrayType>::value ||
               std::is_same<T, BasicType>::value ||
               std::is_same<T, RecordType>::value) {
      auto type_entry = def_types_.Find(name);
      if (type_entry != nullptr)  return (T*)type_entry;
    }

    if(local_zone != nullptr) *local_zone = false;
    if(prev_table_set_ != nullptr)  return prev_table_set_->SearchEntry<T>(name);
    return nullptr;
  }


 private:
  std::string tag_;
  SymbolTable symbols_;
  TypeTable def_types_;
  TableSet* prev_table_set_;
};

} // namespace symbol_table

#endif // PASCC_SYMBOL_TABLE_H_