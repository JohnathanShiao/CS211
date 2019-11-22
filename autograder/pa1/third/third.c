#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int value;
	struct node* next;
}node;

int main(int argc, char** argv)
{
	FILE* fp = fopen(argv[1],"r");
	if(fp == NULL)
		return 0;
	char op;
	int num, hash;
	node** table = malloc(1000*sizeof(node*));
	for(int i = 0;i<1000;i++)
	{
			node* n1 = malloc(1*sizeof(node));
			n1->next = NULL;		
			table[i] = n1;
		}
		node* ptr;
		while(fscanf(fp,"%c\t%d",&op,&num)>0)
		{
			if(op == 'i')
			{
				//printf("%c\t%d\n",op,num);
				if(num<0)
				{
					num*=-1;
					hash = num%1000;
					num*=-1;
				}
				else
					hash = num%1000;
				ptr = table[hash];
				while(ptr->next != NULL)
				{
					if(ptr->value == num)
					{

						printf("duplicate\n");
						break;
					}
					ptr = ptr->next;
				}
				if(ptr->next == NULL)
				{
					if(ptr->value == num)
						printf("duplicate\n");
					else
					{
						node* n1 = malloc(1*sizeof(node));
						n1->value = num;
						n1->next = NULL;
						ptr->next = n1;
						printf("inserted\n");
					}
				}	
			}
			else if(op == 's')
			{
				if(num<0)
				{
					num*=-1;
					hash = num%1000;
					num*=-1;
				}
				else
					hash = num%1000;
				ptr = table[hash];
				ptr = ptr->next;
				while(ptr != NULL)
				{
					if(ptr->value == num)
					{
						printf("present\n");
						break;
					}
					ptr = ptr->next;
				}
				if(ptr == NULL)printf("absent\n");
			}
	}
	/*for(int i = 0;i<6;i++)
	{
		ptr = table[i];
		while(ptr != NULL)
		{
			printf("%d\t",ptr->value);
			ptr = ptr->next;
		}
		printf("\n");
	}*/
	fclose(fp);
	for(int i = 0;i<1000;i++)
	{
		node* temp = table[i];
		while(temp != NULL)
		{
			node* ptr = temp;
			temp = temp->next;
			free(ptr);
		}
	}
	free(table);
	return 0;
}
