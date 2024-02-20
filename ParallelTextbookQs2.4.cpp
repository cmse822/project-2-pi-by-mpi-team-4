#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int numtasks, rank;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::cout << "Hello" << " numtasks:" << numtasks << " rank:" << rank << std::endl;

    if (rank == 0) {
        std::cout << "No.0 rank, numtasks: " << numtasks << std::endl;
    }

    MPI_Finalize();
    return 0;
}
