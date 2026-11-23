# Semáforos en el kernel de xv6

Trabajo final del curso **Sistemas Operativos** (UNSCH). Docente: Ing. Lizbeth Jaico Quispe.

Extensión de **xv6-riscv** (MIT PDOS) que agrega **semáforos contadores** al kernel, accesibles desde programas de usuario mediante cuatro llamadas al sistema. Se validan con problemas clásicos de concurrencia.

## Problema
xv6 solo ofrece spinlocks y sleeplocks dentro del kernel. Los procesos de usuario no tienen ninguna primitiva de sincronización, por lo que pierden actualizaciones al compartir recursos (condición de carrera).

## Solución
| Llamada | Descripción |
|---|---|
| `sem_open(id, valor)` | Crea u obtiene un semáforo con ese identificador |
| `sem_wait(id)` | Decrementa; bloquea si el valor es 0 |
| `sem_post(id)` | Incrementa y despierta a los procesos en espera |
| `sem_close(id)` | Cierra el semáforo; libera la entrada al llegar a 0 referencias |

Archivos principales: `kernel/sem.c` (nuevo), `kernel/sysproc.c`, `kernel/syscall.c`, `kernel/syscall.h`, `kernel/defs.h`, `kernel/main.c`, `user/usys.pl`, `user/user.h`. Las syscalls usan los números 28 a 31.

## Requisitos
- Ubuntu (o WSL2)
- `git`, `make`, `qemu-system-riscv64` y la toolchain `riscv64-linux-gnu-gcc`

```bash
sudo apt install git build-essential gdb-multiarch qemu-system-misc \
     gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
```

## Compilación y ejecución
```bash
git clone https://github.com/fraycuro27-hash/xv6-riscv.git
cd xv6-riscv
git checkout feature/sem-kernel
make clean
make qemu
```
Para salir de xv6: `Ctrl+A` y luego `X`.

Nota: se aumentó `FSSIZE` de 2000 a 4000 en `kernel/param.h` para que la imagen de disco contenga todos los programas de prueba.

## Pruebas
Se ejecutan dentro de xv6, un comando a la vez:

| Prueba | Comando | Resultado esperado |
|---|---|---|
| 0. Syscalls básicas | `t0_basico` | Los 8 valores coinciden con lo esperado |
| 1. Exclusión mutua | `t1_contador` y `t1_contador sem` | Sin sem.: menor que 200. Con sem.: 200 |
| 2. Productor–consumidor | `t2_prodcons` y `t2_prodcons sem` | Con sem.: máximo 5 pendientes |
| 3. Filósofos comensales | `t3_filosofos` y `t3_filosofos nosala` | Con sala termina; sin sala se bloquea |
| 4. Lectores–escritores | `t4_lectores` | Escritores en exclusión; sin VIOLACION |
| 5. Casos límite | `t5_limites` | 0 fallas |
| 6. kill de proceso bloqueado | `t6_kill` | Imprime `t6_kill: OK` |

Las capturas de cada prueba están en `documento_TrabajoFinal/evidencias/`.

## Documentación
La carpeta `documento_TrabajoFinal/` contiene la monografía, la presentación y las evidencias.


## Integrantes
| Integrante | GitHub | Aporte |
|---|---|---|
| [Nombre A] | fraycuro27-hash | Núcleo del kernel (`sem.c`) |
| [Nombre B] | [usuario] | Llamadas al sistema |
| [Nombre C] | [usuario] | Pruebas y documentación |
