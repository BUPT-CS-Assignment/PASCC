#include "type.h"
#include "parser.h"

bool IDVarpartsAttr::ArrayAccessCheck(pascal_type::ArrayType *base_type) {
  std::cout << "variable type:" << base_type << std::endl;
  for (int i = 0; i < var_parts->size(); i++) {
    // check each dim's type
    if ((*var_parts)[i].flag == 0) { //数组
        
    } else { //结构体

    }
  }
  return true;
}