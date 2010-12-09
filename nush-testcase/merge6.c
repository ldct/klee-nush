#include <klee/klee.h>

int main() {

  int result = 0;

  int var0;
  int var1;
  int var2;
  int var3;
  int var4;
  int var5;

  klee_make_symbolic(&var0, sizeof(var0), "input var0");
  klee_make_symbolic(&var1, sizeof(var1), "input var1");
  klee_make_symbolic(&var2, sizeof(var2), "input var2");
  klee_make_symbolic(&var3, sizeof(var3), "input var3");
  klee_make_symbolic(&var4, sizeof(var4), "input var4");
  klee_make_symbolic(&var5, sizeof(var5), "input var5");

  if (var0 == 42)
    result = 1;
     
  if (var1 == 42)
    result = 1;
     
  if (var2 == 42)
    result = 1;
     
  if (var3 == 42)
    result = 1;
     
  if (var4 == 42)
    result = 1;
     
  if (var5 == 42)
    result = 1;
     
  return result;
}
  
