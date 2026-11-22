#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "uso: tiempo prog [args...]\n");
        exit(1);
    }

    int inicio = uptime();
    int pid = fork();

    if(pid < 0){
        fprintf(2, "tiempo: fork fallo\n");
        exit(1);
    }
    if(pid == 0){
        exec(argv[1], &argv[1]);
        fprintf(2, "tiempo: exec %s fallo\n", argv[1]);
        exit(127);
    }

    int status;
    wait(&status);
    int fin = uptime();

    printf("PID %d, codigo de salida %d, tiempo %d ticks\n",
           pid, status, fin - inicio);
    exit(0);
}
