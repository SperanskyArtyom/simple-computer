#pragma once

enum color
{
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
};

int mt_clrscr (void);
int mt_gotoXY (int, int);
int mt_getscreensize (int *rows, int *cols);
