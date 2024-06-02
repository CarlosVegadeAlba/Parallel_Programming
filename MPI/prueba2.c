#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, int **argv){

    MPI_Init(argc, argv);

    int rank, totalP;
    MPI_Comm_size(MPI_COMM_WORLD, &totalP);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Finalize();  

    return 0;
}