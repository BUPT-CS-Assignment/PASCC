// #include "type.h"
#include <cstddef>
#include <cstdio>
#include <unordered_set>

int main() {
  //   std::unordered_multiset<int*> sa, sb;
  //   int a = 1, b = 2, c = 3, d = 4, e = 5;
  //   int *pa = &a, *pb = &b, *pc = &c, *pd = &d, *pe = &e;
  //   sa.insert(pa);
  //   sa.insert(pa);
  //   sa.insert(pb);

  //   sb.insert(pa);
  //   sb.insert(pb);
  // //   sb.insert(pa);
  //   printf("%d\n", sa == sb);
  //   printf("%lu %lu\n", sa.size(),sb.size());
  //   int a = 2;
  //   int **type=new int*;
  //   printf("%d\n", type == nullptr);
  //   if (type != nullptr)
  //     *type = &a;
  //   printf("%d %d\n", *type == &a, **type);
  //   int a = 2;
  //   int *type;
  //   printf("%d\n", &type == nullptr);
  //   if (type != nullptr)
  //     type = &a;
  //   printf("%d %d\n", type == &a, *type);

  float a = 2.0;
  int b[3];
  b[a] = 1;
  printf("%d\n",b[a]);
}