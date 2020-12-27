#include <stdio.h>
#include <math.h>
int main( int argc, char *argv[] )
{
	int n, myid, numprocs, i;
	double PI25DT = 3.141592653589793238462643;
	double mypi, pi, h, sum, x;
	FILE *ifp;

	ifp = fopen("ex4.in","r");
	fscanf(ifp,"%d",&n);
	fclose(ifp);
	printf("number of intervals = %d\n",n);

	h   = 1.0 / (double) n;
	sum = 0.0;

	for (i = 1; i <= n; i++) {
		x = h * ((double)i - 0.5);
		sum += (4.0 / (1.0 + x*x));
		}
		
	mypi = h * sum;

	pi = mypi;
	printf("pi is approximately %.16f, Error is %.16f\n",
		pi, fabs(pi - PI25DT));
	return 0;
}