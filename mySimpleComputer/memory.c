#include "stdio.h"

#define MEMORY_SIZE sizeof(memory) / sizeof(int)

int memory[128];

int sc_memoryInit(void) {
  for (size_t i = 0; i < MEMORY_SIZE; i++)
    memory[i] = 0;
}

int sc_memorySet(int address, int value) {
  if (address < 0 || address >= MEMORY_SIZE)
    return -1;
  if (value <= -(1 << 14) || value >= (1 << 14))
    return -1;
  memory[address] = value;
  return 0;
}

int sc_memoryGet(int address, int *value) {
  if (address < 0 || address >= MEMORY_SIZE)
    return -1;
  if (value == NULL)
    return -1;
  *value = memory[address];
  return 0;
}

int sc_memorySave(char *filename) {
  if (filename == NULL)
    return -1;
  FILE *out = fopen(filename, "wb");
  if (out == NULL)
    return -1;
  size_t writtenSize = fwrite(memory, sizeof(int), MEMORY_SIZE, out);
  fclose(out);
  if (writtenSize != MEMORY_SIZE)
    return -1;
  return 0;
}

int sc_memoryLoad(char *filename) {
  if (filename == NULL)
    return -1;
  FILE *in = fopen(filename, "rb");
  if (in == NULL)
    return -1;
  int memoryBuf[MEMORY_SIZE] = {0};
  size_t readSize = fread(memoryBuf, sizeof(int), MEMORY_SIZE, in);
  fclose(in);
  if (readSize != MEMORY_SIZE)
    return -1;
  for (size_t i = 0; i < MEMORY_SIZE; i++)
    memory[i] = memoryBuf[i];
  return 0;
}
