#include <myReadKey.h>

#include <string.h>
#include <unistd.h>

int
rk_readkey (enum keys *key)
{
  if (key == NULL)
    return -1;
  char buf[5] = { 0 };
  int bytes_read = read (STDIN_FILENO, buf, sizeof (buf));

  if (bytes_read <= 0)
    return -1;

  // Одиночные символы
  if (bytes_read == 1)
    {
      if (buf[0] >= 'a' && buf[0] <= 'z')
        {
          *key = (enum keys) (KEY_A + (buf[0] - 'a'));
          return 0;
        }
      if (buf[0] >= 'A' && buf[0] <= 'Z')
        {
          *key = (enum keys) (KEY_A + (buf[0] - 'A'));
          return 0;
        }
      if (buf[0] >= '0' && buf[0] <= '9')
        {
          *key = (enum keys) (KEY_0 + (buf[0] - '0'));
          return 0;
        }
      switch (buf[0])
        {
        case '+':
          *key = KEY_PLUS;
          return 0;
        case '-':
          *key = KEY_MINUS;
          return 0;
        case '\e':
          *key = KEY_ESC;
          return 0;
        case '\n':
          *key = KEY_ENTER;
          return 0;
        }
    }

  // Последовательности
  if (bytes_read >= 3)
    {
      if (strncmp (buf, "\e[15~", 4) == 0)
        {
          *key = KEY_F5;
          return 0;
        }
      if (strncmp (buf, "\e[17~", 4) == 0)
        {
          *key = KEY_F6;
          return 0;
        }
      if (strncmp (buf, "\e[A", 3) == 0)
        {
          *key = KEY_UP;
          return 0;
        }
      if (strncmp (buf, "\e[B", 3) == 0)
        {
          *key = KEY_DOWN;
          return 0;
        }
      if (strncmp (buf, "\e[C", 3) == 0)
        {
          *key = KEY_RIGHT;
          return 0;
        }
      if (strncmp (buf, "\e[D", 3) == 0)
        {
          *key = KEY_LEFT;
          return 0;
        }
    }

  *key = KEY_UNKNOWN;
  return 0;
}
