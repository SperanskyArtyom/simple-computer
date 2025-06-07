#include <mySimpleComputer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_WORD_LENGTH 32
#define MAX_WORDS 10

int translate_command (char *str, int size);
int parse_line (char *line, int *address, int *sign, int *command,
                int *operand, int i);

int
main (int argc, char *argv[])
{
  if (argc != 3)
    {
      printf ("Неправильное число параметров!\n");
      return 1;
    }

  // Инициализация памяти
  sc_memoryInit ();
  char *input_file = argv[1], *output_file = argv[2];

  // Открываем файл с программой
  FILE *file = fopen (input_file, "r");
  if (!file)
    {
      printf ("Ошибка открытия файла %s\n", input_file);
      return 1;
    }

  int address, sign, command, operand;
  char line[MAX_LINE_LENGTH];

  int i = 1;
  // Читаем программу построчно
  while (fgets (line, sizeof (line), file))
    {
      // Пропускаем комментарии и пустые строки
      if (line[0] == '\n')
        continue;
      // Парсим строку
      if (parse_line (line, &address, &sign, &command, &operand, i))
        {
          return 1;
        }
      int value;
      if (sc_commandEncode (sign, command, operand, &value) == 0)
        {
          // Записываем в память
          if (sc_memorySet (address, value) != 0)
            {
              printf ("Ошибка записи в память по адресу %d\n", address);
              fclose (file);
              return 1;
            }
        }
      else
        {
          printf ("Ошибка кодирования команды по адресу %d\n", address);
          fclose (file);
          return 1;
        }

      i++;
    }

  fclose (file);

  // Сохраняем программу в файл
  if (sc_memorySave (output_file) != 0)
    {
      printf ("Ошибка сохранения программы\n");
      return 1;
    }

  printf ("Программа успешно загружена и сохранена в %s\n", output_file);
  return 0;
}

int
translate_command (char *str, int size)
{
  // Убираем возможные пробелы и приводим к верхнему регистру
  char command[10] = { 0 };
  for (int i = 0, j = 0; i < size && str[i] != '\0'; i++)
    {
      if (str[i] != ' ')
        {
          command[j++] = str[i];
        }
    }

  // Сравниваем с известными командами
  if (strcmp (command, "NOP") == 0)
    return 0x00;
  if (strcmp (command, "CPUINFO") == 0)
    return 0x01;
  if (strcmp (command, "READ") == 0)
    return 0x0A;
  if (strcmp (command, "WRITE") == 0)
    return 0x0B;
  if (strcmp (command, "LOAD") == 0)
    return 0x14;
  if (strcmp (command, "STORE") == 0)
    return 0x15;
  if (strcmp (command, "ADD") == 0)
    return 0x1E;
  if (strcmp (command, "SUB") == 0)
    return 0x1F;
  if (strcmp (command, "DIVIDE") == 0)
    return 0x20;
  if (strcmp (command, "MUL") == 0)
    return 0x21;
  if (strcmp (command, "JUMP") == 0)
    return 0x28;
  if (strcmp (command, "JNEG") == 0)
    return 0x29;
  if (strcmp (command, "JZ") == 0)
    return 0x2A;
  if (strcmp (command, "JP") == 0)
    return 0x3A;
  if (strcmp (command, "JNP") == 0)
    return 0x3B;
  if (strcmp (command, "HALT") == 0)
    return 0x2B;
  if (strcmp (command, "=") == 0)
    return 2;

  // Если команда не найдена
  return -1;
}

int
parse_line (char *line, int *address, int *sign, int *command, int *operand,
            int i)
{
  char words[MAX_WORDS][MAX_WORD_LENGTH] = { 0 };
  char command_name[MAX_WORD_LENGTH];
  int word_count = 0;
  char *p = line;
  char current_word[MAX_WORD_LENGTH] = { 0 };
  int word_pos = 0;

  // Разбиваем строку на слова до символа ';' или конца строки
  while (*p != '\0' && *p != '\n' && word_count < MAX_WORDS)
    {
      if (*p == ';')
        {
          break; // Встретили комментарий - прекращаем обработку
        }

      if (*p == ' ')
        {
          if (word_pos > 0)
            { // Завершаем текущее слово
              current_word[word_pos] = '\0';
              strcpy (words[word_count], current_word);
              word_count++;
              word_pos = 0;
            }
          p++;
        }
      else
        {
          if (word_pos < MAX_WORD_LENGTH - 1)
            {
              current_word[word_pos++] = *p;
            }
          p++;
        }
    }

  // Добавляем последнее слово, если оно есть
  if (word_pos > 0)
    {
      current_word[word_pos] = '\0';
      strcpy (words[word_count], current_word);
      word_count++;
    }

  // Проверяем количество слов
  if (word_count < 3)
    {
      fprintf (stderr,
               "Ошибка в строке %d: строка содержит менее 3 параметров\n", i);
      return 1;
    }

  // Проверяем адрес (должен быть числом)
  char *endptr;
  *address = strtol (words[0], &endptr, 10);
  if (*endptr != '\0')
    {
      fprintf (stderr, "Ошибка в строке %d: адрес должен быть числом\n", i);
      return 1;
    }

  *sign = 0;
  // Копируем имя команды
  strncpy (command_name, words[1], MAX_WORD_LENGTH);
  command_name[MAX_WORD_LENGTH - 1] = '\0';
  if (strcmp (command_name, "=") == 0)
    {
      if (words[2][0] != '+' && words[2][0] != '-')
        {
          fprintf (stderr,
                   "Ошибка в строке %d: число должно начинаться со знака\n",
                   i);
          return 1;
        }
      int buffer = strtol (words[2], &endptr, 16);
      if (*endptr != '\0')
        {
          fprintf (stderr,
                   "Ошибка в строке %d: число содержит недопустимые символы\n",
                   i);
          return 1;
        }
      if (buffer < 0)
        {
          *sign = 1;
          buffer = -buffer;
        }
      if (buffer > 0x7f7f)
        {
          fprintf (stderr, "Ошибка в строке %d: число выходит за диапазон\n",
                   i);
          return 1;
        }
      *operand = buffer & 0x7f;
      *command = (buffer >> 8) & 0x7f;
      return 0;
    }

  *command = translate_command (command_name, strlen (command_name));
  // Проверяем операнд (должен быть числом)
  *operand = strtol (words[2], &endptr, 10);
  if (*endptr != '\0')
    {
      fprintf (stderr, "Ошибка в строке %d: операнд должен быть числом\n", i);
      return 1;
    }

  // Проверяем, есть ли лишние слова перед комментарием
  if (word_count > 3)
    {
      fprintf (stderr,
               "Ошибка в строке %d: слишком много параметров (ожидается 3)\n",
               i);
      return 1;
    }

  return 0;
}
