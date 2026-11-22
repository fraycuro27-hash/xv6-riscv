#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
crear_arbol(int nivel, int max_nivel, int pid_padre)
{
    int pid;

    printf("Nivel %d: PID=%d, padre=%d\n", nivel, getpid(), pid_padre);

    if(nivel < max_nivel){
        pid = fork();

        if(pid < 0){
            printf("Error: fork fallo\n");
            exit(1);
        }

        if(pid == 0){
            // El hijo crea el siguiente nivel
            crear_arbol(nivel + 1, max_nivel, getpid());
            exit(0);
        } else {
            // El padre espera a que termine su hijo
            wait(0);
        }
    }
}

int
main(int argc, char *argv[])
{
    crear_arbol(0, 2, 0);
    exit(0);
}
