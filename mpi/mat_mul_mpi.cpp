/**********************************************************************                                                                                      
 * MPI-based matrix multiplication AxB=C                                                                                                                     
 *********************************************************************/


#include <stdio.h>
#include <random>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#include <fstream>

using namespace std;

#define N  2000         /* number of rows and columns in matrix */

MPI_Status status;

double a[N][N],b[N][N],c[N][N];
//double **a, **b, **c; 

float RandomDouble(double a, double b) {
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}


main(int argc, char **argv)
{ 

  ofstream myfile;
  myfile.open ("matmul_mpi_out.txt");
  srand(time(NULL));
  int numtasks, taskid, numworkers, source, dest, rows, offset, i, j, k;

  //struct timeval start, stop;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

  numworkers = numtasks - 1;

  /*---------------------------- master ----------------------------*/
  if (taskid == 0) {
    printf("mpi_mm has started with %d tasks.\n", numtasks);
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        a[i][j]= RandomDouble(1, 5);
        b[i][j]= RandomDouble(0, 1);
      }
    }

    //gettimeofday(&start, 0);

    /* send matrix data to the worker tasks */
    rows = N / numworkers;
    offset = 0;


    /* Measure start time */
    double start = MPI_Wtime();

    for (dest = 1; dest <= numworkers; dest++)
    {
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
      MPI_Send(&b, N*N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
      offset = offset + rows;
    }

    /* wait for results from all worker tasks */
    for (i = 1; i <= numworkers; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&c[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }

    //gettimeofday(&stop, 0);

    /*printf("Here is the result matrix:\n");
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++)
        printf("%6.2f   ", c[i][j]);
      printf ("\n");
    }*/
    myfile << "Here is the result matrix: " << endl;
    
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++)
        myfile << " " << c[i][j];
      myfile << endl;
    }
    myfile.close();

    /*fprintf(stdout,"Time = %.6f\n\n",
         (stop.tv_sec+stop.tv_usec*1e-6)-(start.tv_sec+start.tv_usec*1e-6));*/
      double finish = MPI_Wtime();
      printf("Done in %f seconds.\n", finish - start);

  }

  /*---------------------------- worker----------------------------*/
  if (taskid > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&a, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

    /* Matrix multiplication */
    for (k = 0; k < N; k++)
    {
      for (i = 0; i < rows; i++)
      {
        c[i][k] = 0.0;
        for (j = 0; j < N; j++)
        {
          c[i][k] = c[i][k] + a[i][j] * b[j][k];
        }
      }
    }


    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}