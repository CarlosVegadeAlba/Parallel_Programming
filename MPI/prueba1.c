#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  // Inicializa el entorno MPI

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);  // Obtiene el número total de procesos

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);  // Obtiene el rango (ID) del proceso actual

    if (world_rank == 0) {
        // Proceso maestro
        int data = 100;  // Mensaje a enviar
        for (int i = 1; i < world_size; i++) {
            MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);  // Enviar mensaje a todos los procesos
        }
    } else {
        // Procesos esclavos
        int data;
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Recibir mensaje del proceso 0
        printf("Proceso %d recibió el dato %d\n", world_rank, data);
    }

    MPI_Finalize();  // Finaliza el entorno MPI
    return 0;
}
