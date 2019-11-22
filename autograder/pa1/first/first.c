#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int isPrime(int x);


int main(int argc, char** argv)
{
	int size = 0,num1 = 0,index = 0;
	FILE *fp = fopen(argv[1],"r");
	if(fp == NULL)
		return 0;
	if(fscanf(fp, "%d\n", &num1)>0)
	{
		size = num1;
//		printf("%d\n", size);
	}	
	int *sizept = malloc(size*4);
	int arr[*sizept];
	while(fscanf(fp, "%d\n", &num1)>0)
	{	
		arr[index] = num1;
		index++;
	}
	int i = 0;
	for(i = 0;i<size;i++)
	{
		//printf("%d\n",arr[i]);
		if(isPrime(arr[i]) == 1)
		{
			printf("yes\n");
		}
		else
		{
			printf("no\n");
		}
	}
	free(sizept);
	fclose(fp);
	return 0;
}

int isPrime(int x)
{
	if(x==0)
	{
		return 1;
	}
	int i = 0;
	for(i = 2;i<x/2;i++)
	{
		if(x%i == 0)
		{
			return 0;
		}
	}
	return isPrime(x/10);
}
