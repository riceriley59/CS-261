/*
 * This program should compute do some simple setting and printing of values,
 * but it's currently broken.  USe GDB and Valgrind to debug it.
 */

#include <stdio.h>

void setval(int* i_ptr, int val) {
  *i_ptr = val;
}

int main(int argc, char** argv) {
  int a;
  setval(&a, 13);
  printf("a: %d\n", a);

  int* b;
  setval(b, 4);
  printf("*b: %d\n", *b);
}
