#include <stdlib.h>
#include <stdio.h>

typedef struct matrices
{
	int** values;
	int row;
	int col;
}matrix;
int** allocMatrix(int row, int col)
{
	int** x = malloc(row * sizeof(int*));
	for(int i = 0;i<row;i++)
		x[i] = malloc(col*sizeof(int));
	return x;
}

void freeMatrix(int** matrix,int row)
{
	for(int i = 0;i<row;i++)
		free(matrix[i]);
	free(matrix);
}

void multiply(matrix* a, matrix* b)
{
	if(a->col!=b->row)
	{
		printf("bad-matrices");
		return;
	}
	else
	{
		int** v1 = a->values;
		int** v2 = b->values;
		int result=0;
		for(int i = 0;i<a->row;i++)
		{
			for(int s = 0;s<b->col;s++)
			{
				for(int d = 0;d<a->col;d++)
				{
					result += v1[i][d] * v2[d][s];
				}
				printf("%d\t", result);
				result = 0;
			}
			printf("\n");
		}
	}
	freeMatrix(a->values,a->row);
	freeMatrix(b->values,b->row);
}


int main(int argc, char** argv)
{
	FILE* fp = fopen(argv[1],"r");
	if(fp == NULL)
		return 0;
	int row, col;
	matrix* a = malloc(sizeof(matrix));
	matrix* b = malloc(sizeof(matrix));
	for(int g = 0;g<2;g++)
	{
		if(fscanf(fp,"%d\t%d",&row,&col)>0);
		int** temp = allocMatrix(row,col);
		int num;
		for(int i = 0;i<row;i++)
		{
			for(int s = 0;s<col;s++)
			{
				if(fscanf(fp,"%d\t",&num)>0)
				{
					temp[i][s] = num;
					//printf("%d\t", temp[i][s]);
				}
			}
			//printf("\n");
		}
		if(g == 0)
		{
			a->values = temp;
			a->row = row;
			a->col = col;
		}
		else
		{
			b->values = temp;
			b->row = row;
			b->col = col;
		}
		//freeMatrix(temp,row);
	}
	multiply(a,b);
	/*int **val = a->values;
	for(int i = 0;i<a->row;i++)
	{
		for(int s = 0;s<a->col;s++)
		{
			printf("%d\t",val[i][s]);
		}
		printf("\n");
	}
	printf("\n");
	val = b->values;
	for(int i = 0;i<b->row;i++)
	{
		for(int s = 0;s<b->col;s++)
		{
			printf("%d\t",val[i][s]);
		}
		printf("\n");
	}*/
	fclose(fp);
	return 0;
}
