#define MEMORY_SIZE sizeof(MEMORY) / sizeof(int)

int MEMORY[128];

int sc_memoryInit(void) {
  for (size_t i = 0; i < MEMORY_SIZE; i++)
    MEMORY[i] = 0;
}

int sc_memorySet(int address, int value) {
  if (address < 0 || address >= MEMORY_SIZE)
    return -1;
  if (value <= -(1 << 14) || value >= (1 << 14))
    return -1;
  MEMORY[address] = value;
  return 0;
}

int sc_memoryGet(int address, int *value) {
  if (address < 0 || address >= MEMORY_SIZE)
    return -1;
  if (value == NULL)
    return -1;
  value = MEMORY[address];
  return 0;
}
