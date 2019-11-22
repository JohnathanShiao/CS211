#include <stdlib.h>
#include <stdio.h>

int skip(char** x, int* row, int* col)
{
	for(int i = *row;i<9;i++)
	{
		for(int s= *col+1 ; s<9;s++)
		{
			if(x[i][s] == '-')
			{
				*row=i;
				*col=s;
				return 1;
			}
		}
	}
	return 0;
}
int* check(char** x, int row, int col)
{
	int* result = calloc(sizeof(int),9);
	char temp; 
	for(int i = 0;i<9;i++)
	{
		if(x[row][i] != '-')
		{
            temp = x[row][i];
            result[temp-'0'-1] +=1;
		}
		if(x[i][col] != '-')
		{
            temp = x[i][col];
			result[temp-'0'-1] +=1;
		}
	}
	int rowstart = row-row%3;
	int colstart = col-col%3;
	for(int i = 0;i<3;i++)
	{
		for(int s = 0;s<3;s++)
		{
			if(x[i+rowstart][s+colstart] != '-')
			{
				temp = x[i+rowstart][s+colstart];
                result[temp-'0'-1]+=1;
			}
		}
	}
	for(int i = 0;i<9;i++)
	{
		if(result[i]>3)
			return NULL;
	}
	return result;
}

char fill(int* res)
{
	char temp = '-';
	int count = 0;
	for(int i = 0;i<9;i++)
	{
		if(res[i] == 0)
			count++;
	}
	if(count>1)
	{
		free(res);
		return '@';
	}
	for(int i = 0;i<9;i++)
	{
		if(res[i] == 0)
		{
			if(i<10)
			{
				temp = i + '0';
				free(res);
				return temp;
			}
			else
			{
				temp = i + '7';
				free(res);
				return temp;
			}
		}
	}
	free(res);
	return temp;
}

int next(char** x, int* row, int* col)
{
	for(int i = 0;i<9;i++)
	{
		for(int s = 0;s<9;s++)
		{
			if(x[i][s] == '-')
			{
				*row = i;
				*col = s;
				return 1;
			}
		}
	}
	return 0;
}

void printdoku(char** x)
{
	for(int i = 0;i<9;i++)
	{
		for(int s = 0;s<9;s++)
		{
			printf("%c\t",x[i][s]);
		}
		printf("\n");
	}
}

void freedoku(char** x)
{
	for(int i = 0;i<9;i++)
		free(x[i]);
	free(x);
}

int solve(char** x)
{
	int row,col;
	//char temp = '-';
	if(!next(x,&row,&col))
		return 1;
	int* arr = check(x,row,col);
	for(int i = 0;i<9;i++)
	{
		if(arr[i] == 0)
		{
            x[row][col] = i+1+ '0';
			if(solve(x))
				return 1;
			x[row][col] = '-';
		}
	}
	return 0;
}
int main(int argc,char**argv)
{
	FILE* fp = fopen(argv[1],"r");
	if(fp == NULL)
		return 0;
	char temp;
	char** doku = malloc(9 * sizeof(char*));
	for(int i = 0;i<9;i++)
	{
		doku[i] = malloc(9*sizeof(char));
		for(int s = 0;s<9;s++)
		{
			if(fscanf(fp,"%c\t",&temp)>0)
				doku[i][s] = temp;
			//printf("%c\t",doku[i][s]);	
		}
		//printf("\n");
	}
	//checking for valid input doku
	// for(int i = 0;i<9;i++)
	// {
	// 	for(int s= 0;s<9;s++)
	// 	{
	// 		if(check(doku,i,s) == NULL)
	// 		{
	// 			printf("no-solution");
	// 			return 0;
	// 		}	
	// 	}
	// }
	if(solve(doku))
		printdoku(doku);
	else
		printf("no-solution");
	freedoku(doku);
	fclose(fp);
	return 0;
}
