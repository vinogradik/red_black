#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
double* Func (int*);
//double* Enter(int*, const char *);
int Print (int, double*);
int PrintS (double*, int);
double* Solve (int, double*);
int main (int argc, char **argv)
{
	int n, i, j, t, my_rank;//, p;
	double start, time;
	double* a;
	double* x;
	char s[10];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
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
                scanf("%s", &s);
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
        /*start = clock()
    //}
	/*x = Solve(n, a);
	//if (my_rank == 0)
	//{
	time = (double)(clock() - start) / CLOCKS_PER_SEC;*/
	PrintS(x, n);


	/*printf("\n%lf\n", time);

*/
    }
    MPI_Finalize();
	return 0;
}
