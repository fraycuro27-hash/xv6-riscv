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
        for(int i = 0; i < 5; i++){
            printf("Hijo (PID %d): iteracion %d\n", getpid(), i);
            pause(1);
        }
        exit(0);
    }
    else {
        for(int i = 0; i < 5; i++){
            printf("Padre (PID %d): iteracion %d\n", getpid(), i);
            pause(1);
        }
        int status;
        int child_pid = wait(&status);
        printf("Padre: hijo %d termino con codigo %d\n", child_pid, status);
        exit(0);
    }
}
