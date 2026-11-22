#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N 5
#define TOTAL 20
#define VACIOS 1
#define LLENOS 2

int
main(int argc, char *argv[])
{
  int usar = (argc > 1 && strcmp(argv[1], "sem") == 0);
  int fd[2];
  if(pipe(fd) < 0){
    printf("error: pipe\n");
    exit(1);
  }
  if(usar){
    sem_open(VACIOS, N);
    sem_open(LLENOS, 0);
  }

  if(fork() == 0){                       // productor
    for(int i = 1; i <= TOTAL; i++){
      char b = i;
      if(usar) sem_wait(VACIOS);
      write(fd[1], &b, 1);
      printf("P %d\n", i);
      if(usar) sem_post(LLENOS);
    }
    exit(0);
  }
  if(fork() == 0){                       // consumidor
    int errores = 0;
    for(int i = 1; i <= TOTAL; i++){
      char b;
      if(usar) sem_wait(LLENOS);
      if(read(fd[0], &b, 1) != 1){
        errores++;
        break;
      }
      printf("        C %d\n", b);
      if(b != i) errores++;
      if(usar) sem_post(VACIOS);
      pause(1);
    }
    printf("consumidor: %d errores\n", errores);
    exit(errores != 0);
  }
  wait(0);
  wait(0);
  if(usar){
    sem_close(VACIOS);
    sem_close(LLENOS);
  }
  printf("%s: fin\n", usar ? "CON semaforos" : "SIN semaforos");
  exit(0);
}
