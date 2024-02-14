/***************************************************************************
 * FILE: ser_pi_calc.c
 * DESCRIPTION:  
 *   Serial pi Calculation - C Version
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  
 * AUTHOR: unknown
 * REVISED: 02/23/12 Blaise Barney
***************************************************************************/
// #include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>


using namespace std;


void srandom (unsigned seed);  
double dboard (int darts);

#define DARTS 10000   	/* number of throws at dartboard */
#define ROUNDS 100   	/* number of times "darts" is iterated */

int main(int argc, char *argv[])
{
double pi;          	/* average of pi after "darts" is thrown */
double avepi;       	/* average pi value for all iterations */
int i, n;
double start_time, end_time, elapsed_time;

int numtasks, rank;

start_time = MPI_Wtime();
   
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);



srandom (rank);            /* seed the random number generator */
avepi = 0;
for (i = 0; i < ROUNDS; i++) {
   /* Perform pi calculation on serial processor */
   pi = dboard(DARTS);
   avepi = ((avepi * i) + pi)/(i + 1); 
   // printf("   After %3d throws, average value of pi = %10.8f\n",
   //       (DARTS * (i + 1)),avepi);
    }    
// printf("\nReal value of PI: 3.1415926535897 \n");


//Compute the average of the results over all processes
double globalmean, globalsum;
int dest = 0;
MPI_Reduce(&avepi,&globalsum,1,MPI_DOUBLE,MPI_SUM,dest,MPI_COMM_WORLD);
cout << "Average Value of PI on Process " << rank << " is " << avepi<< endl;

MPI_Finalize();

end_time = MPI_Wtime();

elapsed_time = end_time - start_time;

//Output results only once 
if (rank == dest) {
   globalmean = globalsum / numtasks;
   cout << "The average value of PI using " << numtasks << " Processors is " << globalmean <<  endl;
   cout << "Time elapsed " << elapsed_time <<endl;
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
