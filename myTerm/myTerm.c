#include <myTerm.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void
writeEscSeq (const char *escSeq)
{
  write (STDOUT_FILENO, escSeq, sizeof (escSeq));
}

int
mt_clrscr (void)
{
  const char *escapeSequence = "\E[H\E[2J";
  writeEscSeq (escapeSequence);
  return 0;
}

int
mt_gotoXY (int x, int y)
{
  char escapeSequence[20];
  snprintf (escapeSequence, sizeof (escapeSequence), "\E[%d;%dH", y, x);
  writeEscSeq (escapeSequence);
  return 0;
}
