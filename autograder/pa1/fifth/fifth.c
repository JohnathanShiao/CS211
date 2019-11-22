#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int** allocSquare(int dim)
{
	int** temp = malloc(dim * sizeof(int*));
	for(int i = 0;i<dim;i++)
	{
		temp[i] = malloc(dim*sizeof(int));
	}
	return temp;
	
}

void isMagic(int** sq, int dim)
{
	int sum = dim* ((pow(dim,2) +1)/2);
	int row=0,col = 0,diagR = 0,diagL = 0;	
	for(int i = 0;i<dim;i++)
	{
		for(int s = 0;s<dim;s++)
		{
			row += sq[i][s];
			col += sq[s][i];
		}
		if(row != sum||col != sum)
		{
			printf("not-magic");
			return;
		}	
		row = 0;
		col = 0;
	}
	for(int i = 0;i<dim;i++)
	{
		diagL += sq[i][i];
		diagR += sq[i][dim-i-1];	
	}
	if(diagL != sum || diagR != sum)
	{
		printf("not-magic");
		return;
	}
	printf("magic");
}
int main(int argc, char** argv)
{
	FILE* fp = fopen(argv[1],"r");
	if(fp == NULL)
		return 0;
	int dim,num;
	if(fscanf(fp,"%d",&dim)>0);
	int** sq = allocSquare(dim);
	for(int i = 0;i<dim;i++)
	{
		for(int s = 0;s<dim;s++)
		{
			if(fscanf(fp,"%d\t",&num)>0)
				sq[i][s] = num;
			//printf("%d\t",sq[i][s]);
		}
		//printf("\n");
	}
	isMagic(sq,dim);
	fclose(fp);
	for(int i = 0;i<dim;i++)
	{
		free(sq[i]);
	}
	free(sq);
	return 0;
}
