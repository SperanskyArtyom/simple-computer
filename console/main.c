#include "console.h"
#include <myBigChars.h>
#include <myReadKey.h>
#include <mySimpleComputer.h>
#include <myTerm.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void updateFlags ();

int
main (int argc, char *argv[])
{

  if (!isatty (fileno (stdout)))
    {
      fprintf (stderr,
               "Ошибка: стандартный поток вывода не является терминалом.\n");
      exit (1);
    }

  int winX, winY;
  mt_getscreensize (&winY, &winX);
  if (winX < 108 || winY < 26)
    {
      fprintf (stderr, "Ошибка: размер окна терминал слишком мал.\n");
      exit (1);
    }

  const char *font_filename = (argc > 1) ? argv[1] : "console/font.bin";
  int fd = open (font_filename, O_RDONLY);
  if (fd < 2)
    {
      fprintf (stderr, "Ошибка чтения файла %s\n", font_filename);
      exit (1);
    }

  int bigchars[36]; // 18 символов × 2 int
  int counted;

  if (bc_bigcharread (fd, bigchars, 18, &counted) == -1)
    {
      fprintf (stderr, "Ошибка чтения файла шрифта\n");
      close (fd);
      exit (1);
    }
  close (fd);

  if (counted != 18)
    {
      fprintf (stderr,
               "Внимание: прочитано %d больших символов (ожидалось 18)\n",
               counted);
      exit (1);
    }

  mt_clrscr ();
  mt_setcursorvisible (0);

  sc_memoryInit ();
  sc_accumulatorInit ();
  sc_icounterInit ();
  sc_regInit ();
  int editingCellAdress = 0, editingCellValue;

  printCells (editingCellAdress);

  updateFlags ();

  printAccumulator ();
  printFlags ();
  printCounters ();
  printCommand ();
  printKeys ();

  sc_memoryGet (editingCellAdress, &editingCellValue);
  printDecodedCommand (editingCellValue);
  printBigCell (bigchars, editingCellAdress);

  printBorders ();

  for (int i = 0; i < 5; i++)
    {
      printTerm (i, 0);
      appendToHist (i, 0);
    }
  while (1)
    {
      enum keys choice;
      rk_mytermregime (1, 0, 1, 0, 1);
      rk_readkey (&choice);
      rk_mytermregime (0, 0, 0, 0, 0);
      switch (choice)
        {
        case KEY_ESC:
          mt_setcursorvisible (1);
          mt_setdefaultcolor ();
          mt_gotoXY (1, winY);
          return 0;

        case KEY_LEFT:
          if (editingCellAdress == 120)
            editingCellAdress = 127;
          else if (editingCellAdress % 10 == 0)
            editingCellAdress += 9;
          else
            editingCellAdress--;

          printCells (editingCellAdress);
          sc_memoryGet (editingCellAdress, &editingCellValue);
          printDecodedCommand (editingCellValue);
          printBigCell (bigchars, editingCellAdress);
          break;

        case KEY_RIGHT:
          if (editingCellAdress == 127)
            editingCellAdress = 120;
          else if (editingCellAdress % 10 == 9)
            editingCellAdress -= 9;
          else
            editingCellAdress++;
          printCells (editingCellAdress);
          sc_memoryGet (editingCellAdress, &editingCellValue);
          printDecodedCommand (editingCellValue);
          printBigCell (bigchars, editingCellAdress);
          break;

        case KEY_UP:
          if (editingCellAdress < 8)
            editingCellAdress += 120;
          else if (editingCellAdress < 10)
            editingCellAdress += 110;
          else
            editingCellAdress -= 10;
          printCells (editingCellAdress);
          sc_memoryGet (editingCellAdress, &editingCellValue);
          printDecodedCommand (editingCellValue);
          printBigCell (bigchars, editingCellAdress);
          break;

        case KEY_DOWN:
          if (editingCellAdress > 119)
            editingCellAdress -= 120;
          else if (editingCellAdress > 117)
            editingCellAdress -= 110;
          else
            editingCellAdress += 10;
          printCells (editingCellAdress);
          sc_memoryGet (editingCellAdress, &editingCellValue);
          printDecodedCommand (editingCellValue);
          printBigCell (bigchars, editingCellAdress);
          break;

        case KEY_ENTER:
          int x = (editingCellAdress % 10) * 6 + 2;
          int y = editingCellAdress / 10 + 2;
          mt_gotoXY (x, y);
          write (STDOUT_FILENO, "     ", 5);
          mt_gotoXY (x, y);
          int value;
          rk_readvalue (&value, 0);
          if (value != -1)
            {
              sc_memorySet (editingCellAdress, value);

              updateFlags ();
              printFlags ();
              sc_memoryGet (editingCellAdress, &editingCellValue);
              printDecodedCommand (editingCellValue);
              printBigCell (bigchars, editingCellAdress);
              printCommand ();
            }
          printCell (editingCellAdress, BLACK, WHITE);
          break;

        case KEY_L:
          rk_mytermrestore ();
          printCells (editingCellAdress);
          sc_memoryGet (editingCellAdress, &editingCellValue);
          printDecodedCommand (editingCellValue);
          printBigCell (bigchars, editingCellAdress);
          updateFlags ();
          printFlags ();
          printCommand ();
          break;

        case KEY_S:
          rk_mytermsave ();
          break;

        case KEY_F5:
          mt_gotoXY (67, 2);
          write (STDOUT_FILENO, "     ", 5);
          mt_gotoXY (67, 2);
          rk_readvalue (&value, 0);
          if (value != -1)
            sc_accumulatorSet (value);
          printAccumulator ();
          break;

        case KEY_F6:
          mt_gotoXY (76, 5);
          write (STDOUT_FILENO, "     ", 5);
          mt_gotoXY (76, 5);
          rk_readvalue (&value, 0);
          if (value != -1 && value < 128)
            sc_icounterSet (value);
          printCounters ();
          printCommand ();
          break;

        default:
          break;
        }
    }
  mt_setcursorvisible (1);
  mt_setdefaultcolor ();
  mt_gotoXY (30, winY);

  return 0;
}

void
updateFlags ()
{
  for (int i = 0; i < 128; i++)
    {
      int sign, command, operand, value;
      sc_memoryGet (i, &value);
      sc_commandDecode (value, &sign, &command, &operand);
      if (sign || sc_commandValidate (command))
        {
          sc_regSet (FLAG_INVALID_CMD, 1);
          return;
        }
    }
  sc_regSet (FLAG_INVALID_CMD, 0);
}
