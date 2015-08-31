#include <stdio.h>
#include <stdlib.h>

double Function(int, int);
double Function(int i, int j)
{
	return (1.0 / (i + j + 1));
}


int Func(int, double*);
int Func(int n, double *a)
{
	int i, j;
	double l;

	for (i = 0; i < n; i++)
	{
		l = 0;
		for (j = 0; j < n; j++)
		{
			a[i * (n + 1) + j] = Function(i, j);
			if ((j % 2) == 0)
			{
				l += a[i * (n + 1) + j];
			}

		}
		a[i * (n + 1) + n] = l;
	}
	return 0;
}

