#include <myTerm.h>

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

void
writeEscSeq (const char *escSeq)
{
  write (STDOUT_FILENO, escSeq, strlen (escSeq));
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

int
mt_getscreensize (int *rows, int *cols)
{
  struct winsize window;
  if (ioctl (STDOUT_FILENO, TIOCGWINSZ, &window) == -1)
    return -1;
  *rows = window.ws_row;
  *cols = window.ws_col;
  return 0;
}

int
mt_setfgcolor (enum colors color)
{
  char escapeSequence[20];
  snprintf (escapeSequence, sizeof (escapeSequence), "\E[%dm", 30 + color);
  writeEscSeq (escapeSequence);
  return 0;
}

int
mt_setbgcolor (enum colors color)
{
  char escapeSequence[20];
  snprintf (escapeSequence, sizeof (escapeSequence), "\E[%dm", 40 + color);
  writeEscSeq (escapeSequence);
  return 0;
}

int
mt_setdefaultcolor (void)
{
  const char *escapeSequence = "\E[%dm";
  writeEscSeq (escapeSequence);
  return 0;
}

int
mt_setcursorvisible (int value)
{
  if (value)
    {
      const char *escapeSequence = "\E[?12;25h";
      writeEscSeq (escapeSequence);
    }
  else
    {
      const char *escapeSequence = "\E[?25l";
      writeEscSeq (escapeSequence);
    }
  return 0;
}

int
mt_delline (void)
{
  const char *escapeSequence = "\E[M";
  writeEscSeq (escapeSequence);
  return 0;
}
