#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  printf("sem_wait(99) sin abrir: %d (esperado -1)\n", sem_wait(99));
  printf("sem_open(1, 2): %d (esperado 0)\n", sem_open(1, 2));
  printf("wait #1: %d (esperado 0)\n", sem_wait(1));
  printf("wait #2: %d (esperado 0)\n", sem_wait(1));
  printf("post: %d (esperado 0)\n", sem_post(1));
  printf("wait #3: %d (esperado 0)\n", sem_wait(1));
  printf("close: %d (esperado 0)\n", sem_close(1));
  printf("close de nuevo: %d (esperado -1)\n", sem_close(1));
  exit(0);
}
