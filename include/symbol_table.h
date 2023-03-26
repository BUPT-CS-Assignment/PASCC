#ifndef PASCC_SYMBOL_TABLE_H_
#define PASCC_SYMBOL_TABLE_H_

#include <unordered_map>
#include <string>
#include "type.h"

namespace symbol_table {

using namespace pascal_type;

template <typename T>
class SymbolTable {
 public:
  SymbolTable() {}
  ~SymbolTable() {}
  bool Insert(std::string name, T *symbol) {
    if (table_.find(name) != table_.end()) {
      return false;
    }
    table_.insert(std::make_pair(name, symbol));
    return true;
  }

  T *Find(std::string name) {
    auto it = table_.find(name);
    if (it != table_.end()) {
      return it->second;
    }
    return nullptr;
  }
 protected:
  std::unordered_map<std::string, T*> table_;
};

class BasicSymbolTable : public SymbolTable<ObjectSymbol> {};
class ArraySymbolTable : public SymbolTable<ObjectSymbol> {};
class FunctionSymbolTable : public SymbolTable<FunctionSymbol> {};
class ConstSymbolTable : public SymbolTable<ConstSymbol> {};
class TypeSymbolTable : public SymbolTable<TypeTemplate> {};

} // namespace symbol_table

#endif // PASCC_SYMBOL_TABLE_H_