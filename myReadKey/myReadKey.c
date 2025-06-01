#include <myReadKey.h>
#include <mySimpleComputer.h>
#include <myTerm.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int
power (int x, int y)
{
  int result = 1;
  while (y > 0)
    {
      result *= x;
      y--;
    }
  return result;
}

void
rk_read (char *buffer, size_t max_size)
{
  int i = 0;
  char c;
  rk_mytermregime (1, 0, 1, 0, 1);
  mt_setcursorvisible (1);
  while (i < max_size)
    {
      read (STDIN_FILENO, &c, 1);

      if (c == '\n')
        {
          break;
        }
      else if (c == 127)
        {
          if (i > 0)
            {
              i--;
              write (STDOUT_FILENO, "\b \b", 3);
            }
        }
      else if (isprint (c))
        {
          buffer[i] = c;
          write (STDOUT_FILENO, &c, 1);
          i++;
        }
    }
  buffer[i] = '\0';
  mt_setcursorvisible (0);
  rk_mytermregime (0, 0, 1, 1, 1);
}

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

int
rk_mytermsave (void)
{
  char buffer[128];
  mt_printMessage ("Введите имя файла для сохранения: ");
  rk_read (buffer, sizeof (buffer));
  int result = sc_memorySave (buffer);
  mt_delline ();
  return result;
}

int
rk_mytermrestore (void)
{
  char buffer[128];
  mt_printMessage ("Введите имя файла для загрузки: ");
  rk_read (buffer, sizeof (buffer));
  int result = sc_memoryLoad (buffer);
  if (result)
    {
      mt_printMessage ("Неверное имя файла!");
      return -1;
    }
  mt_delline ();
  return 0;
}

int
rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint)
{
  struct termios new_termios;
  if (tcgetattr (STDIN_FILENO, &new_termios) != 0)
    return -1;

  if (regime)
    {
      new_termios.c_lflag &= ~ICANON;
      new_termios.c_cc[VTIME] = vtime;
      new_termios.c_cc[VMIN] = vmin;

      if (echo)
        new_termios.c_lflag |= ECHO;
      else
        new_termios.c_lflag &= ~ECHO;

      if (sigint)
        new_termios.c_lflag |= ISIG;
      else
        new_termios.c_lflag &= ~ISIG;
    }
  else
    {
      new_termios.c_lflag |= ICANON;
      new_termios.c_lflag |= ECHO;
      new_termios.c_lflag |= ISIG;
    }

  if (tcsetattr (STDIN_FILENO, TCSANOW, &new_termios) != 0)
    return -1;

  return 0;
}

int
rk_readvalue (int *value, int timeout)
{
  if (value == NULL)
    return -1;

  rk_mytermregime (1, timeout, 1, 0, 1);
  mt_setcursorvisible (1);

  enum keys c;
  char buffer[5] = { 0 };
  int i = 0;
  while (1)
    {
      rk_readkey (&c);
      if (c == KEY_PLUS || c == KEY_MINUS)
        break;
      if (c == KEY_ESC)
        {
          *value = -1;
          rk_mytermregime (0, 0, 0, 0, 0);
          mt_setcursorvisible (0);
          return 0;
        }
    }
  buffer[i] = c;
  write (STDOUT_FILENO, (char *)&c, 1);
  i++;
  for (int j = 0; j < 2; j++)
    {
      while (1)
        {
          rk_readkey (&c);
          if (c >= KEY_0 && c <= KEY_7)
            break;

          if (c == KEY_ESC)
            {
              *value = -1;
              rk_mytermregime (0, 0, 0, 0, 0);
              mt_setcursorvisible (0);
              return 0;
            }
        }
      buffer[i] = c;
      write (STDOUT_FILENO, (char *)&c, 1);
      i++;
      while (1)
        {
          rk_readkey (&c);
          if (c >= KEY_0 && c <= KEY_9)
            break;
          if (c >= KEY_A && c <= KEY_F)
            break;

          if (c == KEY_ESC)
            {
              *value = -1;
              rk_mytermregime (0, 0, 0, 0, 0);
              mt_setcursorvisible (0);
              return 0;
            }
        }
      buffer[i] = c;
      write (STDOUT_FILENO, (char *)&c, 1);
      i++;
    }

  *value = 0;
  i--;
  for (int j = 0; j < 2; j++, i--)
    {
      if (buffer[i] <= '9')
        {
          *value += (buffer[i] - '0') * power (16, j);
        }
      else if (buffer[i] <= 'f')
        *value += (buffer[i] - 'a' + 10) * power (16, j);
    }

  for (int j = 0; j < 2; j++, i--)
    {
      if (buffer[i] <= '9')
        {
          *value += ((buffer[i] - '0') * power (16, j)) << 7;
        }
      else if (buffer[i] <= 'f')
        *value += ((buffer[i] - 'a' + 10) * power (16, j)) << 7;
    }

  if (buffer[i] == '-')
    *value |= (1 << 14);

  rk_mytermregime (0, 0, 0, 0, 0);
  mt_setcursorvisible (0);
  return 0;
}
