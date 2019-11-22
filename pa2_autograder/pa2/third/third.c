#include <stdio.h>
#include <stdlib.h>

typedef struct matrix
{
	double** value;
	int row;
	int col;
}matrix;

double** allocMatrix(int row, int col)
{
	double** temp = malloc(row * sizeof(double*));
	for(int i = 0;i<row;i++)
		temp[i] = malloc(col * sizeof(double));
	return temp;
}

matrix* transpose(matrix* x)
{
	double** temp = allocMatrix(x->col,x->row);
	for(int i = 0;i<x->col;i++)
	{
		for(int s = 0;s<x->row;s++)
		{
			temp[i][s] = x->value[s][i];
		}
	}
	matrix* result = malloc(sizeof(matrix));
	result->value = temp;
	result->row = x->col;
	result->col = x->row;
	return result;
}

matrix* multiply(matrix* a, matrix* b)
{
	if(a->col != b->row)
		return NULL;
	else
	{
		double** temp = allocMatrix(a->row,b->col);
		matrix* x = malloc(sizeof(matrix));
		x->row = a->row;
		x->col = b->col;
		double result = 0;
		for(int i = 0;i<a->row;i++)
		{
			for(int s= 0;s<b->col;s++)
			{
				for(int d = 0;d<a->col;d++)
				{
					result+= a->value[i][d] * b->value[d][s];
				}
				temp[i][s] = result;
				result = 0;
			}
		}
		x->value = temp;
		return x;
	}
}
matrix* invert(matrix* x)
{
	double** aug = allocMatrix(x->row,(x->col)*2);
	for(int i = 0;i<x->row;i++)
	{
		for(int s = 0;s<x->col;s++)
		{
			aug[i][s] = x->value[i][s];
		}
		for(int f = x->col;f<(x->col)*2;f++)
		{
			if(f - x->col == i)
				aug[i][f] = 1;
			else
				aug[i][f] = 0;
		}
	} 
	for(int i = 0;i<x->row;i++)
	{
		if(aug[i][i]!= 1)
		{
			double div= aug[i][i];
			for(int s = 0;s<(x->col)*2;s++)
			{
				aug[i][s] = aug[i][s]/div;
			}
		}
		for(int f = 0;f<x->row;f++)
		{
			if(f!=i)
			{
				double mult = aug[f][i];
				for(int d = 0;d<(x->col)*2;d++)
					aug[f][d] = aug[f][d]- aug[i][d]*mult;
			}
		}
	}
	double** result = allocMatrix(x->row,x->col);
	for(int i = 0;i<x->row;i++)
	{
		for(int s = x->col;s<(x->col)*2;s++)
		{
			result[i][s-x->row] = aug[i][s];
		}
	}
	matrix* res = malloc(sizeof(matrix));
	res->value = result;
	res->row = x->row;
	res->col = x->col;
	return res;
}
void freeMatrix(matrix* x)
{
	for(int i = 0;i<x->row;i++)
		free(x->value[i]);
	free(x->value);
	free(x);
}
void printMatrix(matrix* x)
{
	for(int i = 0;i<x->row;i++)
	{
		for(int s = 0 ;s<x->col;s++)
		{
			printf("%0.0lf\t",x->value[i][s]);
		}
		printf("\n");
	}
}
int main(int argc,char** argv)
{
	FILE* file = fopen(argv[1],"r");
	if(file == NULL)
		return 0;
	int row, col;
	if(fscanf(file,"%d\n",&col)>0);
	if(fscanf(file,"%d\n",&row)>0);
	double** train = allocMatrix(row,col+1);
	double** trainprice = allocMatrix(row,1);
	double temp;
	for(int i = 0;i<row;i++)
	{
		train[i][0] = 1;
		for(int s = 1;s<col+2;s++)
		{
			if(s == col+1)
			{
				if(fscanf(file,"%lf,",&temp)>0)
					trainprice[i][0] = temp;
			}
			else
			{
				if(fscanf(file,"%lf,",&temp)>0)
					train[i][s] = temp;
			}
		}
	}
	matrix* x= malloc(sizeof(matrix));
	x->value=train;
	x->row=row;
	x->col=col+1;
	matrix* y=malloc(sizeof(matrix));
	y->value=trainprice;
	y->row=row;
	y->col=1;
	matrix* transposed = transpose(x);
	matrix* multxx = multiply(transposed,x);
	matrix* inversed = invert(multxx);
	matrix* multix = multiply(inversed,transposed);
	matrix* w = multiply(multix,y);
	freeMatrix(x);
	freeMatrix(y);
	freeMatrix(transposed);
	freeMatrix(multxx);
	freeMatrix(inversed);
	freeMatrix(multix);
	file = fopen(argv[2],"r");
	if(file == NULL)
		return 0;
	if(fscanf(file,"%d\n",&row)>0);
	double** test = allocMatrix(row,col+1);
	for(int i = 0;i<row;i++)
	{
		test[i][0] = 1;
		for(int s= 1;s<col+1;s++)
		{
			if(fscanf(file,"%lf,",&temp)>0)
				test[i][s] = temp;
		}
	}
	matrix* tested = malloc(sizeof(matrix));
	tested->value=test;
	tested->row = row;
	tested->col = col+1;
	matrix* final = multiply(tested,w);
	printMatrix(final);
	freeMatrix(w);
	freeMatrix(tested);
	freeMatrix(final);
	fclose(file);
	return 0;
}
