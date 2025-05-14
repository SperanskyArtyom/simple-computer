#include <myBigChars.h>

#include <stdio.h>
#include <unistd.h>

int
bc_strlen (const char *str)
{
  if (str == NULL)
    return 0;

  int count = 0;
  const char *p = str;

  while (*p != '\0')
    {
      char c = *p;

      if (c <= 0x7F) // Однобайтовый символ
        p++;

      else if ((c & 0xE0) == 0xC0) // Двухбайтовый символ: 110xxxxx
        {
          if ((p[1] & 0xC0) != 0x80) // Некорректный следующая байт
            return 0;
          p += 2;
        }

      else if ((c & 0xF0) == 0xE0) // Трехбайтовый символ: 1110xxxx
        {
          if ((p[1] & 0xC0) != 0x80
              || (p[2] & 0xC0) != 0x80) // Некорректный следующий байт
            return 0;
          p += 3;
        }

      else if ((c & 0xF8) == 0xF0) // Четырехбайтовый символ: 11110xxx
        {
          if ((p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80
              || (p[3] & 0xC0) != 0x80) // Некорректный следующий байт
            return 0;
          p += 4;
        }

      else // Недопустимый первый байт
        return 0;

      count++;
    }

  return count;
}

int
bc_box (int x1, int y1, int x2, int y2, enum colors box_fg, enum colors box_bg,
        char *header, enum colors header_fg, enum colors header_bg)
{
  mt_setfgcolor (box_fg);
  mt_setbgcolor (box_bg);

  write (STDOUT_FILENO, "\E(0", 3);

  // draw top border
  mt_gotoXY (y1, x1);
  write (STDOUT_FILENO, "l", 1);
  for (int i = 1; i < y2 - 1; i++)
    {
      write (STDOUT_FILENO, "q", 1);
    }
  write (STDOUT_FILENO, "k", 1);

  // draw side borders
  for (int i = 1; i < x2 - 1; i++)
    {
      mt_gotoXY (y1, x1 + i);
      write (STDOUT_FILENO, "x", 1);
      mt_gotoXY (y1 + y2 - 1, x1 + i);
      write (STDOUT_FILENO, "x", 1);
    }

  // draw bottom border
  mt_gotoXY (y1, x1 + x2 - 1);
  write (STDOUT_FILENO, "m", 1);
  for (size_t i = 1; i < y2 - 1; i++)
    {
      write (STDOUT_FILENO, "q", 1);
    }
  write (STDOUT_FILENO, "j", 1);

  write (STDOUT_FILENO, "\E(B", 3);

  int headerLen;
  if (header != NULL && (headerLen = bc_strlen (header)))
    {
      mt_setfgcolor (header_fg);
      mt_setbgcolor (header_bg);
      mt_gotoXY (y1 + (y2 - headerLen) / 2, x1);
      write (STDOUT_FILENO, header, headerLen);
    }

  mt_setdefaultcolor ();

  return 0;
}

int
bc_setbigcharpos (int *big, int x, int y, int value)
{
  if (big == NULL)
    return -1;
  if (x < 0 || x > 7 || y < 0 || y > 7)
    return -1;

  int index = x / 4;
  int bit = (x % 4) * 8 + y;

  if (value)
    big[index] |= (1 << bit);
  else
    big[index] &= ~(1 << bit);

  return 0;
}

int
bc_getbigcharpos (int *big, int x, int y, int *value)
{
  if (big == NULL)
    return -1;
  if (value == NULL)
    return -1;
  if (x < 0 || x > 7 || y < 0 || y > 7)
    return -1;

  int index = x / 4;
  int bit = (x % 4) * 8 + y;

  *value = (big[index] >> bit) & 0x1;

  return 0;
}
