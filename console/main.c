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
  int winX, winY;
  mt_getscreensize (&winX, &winY);

  sc_memoryInit ();
  sc_accumulatorInit ();
  sc_icounterInit ();
  sc_regInit ();
  int editingCellAdress = 0, editingCellValue;

  srand (time (NULL));
  int k = rand () % 128 + 1;
  for (int i = 0; i < k; i++)
    {
      int value = rand () % (1 << 15);
      sc_memorySet (i, value);
    }

  mt_clrscr ();
  for (int i = 0; i < 128; i++)
    if (i == editingCellAdress)
      printCell (i, BLACK, WHITE);
    else
      printCell (i, WHITE, BLACK);

  printFlags ();
  printAccumulator ();
  printCounters ();

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
