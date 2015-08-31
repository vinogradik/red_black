#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>

int PrintS (double*, int);
int Func(int, double*);
int Print1 (int, int[], int[], double*);
int Print(int, double*);
int main( int argc, char **argv )
{
    //float floatData[10];
    //double doubleData[20];
    //MPI_Status status;

    int size, rank, i, left, left0, right, k, j, i1, j1, t;
    const double e = 1E-30;
    //double a[12] = {1, 5, 6, 13, 2, -2, 1, 3, 0, 1, 9, 10};
    double *a;
    char s[20];
    int n;
    int *b1,*b2;
    double *x;

	struct
	{
		double s;
       	int   loc;
	} l, L;




    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );


    if( size != 2 )
    {
        if( rank==0 )
        {
            printf("Error: two processes required instead of %d, abort\n", size);
        }
        MPI_Barrier( MPI_COMM_WORLD );
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER );
        return -1;
    }







    if (rank == 0)
    {
        printf("Enter the matrix A\nfrom file: 0\ngenerate: 1\n");
        if(scanf("%d", &t) == 0)
        {
            printf("\nError\n");
            return 1;
        }
    }

    MPI_Bcast(&t, 1, MPI_INT, 0, MPI_COMM_WORLD );

    if (t == 1)
    {
        if (rank == 0)
        {
            printf("Enter n: ");
            fflush(stdout);
            scanf("%d", &n);
            printf("\n");
        }
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD );
        a = (double*) malloc (n * (n + 1) * sizeof(double));
        //printf("%d %d\n", n, rank);
        //usleep(1000000);
        if (rank == 0)
        {
            Func(n, a);
        }

    }
    if (t == 0)
    {

        if (rank == 0)
        {
            printf("Enter file name:\n");
            fflush(stdout);
            scanf("%s", s);
            usleep(1000000);
            //printf("%s\n", s);
            fflush(stdout);
        }
        FILE *fin = fopen("in.txt", "r");
        if (rank == 0)
        {
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
        }
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD );
        a = (double*) malloc(n * (n + 1) * sizeof(double));
        if(rank == 0)
        {
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

        }
        fclose(fin);
    }


    MPI_Bcast(a, n * (n + 1), MPI_DOUBLE, 0, MPI_COMM_WORLD );




    x = (double*) malloc (n * sizeof(double));
	b1 = (int*) malloc (n * sizeof(int));
	b2 = (int*) malloc (n * sizeof(int));


    if( rank==0 )
    {
        for (i = 0; i < n; i++)
        {
            b1[i] = i;
            b2[i] = i;
        }
    }
    MPI_Bcast(b1, n, MPI_INT, 0, MPI_COMM_WORLD );
    MPI_Bcast(b2, n, MPI_INT, 0, MPI_COMM_WORLD );


    /*if (rank == 1)
    {
        for (i = 0; i < n; i++)
            printf("%d ", b1[i]);

    }
    printf("\n");*/

    for (k = 0; k < n; k++)
	{
        /*if (rank == 0)
        {
            Print1(n, b1, b2, a);
        }*/




        /*if (rank == 0)
        {
            printf("before change\n");
            Print1(n, b1, b2, a);
        }*/


        //1
        l.s = fabs(a[b1[k] *(n + 1) + b2[k]]);
		l.loc = k * (n + 1) + k;
		left = rank * (n - k) / size + k;
		if (rank == size - 1)
		{
			right = n;
		}
		else
		{
			right = (rank + 1) * (n - k) / size + k;
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
		MPI_Allreduce (&l, &L, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);
		if (rank == 0)
		{
			if ((L.s < e) && (L.s > -e))
			{
				printf("Det(A)= 0\n");
				return 1;
			}
            //printf("max %d, %lf\n", rank, L.s);
            j1 = L.loc % (n + 1);
            i1 = (L.loc - j1) / (n + 1);
            i = b1[k];
            b1[k] = b1[i1];
            b1[i1] = i;
            j = b2[k];
            b2[k] = b2[j1];
            b2[j1] = j;
        }
		MPI_Bcast(b1, n, MPI_INT, 0, MPI_COMM_WORLD );
        MPI_Bcast(b2, n, MPI_INT, 0, MPI_COMM_WORLD );








        //2
        /*printf("line 127 %d\n", rank);
        fflush(stdout);
        usleep(1000000);
*/


        left = rank * (n - k - 1) / size + k + 1;
		if (rank == size - 1)
		{
			right = n;
		}
		else
		{
			right = (rank + 1) * (n - k - 1) / size + k + 1;
		}
        if (rank == 0)
		{
            for (j = k + 1; j < n; j++)
            {
                a[b1[k] * (n + 1) + b2[j]] = a[b1[k] * (n + 1) + b2[j]] / a[b1[k] * (n + 1) + b2[k]];
            }
            a[b1[k] * (n + 1) + n] = a[b1[k] * (n + 1) + n] / a[b1[k] * (n + 1) + b2[k]];
            a[b1[k] * (n + 1) + b2[k]] = 1;
        }
        MPI_Bcast(a + b1[k] * (n + 1), n + 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
        /*if (rank == 0)
        {
            printf("after  half change\n");
            Print1(n, b1, b2, a);
        }
        printf("line 166 %d k = %d\n", rank, k);
        fflush(stdout);
        usleep(1000000);
*/

        left = rank * ((n - k - 1) / size) + k + 1;
        right = (rank + 1) * ((n - k - 1) / size) + k + 1;
        for (i = left; i < right; i++)
        {
            /*printf("left %d right %d rank %d circle %d\n", left, right, rank, k);
            //printf("line 183 rank %d i = %d\n", rank, i);
            fflush(stdout);
            usleep(1000000);*/
            for (j = k + 1; j < n; j++)
            {
                a[b1[i] * (n + 1) + b2[j]] -= a[b1[i] * (n + 1) + b2[k]] * a[b1[k] * (n + 1) + b2[j]];
            }
            a[b1[i] * (n + 1) + n] -= a[b1[i] * (n + 1) + b2[k]] * a[b1[k] * (n + 1) + n];
            a[b1[i] * (n + 1) + b2[k]] = 0;
            for (j = 0; j < size; j++)
            {
                left0 = j * (n - k - 1) / size + k + 1;
                MPI_Bcast(a + b1[i - left + left0] * (n + 1), n + 1, MPI_DOUBLE, j, MPI_COMM_WORLD);
                /*printf("%d, %d\n", j, rank);
                usleep(1000000);
                if (rank == 0)
                {
                    printf("%d\n", n - k - 1);
                    Print1(n, b1, b2, a);
                }
                usleep(1000000);
                if (rank == 1)
                {
                    printf("1 process\n");
                    Print1(n, b1, b2, a);
                }
                usleep(1000000);*/
            }
        }
        if (rank == 0)
        {
            for (i = ((n - k - 1) / size) * size + k + 1; i < n; i++)
            {
                for (j = k + 1; j < n; j++)
                {
                    a[b1[i] * (n + 1) + b2[j]] -= a[b1[i] * (n + 1) + b2[k]] * a[b1[k] * (n + 1) + b2[j]];
                }
                a[b1[i] * (n + 1) + n] -= a[b1[i] * (n + 1) + b2[k]] * a[b1[k] * (n + 1) + n];
                a[b1[i] * (n + 1) + b2[k]] = 0;
            }
        }
        for (i = ((n - k - 1) / size) * size + k + 1; i < n; i++)
        {
            MPI_Bcast(a + b1[i] * (n + 1), n + 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
        }

        //printf("line 188 %d\n", rank);
        /*fflush(stdout);
        usleep(1000000);

*/
        /*if (rank == 0)
        {
            printf("after change\n");
            Print1(n, b1, b2, a);
        }*/
        /*if (rank == 0)
        {
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
		MPI_Bcast(a, n * (n + 1), MPI_DOUBLE, 0, MPI_COMM_WORLD );*/

        //printf("номер итерации %d\n", k);
    }

















    if (rank == 0)
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





     MPI_Finalize();
    return 0;
}


