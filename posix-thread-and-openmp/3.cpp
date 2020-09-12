#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

using namespace std;

struct timeval start_time, stop_time;


#define MAX  1000000000

int    num_threads;
int    start[100];
double x[MAX];
double Tmin[100];
pthread_t tid[100];




/* --------------------------------------
   Find the min. among my number

   The min is stored in Tmin[s]
   -------------------------------------- */
void *worker(void *arg)
{
   int i, s;
   int n, start, stop;
   double my_min;

   n = MAX/num_threads;

   s = * (int *) arg;    // s can be one of 0, 1, 2, ..., (num_threads-1)

   start = s * n;

   if ( s != (num_threads-1) )
   {
      stop = start + n;
   }
   else
   {
      stop = MAX;
   }

   my_min = x[start];

   for (i = start+1; i < stop; i++ )
   {
//    cout << i << endl;

      if ( x[i] < my_min )
         my_min = x[i];
   }
  
   Tmin[s] = my_min;
   return(NULL);     /* Thread exits (dies) */
}


int main(int argc, char *argv[])
{
   int i;
   double my_min;
   int elapsed;

   /* -----
      Check command line
      ----- */
   if ( argc != 2 )
   {
      cout << "Usage: " << argv[0] << " Num_Threads" << endl;
      exit(1);
   }

   /* -------------------------
      Generate random number
      ------------------------- */
   for (i = 0; i < MAX; i++)
      x[i] = random()/(double)1147483648;

   num_threads = atoi(argv[1]);
   cout << "Using" << num_threads << " threads..." << endl << endl;

   gettimeofday(&start_time, NULL);

   /* ------
      Create threads
      ------ */
   for (i = 0; i < num_threads; i = i + 1)
   {
      start[i] = i;
      if ( pthread_create(&tid[i], NULL, worker, (void *)&start[i]) )
      {
         cout << "Cannot create thread" << endl;
         exit(1);
      }
   }

   for (i = 0; i < num_threads; i = i + 1)
      pthread_join(tid[i], NULL);

   my_min = Tmin[0];
   for (i = 1; i < num_threads; i++)
      if ( Tmin[i] < my_min )
	 my_min = Tmin[i];

   gettimeofday(&stop_time, NULL);

   elapsed = (stop_time.tv_sec*1000000 + stop_time.tv_usec) -
   		(start_time.tv_sec*1000000 + start_time.tv_usec);

   cout << "min = " << my_min << endl << endl; 
   cout << "Elapsed time = " << elapsed << " microseconds" << endl;
}
