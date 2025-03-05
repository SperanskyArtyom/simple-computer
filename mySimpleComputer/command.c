#include <mySimpleComputer.h>

#include <stddef.h>

#define SET_MASK(k) 1 << (k - 1)
#define GET_BIT(value, k) (value >> (k - 1)) & 0x1

int sc_commandEncode(int sign, int command, int operand, int *value) {
  if (sign < 0 || sign > 1)
    return -1;
  if (command < 0 || command > (1 << 7) - 1)
    return -1;
  if (operand < 0 || operand > (1 << 7) - 1)
    return -1;
  if (value == NULL)
    return -1;
  int currentBit = 15;

  if (sign)
    *value |= SET_MASK(currentBit);
  else
    *value &= ~SET_MASK(currentBit);

  for (int i = 7; i > 0; i--) {
    if (GET_BIT(command, i))
      *value |= SET_MASK(--currentBit);
    else
      *value &= ~SET_MASK(--currentBit);
  }

  for (int i = 7; i > 0; i--) {
    if (GET_BIT(operand, i))
      *value |= SET_MASK(--currentBit);
    else
      *value &= ~SET_MASK(--currentBit);
  }

  return 0;
}

int sc_commandDecode(int value, int *sign, int *command, int *operand) {
  if (value < 0 || value > (1 << 15) - 1)
    return -1;
  if (sign == NULL || command == NULL || operand == NULL)
    return -1;
  *operand = value & 0x7F;
  *command = (value >> 7) & 0x7F;
  *sign = (value >> 14) & 0x01;
  return 0;
}

int sc_commandValidate(int command) {
  if (command < 0 || command > (1 << 7) - 1)
    return -1;
  return 0;
}
