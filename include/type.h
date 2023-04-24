#ifndef PASCC_TYPE_H_
#define PASCC_TYPE_H_

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <type_traits>
#include <unordered_map>


namespace pascal_type {

class TypeTemplate {
 public:
  enum class TYPE {
    BASIC,
    RECORD,
    ARRAY
  };
  TypeTemplate() {}
  TypeTemplate(TYPE template_type) : template_type_(template_type) {}
  virtual ~TypeTemplate() {}
  template <typename T> T* DynamicCast() {
    return dynamic_cast<T*>(this);
  }

  TYPE template_type() { return template_type_; }
  bool ComputeType(TypeTemplate* type1, TypeTemplate* type2, std::string op,
                   TypeTemplate** result_type = nullptr);
  bool TypeEqual(TypeTemplate* type1, TypeTemplate* type2);

 protected:
  TYPE template_type_;
};

// basic type including INT, REAL, CHAR, BOOL
class BasicType : public TypeTemplate {
 public:
  enum class BASIC_TYPE { INT, REAL, BOOL, LETTER, VOID};

  BasicType() : TypeTemplate(TYPE::BASIC), basic_type_(BASIC_TYPE::VOID) {}
  BasicType(BASIC_TYPE basic_type) : TypeTemplate(TYPE::BASIC), basic_type_(basic_type) {}

  ~BasicType() {}

  BASIC_TYPE type() { return basic_type_; }
  std::string type_name() {
    switch (basic_type_) {
      case BASIC_TYPE::INT: return "int";
      case BASIC_TYPE::REAL: return "float";
      case BASIC_TYPE::BOOL: return "bool";
      case BASIC_TYPE::LETTER: return "char";
      default: return "void";
    }
  }

 private:
   BASIC_TYPE basic_type_;
};
extern BasicType* TYPE_INT;
extern BasicType* TYPE_REAL;
extern BasicType* TYPE_BOOL;
extern BasicType* TYPE_CHAR;
extern BasicType* TYPE_STRING;
extern BasicType* TYPE_ERROR;


// Array type
class ArrayType : public TypeTemplate {
 public:
  typedef std::pair<int, int> ArrayBound ;
  ArrayType() : TypeTemplate(TYPE::ARRAY) {}
  ArrayType(TypeTemplate* type, std::vector<ArrayBound> bounds)
    : TypeTemplate(TYPE::ARRAY), type_(type), bounds_(bounds) {}

  ~ArrayType() {}
  TypeTemplate* type() { return type_; }
  int dimension() { return bounds_.size(); }
  std::vector<ArrayBound> *bounds() { return &bounds_; }
  bool AccessArray(std::vector<TypeTemplate*> index_types, TypeTemplate **type = nullptr);

 private:
  TypeTemplate* type_;  // basic types or record type
  std::vector<ArrayBound> bounds_;  // multi-dims bounds
  std::vector<TypeTemplate*> bound_types_;  // type of multi-dims
};

// Record type
class RecordType : public TypeTemplate {
 public:
  RecordType() : TypeTemplate(TYPE::RECORD){}
  RecordType(const std::unordered_map<std::string, TypeTemplate*>& type_map)
    : TypeTemplate(TYPE::RECORD){
    types_map_.insert(type_map.begin(), type_map.end());
    types_num_ = types_map_.size();
  }

  ~RecordType() {}

  void InsertType(std::string name, TypeTemplate* type);
  TypeTemplate* Find(std::string name);
  TypeTemplate* operator[](std::string name) { return Find(name); }

 private:
  std::unordered_map<std::string, TypeTemplate*> types_map_;
  int types_num_;
};

class Operation {
 public:
  Operation() {}
  Operation(TypeTemplate* in_type1, TypeTemplate* in_type2, std::string op)
    : in_type1(in_type1), in_type2(in_type2), op(op) {}
  ~Operation() {}
  bool operator==(const Operation& other) const {
    return in_type1 == other.in_type1 && in_type2 == other.in_type2 && op == other.op;
  }
  TypeTemplate* in_type1;
  TypeTemplate* in_type2; 
  std::string op;
};

struct OperationHash {
  std::size_t operator()(const Operation& k) const {
    return ((std::hash<TypeTemplate*>()(k.in_type1) ^
            (std::hash<TypeTemplate*>()(k.in_type2) >> 1)) >> 1) ^
            std::hash<std::string>()(k.op);
  }
};

typedef std::unordered_map<Operation, TypeTemplate*, OperationHash> OperationMap;
extern OperationMap operation_map;

}; // namespace pascal_type

class ConstValue{
public:
  ConstValue(){}
  ~ConstValue(){}
  ConstValue(const ConstValue& other);
  ConstValue(int v, bool to_float = false) {if(to_float) set((float)v); else set(v);}
  ConstValue(float v) {set(v);}
  ConstValue(bool v) {set(v);}
  ConstValue(char v) {set(v);}
  ConstValue(const char* v) {set(std::string(v));}
  ConstValue(std::string v) {set(v); }

  void set(int v) {m_Type = pascal_type::TYPE_INT; m_INT = v;}
  void set(float v) {m_Type = pascal_type::TYPE_REAL; m_REAL = v;}
  void set(bool v) {m_Type = pascal_type::TYPE_BOOL; m_BOOLEAN = v;}
  void set(char v) {m_Type = pascal_type::TYPE_CHAR; m_CHAR = v;}
  void set(std::string v) {m_Type = pascal_type::TYPE_STRING; m_STRING = v;}

  pascal_type::BasicType* type() {return m_Type;}
  template <typename T> T get() {
    if(std::is_same<T, int>::value) return *(T*)(&m_INT);
    else if (std::is_same<T, char>::value)  return *(T*)(&m_CHAR);
    else if (std::is_same<T, float>::value) return *(T*)(&m_REAL);
    else if (std::is_same<T, bool>::value)  return *(T*)(&m_BOOLEAN);
    else if (std::is_same<T, std::string>::value) return *((T*)(&m_STRING));
    else {
      throw std::runtime_error("ConstValue : get() : type " + std::string(typeid(T).name()) + " not supported");
    }
  }

  ConstValue& operator=(const ConstValue& other);
  // operation +
  ConstValue operator+(const ConstValue& other);
  // operation -
  ConstValue operator-(const ConstValue& other);
  // operation *
  ConstValue operator*(const ConstValue& other);
  // operation /
  ConstValue operator/(const ConstValue& other);

private:
  pascal_type::BasicType* m_Type = pascal_type::TYPE_ERROR;
  union {
    int m_INT;
    float m_REAL;
    char m_CHAR;
    bool m_BOOLEAN;
  };
  std::string m_STRING;
};


#endif // PASCC_TYPE_H_