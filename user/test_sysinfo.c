#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/sysinfo.h"

int
main(void)
{
  struct sysinfo info;
  if(sysinfo(&info) < 0){
    printf("Error ejecutando sysinfo\n");
    exit(1);
  }
  printf("=== ESTADO DEL SISTEMA (sysinfo) ===\n");
  printf("Memoria libre   : %d bytes\n", (int)info.freemem);
  printf("Procesos activos: %d\n", (int)info.nproc);
  printf("===================================\n");
  exit(0);
}
