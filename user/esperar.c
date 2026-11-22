#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int codigos[3] = {10, 20, 30};
    int pid;
    int estado;

    // Crear 3 procesos hijos
    for(int i = 0; i < 3; i++){
        pid = fork();

        if(pid < 0){
            printf("Error: fork fallo\n");
            exit(1);
        }

        if(pid == 0){
            // Cada hijo termina con un codigo diferente
            exit(codigos[i]);
        }
    }

    // El padre espera a los 3 hijos
    for(int i = 0; i < 3; i++){
        pid = wait(&estado);
        printf("Hijo PID=%d termino con codigo %d\n", pid, estado);
    }

    exit(0);
}
