#include <myBigChars.h>

#include <stddef.h>

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
        {
          p++;
        }

      else if ((c & 0xE0) == 0xC0) // Двухбайтовый символ: 110xxxxx
        {
          if ((p[1] & 0xC0) != 0x80) // Некорректный следующая байт
            {
              return 0;
            }
          p += 2;
        }

      else if ((c & 0xF0) == 0xE0) // Трехбайтовый символ: 1110xxxx
        {
          if ((p[1] & 0xC0) != 0x80
              || (p[2] & 0xC0) != 0x80) // Некорректный следующий байт
            {
              return 0;
            }
          p += 3;
        }

      else if ((c & 0xF8) == 0xF0) // Четырехбайтовый символ: 11110xxx
        {
          if ((p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80
              || (p[3] & 0xC0) != 0x80) // Некорректный следующий байт
            {
              return 0;
            }
          p += 4;
        }

      else // Недопустимый первый байт
        {
          return 0;
        }

      count++;
    }

  return count;
}
