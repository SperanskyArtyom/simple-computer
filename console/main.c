#include "console.h"
#include <mySimpleComputer.h>
#include <myTerm.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int
main ()
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

  int value;
  sc_commandEncode (1, 0x00, 1, &value);
  sc_memorySet (12, value);
  sc_icounterSet (12);

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

  mt_setcursorvisible (0);
  for (int i = 0; i < 7; i++)
    {
      printTerm (i, 0);
      appendToHist (i, 0);
      sleep (1);
    }

  mt_setcursorvisible (1);
  write (STDOUT_FILENO, "\n", 2);
  return 0;
}
