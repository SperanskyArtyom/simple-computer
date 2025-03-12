#include <myTerm.h>

#include <string.h>
#include <unistd.h>

int
mt_clrscr (void)
{
  const char *clear_screen = "\E[H\E[2J";
  write (STDOUT_FILENO, clear_screen, strlen (clear_screen));
  return 0;
}
