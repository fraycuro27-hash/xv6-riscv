#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    int pid = fork();

    if(pid < 0){
        fprintf(2, "Error: no se pudo crear el hijo\n");
        exit(1);
    }
    else if(pid == 0){
        printf("Hijo: mi PID es %d\n", getpid());
        printf("Hijo: terminando con exit...\n");
        exit(0);
    }
    else {
        int status;
        int child_pid = wait(&status);
        printf("Padre: cree un hijo con PID %d\n", pid);
        printf("Padre: hijo %d termino con codigo %d\n", child_pid, status);
        exit(0);
    }
}
