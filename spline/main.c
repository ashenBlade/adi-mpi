#include "stdio.h"
#include "mpi.h"

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int up = rank == 0 ? MPI_PROC_NULL : rank - 1,
            down = rank == size - 1 ? MPI_PROC_NULL : rank + 1;;

    printf("%i %i\n", up, down);

    MPI_Finalize();
    return 0;
}
