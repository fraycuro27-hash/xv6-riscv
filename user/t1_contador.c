#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define NPROCS 4
#define ITER 50
#define SEM_ID 1

static int
leer(void)
{
  char buf[16];
  int fd = open("contador.txt", O_RDONLY);
  if(fd < 0)
    return 0;
  int n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if(n < 0)
    n = 0;
  buf[n] = 0;
  return atoi(buf);
}

static void
incrementar(void)
{
  int v = leer() + 1;
  int fd = open("contador.txt", O_CREATE | O_WRONLY | O_TRUNC);
  fprintf(fd, "%d", v);
  close(fd);
}

int
main(int argc, char *argv[])
{
  int usar = (argc > 1 && strcmp(argv[1], "sem") == 0);
  int fd = open("contador.txt", O_CREATE | O_WRONLY | O_TRUNC);
  fprintf(fd, "0");
  close(fd);

  if(usar && sem_open(SEM_ID, 1) < 0){
    printf("error: sem_open\n");
    exit(1);
  }
  for(int i = 0; i < NPROCS; i++){
    int pid = fork();
    if(pid < 0){
      printf("error: fork\n");
      exit(1);
    }
    if(pid == 0){
      for(int j = 0; j < ITER; j++){
        if(usar) sem_wait(SEM_ID);
        incrementar();
        if(usar) sem_post(SEM_ID);
      }
      exit(0);
    }
  }
  for(int i = 0; i < NPROCS; i++)
    wait(0);
  if(usar) sem_close(SEM_ID);

  printf("%s: contador final = %d (esperado %d)\n",
         usar ? "CON semaforo" : "SIN semaforo", leer(), NPROCS * ITER);
  exit(0);
}
