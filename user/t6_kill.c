#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SEM 600

int
main(void)
{
  sem_open(SEM, 0);
  int pid = fork();
  if(pid == 0){
    printf("hijo: esperando en sem_wait...\n");
    sem_wait(SEM);
    printf("hijo: ERROR, sigue vivo tras kill\n");
    exit(0);
  }
  pause(10);
  printf("padre: kill(%d) devolvio %d\n", pid, kill(pid));
  int st;
  int r = wait(&st);
  printf("padre: wait devolvio %d (esperado %d), estado %d\n", r, pid, st);
  printf("padre: sem_close devolvio %d (esperado 0)\n", sem_close(SEM));
  printf("t6_kill: %s\n", r == pid ? "OK" : "FALLA");
  exit(0);
}
