#ifndef PASCC_SYMBOL_TABLE_H_
#define PASCC_SYMBOL_TABLE_H_

#include <string>
#include <unordered_map>

#include "log.h"
#include "symbol.h"
#include "type.h"

namespace pascals {

using namespace pascals;

// table template
template <typename T>
class SymbolTableTemplate {
 public:
  SymbolTableTemplate(std::string tag = "") : tag_(tag) {}
  ~SymbolTableTemplate() {}
  // insert symbol
  bool Insert(std::string name, T* symbol) {
    if (table_.find(name) != table_.end()) {
      return false;
    }
    table_.insert(std::make_pair(name, symbol));
    return true;
  }

  // find by name
  T* FindName(std::string name) {
    auto it = table_.find(name);
    if (it != table_.end()) return it->second;
    return nullptr;
  }
  T* operator[](std::string name) { return FindName(name); }

 protected:
  std::string tag_;
  std::unordered_map<std::string, T*> table_;
};

/**
 * @brief type table
 */
class TypeTable : public SymbolTableTemplate<TypeTemplate> {
 public:
  TypeTable(std::string tag = "") : SymbolTableTemplate(tag + "_type") {
    log_debug("TypeTable(): create '%s'", tag_.c_str());
  }
  ~TypeTable() {
    log_debug("~TypeTable(): delete %d symbols from '%s'", table_.size(),
              tag_.c_str());
    for (auto it = table_.begin(); it != table_.end(); ++it) {
      if (!(it->second->template_type() == TypeTemplate::TYPE::BASIC))
        delete it->second;
    }
  }

  // find by name with type check
  template <typename T>
  T* Find(std::string name) {
    auto entry = (*this)[name];
    if (entry == nullptr) return nullptr;
    if (std::is_same<T, TypeTemplate>::value ||
        std::is_same<T, BasicType>::value &&
            entry->template_type() == TypeTemplate::TYPE::BASIC ||
        std::is_same<T, ArrayType>::value &&
            entry->template_type() == TypeTemplate::TYPE::ARRAY ||
        std::is_same<T, RecordType>::value &&
            entry->template_type() == TypeTemplate::TYPE::RECORD) {
      return dynamic_cast<T*>(entry);
    }
    return nullptr;
  }
};

/**
 * @brief symbol table
 */
class SymbolTable : public SymbolTableTemplate<ObjectSymbol> {
 public:
  SymbolTable(std::string tag = "") : SymbolTableTemplate(tag + "_symbols") {
    log_debug("SymbolTable(): create '%s'", tag_.c_str());
  }
  ~SymbolTable() {
    log_debug("~SymbolTable(): delete %d symbols from '%s'", table_.size(),
              tag_.c_str());
    for (auto it = table_.begin(); it != table_.end(); ++it) {
      delete it->second;
    }
  }

  // find by name with type check
  template <typename T>
  T* Find(std::string name) {
    auto entry = (*this)[name];
    if (entry == nullptr) return nullptr;
    if (std::is_same<T, ObjectSymbol>::value ||
        std::is_same<T, FunctionSymbol>::value &&
            entry->symbol_type() == ObjectSymbol::SYMBOL_TYPE::FUNCTION ||
        std::is_same<T, ConstSymbol>::value &&
            entry->symbol_type() == ObjectSymbol::SYMBOL_TYPE::CONST) {
      return dynamic_cast<T*>(entry);
    }
    return nullptr;
  }
};

/**
 * @brief table set including symbol table and type table
 */
class TableSet {
 public:
  TableSet(std::string tag, TableSet* pre_set)
      : tag_(tag),
        prev_table_set_(pre_set),
        symbols_(SymbolTable(tag)),
        def_types_(TypeTable(tag)) {
    log_debug("TableSet(): created '%s'", tag.c_str());
  }
  ~TableSet() {
    prev_table_set_ = nullptr;
    log_debug("~TableSet(): delete '%s'", tag_.c_str());
  }

  // getter and setter
  SymbolTable* symbols() { return &symbols_; }
  TypeTable* def_types() { return &def_types_; }
  TableSet* previous() { return prev_table_set_; }
  std::string tag() { return tag_; }

  // united insert entry
  template <typename T>
  bool Insert(std::string name, T* symbol) {
    ObjectSymbol* object_flag = symbols_.FindName(name);
    TypeTemplate* type_flag = SearchEntry<TypeTemplate>(name);
    if (object_flag != nullptr || type_flag != nullptr) {
      return false;
    }
    if (std::is_same<T, ObjectSymbol>::value ||
        std::is_same<T, ConstSymbol>::value ||
        std::is_same<T, FunctionSymbol>::value) {
      symbols_.Insert(name, (ObjectSymbol*)symbol);
    } else if (std::is_same<T, TypeTemplate>::value ||
               std::is_same<T, ArrayType>::value ||
               std::is_same<T, BasicType>::value ||
               std::is_same<T, RecordType>::value) {
      def_types_.Insert(name, (TypeTemplate*)symbol);
    }
    return true;
  }

  /**
   * Unified search entry of Symbol (from symbol-table) or Type (form
   * type-table)
   * @tparam T ObjectSymbol || TypeTemplate | ArrayType | RecordType | BasciType
   * @param name name of identifier
   * @param local_zone if is searched from current layer
   * @return T-pointer
   */
  template <typename T>
  T* SearchEntry(std::string name, bool* local_zone = nullptr) {
    if (local_zone != nullptr) *local_zone = true;
    if (std::is_same<T, ObjectSymbol>::value ||
        std::is_same<T, ConstSymbol>::value ||
        std::is_same<T, FunctionSymbol>::value) {
      auto symbol_entry = symbols_.Find<T>(name);
      if (symbol_entry != nullptr) return (T*)symbol_entry;
    } else if (std::is_same<T, TypeTemplate>::value ||
               std::is_same<T, ArrayType>::value ||
               std::is_same<T, BasicType>::value ||
               std::is_same<T, RecordType>::value) {
      auto type_entry = def_types_.Find<T>(name);
      if (type_entry != nullptr) return (T*)type_entry;
    }

    if (local_zone != nullptr) *local_zone = false;
    if (prev_table_set_ != nullptr)
      return prev_table_set_->SearchEntry<T>(name);
    return nullptr;
  }

 private:
  std::string tag_;
  SymbolTable symbols_;       // symbol table
  TypeTable def_types_;       // type table
  TableSet* prev_table_set_;  // up-layer table set
};

}  // namespace pascals

#endif  // PASCC_SYMBOL_TABLE_H_