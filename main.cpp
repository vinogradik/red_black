#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>
double* Func (int*);
//double* Enter(int*, const char *);
int Print (int, double*);
int PrintS (double*, int);
int Print1 (int, int[], int[], double*);
//double* Solve (int, double*);
int main (int argc, char **argv)
{
	int n, i, j, t, my_rank, p;
	//double start, time;
	double* a;
	double* x;
	char s[10];
	struct {
		double s;
        	int   loc;
	} l, L;
    int i1, j1, k, left, right;
	const double e = 1E-10;
    int *b1, *b2;
	x = (double*) malloc (n * sizeof(double));
	b1 = (int*) malloc (n * sizeof(int));
	b2 = (int*) malloc (n * sizeof(int));

	MPI_Init(&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	if (my_rank == 0)
    {
        printf("Enter the matrix A\nfrom file: 0\ngenerate: 1\n");
        if(scanf("%d", &t) == 0)
        {
            printf("\nError\n");
            return 1;
        }
        switch (t)
        {
        case 0:
            {
                printf("Enter file name:\n");
                scanf("%s", s);
                fflush(stdout);
                //a = Enter(&n, &s);
                FILE *fin = fopen(s, "r");
                if (fin == NULL)
                {
                    printf("\nError opening file\n");
                    return 1;
                }

                if (fscanf(fin, "%d", &n) != 1)
                {
                    printf("\nError reading file\n");
                    return 1;
                }
                a = (double*) malloc(n * (n + 1) * sizeof(double));
                if (a == NULL)
                {
                    printf ("\nError1\n");
                    return 1;
                }


                for (i = 0; i < n; i++)
                {
                    for (j = 0; j < n + 1; j++)
                    {
                        if (fscanf(fin, "%lf", &a[i * (n + 1) + j]) != 1)
                        {
                            printf("\nError reading file\n");
                            return 1;
                        }
                    }
                }
                Print(n, a);
                fclose(fin);
                break;
            }
            case 1:
            {
                printf("Enter n: ");
                scanf("%d", &n);
                printf("\n");
                a = Func(&n);
                break;
            }
        default:
            {
                printf("\nError\n");
                return 1;
            }

        }
    }

//начало solve

	//if (my_rank == 0)
	//{
        for (i = 0; i < n; i++)
        {
            b1[i] = i;
            b2[i] = i;
        }
   // }
    k = 0;
	for (k = 0; k < n; k++)
	{
        MPI_Allreduce (&b1, &b1, n, MPI_INT, MPI_MAXLOC, MPI_COMM_WORLD);
        //MPI_Bcast(b2, n, MPI_INT, 0, MPI_COMM_WORLD);
       /* l.s = fabs(a[b1[k] *(n + 1) + b2[k]]);
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
					l.loc = i * (n + 1) + j;
				}
			}
		}
		printf("half %d, %lf, left: %d, right: %d\n", my_rank, l.s, left, right);
		//fflush(stdout);
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Allreduce (&l, &L, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);
		if (my_rank == 0)
		{
			if ((L.s < e) && (L.s > -e))
			{
				printf("Det(A)= 0\n");
				return 1;
			}
        }
        printf("max %d, %lf\n", my_rank, L.s);*/
       /* j1 = L.loc % n;
        i1 = (L.loc - j1) / n;
		i = b1[k];
		b1[k] = b1[i1];
		b1[i1] = i;
		j = b2[k];
		b2[k] = b2[j1];
		b2[j1] = j;
        if (my_rank == 0) Print1(n, b1, b2, a);
        if (my_rank == 0) printf("%dhehe\n", k);*/
		/*left = my_rank * (n - k - 1) / p + k + 1;
		if (my_rank == p - 1)
		{
			right = n;
		}
		else
		{
			right = (my_rank + 1) * (n - k - 1) / p + k + 1;
		}
		if (my_rank == 0)
		{*/
		/*if (my_rank == -1)
		{
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

		}*/

   // }

	/*
	if (my_rank == -1)
	{
        for (k = n - 1; k >= 0; k--)
        {
            x[b2[k]] = 0;
            for (i = k + 1; i < n; i++)
            {
                x[b2[k]] += a[b1[k] * (n + 1) + b2[i]] * x[b2[i]];
            }
            x[b2[k]] = a[b1[k] * (n + 1) + n] - x[b2[k]];
        }

        PrintS(x, n);
    }
*/


    MPI_Finalize();
	return 0;
}

