#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

/* Escribe un programa en C utilizando la biblioteca MPI (Message Passing Interface) 
que realice las siguientes operaciones:

1. Inicializa el entorno MPI.

2. Determina el rango (identificador) del proceso actual y el número total de procesos.

3. Si el proceso es el maestro (proceso 0), envía un mensaje 
(por ejemplo, una cadena de caracteres) a todos los demás procesos.

4. Si el proceso no es el maestro, recibe el mensaje enviado por el proceso maestro.

5. Imprime el mensaje recibido junto con el identificador del proceso que lo recibió.

6. Finaliza el entorno MPI. 
*/

int main(int argc, char **argv){

    int myRank, size, i;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    char message[128];

    if(myRank==0){
        for(i=0; i<size; i++){
            sprintf(message, "Hola desde el proceso maestro al proceso %d!", i);
            MPI_Send(message, 128, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }

    }else{
        MPI_Recv(message, 128, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("El proceso %d, ha recibido el mensaje: %s\n", myRank, message);
    }

    MPI_Finalize();
    return 0;
}