#include <myBigChars.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
bc_strlen (const char *str)
{
  if (str == NULL)
    return 0;

  int count = 0;
  const unsigned char *p = (const unsigned char *)str;

  while (*p != '\0')
    {
      if (*p <= 0x7F) // Однобайтовый символ
        {
          p++;
          count++;
        }
      else if ((p[0] & 0xE0) == 0xC0) // Двухбайтовый символ
        {
          if ((p[1] & 0xC0) != 0x80) // Некорректный следующий байт
            return 0;
          p += 2;
          count++;
        }
      else if ((p[0] & 0xF0) == 0xE0) // Трехбайтовый символ
        {
          if ((p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80)
            return 0;
          p += 3;
          count++;
        }
      else if ((p[0] & 0xF8) == 0xF0) // Четырехбайтовый символ
        {
          if ((p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80
              || (p[3] & 0xC0) != 0x80)
            return 0;
          p += 4;
          count++;
        }
      else // Недопустимый первый байт
        {
          return 0;
        }
    }

  return count;
}

int
bc_printA (char *str)
{
  if (str == NULL)
    return -1;

  write (STDOUT_FILENO, "\E(0", 3);
  write (STDOUT_FILENO, str, strlen (str));
  write (STDOUT_FILENO, "\E(B", 3);

  return 0;
}

int
bc_box (int x1, int y1, int x2, int y2, enum colors box_fg, enum colors box_bg,
        char *header, enum colors header_fg, enum colors header_bg)
{
  mt_setfgcolor (box_fg);
  mt_setbgcolor (box_bg);

  // draw top border
  mt_gotoXY (y1, x1);
  bc_printA ("l");
  write (STDOUT_FILENO, "\E(0", 3);
  for (int i = 1; i < y2 - 1; i++)
    {
      bc_printA ("q");
    }
  bc_printA ("k");

  // draw side borders
  for (int i = 1; i < x2 - 1; i++)
    {
      mt_gotoXY (y1, x1 + i);
      bc_printA ("x");
      mt_gotoXY (y1 + y2 - 1, x1 + i);
      bc_printA ("x");
    }

  // draw bottom border
  mt_gotoXY (y1, x1 + x2 - 1);
  bc_printA ("m");
  for (size_t i = 1; i < y2 - 1; i++)
    {
      bc_printA ("q");
    }
  bc_printA ("j");

  int headerLen;
  if (header != NULL && (headerLen = bc_strlen (header)))
    {
      mt_setfgcolor (header_fg);
      mt_setbgcolor (header_bg);
      mt_gotoXY (y1 + (y2 - headerLen - 2) / 2, x1);
      write (STDOUT_FILENO, " ", 1);
      write (STDOUT_FILENO, header, strlen (header));
      write (STDOUT_FILENO, " ", 1);
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

int
bc_printbigchar (int big[2], int x, int y, enum colors fg, enum colors bg)
{
  if (big == NULL)
    return -1;

  mt_setfgcolor (fg);
  mt_setbgcolor (bg);

  for (int i = 0; i < 8; i++)
    {
      int value;
      mt_gotoXY (y, x + i);
      for (int j = 0; j < 8; j++)
        {
          if (bc_getbigcharpos (big, i, j, &value))
            return -1;
          bc_printA (value ? "a" : " ");
        }
    }
  mt_setdefaultcolor ();

  return 0;
}

int
bc_bigcharwrite (int fd, int *big, int count)
{
  if (big == NULL)
    return -1;
  if (fd < 0 || count <= 0)
    return -1;

  for (int i = 0; i < count; i++)
    {
      if (write (fd, &big[i * 2], sizeof (int) * 2) != sizeof (int) * 2)
        {
          return -1;
        }
    }

  return 0;
}

int
bc_bigcharread (int fd, int *big, int need_count, int *count)
{
  if (big == NULL)
    return -1;
  if (count == NULL)
    return -1;
  if (fd < 0 || need_count <= 0)
    return -1;

  *count = 0;

  for (int i = 0; i < need_count; i++)
    {
      ssize_t bytes_read = read (fd, &big[i * 2], sizeof (int) * 2);

      if (bytes_read == 0)
        break;
      if (bytes_read != sizeof (int) * 2)
        return -1;

      (*count)++;
    }

  return (*count > 0) ? 0 : -1;
}
