#include <console.h>
#include <myBigChars.h>
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
const int accumulatorBlockWidth = 23;
const int accumulatorBlockHeight = 3;

const int flagsBlockX = accumulatorBlockX + accumulatorBlockWidth;
const int flagsBlockY = 1;
const int flagsBlockWidth = accumulatorBlockWidth + 1;
const int flagsBlockHeight = accumulatorBlockHeight;

const int icounterBlockX = accumulatorBlockX;
const int icounterBlockY = accumulatorBlockY + accumulatorBlockHeight;
const int icounterBlockWidth = accumulatorBlockWidth;
const int icounterBlockHeight = accumulatorBlockHeight;

const int commandBlockX = flagsBlockX;
const int commandBlockY = icounterBlockY;
const int commandBlockWidth = flagsBlockWidth;
const int commandBlockHeight = accumulatorBlockHeight;

const int editingCellBlockX = 1;
const int editingCellBlockY = memoryBlockY + memoryBlockHeight;
const int editingCellBlockWidth = memoryBlockWidth;
const int editingCellBlockHeight = 3;

const int zoomedCellBlockX = accumulatorBlockX;
const int zoomedCellBlockY = icounterBlockY + icounterBlockHeight;
const int zoomedCellBlockWidth = accumulatorBlockWidth + flagsBlockWidth;
const int zoomedCellBlockHeight = 12;

const int cashBlockX = 1;
const int cashBlockY = editingCellBlockY + editingCellBlockHeight;
const int cashBlockWidth = 11 * 6;
const int cashBlockXHeight = 7;

const int inOutBlockX = cashBlockX + cashBlockWidth;
const int inOutBlockY = cashBlockY;
const int inOutBlockWidth = 11;
const int inOutBlockHeight = cashBlockXHeight;

const int keysBlockX = inOutBlockX + inOutBlockWidth;
const int keysBlockY = inOutBlockY;
const int keysBlockWidth = 31;
const int keysBlockHeight = inOutBlockHeight;

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
  if (address == -1)
    {
      for (int i = 0; i < 4; i++)
        {
          termHist[i] = NULL;
          mt_gotoXY (inOutBlockX + 1, inOutBlockY + i + 1);
          write (STDOUT_FILENO, "        ", 8);
        }
      mt_gotoXY (inOutBlockX + 1, inOutBlockY + 5);
      write (STDOUT_FILENO, "        ", 8);
      return;
    }
  int value, sign, command, operand;
  sc_memoryGet (address, &value);
  sc_commandDecode (value, &sign, &command, &operand);
  snprintf (buffer, sizeof (buffer), "%02d%c%c%02X%02X", address,
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
  else
    decValue = value;
  snprintf (buffer, sizeof (buffer),
            "dec: %06d | oct: %05o | hex: %04X \tbin: ", decValue, value,
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

  snprintf (buffer, sizeof (buffer), "sc: %c%02X%02X ", sign ? '-' : '+',
            command, operand);
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
  mt_gotoXY (icounterBlockX + 2, icounterBlockY + 1);
  snprintf (buffer, sizeof (buffer), "T: 00");
  write (STDOUT_FILENO, buffer, strlen (buffer));
  mt_gotoXY (icounterBlockX + icounterBlockWidth / 2 + 1, icounterBlockY + 1);
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
  int value, sign, command, operand;
  sc_memoryGet (address, &value);
  sc_commandDecode (value, &sign, &command, &operand);
  snprintf (buffer, sizeof (buffer), "%02d%c%c%02X%02X", address,
            input ? '>' : '<', sign ? '-' : '+', command, operand);

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
  int commandTextX = commandBlockX + (commandBlockWidth - 9) / 2;
  mt_gotoXY (commandTextX, commandBlockY + 1);

  if (sign || sc_commandValidate (command))
    write (STDOUT_FILENO, "!", 1);
  else
    write (STDOUT_FILENO, "+", 1);

  snprintf (buffer, sizeof (buffer), " %02X : %02X", command, operand);

  write (STDOUT_FILENO, buffer, strlen (buffer));
}

void
printBorders (void)
{
  bc_box (memoryBlockY, memoryBlockX, memoryBlockHeight, memoryBlockWidth,
          WHITE, BLACK, "Оперативная память", RED, BLACK);
  bc_box (accumulatorBlockY, accumulatorBlockX, accumulatorBlockHeight,
          accumulatorBlockWidth, WHITE, BLACK, "Аккумулятор", RED, BLACK);
  bc_box (flagsBlockY, flagsBlockX, flagsBlockHeight, flagsBlockWidth, WHITE,
          BLACK, "Регистр флагов", RED, BLACK);
  bc_box (icounterBlockY, icounterBlockX, icounterBlockHeight,
          icounterBlockWidth, WHITE, BLACK, "Счётчик комманд", RED, BLACK);
  bc_box (commandBlockY, commandBlockX, commandBlockHeight, commandBlockWidth,
          WHITE, BLACK, "Команда", RED, BLACK);
  bc_box (editingCellBlockY, editingCellBlockX, editingCellBlockHeight,
          editingCellBlockWidth, WHITE, BLACK, "Редактируемая ячейка (формат)",
          RED, WHITE);
  bc_box (zoomedCellBlockY, zoomedCellBlockX, zoomedCellBlockHeight,
          zoomedCellBlockWidth, WHITE, BLACK,
          "Редактируемая ячейка (увеличено)", RED, WHITE);
  bc_box (cashBlockY, cashBlockX, cashBlockXHeight, cashBlockWidth, WHITE,
          BLACK, "Кеш процессора", GREEN, WHITE);
  bc_box (inOutBlockY, inOutBlockX, inOutBlockHeight, inOutBlockWidth, WHITE,
          BLACK, "IN--OUT", GREEN, WHITE);
  bc_box (keysBlockY, keysBlockX, keysBlockHeight, keysBlockWidth, WHITE,
          BLACK, "Клавиши", GREEN, WHITE);
}

void
printBigCell (int *bigchars, int adress)
{
  int buffer[2];
  int value;
  sc_memoryGet (adress, &value);

  if ((value >> 14) & 0x1)
    {
      buffer[0] = bigchars[17 * 2];
      buffer[1] = bigchars[17 * 2 + 1];
    }
  else
    {
      buffer[0] = bigchars[16 * 2];
      buffer[1] = bigchars[16 * 2 + 1];
    }
  bc_printbigchar (buffer, zoomedCellBlockY + 2, zoomedCellBlockX + 3, WHITE,
                   BLACK);

  int ch;
  for (int i = 0; i < 2; i++)
    {
      ch = value & 0xF;
      buffer[0] = bigchars[ch * 2];
      buffer[1] = bigchars[ch * 2 + 1];
      bc_printbigchar (buffer, zoomedCellBlockY + 2,
                       zoomedCellBlockX + 3 + 8 * (4 - 2 * i), WHITE, BLACK);
      value >>= 4;

      ch = value & 0x7;
      buffer[0] = bigchars[ch * 2];
      buffer[1] = bigchars[ch * 2 + 1];
      bc_printbigchar (buffer, zoomedCellBlockY + 2,
                       zoomedCellBlockX + 3 + 8 * (4 - 2 * i - 1), WHITE,
                       BLACK);
      value >>= 3;
    }
  mt_gotoXY (zoomedCellBlockX + 3,
             zoomedCellBlockY + zoomedCellBlockHeight - 2);
  mt_setfgcolor (BLUE);
  char str[64];
  snprintf (str, 128, "Номер редактируемой ячейки %03d", adress);
  write (STDOUT_FILENO, str, strlen (str));
  mt_setdefaultcolor ();
}

void
printKeys ()
{
  char buffer[128];

  mt_gotoXY (keysBlockX + 1, keysBlockY + 1);
  snprintf (buffer, sizeof (buffer), "l - load  s - save  i - reset");
  write (STDOUT_FILENO, buffer, strlen (buffer));

  mt_gotoXY (keysBlockX + 1, keysBlockY + 2);
  snprintf (buffer, sizeof (buffer), "r - run   t - step");
  write (STDOUT_FILENO, buffer, strlen (buffer));

  mt_gotoXY (keysBlockX + 1, keysBlockY + 3);
  snprintf (buffer, sizeof (buffer), "ESC - выход");
  write (STDOUT_FILENO, buffer, strlen (buffer));

  mt_gotoXY (keysBlockX + 1, keysBlockY + 4);
  snprintf (buffer, sizeof (buffer), "F5 - accumulator");
  write (STDOUT_FILENO, buffer, strlen (buffer));

  mt_gotoXY (keysBlockX + 1, keysBlockY + 5);
  snprintf (buffer, sizeof (buffer), "F6 - instruction counter");
  write (STDOUT_FILENO, buffer, strlen (buffer));
}

void
printCells (int editingCellAdress)
{
  for (int i = 0; i < 128; i++)
    if (i == editingCellAdress)
      printCell (i, BLACK, WHITE);
    else
      printCell (i, WHITE, BLACK);
}
