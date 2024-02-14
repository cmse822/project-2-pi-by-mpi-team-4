#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Open /proc/self/stat file to read CPU number
    std::ifstream stat_file("/proc/self/stat");
    std::string stat_line;
    std::getline(stat_file, stat_line);
    stat_file.close();

    // Extract CPU number from stat_line
    size_t start_pos = stat_line.find('(') + 1;
    size_t end_pos = stat_line.find(')');
    std::string cpu_str = stat_line.substr(start_pos, end_pos - start_pos);

    std::cout << "Process " << rank << " is running on CPU: " << cpu_str << std::endl;

    MPI_Finalize();
    return 0;
}
