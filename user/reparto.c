#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pipe_pares[2];
    int pipe_impares[2];
    int pid1, pid2;
    int suma_pares = 0;
    int suma_impares = 0;
    int total;
    int numeros[20];

    // Crear la lista de numeros del 1 al 20
    for(int i = 0; i < 20; i++){
        numeros[i] = i + 1;
    }

    // Crear los dos pipes antes de los fork()
    if(pipe(pipe_pares) < 0 || pipe(pipe_impares) < 0){
        printf("Error: pipe fallo\n");
        exit(1);
    }

    // Crear primer hijo: calcula la suma de pares
    pid1 = fork();

    if(pid1 < 0){
        printf("Error: fork fallo\n");
        exit(1);
    }

    if(pid1 == 0){
        close(pipe_pares[0]);
        close(pipe_impares[0]);
        close(pipe_impares[1]);

        for(int i = 0; i < 20; i++){
            if(numeros[i] % 2 == 0){
                suma_pares += numeros[i];
            }
        }

        write(pipe_pares[1], &suma_pares, sizeof(int));

        close(pipe_pares[1]);
        exit(0);
    }

    // Crear segundo hijo: calcula la suma de impares
    pid2 = fork();

    if(pid2 < 0){
        printf("Error: fork fallo\n");
        exit(1);
    }

    if(pid2 == 0){
        close(pipe_impares[0]);
        close(pipe_pares[0]);
        close(pipe_pares[1]);

        for(int i = 0; i < 20; i++){
            if(numeros[i] % 2 != 0){
                suma_impares += numeros[i];
            }
        }

        write(pipe_impares[1], &suma_impares, sizeof(int));

        close(pipe_impares[1]);
        exit(0);
    }

    // El padre no escribe en ninguno de los pipes
    close(pipe_pares[1]);
    close(pipe_impares[1]);

    // El padre recibe los resultados
    read(pipe_pares[0], &suma_pares, sizeof(int));
    read(pipe_impares[0], &suma_impares, sizeof(int));

    close(pipe_pares[0]);
    close(pipe_impares[0]);

    // Esperar a que terminen ambos hijos
    wait(0);
    wait(0);

    total = suma_pares + suma_impares;

    printf("Hijo pares: suma = %d\n", suma_pares);
    printf("Hijo impares: suma = %d\n", suma_impares);
    printf("Total combinado: %d\n", total);

    exit(0);
}
