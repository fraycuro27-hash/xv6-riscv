#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NHIJOS 4

// Trabajo intensivo de CPU (sin llamadas al sistema)
void
busy(int carga)
{
  for(volatile uint64 i = 0; i < (uint64)carga * 1000000; i++)
    ;
}

int
main(int argc, char *argv[])
{
  int carga = (argc > 1) ? atoi(argv[1]) : 500;
  int prio[NHIJOS] = { 5, 5, 5, 5 };   // Mayor numero = mayor prioridad
  int t0, i, pid;

  // El padre toma la prioridad maxima para crear a todos
  // los hijos antes de que alguno se ejecute
  setpriority(getpid(), 10);
  t0 = uptime();

  for(i = 0; i < NHIJOS; i++){
    pid = fork();
    if(pid < 0){
      fprintf(2, "Error en fork\n");
      exit(1);
    }
    if(pid == 0){
      int ini = uptime() - t0;
      busy(carga);
      printf("Hijo %d (PID=%d, prio=%d): inicio=%d fin=%d ticks\n",
             i, getpid(), prio[i], ini, uptime() - t0);
      exit(0);
    }
    setpriority(pid, prio[i]);   // El padre fija la prioridad del hijo
  }

  for(i = 0; i < NHIJOS; i++)
    wait(0);

  printf("=== Fin del test de prioridad ===\n");
  exit(0);
}

