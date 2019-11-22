#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	char* temp;
	int count = 0;
	for(int i = 1;i<argc;i++)
	{
		temp = argv[i];
		while(temp[count] != '\0')
		{
			//printf("%c\t",temp[count]);
			count++;
		}
		printf("%c",temp[count-1]);
		count = 0;
	}
	printf("\n");
	return 0;
}
