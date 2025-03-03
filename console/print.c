#include "console.h"
#include <mySimpleComputer.h>
#include <stdio.h>

void printCell(int address) {
  int value, sign, command, operand;
  sc_memoryGet(address, &value);
  sc_commandDecode(value, &sign, &command, &operand);
  if (sign)
    printf("+");
  else
    printf("-");
  printf("%X%X", command, operand);
}
