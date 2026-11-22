#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N 5
#define COMIDAS 3
#define TENEDOR 10
#define SALA 20

int
main(int argc, char *argv[])
{
  int sala = !(argc > 1 && strcmp(argv[1], "nosala") == 0);
  for(int i = 0; i < N; i++)
    sem_open(TENEDOR + i, 1);
  if(sala)
    sem_open(SALA, N - 1);

  for(int i = 0; i < N; i++){
    if(fork() == 0){
      int izq = TENEDOR + i;
      int der = TENEDOR + (i + 1) % N;
      for(int k = 1; k <= COMIDAS; k++){
        if(sala) sem_wait(SALA);
        sem_wait(izq);
        pause(1);
        sem_wait(der);
        printf("F%d come (%d)\n", i, k);
        pause(1);
        sem_post(der);
        sem_post(izq);
        if(sala) sem_post(SALA);
        pause(1);
      }
      exit(0);
    }
  }
  for(int i = 0; i < N; i++)
    wait(0);
  for(int i = 0; i < N; i++)
    sem_close(TENEDOR + i);
  if(sala)
    sem_close(SALA);
  printf("%s: todos terminaron\n", sala ? "CON sala" : "SIN sala");
  exit(0);
}
