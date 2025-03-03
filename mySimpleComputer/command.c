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
  int currentBit = 1;

  for (int i = 1; i < 8; i++) {
    if (GET_BIT(operand, i))
      *value |= SET_MASK(currentBit++);
    else
      *value &= ~SET_MASK(currentBit++);
  }
  for (int i = 1; i < 8; i++) {
    if (GET_BIT(command, i))
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

int sc_commandDecode(int value, int *sign, int *command, int *operand) {
  if (value < 0 || value > (1 << 15) - 1)
    return -1;
  if (sign == NULL || command == NULL || operand == NULL)
    return -1;
  *sign = *command = *operand = 0;
  int currentBit = 1;
  for (int i = 0; i < 8; i++) {
    if (GET_BIT(value, currentBit++))
      *operand |= SET_MASK(i);
    else
      *operand &= ~SET_MASK(i);
  }

  for (int i = 0; i < 8; i++) {
    if (GET_BIT(value, currentBit++))
      *command |= SET_MASK(i);
    else
      *command &= ~SET_MASK(i);
  }

  *sign = GET_BIT(value, currentBit++);
  return 0;
}

int sc_commandValidate(int command) {
  if (command < 0 || command > (1 << 7) - 1)
    return -1;
  return 0;
}
