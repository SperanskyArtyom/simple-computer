#include "console.h"
#include <mySimpleComputer.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int memoryBlockX = 1;
const int memoryBlockY = 1;
const int memoryBlockWidth = 10 * 6 + 1;

const int accumulatorBlockX = memoryBlockX + memoryBlockWidth;
const int accumulatorBlockY = 1;
const int accumulatorWidth = 23;

const int flagsBlockX = accumulatorBlockX + accumulatorWidth;
const int flagsBlockY = 1;
const int flagsBlockWidth = 23;

void
printCell (int address, enum colors fg, enum colors bg)
{
  int value, sign, command, operand;
  sc_memoryGet (address, &value);
  sc_commandDecode (value, &sign, &command, &operand);

  int x = (address % 10) * 6 + 1 + memoryBlockX;
  int y = address / 10 + 1 + memoryBlockY;

  mt_setfgcolor (fg);
  mt_setbgcolor (bg);
  mt_gotoXY (x, y);

  char buffer[20];
  snprintf (buffer, sizeof (buffer), "%c%02X%02X", sign ? '-' : '+', command,
            operand);
  write (STDOUT_FILENO, buffer, strlen (buffer));
}

void
printFlags (void)
{
  int flag;
  const int flagX = flagsBlockX + (flagsBlockWidth / -(5 * 2)) / 2;
  char buffer[5];
  mt_gotoXY (flagX, flagsBlockY + 1);

  sc_regGet (FLAG_OVERFLOW, &flag);
  snprintf (buffer, sizeof (buffer), "%c ", flag ? 'P' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));

  sc_regGet (FLAG_DIV_ZERO, &flag);
  snprintf (buffer, sizeof (buffer), "%c ", flag ? '0' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));

  sc_regGet (FLAG_MEM_OOB, &flag);
  snprintf (buffer, sizeof (buffer), "%c ", flag ? 'M' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));

  sc_regGet (FLAG_IGNORE_CLOCK, &flag);
  snprintf (buffer, sizeof (buffer), "%c ", flag ? 'T' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));

  sc_regGet (FLAG_INVALID_CMD, &flag);
  snprintf (buffer, sizeof (buffer), "%c ", flag ? 'E' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));
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
