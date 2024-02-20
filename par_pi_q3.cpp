/***************************************************************************
 * FILE: ser_pi_calc.c
 * DESCRIPTION:  
 *   Parallel pi Calculation - C Version
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  
 * AUTHOR: unknown
 * REVISED: 02/23/12 Blaise Barney
 * Dan - Note that this version taked the number of darts and the output file 
 *       name as an input via the cmd arguments.
***************************************************************************/
// #include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

void srandom (unsigned seed);  
double dboard (int darts);

#define DARTS 1E7	 /* number of throws at dartboard */

int main(int argc, char *argv[])
{
   int ROUNDS;
   double pi,pierror;          	/* average of pi after "darts" is thrown */
   double avepi,localsumpi;       	/* average pi value for all iterations */
   int i, n, localrounds, modrounds;
   double start_time, end_time, elapsed_time;
   int numtasks, rank;

   ROUNDS = stod(argv[1]);  // Convert command-line argument to an integer
   const char* csvfile = argv[2]; //get the name for the csv output file

   start_time = MPI_Wtime();
      
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   srandom (rank);            /* seed the random number generator */

   localsumpi = 0.0;
   localrounds = ROUNDS/numtasks;
   modrounds = ROUNDS % numtasks; //need this if rounds not divisible by numtasks

   // Add the remaining rounds to each one until all rounds are done 
   if (modrounds != 0){
      if (rank < modrounds) {
         localrounds = localrounds + 1; 
      } 
   }

   for (i = 0; i < localrounds; i++) {
      pi = dboard(DARTS);
      localsumpi = localsumpi + pi;
   }

   //Compute the average of the results over all processes
   double globalmean, globalsum;
   int dest = 0;
   MPI_Reduce(&localsumpi,&globalsum,1,MPI_DOUBLE,MPI_SUM,dest,MPI_COMM_WORLD);

   MPI_Finalize();

   end_time = MPI_Wtime();

   elapsed_time = end_time - start_time;

   //Output results only once 
   if (rank == dest) {
      globalmean = double(globalsum) / double(ROUNDS);
      pierror = globalmean - 3.141592653589793;
      cout << "Darts, # proc, pi, time, error,  " << DARTS <<','
      << numtasks << ',' <<  globalmean<< "," << elapsed_time << "," << pierror  << endl;

      //things for output file
      std::ifstream infile(csvfile);
         //check if file is empty
         bool is_empty = infile.peek() == std::ifstream::traits_type::eof();
         infile.close();

         // Open the CSV file for writing (appending mode)
         std::ofstream file(csvfile, std::ios_base::app); // app = append mode

         // Write header if the file is empty
         if (is_empty) {
               file << "Darts, Rounds, # proc, pi, time, error" << std::endl;
         }

         // Write results to the CSV file
         if (file.is_open()) {
               file << DARTS << ',' << ROUNDS << ',' << numtasks << ',' << globalmean << ',' << elapsed_time << ',' << pierror << std::endl;
               file.close();
         } else {
               std::cerr << "Error: Unable to open file for writing." << std::endl;
         }
   }
} //End of main function. 


/*****************************************************************************
 * dboard
 *****************************************************************************/
#define sqr(x)	((x)*(x))
long random(void);

double dboard(int darts)
{
   double x_coord,       /* x coordinate, between -1 and 1  */
          y_coord,       /* y coordinate, between -1 and 1  */
          pi,            /* pi  */
          r;             /* random number scaled between 0 and 1  */
   int score,            /* number of darts that hit circle */
       n;
   unsigned int cconst;  /* must be 4-bytes in size */
/*************************************************************************
 * The cconst variable must be 4 bytes. We check this and bail if it is
 * not the right size
 ************************************************************************/
   if (sizeof(cconst) != 4) {
      printf("Wrong data size for cconst variable in dboard routine!\n");
      printf("See comments in source file. Quitting.\n");
      exit(1);
      }
   /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
   cconst = 2 << (31 - 1);
   score = 0;

/***********************************************************************
 * Throw darts at board.  Done by generating random numbers
 * between 0 and 1 and converting them to values for x and y
 * coordinates and then testing to see if they "land" in
 * the circle."  If so, score is incremented.  After throwing the
 * specified number of darts, pi is calculated.  The computed value
 * of pi is returned as the value of this function, dboard.
 ************************************************************************/

   for (n = 1; n <= darts; n++) {
      /* generate random numbers for x and y coordinates */
      r = (double)random()/cconst;
      x_coord = (2.0 * r) - 1.0;
      r = (double)random()/cconst;
      y_coord = (2.0 * r) - 1.0;

      /* if dart lands in circle, increment score */
      if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
         score++;
      }

   /* calculate pi */
   pi = 4.0 * (double)score/(double)darts;
   return(pi);
} 
