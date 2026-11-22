#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    int pid;

    pid = fork();

    if(pid < 0){
        fprintf(2, "Error: no se pudo crear el hijo\n");
        exit(1);
    }
    else if(pid == 0){
        printf("Hijo (PID %d): ejecutare echo con varios argumentos\n", getpid());

        char *argv[] = { "echo", "uno", "dos", "tres", "cuatro", 0 };

        exec("echo", argv);

        // Solo se llega aqui si exec falla
        fprintf(2, "Error: exec fallo\n");
        exit(1);
    }
    else {
        int status;
        printf("Padre: cree un hijo con PID %d\n", pid);
        int child_pid = wait(&status);
        printf("Padre: hijo %d termino con codigo %d\n", child_pid, status);
        exit(0);
    }
}
