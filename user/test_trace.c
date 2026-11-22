#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  printf("=== INICIO DE PRUEBA TRACE ===\n");

  // Rastrear SYS_getpid (bit 1 << 11) y SYS_getppid (bit 1 << 24)
  trace((1 << 11) | (1 << 24));

  getpid();
  getppid();

  printf("=== FIN DE PRUEBA TRACE ===\n");
  exit(0);
}
