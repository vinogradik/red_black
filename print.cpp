#include <stdio.h>
int Print(int, double *);
int Print(int n, double *a)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf(" %lf ", a[i * (n + 1) + j]);
		}
		printf("| %lf\n", a[i * (n + 1) + n]);
	}
	printf("\n");
	return 0;
}
	
int Print1 (int, int[], int[], double*);
int Print1 (int n, int b1[], int b2[], double *a)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf(" %lf ", a[b1[i] * (n + 1) + b2[j]]);
		}
		printf("| %lf\n", a[b1[i] * (n + 1) + n]);
	}
	printf("\n");
	return 0;
}

int PrintS (double*, int); 
int PrintS (double *x, int n)
{
	const int m = 8;
	int i;
	for (i = 0; ((i < n) && (i < m)); i++)
	{
		printf("X%d: %lf\n", i + 1, x[i]);
	}
	return 0;

}