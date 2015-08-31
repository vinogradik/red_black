#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double* Solve (int, double*, int, char**);
int Print (int, double*);
int Print1 (int, int[], int[], double*);


double* Solve(const int n, double* a)
{
	/*struct {
		double s;
        	int   loc;
	} fd, L;*/
	double l;
	int i, j, i1, j1, k; //my_rank, p = 2, left, right;
	const double e = 1E-10;
	double *x;
	int *b1, *b2;
	x = (double*) malloc (n * sizeof(double));
	b1 = (int*) malloc (n * sizeof(int));
	b2 = (int*) malloc (n * sizeof(int));

	for (i = 0; i < n; i++)
	{
		b1[i] = i;
		b2[i] = i;
	}
	/*MPI_Init(&argc, &argv);
	MPI_Finalize();
	MPI_Init (argc, argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
	*/for (k = 0; k < n; k++)
	{
        l = fabs(a[b1[k] *(n + 1) + b2[k]]);
		i1 = k;
		j1 = k;
		for (i = k; i < n; i++)
		{
			for (j = k; j < n; j++)
			{
				if (fabs(a[b1[i] * (n + 1) + b2[j]]) > l)
				{
					l = fabs(a[b1[i] * (n + 1) + b2[j]]);
					i1 = i;
					j1 = j;
				}
			}
		}
		if ((l < e) && (l > -e))
		{
			printf("Det(A)= 0\n");
			exit(1);
		}
		i = b1[k];
		//printf("%lf\n", l);
		b1[k] = b1[i1];
		b1[i1] = i;
		j = b2[k];
		b2[k] = b2[j1];
		b2[j1] = j;
		/*l.s = fabs(a[b1[k] *(n + 1) + b2[k]]);
		l.loc = k * (n + 1) + k;
		left = my_rank * (n - k) / p + k;
		if (my_rank == p - 1)
		{
			right = n;
		}
		else
		{
			right = (my_rank + 1) * (n - k) / p + k;
		}
		for (i = left; i < right; i++)
		{
			for (j = k; j < n; j++)
			{
				if (fabs(a[b1[i] * (n + 1) + b2[j]]) > l.s)
				{
					l.s = fabs(a[b1[i] * (n + 1) + b2[j]]);
					l.loc = i * (n + 1) + j;									}
			}
		}
		MPI_Allreduce (&l, &L, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);
		if (my_rank == 0)
		{
			if ((L.s < e) && (L.s > -e))
			{
				printf("Det(A)= 0\n");
				exit(1);
			}
		}
		MPI_Finalize ();
		j1 = L.loc % n;
		i1 = (L.loc - j1) / n;
		i = b1[k];
		b1[k] = b1[i1];
		b1[i1] = i;
		j = b2[k];
		b2[k] = b2[j1];
		b2[j1] = j;

		left = my_rank * (n - k - 1) / p + k + 1;*/
		/*if (my_rank == p - 1)
		{
			right = n;
		}
		else
		{
			right = (my_rank + 1) * (n - k - 1) / p + k + 1;
		}
		if (my_rank == 0)
		{*/
		for (j = k + 1; j < n; j++)
		{
			a[b1[k] * (n + 1) + b2[j]] = a[b1[k] * (n + 1) + b2[j]] / a[b1[k] * (n + 1) + b2[k]];
		}


		a[b1[k] * (n + 1) + n] = a[b1[k] * (n + 1) + n] / a[b1[k] * (n + 1) + b2[k]];
		a[b1[k] * (n + 1) + b2[k]] = 1;
		for (i = k + 1; i < n; i++)
		{
			for (j = k + 1; j < n; j++)
			{
				a[b1[i] * (n + 1) + b2[j]] -= a[b1[i] * (n + 1) + b2[k]] * a[b1[k] * (n + 1) + b2[j]];
			}
			a[b1[i] * (n + 1) + n] -= a[b1[i] * (n + 1) + b2[k]] * a[b1[k] * (n + 1) + n];
			a[b1[i] * (n + 1) + b2[k]] = 0;

		}


	}

	//Обратный Шаг
	for (k = n - 1; k >= 0; k--)
	{
		x[b2[k]] = 0;
		for (i = k + 1; i < n; i++)
		{
			x[b2[k]] += a[b1[k] * (n + 1) + b2[i]] * x[b2[i]];
		}
		x[b2[k]] = a[b1[k] * (n + 1) + n] - x[b2[k]];
	}

	return x;
}

