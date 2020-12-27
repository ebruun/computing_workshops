#include "mpi.h"
#include <stdio.h>
#include <math.h>
int main( int argc, char *argv[] )
{
    int n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;
    FILE *ifp;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    n=0;

    if (myid == 0) {
       ifp = fopen("nslices.in","r");
       fscanf(ifp,"%d",&n);
       fclose(ifp);
    }
 /* Global communication. Process 0 "broadcasts" n to all other processes */
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("myid=%d  number of intervals = %d\n",myid,n);

    h   = 1.0 / (double) n;
    sum = 0.0;
 /* Split the loop between all the MPI tasks */
    for (i = myid*n/numprocs+1; i <= (myid+1)*n/numprocs; i++) {
        x = h * ((double)i - 0.5);
        sum += (4.0 / (1.0 + x*x));
    }
    mypi = h * sum;


 /* Global reduction. All processes send their value of mypi to each other
    and they all add them up locally (MPI_SUM) */
    MPI_Allreduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);


    printf("myid=%d  pi is approximately %.16f, Error is %.16f\n",
               myid, pi, fabs(pi - PI25DT));

    MPI_Finalize();
    return 0;
}
