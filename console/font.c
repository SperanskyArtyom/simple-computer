#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <myBigChars.h>

// Структура для хранения большого символа (8x8)
typedef struct
{
  int code;       // Код символа
  char repr;      // Символьное представление
  int pattern[2]; // Шаблон символа (2 int по 32 бита)
} BigChar;

// Функция для создания шаблона символа
void
create_char_pattern (int *big, char symbol)
{
  memset (big, 0, sizeof (int) * 2);

  switch (symbol)
    {
    case '0':
      for (int i = 0; i < 8; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
          bc_setbigcharpos (big, i, 6, 1);
        }
      for (int j = 2; j < 6; j++)
        {
          bc_setbigcharpos (big, 0, j, 1);
          bc_setbigcharpos (big, 7, j, 1);
        }
      break;

    case '1':
      for (int i = 0; i < 8; i++)
        {
          bc_setbigcharpos (big, i, 4, 1);
        }
      bc_setbigcharpos (big, 1, 3, 1);
      bc_setbigcharpos (big, 2, 2, 1);

      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 7, i, 1);
        }
      break;

    case '2':
      for (int j = 2; j < 7; j++)
        {
          bc_setbigcharpos (big, 0, j, 1);
          bc_setbigcharpos (big, 7, j, 1);
        }
      for (int i = 0; i < 6; i++)
        {
          bc_setbigcharpos (big, i + 2, 6 - i, 1);
        }
      bc_setbigcharpos (big, 1, 1, 1);
      bc_setbigcharpos (big, 1, 6, 1);
      bc_setbigcharpos (big, 2, 1, 1);
      break;

    case '3':
      for (int i = 2; i < 6; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 7, i, 1);
        }
      for (int i = 0; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 6, 1);
          bc_setbigcharpos (big, i, 6, 1);
        }
      bc_setbigcharpos (big, 1, 1, 1);
      bc_setbigcharpos (big, 6, 1, 1);
      bc_setbigcharpos (big, 3, 4, 1);
      bc_setbigcharpos (big, 3, 5, 1);
      bc_setbigcharpos (big, 4, 4, 1);
      bc_setbigcharpos (big, 4, 5, 1);
      break;

    case '4':
      for (int i = 0; i < 8; i++)
        {
          bc_setbigcharpos (big, i, 6, 1);
        }
      for (int i = 0; i < 5; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
        }
      for (int i = 2; i < 6; i++)
        {
          bc_setbigcharpos (big, 4, i, 1);
        }
      break;

    case '5':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 4, i, 1);
          bc_setbigcharpos (big, 7, i, 1);
        }
      for (int i = 1; i < 4; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
        }

      for (int i = 5; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 6, 1);
        }
      break;

    case '6':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 4, i, 1);
          bc_setbigcharpos (big, 7, i, 1);
        }
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
        }

      for (int i = 5; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 6, 1);
        }
      break;

    case '7':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
        }
      for (int i = 1; i < 8; i++)
        {
          bc_setbigcharpos (big, i, 6, 1);
        }
      break;

    case '8':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 4, i, 1);
          bc_setbigcharpos (big, 7, i, 1);
        }
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
          bc_setbigcharpos (big, i, 6, 1);
        }
      break;

    case '9':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 4, i, 1);
          bc_setbigcharpos (big, 7, i, 1);
        }
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 6, 1);
        }

      for (int i = 1; i < 5; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
        }
      break;

    case 'A':
      for (int i = 2; i < 8; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
          bc_setbigcharpos (big, i, 6, 1);
        }
      for (int i = 2; i < 6; i++)
        {
          bc_setbigcharpos (big, 4, i, 1);
        }
      bc_setbigcharpos (big, 0, 3, 1);
      bc_setbigcharpos (big, 0, 4, 1);
      bc_setbigcharpos (big, 1, 2, 1);
      bc_setbigcharpos (big, 1, 5, 1);
      break;

    case 'B':
      for (int i = 1; i < 5; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 4, i, 1);
          bc_setbigcharpos (big, 7, i, 1);
        }
      bc_setbigcharpos (big, 7, 5, 1);
      bc_setbigcharpos (big, 6, 6, 1);
      bc_setbigcharpos (big, 5, 6, 1);
      bc_setbigcharpos (big, 4, 5, 1);
      bc_setbigcharpos (big, 3, 5, 1);
      bc_setbigcharpos (big, 2, 5, 1);
      bc_setbigcharpos (big, 1, 5, 1);
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
        }
      break;

    case 'C':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 7, i, 1);
        }
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
        }
      break;

    case 'D':
      for (int i = 1; i < 5; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 7, i, 1);
        }
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
        }
      for (int i = 2; i < 6; i++)
        {
          bc_setbigcharpos (big, i, 6, 1);
        }
      bc_setbigcharpos (big, 1, 5, 1);
      bc_setbigcharpos (big, 6, 5, 1);
      break;

    case 'E':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 4, i, 1);
          bc_setbigcharpos (big, 7, i, 1);
        }
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
        }
      break;

    case 'F':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 0, i, 1);
          bc_setbigcharpos (big, 4, i, 1);
        }
      for (int i = 1; i < 8; i++)
        {
          bc_setbigcharpos (big, i, 1, 1);
        }
      break;

    case '+':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, i, 3, 1);
          bc_setbigcharpos (big, i, 4, 1);
          bc_setbigcharpos (big, 3, i, 1);
          bc_setbigcharpos (big, 4, i, 1);
        }
      break;

    case '-':
      for (int i = 1; i < 7; i++)
        {
          bc_setbigcharpos (big, 3, i, 1);
          bc_setbigcharpos (big, 4, i, 1);
        }
      break;
    default:
      // Квадрат для неизвестных символов
      for (int i = 2; i < 6; i++)
        {
          for (int j = 2; j < 6; j++)
            {
              bc_setbigcharpos (big, i, j, 1);
            }
        }
      break;
    }
}

int
save_font (const char *filename, int *big, int count)
{
  int fd = open (filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if (fd < 0)
    {
      perror ("Failed to open or create file");
      return -1;
    }

  int result = bc_bigcharwrite (fd, big, count);

  close (fd);

  return result;
}

int
main ()
{
  // Создаем массив больших символов согласно таблице
  BigChar font[] = {
    { 0, '0', { 0, 0 } },  { 1, '1', { 0, 0 } },  { 2, '2', { 0, 0 } },
    { 3, '3', { 0, 0 } },  { 4, '4', { 0, 0 } },  { 5, '5', { 0, 0 } },
    { 6, '6', { 0, 0 } },  { 7, '7', { 0, 0 } },  { 8, '8', { 0, 0 } },
    { 9, '9', { 0, 0 } },  { 10, 'A', { 0, 0 } }, { 11, 'B', { 0, 0 } },
    { 12, 'C', { 0, 0 } }, { 13, 'D', { 0, 0 } }, { 14, 'E', { 0, 0 } },
    { 15, 'F', { 0, 0 } }, { 16, '+', { 0, 0 } }, { 17, '-', { 0, 0 } }
  };
  int patterns[36]; // 18 символов × 2 int
  for (int i = 0; i < 18; i++)
    {
      create_char_pattern (font[i].pattern, font[i].repr);
      patterns[2 * i] = font[i].pattern[0];     // Первый int
      patterns[2 * i + 1] = font[i].pattern[1]; // Второй int
    }

  const char filename[] = "console/font.bin";
  save_font (filename, patterns, 18);
  // Сохраняем шрифт в файл
  printf ("Font generated successfully in %s\n", filename);
  return 0;
}
