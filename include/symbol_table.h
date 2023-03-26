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

class TableSet {
 public:
  TableSet(std::string tag, TableSet* pre_set) : tag_(tag), prev_table_set_(pre_set) {
    BasicTypeInit();
  }
  ~TableSet() {}

  TableSet *prev_table_set() { return prev_table_set_; }
  std::string tag() { return tag_; }
  std::unordered_map<std::string, TableSet*> *next_table_sets() { return &next_table_sets_; }


  template <typename T> T* table_ptr(){
    if(std::is_same<T, BasicSymbolTable>::value){
      return (T*)&basic_table_;
    }else if(std::is_same<T, ArraySymbolTable>::value) {
      return (T*)&array_table_;
    }else if(std::is_same<T, FunctionSymbolTable>::value) {
      return (T*)&function_table_;
    }else if(std::is_same<T, ConstSymbolTable>::value) {
      return (T*)&const_table_;
    }else if(std::is_same<T, TypeSymbolTable>::value) {
      return (T*)&type_table_;
    }
  }

  template <typename T>
  T* SearchEntry(std::string name) {
    if(std::is_same<T,ObjectSymbol>::value) {
      auto basic_entry = basic_table_.Find(name);
      if (basic_entry != nullptr) {
        return (T*)basic_entry;
      }
      auto array_entry = array_table_.Find(name);
      if (array_entry != nullptr) {
        return (T*)array_entry;
      }
    } else if (std::is_same<T,FunctionSymbol>::value) {
      auto function_entry = function_table_.Find(name);
      if (function_entry != nullptr) {
        return (T*)function_entry;
      }
    } else if (std::is_same<T,ConstSymbol>::value) {
      auto const_entry = const_table_.Find(name);
      if (const_entry != nullptr) {
        return (T*)const_entry;
      }
    } else if (std::is_same<T,TypeTemplate>::value ||
               std::is_same<T, BasicType>::value ||
               std::is_same<T, ArrayType>::value ||
               std::is_same<T, RecordType>::value) {
      auto type_entry = type_table_.Find(name);
      if (type_entry != nullptr) {
        return (T*)type_entry;
      }
    }

    if(prev_table_set_ != nullptr){
      return prev_table_set_->SearchEntry<T>(name);
    }else{
      return nullptr;
    }
  }


  TableSet* CreateNext(std::string tag) {
    TableSet* table_set = new TableSet(tag, this);
    next_table_sets_.insert(std::make_pair(tag, table_set));
    return table_set;
  }

  void LoadFromJson(nlohmann::json&);


 private:
  std::string tag_;
  BasicSymbolTable basic_table_;
  ArraySymbolTable array_table_;
  FunctionSymbolTable function_table_;
  ConstSymbolTable const_table_;
  TypeSymbolTable type_table_;

  TableSet* prev_table_set_;
  std::unordered_map<std::string, TableSet*> next_table_sets_;
  
  void BasicTypeInit() {
    if(prev_table_set_ == nullptr){
      type_table_.Insert("bool",new BasicType());
      type_table_.Insert("char",new BasicType());
      type_table_.Insert("int",new BasicType());
      type_table_.Insert("real",new BasicType());
    } else {
      type_table_.Insert("bool",prev_table_set_->table_ptr<TypeSymbolTable>()->Find("bool"));
      type_table_.Insert("char",prev_table_set_->table_ptr<TypeSymbolTable>()->Find("char"));
      type_table_.Insert("int",prev_table_set_->table_ptr<TypeSymbolTable>()->Find("int"));
      type_table_.Insert("real",prev_table_set_->table_ptr<TypeSymbolTable>()->Find("real"));
    }
  }

};

} // namespace symbol_table

#endif // PASCC_SYMBOL_TABLE_H_