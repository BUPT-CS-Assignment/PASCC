#ifndef PASCC_TYPE_H_
#define PASCC_TYPE_H_

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <string.h>
#include <type_traits>
#include <unordered_map>


namespace pascals {

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
  bool StringLike();

 protected:
  TYPE template_type_;
};
using pType = std::shared_ptr<TypeTemplate>;

// basic type including INT, REAL, CHAR, BOOL
class BasicType : public TypeTemplate {
 public:
  enum class BASIC_TYPE { INT, REAL, BOOL, CHAR, NONE};

  BasicType() : TypeTemplate(TYPE::BASIC), basic_type_(BASIC_TYPE::NONE) {}
  BasicType(BASIC_TYPE basic_type) : TypeTemplate(TYPE::BASIC), basic_type_(basic_type) {}

  ~BasicType() {}

  BASIC_TYPE type() { return basic_type_; }
  std::string type_name() {
    switch (basic_type_) {
      case BASIC_TYPE::INT: return "int";
      case BASIC_TYPE::REAL: return "float";
      case BASIC_TYPE::BOOL: return "int";
      case BASIC_TYPE::CHAR: return "char";
      default: return "void";
    }
  }

 private:
   BASIC_TYPE basic_type_;
};
using pBasicType = std::shared_ptr<BasicType>;

extern pBasicType TYPE_INT;
extern pBasicType TYPE_REAL;
extern pBasicType TYPE_BOOL;
extern pBasicType TYPE_CHAR;
extern pBasicType TYPE_NONE;
extern pBasicType TYPE_ERROR;
extern pBasicType TYPE_STRINGLIKE;


// Array type
class ArrayType : public TypeTemplate {
 public:
  // Array bound
  struct ArrayBound{
   pBasicType type_;
   int lb_;
   int ub_;
   // functions
   ArrayBound() : type_(TYPE_NONE), lb_(0), ub_(0) {};
   ArrayBound(const ArrayBound& b2) : type_(b2.type_), lb_(b2.lb_), ub_(b2.ub_) {};
   ArrayBound& operator=(const ArrayBound& b2);
   bool operator==(const ArrayBound& b2) const;
  };

  ArrayType() : TypeTemplate(TYPE::ARRAY), base_type_(TYPE_NONE) {}
  ArrayType(pType type) : TypeTemplate(TYPE::ARRAY), base_type_(type) {}
  ArrayType(pType type, std::vector<ArrayBound> bounds)
    : TypeTemplate(TYPE::ARRAY), base_type_(type), bounds_(std::move(bounds)) {}
  ArrayType(const ArrayType& a2);
  ~ArrayType() {}

  const pType base_type() { return base_type_; }
  bool Valid() { return base_type_ != TYPE_NONE && base_type_ != TYPE_ERROR;}
  size_t dims() { return bounds_.size(); }     // get dimensions
  std::vector<ArrayBound>& bounds() { return bounds_; }  // get bounds
  ArrayBound bound(size_t i) { return bounds_[i]; }     // get bound of dimension i
  bool StringLike(int access_layer = 0);            // check if string-like (array of char)

  ArrayType& operator=(const ArrayType& a2);
  bool operator==(const ArrayType& a2) const;
  std::shared_ptr<ArrayType> Visit(std::vector<pType>& v_types); // visit array
  std::shared_ptr<ArrayType> Visit(unsigned int v_layer);            // visit array

 private:
  pType base_type_;  // basic types or record type
  std::vector<ArrayBound> bounds_;  // multi-dims bounds
};
using pArrayType = std::shared_ptr<ArrayType>;

// Record type
class RecordType : public TypeTemplate {
 public:
  typedef std::unordered_map<std::string, pType> RecordInfo;
  RecordType() : TypeTemplate(TYPE::RECORD){}
  RecordType(std::unordered_map<std::string, pType> types_map)
    : TypeTemplate(TYPE::RECORD), types_map_(types_map) {}
  ~RecordType() {}

  bool add(std::string name, pType type);
  pType Find(std::string name);
  pType Visit(std::vector<std::string>& names);

 private:
  std::unordered_map<std::string, pType> types_map_;
};
using pRecordType = std::shared_ptr<RecordType>;

class Operation {
 public:
  Operation() {}
  Operation(pBasicType in_type1, pBasicType in_type2, std::string op)
    : in_type1(in_type1), in_type2(in_type2), op(std::move(op)) {}
  ~Operation() {}
  bool operator==(const Operation& other) const {
    return in_type1.get() == other.in_type1.get() &&
           in_type2.get() == other.in_type2.get() &&
           op == other.op;
  }
  pBasicType in_type1;
  pBasicType in_type2;
  std::string op;
};
struct OperationHash {
  std::size_t operator()(const Operation& k) const {
    return ((std::hash<pBasicType>()(k.in_type1) ^
            (std::hash<pBasicType>()(k.in_type2) >> 1)) >> 1) ^
            std::hash<std::string>()(k.op);
  }
};
typedef std::unordered_map<Operation, pBasicType, OperationHash> OperationMap;
extern OperationMap operation_map;




/********************************************
  *             Type Assertion              *
 ********************************************/
 /**
 * @brief compute the result type of two basic types
 * @param type1 type 1
 * @param type2 type 2
 * @param op operation string
 * @return
 */
pBasicType compute(pType type1, pType type2, std::string op);

/**
 * @brief check if a type is basic type
 * @param t ptr to type
 * @return
 */
bool is_basic(pType t);

/**
 * @brief check if two types are the same
 * @param t1 type 1
 * @param t2 type 2
 * @return
 */
bool is_same(pType t1, pType t2);

/**
 * @brief check if two array types with visits are the same
 * @param t1 ptr to array 1
 * @param vdim1 Visit dimensions 1
 * @param t2 ptr to array 2
 * @param vdim2 Visit dimensions 2
 * @return
 */
bool is_same(ArrayType* t1, int vdim1, ArrayType* t2, int vdim2);

/**
 * @brief Visit array and check if is the same with the input type
 * @param t1 ptr to array
 * @param vdim Visit dimensions
 * @param t2 ptr to result-assertion type
 * @return
 */
bool is_same(ArrayType* t1, int vdim, pBasicType t2);

/**
 * @brief check if two record types with access are the same
 * @param t1 ptr to record 1
 * @param n1 Visit vector 1 with names
 * @param t2 ptr to record 2
 * @param n2 Visit vector 2 with names
 * @return
 */
bool is_same(RecordType* t1, std::vector<std::string> n1, RecordType* t2, std::vector<std::string> n2);

/**
 * @brief Visit record and check if is the same with the input type
 * @param t1 ptr to record type
 * @param n1 Visit vector with names
 * @param t2 ptr to result-assertion type
 * @return
 */
bool is_same(RecordType* t1, std::vector<std::string> n1, pType t2);


}; // namespace pascals

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

  void set(int v) {m_Type = pascals::TYPE_INT; m_INT = v;}
  void set(float v) {m_Type = pascals::TYPE_REAL; m_REAL = v;}
  void set(bool v) {m_Type = pascals::TYPE_BOOL; m_BOOLEAN = v;}
  void set(char v) {m_Type = pascals::TYPE_CHAR; m_CHAR = v;}
  void set(std::string v) {m_Type = pascals::TYPE_STRINGLIKE; m_STRING = v;}

  const pascals::pBasicType& type() {return m_Type;}
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
  pascals::pBasicType m_Type = nullptr;
  union {
    int m_INT;
    float m_REAL;
    char m_CHAR;
    bool m_BOOLEAN;
  };
  std::string m_STRING;
};


#endif // PASCC_TYPE_H_