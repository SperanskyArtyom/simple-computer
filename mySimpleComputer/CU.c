#include <console.h>
#include <myReadKey.h>
#include <mySimpleComputer.h>
#include <myTerm.h>

void
icounterStep ()
{
  int adress;
  sc_icounterGet (&adress);
  sc_icounterSet (++adress);
}

void
CU (void)
{
  mt_printMessage ("");
  int adress, value, command, operand, sign;

  // a. Получаем текущаю команду.
  sc_icounterGet (&adress);
  sc_memoryGet (adress, &value);

  // b. Декодируем команду
  sc_commandDecode (value, &sign, &command, &operand);
  if (sc_commandValidate (command))
    {
      sc_regSet (FLAG_INVALID_CMD, 1);
      sc_regSet (FLAG_IGNORE_CLOCK, 1);
      return;
    }
  switch (command)
    {
    case 0x00: // NOP
      icounterStep ();
      break;

    case 0x01: // CPUINFO
      mt_printMessage ("ИП 317: Калашникова Варвара Алекссевна, Сперанский "
                       "Артём Александрович");
      icounterStep ();
      break;

    case 0x0A: // READ
      if (operand > 127)
        {
          sc_regSet (FLAG_MEM_OOB, 1);
          sc_regSet (FLAG_IGNORE_CLOCK, 1);
          mt_printMessage ("READ error: memory out of bounds");
          break;
        }
      mt_printMessage ("Введите значение: ");
      rk_readvalue (&value, 0);
      sc_memorySet (operand, value);
      printTerm (operand, 1);
      appendToHist (operand, 1);
      icounterStep ();
      break;

    case 0x0B: // WRITE

      if (operand > 127)
        {
          sc_regSet (FLAG_MEM_OOB, 1);
          sc_regSet (FLAG_IGNORE_CLOCK, 1);
          mt_printMessage ("WRITE error: memory out of bounds");
          break;
        }
      printTerm (operand, 0);
      appendToHist (operand, 0);
      icounterStep ();
      break;

    case 0x14: // LOAD
      if (sc_memoryGet (operand, &value))
        {
          sc_regSet (FLAG_MEM_OOB, 1);
          sc_regSet (FLAG_IGNORE_CLOCK, 1);
          mt_printMessage ("LOAD error: memory out of bounds");
          break;
        }
      sc_accumulatorSet (value);
      icounterStep ();
      break;

    case 0x15: // STORE
      if (sc_accumulatorGet (&value))
        {
          sc_regSet (FLAG_MEM_OOB, 1);
          sc_regSet (FLAG_IGNORE_CLOCK, 1);
          mt_printMessage ("STORE error: memory out of bounds");
          break;
        }
      sc_memorySet (operand, value);
      icounterStep ();
      break;

    case 0x28: // JUMP
      if (sc_icounterSet (operand))
        {
          sc_regSet (FLAG_MEM_OOB, 1);
          sc_regSet (FLAG_IGNORE_CLOCK, 1);
          mt_printMessage ("JUMP error: memory out of bounds");
        }
      break;

    case 0x29: // JNEG
      sc_accumulatorGet (&value);
      if (value & (1 << 14))
        {
          if (sc_icounterSet (operand))
            {
              sc_regSet (FLAG_MEM_OOB, 1);
              sc_regSet (FLAG_IGNORE_CLOCK, 1);
              mt_printMessage ("JNEG error: memory out of bounds");
            }
        }
      else
        icounterStep ();
      break;

    case 0x2A: // JZ
      sc_accumulatorGet (&value);
      if (!value)
        {
          if (sc_icounterSet (operand))
            {
              sc_regSet (FLAG_MEM_OOB, 1);
              sc_regSet (FLAG_IGNORE_CLOCK, 1);
              mt_printMessage ("JNEG error: memory out of bounds");
            }
        }
      else
        icounterStep ();
      break;

    case 0x2B: // HALT
      sc_regSet (FLAG_IGNORE_CLOCK, 1);
      break;

    default:
      if (ALU (command, operand))
        {
          sc_regSet (FLAG_IGNORE_CLOCK, 1);
          mt_printMessage ("Error while ALU operation");
          break;
        }
      icounterStep ();
      break;
    }
}
