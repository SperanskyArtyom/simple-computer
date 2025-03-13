#include "console.h"
#include <mySimpleComputer.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int memoryBlockX = 1;
const int memoryBlockY = 1;
const int memoryBlockWidth = 10 * 6 + 1;
const int memoryBlockHeight = 15;

const int accumulatorBlockX = memoryBlockX + memoryBlockWidth;
const int accumulatorBlockY = 1;
const int accumulatorWidth = 23;

const int flagsBlockX = accumulatorBlockX + accumulatorWidth;
const int flagsBlockY = 1;
const int flagsBlockWidth = 23;

const int editingCellBlockX = 1;
const int editingCellBlockY = memoryBlockY + memoryBlockHeight;
const int editingCellBlockWidth = memoryBlockWidth;
const int editingCellBlockHeight = 3;

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
  mt_setdefaultcolor ();
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
  char buffer[50];

  mt_gotoXY (editingCellBlockX + 1, editingCellBlockY + 1);
  snprintf (buffer, sizeof (buffer),
            "dec: %05d | oct: %05o | hex: %04X\tbin: ", value, value, value);
  write (STDOUT_FILENO, buffer, strlen (buffer));
  for (int i = 14; i >= 0; i--)
    {
      snprintf (buffer, sizeof (buffer), "%d", value & (1 << i) ? 1 : 0);
      write (STDOUT_FILENO, buffer, strlen (buffer));
    }
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
