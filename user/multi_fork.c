#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    int i;
    int n_hijos = 3;  // Numero de hijos a crear

    for(i = 0; i < n_hijos; i++){
        int pid = fork();

        if(pid < 0){
            fprintf(2, "Error en fork para hijo %d\n", i);
            exit(1);
        }

        // Codigo del hijo
        if(pid == 0){
            printf("Hijo %d (PID=%d) iniciado\n", i, getpid());

            // Cada hijo tiene un retardo distinto
            for(int j = 0; j < 5; j++){
                printf("Hijo %d (PID=%d): paso %d\n", i, getpid(), j);
                pause(10 * (i + 1));
            }

            printf("Hijo %d (PID=%d) fin\n", i, getpid());
            exit(0);
        }
    }

    // Codigo del padre: esperar a todos los hijos
    for(i = 0; i < n_hijos; i++){
        int child_pid = wait(0);
        printf("Padre: hijo %d ha terminado\n", child_pid);
    }

    printf("Padre: todos los hijos han terminado\n");
    exit(0);
}
