#include "stddef.h"

#define SET_MASK(k) 1 << (k - 1)

int sc_commandEncode(int sign, int command, int operand, int *value) {
  if (sign < 0 || sign > 1)
    return -1;
  if (command < 0 || command > (1 << 7) - 1)
    return -1;
  if (operand < 0 || operand > (1 << 7) - 1)
    return -1;
  if (value == NULL)
    return -1;
  int currentBit = 1;

  for (int i = 1; i < 8; i++) {
    if ((operand >> (i - 1)) & 0x1)
      *value |= SET_MASK(currentBit++);
    else
      *value &= ~SET_MASK(currentBit++);
  }
  for (int i = 1; i < 8; i++) {
    if ((command >> (i - 1)) & 0x1)
      *value |= SET_MASK(currentBit++);
    else
      *value &= ~SET_MASK(currentBit++);
  }

  if (sign)
    *value |= SET_MASK(currentBit);
  else
    *value &= ~SET_MASK(currentBit);
  return 0;
}
