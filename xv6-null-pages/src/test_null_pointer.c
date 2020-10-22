#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  char *null_pointer = 0;
  printf(1, "Null pointer: %d\n", *null_pointer);
  exit();
}