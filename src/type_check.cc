#include "type.h"
#include <unordered_set>
#include <vector>
#include <string>

using pascal_type::TYPE_INT;
using pascal_type::TYPE_CHAR;
using pascal_type::TYPE_BOOL;
using pascal_type::TYPE_REAL;

// TODO use pascal_type::is_basic()
//#define IN_BASIC_TYPE(a)                                                       \
//  (a == INT) || (a == CHAR) || (a == BOOL) || (a == _recalloc)


// 这一段结构体声明之后应该放在别的地方
typedef struct VarParts{
    bool flag;//0 for Record.1 for array
    std::vector<pascal_type::TypeTemplate*> subscript;
    std::string name;
 }VarParts;

///////
// TODO : check use of pascal_type::is_same()

//// 基本与Record类型的类型检查
//bool CheckType(pascal_type::TypeTemplate *a, pascal_type::TypeTemplate *b) {
////   if (IN_BASIC_TYPE(a) || IN_BASIC_TYPE(b))
//    return a == b;
////   else {
////     return false;
////   }
//}
//
//// 函数参数的批量类型检查,传入两个Vector
//// Deprecated function
//// Use FunctionSymbol*() -> AssertParams(vector<BasicType *> a)
////bool CheckType(std::vector<pascal_type::TypeTemplate *> la,
////                    std::vector<pascal_type::TypeTemplate *> lb) {
////
////  if (la.size() != lb.size())
////    return false;
////  for (int i = 0; i < la.size(); i++) {
////    if (!CheckType(la[i], lb[i]))
////      return false;
////  }
////  return true;
////}
//
//// 处理数组情况所需要的api
//bool AssertArrayAccess(pascal_type::ArrayType a,
//                        std::vector<pascal_type::TypeTemplate *> param,
//                        pascal_type::TypeTemplate **baisc_type) {
//  return a.AccessArray(param, baisc_type);
//  /*
//
//  // 处理数组的(伪)代码
//  // 设有两个数组访问要比对, 如if(a[2][3].x==b[2][3].x)
//
//  // 声明两个数组对应的基本元素,假设是Record
//
//    pascal_type::ArrayType c, d;
//    std::vector<pascal_type::TypeTemplate *> param_c, param_d;
//    // param里insert各下标的类型...
//    pascal_type::TypeTemplate *pc, *pd;
//    // 进行函数调用后,pa与pb会被赋上基本元素的类型
//    if (IsLegalArrayAccess(c, param_c, &pc) &&
//        IsLegalArrayAccess(d, param_d, &pd)) {
//      return CheckType(pc, pd);
//    }
//    else {
//      // RaiseError
//    }
//*/
//}
//
//
//
//// unfinished
//bool LegalAccess(pascal_type::TypeTemplate *a,
//                        std::vector<VarParts> var,
//                        pascal_type::TypeTemplate **result){
//    //
//    auto tmp_a = a;
//    bool is_in_array=0;
//    std::vector<pascal_type::TypeTemplate*> cur_subscript;
//    for(int i=0;i<var.size();i++){
//        if(is_in_array==0){
//            if(var[i].flag==0){
//                // 从符号表中由名字取出属性
//                // tmp_a = SymbleTable.find(tmp_a, var[i].name);
//            }else{
//                pascal_type::TypeTemplate *access_result;
//                // 需要进行类型转换，此处写得很强行
//                if(AssertArrayAccess(*(pascal_type::ArrayType*)tmp_a, var[i].subscript,&access_result)){
//                    tmp_a = access_result;
//                }else{
//                    cur_subscript.assign(var[i].subscript.begin(), var[i].subscript.end());
//                    is_in_array=1;
//                }
//            }
//        }else{
//            if(var[i].flag==0){
//                // error(TyoeError);
//                return false;
//            }else{
//                pascal_type::TypeTemplate *access_result;
//                cur_subscript.assign(var[i].subscript.begin(), var[i].subscript.end());
//                if(AssertArrayAccess(*(pascal_type::ArrayType*)tmp_a,cur_subscript,&access_result)){
//                    tmp_a = access_result;
//                    is_in_array=0;
//                    cur_subscript.clear();
//                }
//            }
//        }
//    }
//    if(is_in_array=0){
//        *result=tmp_a;
//        return true;
//    }else{
//        *result=nullptr;
//        return false;
//    }
//
//}