#define FLAG_OVERFLOW 1 << 0
#define FLAG_DIV_ZERO 1 << 1
#define FLAG_MEM_OOB 1 << 2
#define FLAG_INVALID_CMD 1 << 3
#define FLAG_IGNORE_CLOCK 1 << 4

int accumulator, icounter, reg;

int sc_regInit(void) {
  reg = 0 | FLAG_IGNORE_CLOCK;
  return 0;
}

int sc_regSet(int flag, int value) {
  if (flag != FLAG_OVERFLOW || flag != FLAG_DIV_ZERO || flag != FLAG_MEM_OOB ||
      flag != FLAG_INVALID_CMD || flag != FLAG_IGNORE_CLOCK) {
    return -1;
  }
  if (value)
    reg = reg | flag;
  else
    reg = reg & (~flag);
  return 0;
}
