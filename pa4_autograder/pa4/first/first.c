#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct node
{
    size_t tag;
    struct node* next;
}node;

int chits = 0;
int cmiss = 0;
int memread = 0;
int memwrite = 0;
int chits2 = 0;
int cmiss2 = 0;
int memread2 = 0;
int policy;
int fetch=0;
int a;

void insert(node* temp, node** sets,size_t address, int offset, int ssize);

size_t tag(size_t address, int offset, int ssize)
{
    return address>>(offset+ssize);
}

size_t index(size_t address, int offset, int ssize)
{
    return (address>>offset) % (0x1<<ssize);
}

int valid(int c, int b,char* pol,char* asso,int pre)
{
    int valid = 0;
    if(c != 0 && (c &(c-1))==0)
        valid++;
    if(b != 0 && (b &(b-1))==0)
        valid++;
    if(strcmp(pol, "fifo")==0 || strcmp(pol, "lru")==0)
    {
        if(strcmp(pol,"fifo") == 0)
            policy = 0;
        else
            policy = 1;
        valid++;
    }
    if(strcmp(asso, "direct")==0 || strcmp(asso,"assoc")==0)
    {
        if(strcmp(asso,"direct")==0)
            a = 1;
        else
            a = c/b;
        valid++;
    }
    if(strlen(asso)>=7)
    {
        char* temp = "assoc:";
        int i = 0;
        while(i<6)
        {
            if(asso[i] != temp[i])
                return 0;
            i++;
        }
        asso = &asso[6];
        i = atoi(asso);
        if(i!=0 && (i&(i-1))==0)
        {
            a = i;
            valid++;
        }
    }
    if(pre >= 0)
        valid++;
    if(valid !=5)
        return 0;
    return 1;
}

int search(node** sets, size_t address, int offset, int ssize)
{
    
    int hash = index(address,offset,ssize);
    size_t t = tag(address,offset,ssize);
    node* ptr = sets[hash];
    node* prev = NULL;
    while(ptr != NULL)
    {
        if(ptr->tag == t)
        {
            if(policy == 1 && fetch == 0)
            {
                if(prev==NULL)
                    sets[hash] = sets[hash]->next;
                else
                    prev->next = ptr->next;
                ptr->next = NULL;
                insert(ptr,sets,address,offset,ssize);
            }
            return 1;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    return 0;
}

void eject(node** sets, int hash)
{
    node* temp = sets[hash];
    sets[hash] = sets[hash]->next;
    free(temp);
}

void insert(node* temp, node** sets,size_t address, int offset, int ssize)
{
    int hash = index(address,offset,ssize);
    node* ptr = sets[hash];
    node* prev = NULL;
    int counter = 0;
    while(ptr !=NULL)
    {
        ptr = ptr->next;
        counter++;
    }
    ptr = sets[hash];
    if(counter<a)
    {
        while(ptr!=NULL)
        {
            prev = ptr;
            ptr = ptr->next;
        }
        if(prev == NULL)
            sets[hash] = temp;
        else
            prev->next = temp;
    }
    else
    {
        eject(sets,hash);
        insert(temp,sets,address,offset,ssize);
    }
}

void retrieve(node** sets, size_t address, int offset, int ssize)
{
    if(!search(sets,address,offset,ssize))
    {
        cmiss+=1;
        memread+=1;
        node* temp = malloc(sizeof(node));
        temp->tag = tag(address,offset,ssize);
        temp->next = NULL;
        insert(temp,sets,address,offset,ssize);
    }
    else
        chits+=1;
    
}

void retrievep(node** sets,size_t address, int offset, int ssize,int pre)
{
    int adj = 0x1<<offset;
    if(search(sets,address,offset,ssize))
        chits2+=1;
    else
    {
        cmiss2+=1;
        memread2+=1;
        fetch = 1;
        node* temp = malloc(sizeof(node));
        temp->tag = tag(address,offset,ssize);
        temp->next = NULL;
        insert(temp,sets,address,offset,ssize);
        for(int i = 0;i<pre;i++)
        {
            address+=adj;
            if(!search(sets,address,offset,ssize))
            {
                memread2+=1;
                node* temp = malloc(sizeof(node));
                temp->tag = tag(address,offset,ssize);
                temp->next = NULL;
                insert(temp,sets,address,offset,ssize);
            }
        }
        fetch = 0;
    }
}

void freeList(node* list)
{
    node* temp;
    while(list!=NULL)
    {
        temp = list;
        list = list->next;
        free(temp);
    }
}

void freeAll(node** sets,int csize,int bsize)
{
   int setsize = csize/(bsize*a);
   for(int i = 0;i<setsize;i++)
   {
       freeList(sets[i]);
   }
}

int main(int argc, char** argv)
{
    if(argc!=7)
        return 0; 
    int csize = atoi(argv[1]);
    int bsize = atoi(argv[2]);
    char* policy = argv[3];
    char* asso = argv[4];
    int prefetch = atoi(argv[5]);
    if(!valid(csize,bsize,policy,asso,prefetch))
    {
        printf("error");
        return 0;
    }
    FILE* fp = fopen(argv[6],"r");
    if (fp == NULL)
    {
        printf("error");
        return 0;
    }
    int ssize = log2(csize/(bsize*a));
    int offset = log2(bsize);
    char op;
    size_t add;
    node** table = malloc((csize/(bsize*a))*sizeof(node*));
    node** table2 = malloc((csize/(bsize*a))*sizeof(node*));
    while(fscanf(fp,"\n%c %zx",&op,&add)>0)
    {
        if(op == '#')
            break;
        if(op == 'W')
            memwrite++;
        retrieve(table,add,offset,ssize);
        retrievep(table2,add,offset,ssize,prefetch);
    }
    printf("no-prefetch\nMemory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", memread,memwrite,chits,cmiss);
    printf("with-prefetch\nMemory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", memread2,memwrite,chits2,cmiss2);
    freeAll(table,csize,bsize);
    freeAll(table2,csize,bsize);
    fclose(fp);
    return 0;
}