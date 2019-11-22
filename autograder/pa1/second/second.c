#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
	int num;
	struct node* next;
} node;

int main(int argc, char** argv)
{
	int num1 = 0, size = 0,repeat = 0;
	char op;
	FILE *fp = fopen(argv[1], "r");
	node* head = malloc(1*sizeof(node));
	node* ptr = head;
	head->num = -2147483648;
	head->next = NULL;
	if(fp == NULL)
	{
		printf("error");
		return 0;
	}
	/*else
	{
		fseek(fp, 0, EOF);
		empty = ftell(fp);
		if(empty == 0)
		{
			printf("0\n");
			return 0;
		}
	}*/
	
	while(fscanf(fp, "%c\t%d\n", &op, &num1)>0)
	{
		ptr = head;
		if(op == 'i')
		{
			if(head->num == num1)
			{
				repeat = 1;
				size++;
				//printf("there has been a repeat");
			}	
			else
			{
				while(ptr->num < num1)
				{
					if(ptr->num == num1)
						break;
					else if(ptr->next == NULL)
					{
						node* n1 = malloc(1*sizeof(node));
						n1->num = num1;
						n1->next = NULL;
						ptr->next = n1;
						size++;
					}
					else if(ptr->next->num > num1)
					{
						node* n1 = malloc(1*sizeof(node));
						n1->num = num1;
						n1->next = ptr->next;
						ptr->next = n1;
						size++;
					}
					ptr = ptr->next;
				}
			}
		}
		else
		{
			if(size != 0)
			{
				if(num1 == -2147483648)
				{
					repeat = 0;
					size--;
				}
				else
				{
					while(ptr->next != NULL)
					{
						if (ptr->next->num == num1)
						{
							ptr->next = ptr->next->next;
							size--;
							break;
						}
						ptr = ptr->next;
					}
				}
			}
		}
	}
	if(repeat == 0)
	{
		head = head->next;
	}
	ptr = head;
	printf("%d\n", size);
	while(ptr !=NULL)
	{
		if(ptr->num == head->num)
			printf("%d",head->num);
		else
			printf("\t%d",ptr->num);
		ptr = ptr->next;
	}	
	fclose(fp);
	return 0;
}
