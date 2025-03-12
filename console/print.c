#include "console.h"
#include <mySimpleComputer.h>
#include <stdio.h>

void
printCell (int address)
{
  int value, sign, command, operand;
  sc_memoryGet (address, &value);
  sc_commandDecode (value, &sign, &command, &operand);

  printf ("%c%02X%02X", sign ? '-' : '+', command, operand);
}

void
printFlags (void)
{
  int flag;
  sc_regGet (FLAG_OVERFLOW, &flag);
  printf ("%c", flag ? 'P' : '_');

  sc_regGet (FLAG_DIV_ZERO, &flag);
  printf ("%c", flag ? '0' : '_');

  sc_regGet (FLAG_MEM_OOB, &flag);
  printf ("%c", flag ? 'M' : '_');

  sc_regGet (FLAG_IGNORE_CLOCK, &flag);
  printf ("%c", flag ? 'T' : '_');

  sc_regGet (FLAG_INVALID_CMD, &flag);
  printf ("%c", flag ? 'E' : '_');
}

void
printDecodedCommand (int value)
{
  printf ("dec: %05d | oct: %05o | hex: %04X\tbin: ", value, value, value);
  for (int i = 14; i >= 0; i--)
    printf ("%d", value & (1 << i) ? 1 : 0);
}

void
printAccumulator (void)
{
  int value, sign, command, operand;
  sc_accumulatorGet (&value);
  sc_commandDecode (value, &sign, &command, &operand);

  printf ("sc: %c%X%X ", sign ? '-' : '+', command, operand);
  printf ("hex: %04X", value);
}

void
printCounters (void)
{
  int value, sign, command, operand;
  sc_icounterGet (&value);
  sc_commandDecode (value, &sign, &command, &operand);

  printf ("IC: %c%X%X", sign ? '-' : '+', command, operand);
}
