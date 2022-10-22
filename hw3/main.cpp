#include <iostream>
#include "mpi/mpi.h"

int main(int argc, char** argv) {
    int numtask, rank;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtask);

    std::cout << "Hello, " + std::to_string(rank) + " " + std::to_string(numtask) + "\n";

    MPI_Finalize();
}
