#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int iteraciones = 50000;
  char *texto = "hola";

  // 1. Medición de strlen() en espacio de usuario
  int t1 = uptime();
  for(int i = 0; i < iteraciones; i++){
    volatile int len = strlen(texto);
    (void)len;
  }
  int t2 = uptime();

  // 2. Medición de contar_caracteres() mediante llamada al sistema
  int t3 = uptime();
  for(int i = 0; i < iteraciones; i++){
    volatile int len = contar_caracteres(texto);
    (void)len;
  }
  int t4 = uptime();

  printf("=== BENCHMARK DE RENDIMIENTO (%d iteraciones) ===\n", iteraciones);
  printf("strlen() en usuario        : %d ticks\n", t2 - t1);
  printf("contar_caracteres() syscall: %d ticks\n", t4 - t3);
  printf("==================================================\n");

  exit(0);
}
