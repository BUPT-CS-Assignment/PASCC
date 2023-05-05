#include <stdio.h>
#include <string.h>
/// [testlogic] created on 2023/5/5
int a, b;
int main() {
  a = 1;
  b = 0;
  printf("%d\n", a && b);
  printf("%d\n", a || b);
  printf("%d\n", !a);
}
