#include <mySimpleComputer.h>

#define SIGN_BIT_MASK (1 << 14)
#define MAX_VALUE 0b011111111111111
#define MIN_VALUE 0b100000000000000

int
ALU (int command, int operand)
{
  int a, b, result, sign;
  if (sc_memoryGet (operand, &b))
    {
      sc_regSet (FLAG_MEM_OOB, 1);
      return -1;
    }
  sc_accumulatorGet (&a);

  switch (command)
    {
    case 0x1E: // ADD
      result = a + b;
      if (((a & SIGN_BIT_MASK) == (b & SIGN_BIT_MASK))
          && ((a & SIGN_BIT_MASK) != (result & SIGN_BIT_MASK)))
        {
          sc_regSet (FLAG_OVERFLOW, 1);
          result = (a & SIGN_BIT_MASK) ? MIN_VALUE : MAX_VALUE;
        }
      result = result & 0x7FFF;
      break;

    case 0x1F: // SUB
      result = a + (~b + 1);
      if (((a & SIGN_BIT_MASK) != (b & SIGN_BIT_MASK))
          && ((a & SIGN_BIT_MASK) != (result & SIGN_BIT_MASK)))
        {
          sc_regSet (FLAG_OVERFLOW, 1);
          result = (a & SIGN_BIT_MASK) ? MIN_VALUE : MAX_VALUE;
        }
      result = result & 0x7FFF;
      break;

    case 0x20: // DIVIDE
      sign = (a & SIGN_BIT_MASK) != (b & SIGN_BIT_MASK);
      if (a & SIGN_BIT_MASK)
        a = (~a + 1) & 0x7FFF;
      if (b & SIGN_BIT_MASK)
        b = (~b + 1) & 0x7FFF;
      if (b == 0)
        {
          sc_regSet (FLAG_DIV_ZERO, 1);
          return -1;
        }
      result = a / b;
      if (sign)
        result = (~result + 1) & 0x7FFF;
      break;

    case 0x21: // MUL
      sign = (a & SIGN_BIT_MASK) != (b & SIGN_BIT_MASK);
      if (a & SIGN_BIT_MASK)
        a = (~a + 1) & 0x7FFF;
      if (b & SIGN_BIT_MASK)
        b = (~b + 1) & 0x7FFF;
      result = a * b;
      if ((sign && result > MIN_VALUE) || (!sign && result > MAX_VALUE))
        {
          sc_regSet (FLAG_OVERFLOW, 1);
          result = sign ? MIN_VALUE : MAX_VALUE;
          break;
        }
      if (sign)
        result = (~result + 1) & 0x7FFF;
      break;

    default:
      sc_regSet (FLAG_INVALID_CMD, 1);
    }

  sc_accumulatorSet (result);
  return 0;
}
