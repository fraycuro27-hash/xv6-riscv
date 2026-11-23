# Revisión de las pruebas (Briam Chachayma)

Pruebas ejecutadas sobre la rama `feature/sem-kernel` con `make qemu CPUS=3`.

Entorno: Debian, QEMU 10.0.13, riscv64-linux-gnu-gcc 14.2.0.

| Prueba | Resultado |
|---|---|
| t0_basico | 8 de 8 valores correctos |
| t1_contador | Sin semáforo 164; con semáforo 200 |
| t2_prodcons | 0 errores; con semáforos, máximo 5 pendientes |
| t3_filosofos | Con sala, 15 comidas; sin sala (nosala) se bloquea |
| t4_lectores | Escritores en exclusión; sin violaciones |
| t5_limites | 0 fallas |
| t6_kill | t6_kill: OK |

Observación: `exec ... failed` apareció de forma intermitente (con t3_filosofos), igual que el error 1 de la Tabla 7 de la monografía. Al repetir el comando, el programa arrancó.
