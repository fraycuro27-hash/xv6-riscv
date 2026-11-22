#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int fd[2];
    int pid;
    int numeros[5] = {3, 7, 1, 9, 4};
    int numero;
    int suma = 0;

    if(pipe(fd) < 0){
        printf("Error: pipe fallo\n");
        exit(1);
    }

    pid = fork();

    if(pid < 0){
        printf("Error: fork fallo\n");
        exit(1);
    }

    if(pid == 0){
        close(fd[0]);

        printf("Hijo envia: ");

        for(int i = 0; i < 5; i++){
            printf("%d ", numeros[i]);
            write(fd[1], &numeros[i], sizeof(int));
        }

        printf("\n");

        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);

        printf("Padre recibe: ");

        while(read(fd[0], &numero, sizeof(int)) > 0){
            printf("%d ", numero);
            suma += numero;
        }

        printf("\n");

        close(fd[0]);

        wait(0);

        printf("Suma total: %d\n", suma);

        exit(0);
    }
}
