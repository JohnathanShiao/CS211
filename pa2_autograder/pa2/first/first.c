#include <stdlib.h>
#include <stdio.h>

int skip(char** x, int* row, int* col)
{
	for(int i = *row;i<16;i++)
	{
		for(int s= *col+1 ; s<16;s++)
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
	int* result = calloc(sizeof(int),16);
	char temp; 
	for(int i = 0;i<16;i++)
	{
		if(x[row][i] != '-')
		{
			if(x[row][i] >= '0' && x[row][i] <= '9')
			{
				temp = x[row][i];
				result[temp-'0'] +=1;
			}
			else
			{
				temp = x[row][i];
				result[temp-'7'] += 1;
			}	
		}
		if(x[i][col] != '-')
		{
			if(x[i][col] >= '0' && x[i][col] <= '9')
			{
				temp = x[i][col];
				result[temp-'0']+=1;
			}
			else
			{
				temp=x[i][col];
				result[temp-'7']+=1;
			}
		}
	}
	int rowstart = row-row%4;
	int colstart = col-col%4;
	for(int i = 0;i<4;i++)
	{
		for(int s = 0;s<4;s++)
		{
			if(x[i+rowstart][s+colstart] != '-')
			{
				if(x[i+rowstart][s+colstart] >= '0' && x[i+rowstart][s+colstart] <= '9')
				{
					temp = x[i+rowstart][s+colstart];
					result[temp-'0']+=1;
				}
				else
				{
					temp=x[i+rowstart][s+colstart];
					result[temp-'7']+=1;
				}
			}
		}
	}
	for(int i = 0;i<16;i++)
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
	for(int i = 0;i<16;i++)
	{
		if(res[i] == 0)
			count++;
	}
	if(count>1)
	{
		free(res);
		return '@';
	}
	for(int i = 0;i<16;i++)
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
	for(int i = 0;i<16;i++)
	{
		for(int s = 0;s<16;s++)
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
	for(int i = 0;i<16;i++)
	{
		for(int s = 0;s<16;s++)
		{
			printf("%c\t",x[i][s]);
		}
		printf("\n");
	}
}

void freedoku(char** x)
{
	for(int i = 0;i<16;i++)
		free(x[i]);
	free(x);
}
int main(int argc,char**argv)
{
	FILE* fp = fopen(argv[1],"r");
	if(fp == NULL)
		return 0;
	char temp;
	char** doku = malloc(16 * sizeof(char*));
	for(int i = 0;i<16;i++)
	{
		doku[i] = malloc(16*sizeof(char));
		for(int s = 0;s<16;s++)
		{
			if(fscanf(fp,"%c\t",&temp)>0)
				doku[i][s] = temp;
			//printf("%c\t",doku[i][s]);	
		}
		//printf("\n");
	}
	//checking for valid input doku
	for(int i = 0;i<16;i++)
	{
		for(int s= 0;s<16;s++)
		{
			if(check(doku,i,s) == NULL)
			{
				printf("no-solution");
				return 0;
			}	
		}
	}
	int row,col;
	while(next(doku,&row,&col))
	{
		if(fill(check(doku,row,col)) == '-')
		{
			printf("no-solution");
			return 0;
		}
		while(fill(check(doku,row,col))=='@')
		{
			if(!skip(doku,&row,&col))
			{
				printf("no-solution");
				return 0;
			}
			skip(doku,&row,&col);
		}
		doku[row][col] = fill(check(doku,row,col));
	}
	printdoku(doku);
	freedoku(doku);
	fclose(fp);
	return 0;
}
