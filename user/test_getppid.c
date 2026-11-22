#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int parent_pid = getpid();
  printf("[PADRE] Mi PID es: %d | PPID: %d\n", parent_pid, getppid());

  int pid = fork();
  if(pid < 0){
    printf("Error en fork\n");
    exit(1);
  }

  if(pid == 0){
    printf("[HIJO] Mi PID es: %d | PPID inicial: %d\n", getpid(), getppid());
    pause(10);
    printf("[HIJO HUERFANO] Tras la muerte del padre, mi PPID es: %d\n", getppid());
    exit(0);
  } else {
    printf("[PADRE] Finalizando para dejar al hijo huerfano...\n");
    exit(0);
  }
}
