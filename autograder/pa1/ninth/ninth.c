#include <stdlib.h>
#include <stdio.h>

typedef struct treenode
{
	int value;
	struct treenode* left;
	struct treenode* right;
	int height;
}treenode;

void  search(int num,treenode* root)
{
	if(root == NULL||root->value == num)
	{
		if(root == NULL)
			printf("absent\n");
		else
			printf("present %d\n",root->height);
	}
	else if(root->value > num)
		search(num,root->left);
	else
		search(num,root->right);
}

treenode* insert(int num, treenode* root, int height)
{
	if(root == NULL)
	{
		treenode* temp = malloc(sizeof(treenode));
		temp->value = num;
		temp->height = height+1;
		temp->left = NULL;
		temp->right = NULL;
		printf("inserted %d\n",temp->height);
		return temp;
	}
	else if(root->value == num)
		printf("duplicate\n");
	else if(root->value > num)
		root->left = insert(num,root->left,height+1);
	else
		root-> right = insert(num,root->right,height+1);
	return root;
}

treenode* minimum(treenode* root)
{
	treenode* min = root;
	while(min != NULL && min->left!=NULL)
		min = min->left;
	return min;
}

void heightchange(treenode* root)
{
	if(root == NULL)
		return;
	heightchange(root->left);
	heightchange(root->right);
	root->height -=1;
		
}
treenode* delete(int num, treenode* root)
{
	if(root == NULL)
	{
		printf("fail\n");
	}
	else if(root->value > num)
	{
		root->left = delete(num,root->left);
	}
	else if(root->value < num)
	{
		root->right = delete(num,root->right);
	}
	else
	{
		if(root->left == NULL || root->right == NULL)
		{
			if(root->left != NULL)
			{
				treenode* temp = root->left;
				heightchange(root->left);
				free(root);
				printf("success\n");
				return temp;
			}
			else if(root->right != NULL)
			{
				treenode* temp = root->right;
				heightchange(root->right);
				free(root);
				printf("success\n");
				return temp;
			}
			free(root);
			printf("success\n");
			return NULL;
		}
		treenode* temp = minimum(root->right);
		root->value = temp->value;
		root->right = delete(temp->value, root->right);
	}
	return root;
}

void freeAll(treenode* root)
{
	if(root == NULL)
		return;
	freeAll(root->left);
	freeAll(root->right);
	free(root);
}
int main(int arg,char** argv)
{
	FILE* fp = fopen(argv[1],"r");
	if(fp ==NULL)
	{
		printf("error");
		return 0;
	}
	int num;
	char op;
	treenode* root = NULL;
	while(fscanf(fp,"%c\t%d\n",&op,&num)>EOF)
	{
		if(op == 'i')
			root = insert(num,root,0);
		else if(op == 's')
			search(num,root);
		else
			root = delete(num,root); 
	}
	fclose(fp);
	freeAll(root);
	return 0;
}
