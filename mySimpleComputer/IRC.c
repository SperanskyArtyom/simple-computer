#include <console.h>
#include <mySimpleComputer.h>

#include <signal.h>
#include <sys/time.h>

void
IRC (int signum)
{
  switch (signum)
    {
    case SIGUSR1:
      sc_accumulatorInit ();
      sc_icounterInit ();
      sc_regInit ();
      sc_memoryInit ();
      appendToHist (-1, 0);
      break;

    case SIGALRM:
      CU ();
      break;

    default:
      break;
    }
}
