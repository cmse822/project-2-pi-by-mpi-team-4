#include <stdlib.h>
#include <mpi.h>
#include <iostream>

using namespace std;

void srandom(unsigned seed);
double dboard(int darts);
long random(void);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <number_of_darts> <number_of_rounds>" << endl;
        exit(1);
    }
    
    int darts = atoi(argv[1]);
    int rounds = atoi(argv[2]);

    double pi;
    double localSumPi = 0;
    int numTasks, rank;
    double startTime, endTime, elapsedTime;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srandom(rank);
    int localRounds = rounds / numTasks;
    int modRounds = rounds % numTasks;

    if (modRounds != 0 && rank == numTasks - 1) {
        localRounds += modRounds;
    }

    for (int i = 0; i < localRounds; i++) {
        pi = dboard(darts);
        localSumPi += pi;
    }

    double globalSum;
    MPI_Allreduce(&localSumPi, &globalSum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    MPI_Finalize();

    endTime = MPI_Wtime();
    elapsedTime = endTime - startTime;

    if (rank == numTasks - 1) {
        double globalMean = globalSum / rounds;
        cout << "The average value of PI using " << numTasks << " processors is " << globalMean << endl;
        cout << "Time elapsed: " << elapsedTime << " seconds" << endl;
    }

    return 0;
}

double dboard(int darts) {
    double xCoord, yCoord, pi, r;
    int score = 0;
    unsigned int cConst = 2 << (31 - 1);

    if (sizeof(cConst) != 4) {
        cerr << "Wrong data size for cConst variable in dboard routine!" << endl;
        exit(1);
    }

    for (int n = 1; n <= darts; n++) {
        r = static_cast<double>(random()) / cConst;
        xCoord = (2.0 * r) - 1.0;
        r = static_cast<double>(random()) / cConst;
        yCoord = (2.0 * r) - 1.0;

        if ((xCoord * xCoord + yCoord * yCoord) <= 1.0)
            score++;
    }

    pi = 4.0 * static_cast<double>(score) / static_cast<double>(darts);
    return pi;
}