#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

static int fallas = 0;

static void
chk(char *desc, int obtenido, int esperado)
{
  printf("%s: %d (esperado %d) %s\n", desc, obtenido, esperado,
         obtenido == esperado ? "OK" : "FALLA");
  if(obtenido != esperado)
    fallas++;
}

int
main(void)
{
  int i;
  chk("a1 sem_wait inexistente", sem_wait(99), -1);
  chk("a2 sem_post inexistente", sem_post(99), -1);
  chk("a3 sem_close inexistente", sem_close(99), -1);
  chk("b  sem_open valor negativo", sem_open(5, -1), -1);

  for(i = 0; i < 16; i++)
    if(sem_open(200 + i, 1) != 0)
      break;
  chk("c1 abrir 16 semaforos", i, 16);
  chk("c2 abrir el 17", sem_open(300, 1), -1);
  for(i = 0; i < 16; i++)
    sem_close(200 + i);

  sem_open(400, 1);
  sem_close(400);
  chk("d1 reabrir tras cerrar", sem_open(400, 3), 0);
  int ok = 0;
  for(i = 0; i < 3; i++)
    if(sem_wait(400) == 0)
      ok++;
  chk("d2 tres wait con valor 3", ok, 3);
  sem_close(400);

  sem_open(500, 0);
  int pid = fork();
  if(pid == 0){
    int r = sem_wait(500);
    printf("e  hijo: sem_wait tras close: %d (esperado -1) %s\n",
           r, r == -1 ? "OK" : "FALLA");
    exit(r == -1 ? 0 : 1);
  }
  pause(5);
  sem_close(500);
  int st;
  wait(&st);
  chk("e  hijo termino con estado", st, 0);

  printf("t5_limites: %d fallas\n", fallas);
  exit(fallas != 0);
}
