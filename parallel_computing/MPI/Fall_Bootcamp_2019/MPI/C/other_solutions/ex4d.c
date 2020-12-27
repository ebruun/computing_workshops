#include "mpi.h"
#include <stdio.h>
#include <math.h>
int main( int argc, char *argv[] )
{
    int n, myid, numprocs, i, j, tag;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;
    FILE *ifp;
    MPI_Status  Stat;

    n = 1;
    tag = 1;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    if (myid == 0) {
       ifp = fopen("nslices.in","r");
       fscanf(ifp,"%d",&n);
       fclose(ifp);
       for (j = 1; j < numprocs; j++) {
           MPI_Send(&n, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
       }
    }
    else {
       MPI_Recv(&n, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &Stat);
    }
    printf("number of intervals = %d\n",n);

    h   = 1.0 / (double) n;
    sum = 0.0;
    for (i = 1; i <= n; i++) {
        x = h * ((double)i - 0.5);
        sum += (4.0 / (1.0 + x*x));
    }
    mypi = h * sum;

    pi = mypi;
    printf("myid=%d  pi is approximately %.16f, Error is %.16f\n",
               myid, pi, fabs(pi - PI25DT));

    MPI_Finalize();
    return 0;
}