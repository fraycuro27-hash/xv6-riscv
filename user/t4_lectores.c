#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MUTEX 30
#define ESCR 31
#define NLECT 3
#define NESCR 2

static int
leer(char *nombre)
{
  char buf[16];
  int fd = open(nombre, O_RDONLY);
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
guardar(char *nombre, int v)
{
  int fd = open(nombre, O_CREATE | O_WRONLY | O_TRUNC);
  fprintf(fd, "%d", v);
  close(fd);
}

static void
lector(int id)
{
  pause(id * 2);
  sem_wait(MUTEX);
  int rc = leer("rc.txt") + 1;
  guardar("rc.txt", rc);
  if(rc == 1)
    sem_wait(ESCR);
  sem_post(MUTEX);

  if(leer("esc.txt") != 0)
    printf("VIOLACION: lector %d con escritor activo\n", id);
  printf("L%d entra (activos=%d)\n", id, rc);
  pause(8);
  printf("L%d sale\n", id);

  sem_wait(MUTEX);
  rc = leer("rc.txt") - 1;
  guardar("rc.txt", rc);
  if(rc == 0)
    sem_post(ESCR);
  sem_post(MUTEX);
}

static void
escritor(int id)
{
  pause(5 + id);
  sem_wait(ESCR);
  if(leer("rc.txt") != 0 || leer("esc.txt") != 0)
    printf("VIOLACION: escritor %d con otros procesos activos\n", id);
  guardar("esc.txt", 1);
  printf("E%d ESCRIBE\n", id);
  pause(3);
  printf("E%d termina\n", id);
  guardar("esc.txt", 0);
  sem_post(ESCR);
}

int
main(void)
{
  guardar("rc.txt", 0);
  guardar("esc.txt", 0);
  sem_open(MUTEX, 1);
  sem_open(ESCR, 1);

  for(int i = 0; i < NLECT; i++){
    if(fork() == 0){
      lector(i);
      exit(0);
    }
  }
  for(int i = 0; i < NESCR; i++){
    if(fork() == 0){
      escritor(i);
      exit(0);
    }
  }
  for(int i = 0; i < NLECT + NESCR; i++)
    wait(0);
  sem_close(MUTEX);
  sem_close(ESCR);
  printf("t4_lectores: fin\n");
  exit(0);
}
