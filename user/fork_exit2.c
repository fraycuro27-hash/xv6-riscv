#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    int pid = fork();

    if(pid < 0){
        fprintf(2, "Error en fork\n");
        exit(1);
    }
    else if(pid == 0){
        printf("Hijo: mi PID es %d, termino con exit(7)\n", getpid());
        exit(7);
    }
    else {
        int status;
        int child_pid = wait(&status);
        printf("Padre: hijo %d termino con codigo %d\n", child_pid, status);
        exit(0);
    }
}
