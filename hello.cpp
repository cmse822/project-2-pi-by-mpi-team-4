#include <iostream>
#include <mpi.h>

using namespace std;

cout << "Before initialize" << endl;

int main(int argc, char* argv[]) 
{
    int numtasks, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    cout << "Hello, World! I am process " << rank << " of " << numtasks << endl;
    MPI_Finalize();

    cout << "After initialize" << endl;
    return 0;
}
