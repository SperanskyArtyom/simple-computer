#include "console.h"
#include <mySimpleComputer.h>
#include <stdio.h>

void printCell(int address) {
  int value, sign, command, operand;
  sc_memoryGet(address, &value);
  sc_commandDecode(value, &sign, &command, &operand);

  printf("%c%X%X", sign ? '+' : '-', command, operand);
}

void printFlags(void) {
  int flag;
  sc_regGet(FLAG_OVERFLOW, &flag);
  printf("%c", flag ? 'P' : '_');

  sc_regGet(FLAG_DIV_ZERO, &flag);
  printf("%c", flag ? '0' : '_');

  sc_regGet(FLAG_MEM_OOB, &flag);
  printf("%c", flag ? 'M' : '_');

  sc_regGet(FLAG_IGNORE_CLOCK, &flag);
  printf("%c", flag ? 'T' : '_');

  sc_regGet(FLAG_INVALID_CMD, &flag);
  printf("%c", flag ? 'E' : '_');
}

void printDecodedCommand(int value) {
  int sign, command, operand;
  sc_commandDecode(value, &sign, &command, &operand);

  printf("%c %X : %X", sign ? '+' : '-', command, operand);
}
