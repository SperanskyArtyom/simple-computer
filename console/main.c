#include "console.h"
#include <mySimpleComputer.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  int sc_memoryInit();
  int sc_accumulatorInit();
  int sc_icounterInit();
  int sc_regInit();

  srand(time(NULL));
  int k = rand() % 128 + 1;
  for (int i = 0; i < k; i++) {
    int value = rand() % (1 << 15);
    sc_memorySet(i, value);
  }

  printf("Memory:\n");
  for (int i = 0; i < 128; i++) {
    printCell(i);
    if ((i + 1) % 10 == 0)
      printf("\n");
    else
      printf(" ");
  }

  int commandStatus = sc_memorySet(0, -1);
  printf("\n\nStatus of sc_memorySet with invalid value: %d\n", commandStatus);

  sc_regSet(FLAG_OVERFLOW, rand() % 2);
  sc_regSet(FLAG_DIV_ZERO, rand() % 2);
  sc_regSet(FLAG_MEM_OOB, rand() % 2);
  sc_regSet(FLAG_INVALID_CMD, rand() % 2);
  sc_regSet(FLAG_IGNORE_CLOCK, rand() % 2);

  printf("\nFlags: ");
  printFlags();

  commandStatus = sc_regSet(10, 1);
  printf("\n\nStatus of sc_regSet with invalid flag: %d\n", commandStatus);

  return 0;
}
