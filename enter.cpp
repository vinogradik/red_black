#include <stdio.h>
#include <stdlib.h>
int Enter(int, const char *, double*);
int Enter(int n, const char *s, double* a)
{
	int i, j, m;
	FILE *fin = fopen(s, "r");
	if (fin == NULL)
	{
		printf("\nError opening file\n");
		exit(1);
	}

	if (fscanf(fin, "%d", &m) != 1)
	{
		printf("\nError reading file\n");
		exit(1);
	}



	for (i = 0; i < m; i++)
	{
		for (j = 0; j < m + 1; j++)
		{
			if (fscanf(fin, "%lf", &a[i * (m + 1) + j]) != 1)
			{
				printf("\nError reading file\n");
				exit(1);
			}
		}
	}
	*n = m;
	fclose(fin);
	return 0;
}

