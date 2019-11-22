#include <stdlib.h>
#include <stdio.h>

void makePig(char* word)
{
	if(word[0] == 'a' || word[0] == 'e' || word[0] == 'i' || word [0] == 'o'|| word[0] == 'u'||word[0] == 'A' || word[0] == 'E' || word[0] == 'I' || word [0] == 'O'|| word[0] == 'U')
	{
		printf("%syay ", word);
	}
	else
	{
		int length = 0;
		while(word[length] != '\0')
			length++;
		char* head = malloc(length*sizeof(char));
		char* tail = malloc(length*sizeof(char));
		int count = 0;
		while(word[count] != 'a' && word[count] != 'e' && word[count] != 'i' && word[count] != 'o' &&word[count] != 'u'&& word[count] != 'A' && word[count] != 'E' && word[count] != 'I' && word [count] != 'O'&& word[count] != 'U')
		{
			head[count] = word[count];
			count++;
		}
		int index= 0;
		while(word[count] != '\0')
		{
			tail[index] = word[count];
			index++;
			count++;
		}
		printf("%s%say ",tail,head);
		free(head);
		free(tail);
	}
}

int main(int argc, char** argv)
{
	for(int i = 1;i<argc;i++)
	{
		makePig(argv[i]);
	}
	return 0;
}

