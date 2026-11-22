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
        printf("Hijo (PID %d): PPID antes = %d\n", getpid(), getppid());
        pause(50);
        printf("Hijo (PID %d): PPID despues = %d\n", getpid(), getppid());
        exit(0);
    }
    else {
        pause(10);
        printf("Padre (PID %d): termino sin wait\n", getpid());
        exit(0);
    }
}
