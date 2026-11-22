#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    printf("\n============================================\n");
    printf("   DEMOSTRACION DE PROCESOS EN XV6 (IS-380)\n");
    printf("============================================\n");
    
    int pid_padre = getpid();
    printf("[PADRE] Proceso Padre iniciado con PID: %d\n", pid_padre);

    printf("[PADRE] Ejecutando fork() para crear proceso hijo...\n");
    int pid = fork();

    if (pid < 0) {
        printf("[ERROR] Fallo en la llamada fork()\n");
        exit(1);
    } 
    else if (pid == 0) {
        printf("  --> [HIJO] Creado exitosamente con PID: %d\n", getpid());
        printf("  --> [HIJO] Ejecutando comando 'ls' mediante exec()...\n");
        
        char *args[] = {"ls", 0};
        exec("ls", args);
        
        printf("  --> [ERROR] Fallo la ejecucion de exec()\n");
        exit(1);
    } 
    else {
        printf("[PADRE] Esperando a que el hijo (PID: %d) termine con wait()...\n", pid);
        wait(0);
        printf("[PADRE] El proceso hijo ha finalizado su ejecucion.\n");
        printf("[PADRE] Proceso Padre termina correctamente.\n");
        printf("============================================\n\n");
    }

    exit(0);
}
