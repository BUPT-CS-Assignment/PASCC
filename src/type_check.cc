#include "type.h"
#include <unordered_set>
#include <vector>

//以下的具体值需要符号表生成后才能填上，后续还需要修改
pascal_type::TypeTemplate *Type_int;
pascal_type::TypeTemplate *Type_real;
pascal_type::TypeTemplate *Type_char;
pascal_type::TypeTemplate *Type_bool;
#define IN_BASIC_TYPE(a)                                                       \
  (a == Type_int) || (a == Type_real) || (a == Type_char) || (a == Type_bool)

// 基本与Record类型的类型检查
bool CheckType(pascal_type::TypeTemplate *a, pascal_type::TypeTemplate *b) {
//   if (IN_BASIC_TYPE(a) || IN_BASIC_TYPE(b))
    return a == b;
//   else {
//     return false;
//   }
}

// 函数参数的批量类型检查,传入两个Vector
bool CheckType(std::vector<pascal_type::TypeTemplate *> la,
                    std::vector<pascal_type::TypeTemplate *> lb) {
  if (la.size() != lb.size())
    return false;
  for (int i = 0; i < la.size(); i++) {
    if (!CheckType(la[i], lb[i]))
      return false;
  }
  return true;
}

// 处理数组情况所需要的api
bool IsLegalArrayAccess(pascal_type::ArrayType a,
                        std::vector<pascal_type::TypeTemplate *> param,
                        pascal_type::TypeTemplate **baisc_type) {
  return a.AccessArray(param, baisc_type);
  /*

  // 处理数组的(伪)代码
  // 设有两个数组访问要比对, 如if(a[2][3].x==b[2][3].x)

  // 声明两个数组对应的基本元素,假设是Record

    pascal_type::ArrayType c, d;
    std::vector<pascal_type::TypeTemplate *> param_c, param_d;
    // param里insert各下标的类型...
    pascal_type::TypeTemplate *pc, *pd;
    // 进行函数调用后,pa与pb会被赋上基本元素的类型
    if (IsLegalArrayAccess(c, param_c, &pc) &&
        IsLegalArrayAccess(d, param_d, &pd)) {
      return CheckType(pc, pd);
    }
    else {
      // RaiseError
    }
*/
}

