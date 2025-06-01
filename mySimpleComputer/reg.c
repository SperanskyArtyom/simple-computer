#include <mySimpleComputer.h>

#include <stddef.h>

#define FLAG_GET(flag) (reg >> flag) & 0x1
#define FLAG_SET_TRUE(flag) reg = reg | (1 << flag)
#define FLAG_SET_FALSE(flag) reg = reg & (~(1 << flag))

int accumulator, icounter, reg;

int
sc_regInit (void)
{
  reg = 0;
  sc_regSet (FLAG_IGNORE_CLOCK, 1);
  return 0;
}

int
sc_regSet (int flag, int value)
{
  if (flag != FLAG_OVERFLOW && flag != FLAG_DIV_ZERO && flag != FLAG_MEM_OOB
      && flag != FLAG_INVALID_CMD && flag != FLAG_IGNORE_CLOCK)
    {
      return -1;
    }
  if (value)
    FLAG_SET_TRUE (flag);
  else
    FLAG_SET_FALSE (flag);
  return 0;
}

int
sc_regGet (int flag, int *value)
{
  if (flag != FLAG_OVERFLOW && flag != FLAG_DIV_ZERO && flag != FLAG_MEM_OOB
      && flag != FLAG_INVALID_CMD && flag != FLAG_IGNORE_CLOCK)
    {
      return -1;
    }
  if (value == NULL)
    return -1;
  *value = FLAG_GET (flag);
  return 0;
}

int
sc_accumulatorInit (void)
{
  accumulator = 0;
  return 0;
}

int
sc_accumulatorSet (int value)
{
  if (value < 0 || value > (1 << 15) - 1)
    return -1;
  accumulator = value;
  return 0;
}

int
sc_accumulatorGet (int *value)
{
  if (value == NULL)
    return -1;
  *value = accumulator;
  return 0;
}

int
sc_icounterInit (void)
{
  icounter = 0;
  return 0;
}

int
sc_icounterSet (int value)
{
  if (value < 0 || value > 127)
    return -1;
  icounter = value;
  return 0;
}

int
sc_icounterGet (int *value)
{
  if (value == NULL)
    return -1;
  *value = icounter;
  return 0;
}
