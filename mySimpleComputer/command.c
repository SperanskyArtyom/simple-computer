#include <mySimpleComputer.h>

#include <stddef.h>

#define SET_MASK(k) 1 << (k - 1)
#define GET_BIT(value, k) (value >> (k - 1)) & 0x1

int commands[]
    = { 0x00, 0x01, 0x0A, 0x0B, 0x14, 0x15, 0x1E, 0x1F, 0x20, 0x21,
        0x28, 0x29, 0x2A, 0x2B, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
        0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42,
        0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C };

int
sc_commandEncode (int sign, int command, int operand, int *value)
{
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
    *value |= SET_MASK (currentBit);
  else
    *value &= ~SET_MASK (currentBit);

  for (int i = 7; i > 0; i--)
    {
      if (GET_BIT (command, i))
        *value |= SET_MASK (--currentBit);
      else
        *value &= ~SET_MASK (--currentBit);
    }

  for (int i = 7; i > 0; i--)
    {
      if (GET_BIT (operand, i))
        *value |= SET_MASK (--currentBit);
      else
        *value &= ~SET_MASK (--currentBit);
    }

  return 0;
}

int
sc_commandDecode (int value, int *sign, int *command, int *operand)
{
  if (value < 0 || value > (1 << 15) - 1)
    return -1;
  if (sign == NULL || command == NULL || operand == NULL)
    return -1;
  *operand = value & 0x7F;
  *command = (value >> 7) & 0x7F;
  *sign = (value >> 14) & 0x01;
  return 0;
}

int
sc_commandValidate (int command)
{
  for (size_t i = 0; i < sizeof (commands) / sizeof (commands[0]); i++)
    if (command == commands[i])
      return 0;
  return -1;
}
