#include "types.h"
#include "types.h"
#include "user.h"


int
main(int argc, char *argv[])
{
  char *null_pointer = 0;
  printf(1, "Dereference null pointer:\n");
  printf(1, "Null pointer: %d\n", *null_pointer);

  int ret = mprotect((void*)4096, 1);
  printf(1, "XV6_TEST_OUTPUT: Return value of mprotect with len 1: %d\n", ret);

  printf(1, "\n");
  ret = munprotect((void*)4096, 1);
  printf(1, "XV6_TEST_OUTPUT: Return value of munprotect with len 1: %d\n", ret);

  printf(1, "\n");
  ret = mprotect((void*)4096, 3);
  printf(1, "XV6_TEST_OUTPUT: Return value of munprotect with len 3: %d\n", ret);

  printf(1, "\n");
  ret = munprotect((void*)4096, 3);
  printf(1, "XV6_TEST_OUTPUT: Return value of munprotect with len 3: %d\n", ret);

  *(char**)0x2000 = "this should not cause a pagefault";

  exit();
}