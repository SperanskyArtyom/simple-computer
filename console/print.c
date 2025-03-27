#include "console.h"
#include <mySimpleComputer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int memoryBlockX = 1;
const int memoryBlockY = 1;
const int memoryBlockWidth = 10 * 6 + 1;
const int memoryBlockHeight = 15;

const int accumulatorBlockX = memoryBlockX + memoryBlockWidth;
const int accumulatorBlockY = 1;
const int accumulatorBlockWidth = 21;
const int accumulatorBlockHeight = 3;

const int flagsBlockX = accumulatorBlockX + accumulatorBlockWidth;
const int flagsBlockY = 1;
const int flagsBlockWidth = accumulatorBlockWidth;
const int flagsBlockHeight = accumulatorBlockHeight;

const int icounterBlockX = accumulatorBlockX;
const int icounterBlockY = accumulatorBlockY + accumulatorBlockHeight;
const int icounterBlockWidth = accumulatorBlockWidth;
const int icounterBlockHeight = accumulatorBlockHeight;

const int commandBlockX = flagsBlockX;
const int commandBlockY = icounterBlockY;
const int commandBlockWidth = accumulatorBlockWidth;
const int commandBlockHeight = accumulatorBlockHeight;

const int editingCellBlockX = 1;
const int editingCellBlockY = memoryBlockY + memoryBlockHeight;
const int editingCellBlockWidth = memoryBlockWidth;
const int editingCellBlockHeight = 3;

const int cashBlockX = 1;
const int cashBlockY = editingCellBlockY + editingCellBlockHeight;
const int cashBlockWidth = 11 * 6;
const int cashBlockXHeight = 7;

const int inOutBlockX = cashBlockX + cashBlockWidth;
const int inOutBlockY = cashBlockY;
const int inOutBlockWidth = 11;
const int inOutBlockHeight = 7;

char *termHist[4] = { NULL };

void
shiftHist ()
{
  for (int i = 0; i < 3; i++)
    termHist[i] = termHist[i + 1];
  termHist[3] = malloc (sizeof (char[20]));
}

void
appendToHist (int address, int input)
{
  char buffer[20];
  int value, sign, command, operand;
  sc_memoryGet (address, &value);
  sc_commandDecode (value, &sign, &command, &operand);
  snprintf (buffer, sizeof (buffer), "%02d%c %c%02X%02X", address,
            input ? '>' : '<', sign ? '-' : '+', command, operand);

  int i;
  for (i = 0; i < 4; i++)
    if (termHist[i] == NULL)
      {
        termHist[i] = malloc (sizeof (char[20]));
        break;
      }
  if (i == 4)
    {
      shiftHist ();
      i--;
    }
  strcpy (termHist[i], buffer);
}

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
  const int flagX = flagsBlockX + (flagsBlockWidth - (4 * 3 + 1)) / 2;
  char buffer[5];
  mt_gotoXY (flagX, flagsBlockY + 1);

  sc_regGet (FLAG_OVERFLOW, &flag);
  snprintf (buffer, sizeof (buffer), "%c  ", flag ? 'P' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));

  sc_regGet (FLAG_DIV_ZERO, &flag);
  snprintf (buffer, sizeof (buffer), "%c  ", flag ? '0' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));

  sc_regGet (FLAG_MEM_OOB, &flag);
  snprintf (buffer, sizeof (buffer), "%c  ", flag ? 'M' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));

  sc_regGet (FLAG_IGNORE_CLOCK, &flag);
  snprintf (buffer, sizeof (buffer), "%c  ", flag ? 'T' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));

  sc_regGet (FLAG_INVALID_CMD, &flag);
  snprintf (buffer, sizeof (buffer), "%c  ", flag ? 'E' : '_');
  write (STDOUT_FILENO, buffer, strlen (buffer));
}

void
printDecodedCommand (int value)
{
  char buffer[50];

  mt_gotoXY (editingCellBlockX + 1, editingCellBlockY + 1);
  int decValue;
  if ((value >> 14) & 0x1)
    {
      decValue = -((~value & 0x3FFF) + 1);
    }
  snprintf (buffer, sizeof (buffer),
            "dec: %05d | oct: %05o | hex: %04X\tbin: ", decValue, value,
            value);
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

  char buffer[20];
  mt_gotoXY (accumulatorBlockX + 2, accumulatorBlockY + 1);

  snprintf (buffer, sizeof (buffer), "sc: %c%X%X ", sign ? '-' : '+', command,
            operand);
  write (STDOUT_FILENO, buffer, strlen (buffer));
  snprintf (buffer, sizeof (buffer), "hex: %04X", value);
  write (STDOUT_FILENO, buffer, strlen (buffer));
}

void
printCounters (void)
{
  int value, sign, command, operand;
  sc_icounterGet (&value);
  sc_commandDecode (value, &sign, &command, &operand);

  char buffer[20];
  mt_gotoXY (icounterBlockX + icounterBlockWidth / 2, icounterBlockY + 1);
  snprintf (buffer, sizeof (buffer), "IC: %c%02X%02X", sign ? '-' : '+',
            command, operand);
  write (STDOUT_FILENO, buffer, strlen (buffer));
}

int
printTermHist ()
{
  int i;
  for (i = 0; i < 4; i++)
    {
      if (termHist[i] == NULL)
        break;
      mt_gotoXY (inOutBlockX + 1, inOutBlockY + i + 1);
      write (STDOUT_FILENO, termHist[i], strlen (termHist[i]));
    }
  return i;
}

void
printTerm (int address, int input)
{
  int i = printTermHist ();
  char buffer[20];
  if (input)
    snprintf (buffer, sizeof (buffer), "%02d> ", address);
  else
    {
      int value, sign, command, operand;
      sc_memoryGet (address, &value);
      sc_commandDecode (value, &sign, &command, &operand);
      snprintf (buffer, sizeof (buffer), "%02d< %c%02X%02X", address,
                sign ? '-' : '+', command, operand);
    }

  mt_gotoXY (inOutBlockX + 1, inOutBlockY + i + 1);
  write (STDOUT_FILENO, buffer, strlen (buffer));
}

void
printCommand (void)
{
  int commandAddress, value, sign, command, operand;
  sc_icounterGet (&commandAddress);
  sc_memoryGet (commandAddress, &value);
  sc_commandDecode (value, &sign, &command, &operand);

  char buffer[20];
  int commandTextX;

  if (sign || sc_commandValidate (command))
    {
      commandTextX = commandBlockX + commandBlockWidth / 2;
      strcpy (buffer, "!");
    }
  else
    {
      commandTextX = commandBlockX + (commandBlockWidth - 9) / 2;
      snprintf (buffer, sizeof (buffer), "+ %02X : %02X", command, operand);
    }
  mt_gotoXY (commandTextX, commandBlockY + 1);
  write (STDOUT_FILENO, buffer, strlen (buffer));
}
