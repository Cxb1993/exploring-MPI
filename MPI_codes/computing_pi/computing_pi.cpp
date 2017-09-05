/*
 * In this code the value of pi is estimated by estimating the value of
 * the integral of:
 *
 *	4 / (1 + x^2)
 *
 * over the interval [0,1]. The numerical integration method used for
 * this is the composite trapezoidal rule, whose error scales as
 * the inverse square of N, the number of partitions.
 *
 * The code is parallelised using MPI. In particular, we use two
 * collective operations: MPI_Bcast and MPI_Reduce.
 *
 *	AUTHOR: Murray Cutforth
 *	DATE:   04/09/2017
 */

#include <cmath>
#include <iostream>
#include "mpi.h"

double f(double x)
{
	return 4.0 / (1.0 + x * x);
}

int main(int argc, char *argv[])
{
	/*
	 * Set up MPI environment
	 */
	int myid, numprocs;
	double t1, t2;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	/*
	 * Prompt user for number of intervals
	 */
	long long int N;
	if (myid == 0)
	{
		std::cout << "Please enter the number of partitions to use in the numerical integration: " << std::endl;
		std::cin >> N;
		t1 = MPI_Wtime();
	}

	/*
	 * Share the number of partitions with all processes
	 */
	MPI_Bcast(&N, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
	double dx = 1.0 / N;

	/*
	 * Split up the domain between processes
	 */
	long long int i_start = (myid * N) / numprocs;
	long long int i_end = ((myid + 1) * N) / numprocs;

	/*
	 * Apply trapezium rule to each subdomain
	 */
	double partialsum = 0.0;
	for (long long int i=i_start; i<i_end; i++)
	{
		double fL = f(i * dx);
		double fR = f((i + 1) * dx);
		partialsum += 0.5 * dx * (fL + fR);
	}

	/*
	 * Gather all partial sums back to root process
	 */
	double pi;
	MPI_Reduce(&partialsum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (myid == 0)
	{
		t2 = MPI_Wtime();
		std::cout.precision(17);
		std::cout << "Value of pi calculated as: " << pi;
		std::cout << " in " << t2 - t1 << "s." << std::endl;
	}

	MPI_Finalize();
	return 0;
}
