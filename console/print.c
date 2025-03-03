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

void printFlags(void) {
  int flag;
  sc_regGet(FLAG_OVERFLOW, &flag);
  if (flag)
    printf("P");
  else
    printf("_");
  sc_regGet(FLAG_DIV_ZERO, &flag);
  if (flag)
    printf("0");
  else
    printf("_");
  sc_regGet(FLAG_MEM_OOB, &flag);
  if (flag)
    printf("M");
  else
    printf("_");
  sc_regGet(FLAG_IGNORE_CLOCK, &flag);
  if (flag)
    printf("T");
  else
    printf("_");
  sc_regGet(FLAG_INVALID_CMD, &flag);
  if (flag)
    printf("E");
  else
    printf("_");
}
