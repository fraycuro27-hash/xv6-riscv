#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

char path[512];
char *p;

int
buscar_nombre(char *directorio, int inum_actual)
{
    int fd;
    struct dirent de;

    fd = open(directorio, 0);

    if(fd < 0)
        return -1;

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;

        if(de.inum == inum_actual){
            p -= strlen(de.name);
            memmove(p, de.name, strlen(de.name));

            p--;
            *p = '/';

            close(fd);
            return 0;
        }
    }

    close(fd);
    return -1;
}

int
main(int argc, char *argv[])
{
    int fd;
    struct stat st;
    int inum_actual;
    int inum_padre;

    // Obtener el inode del directorio actual
    fd = open(".", 0);

    if(fd < 0){
        printf("pwd: no se pudo abrir el directorio actual\n");
        exit(1);
    }

    if(fstat(fd, &st) < 0){
        printf("pwd: no se pudo obtener informacion\n");
        close(fd);
        exit(1);
    }

    inum_actual = st.ino;

    close(fd);

    // Preparar el buffer para construir la ruta
    p = path + sizeof(path);
    *--p = '\0';

    // Recorrer los directorios hasta llegar a la raiz
    while(inum_actual != 1){

        // Abrir el directorio padre
        fd = open("..", 0);

        if(fd < 0){
            printf("pwd: error al abrir directorio padre\n");
            exit(1);
        }

        if(fstat(fd, &st) < 0){
            printf("pwd: error al obtener inode padre\n");
            close(fd);
            exit(1);
        }

        inum_padre = st.ino;

        close(fd);

        // Buscar el nombre del directorio actual
        if(buscar_nombre("..", inum_actual) < 0){
            printf("pwd: no se pudo encontrar el nombre\n");
            exit(1);
        }

        // Subir al directorio padre
        if(chdir("..") < 0){
            printf("pwd: error al cambiar al directorio padre\n");
            exit(1);
        }

        inum_actual = inum_padre;
    }

    // Mostrar la ruta construida
    if(*p == '\0')
        printf("/\n");
    else
        printf("%s\n", p);

    exit(0);
}
