#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char** argv) {
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size != 2) {
        if (rank == 0) {
            printf("Number of processes must be 2. Given: %i", size);
        }
        MPI_Finalize();
        exit(1);
    }
    int a[5];
    a[0] = 10;
    a[1] = rank;
    if (rank == 1) {
        MPI_Send(&a[1], 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&a[0], 1, MPI_INT, 1, 99, MPI_COMM_WORLD, &status);
        printf("%i", a[0]);
    }

    MPI_Finalize();
    return 0;
}
