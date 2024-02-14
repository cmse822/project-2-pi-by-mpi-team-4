#include <mpi.h>
#include <iostream>
#include <string>
#include <sched.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the CPU number using sched_getcpu() function
    int cpu_number = sched_getcpu();

    std::cout << "Process " << rank << " is running on CPU: " << cpu_number << std::endl;

    MPI_Finalize();
    return 0;
}
