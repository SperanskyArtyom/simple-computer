#include "console.h"
#include <myBigChars.h>
#include <mySimpleComputer.h>
#include <myTerm.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
  if (winX < 103 || winY < 26)
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
    }

  mt_clrscr ();

  sc_memoryInit ();
  sc_accumulatorInit ();
  sc_icounterInit ();
  sc_regInit ();
  int editingCellAdress = 0, editingCellValue;

  srand (time (NULL));
  for (int i = 0; i < 12; i++)
    {
      int value = rand () % (1 << 15);
      sc_memorySet (i, value);
    }
  sc_memorySet (0, 6708);

  int value;
  sc_commandEncode (0, 0x00, 1, &value);
  sc_memorySet (12, value);
  sc_icounterSet (1);

  for (int i = 0; i < 128; i++)
    if (i == editingCellAdress)
      printCell (i, BLACK, WHITE);
    else
      printCell (i, WHITE, BLACK);

  printAccumulator ();
  printFlags ();
  printCounters ();
  printCommand ();

  sc_memoryGet (editingCellAdress, &editingCellValue);
  printDecodedCommand (editingCellValue);

  printBorders ();

  mt_setcursorvisible (0);
  for (int i = 0; i < 5; i++)
    {
      printTerm (i, 0);
      appendToHist (i, 0);
    }

  mt_setcursorvisible (1);
  mt_gotoXY (winX, winY);
  mt_delline ();
  return 0;
}
