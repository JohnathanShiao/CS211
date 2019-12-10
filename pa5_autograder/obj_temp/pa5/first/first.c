#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int numin=0;
int numout=0;
int reverse = 0;

typedef struct node{
    char* name;
    int val;
    struct node* next;
}node;

typedef struct gate
{
    char* op;
    int order;
    struct node* inputs;
    struct node* outputs;
    struct node* selectors;
    struct gate* next;
}gate;

void notgate(node* in, node* out)
{
    if(in->val == 1)
        out->val = 0;
    else
        out->val = 1;
}

void andgate(node* in1, node* in2, node* out)
{
    if(in1->val == 1 && in2->val == 1)
        out->val = 1;
    else
        out->val = 0;
}

void nandgate(node* in1, node* in2,node* out)
{
    if(in1->val == 1 && in2->val == 1)
        out->val = 0;
    else
        out->val = 1;
}


void orgate(node* in1, node* in2, node* out)
{
    if(in1->val == 1||in2->val == 1)
        out->val = 1;
    else
        out->val = 0;
}

void norgate(node* in1, node* in2, node* out)
{
    if(in1->val == 1||in2->val == 1)
        out->val = 0;
    else
        out->val = 1;
}

void xorgate(node* in1, node* in2, node* out)
{
    if(in1->val == 1 && in2->val == 0)
        out->val = 1;
    else if(in1->val == 0 && in2->val ==1)
        out->val = 1;
    else
        out->val = 0;
}

void xnorgate(node* in1, node* in2, node* out)
{
    if(in1->val == 1 && in2->val == 0)
        out->val = 0;
    else if(in1->val == 0 && in2->val ==1)
        out->val = 0;
    else
        out->val = 1;
}

node* insert(node* list, node* temp)
{
    node* prev = NULL;
    node* ptr = list;
    while(ptr!=NULL)
    {
        prev = ptr;
        ptr = ptr->next;
    }
    if(prev == NULL)
        list = temp;
    else
        prev->next = temp;
    return list;
}

gate* append(gate* circuit, gate* temp)
{
    gate* prev = NULL;
    gate* ptr = circuit;
    while(ptr!=NULL)
    {
        prev = ptr;
        ptr = ptr->next;
    }
    if(prev == NULL)
        circuit = temp;
    else
        prev->next = temp;
    return circuit;
}

gate* eject(gate* circuit,gate* temp)
{
    gate* ptr = circuit;
    gate* prev = NULL;
    while(ptr!=NULL)
    {
        if(ptr->order == temp->order)
        {
            if(prev == NULL)
                circuit = circuit->next;
            else
                prev->next = ptr->next;
            temp->next = NULL;
            break;
        }
        prev = ptr;
        ptr=ptr->next;
    }
    circuit = append(circuit,temp);
    return circuit;
}

gate* initgate(gate* cir)
{
    cir->op = malloc(100*sizeof(char));
    cir->order = -1;
    cir->next = NULL;
    cir->inputs = NULL;
    cir->outputs = NULL;
    cir->selectors = NULL;
    return cir;
}

node* initnode(node* temp)
{
    temp->name = malloc(100*sizeof(char));
    temp->next = NULL;
    temp->val = -1;
    return temp;
}

void readFile(FILE* fp,node* vars, gate* circuit)
{
    char* temp = malloc(100*sizeof(char));
    int counter = 0;
    int num;
    while(fscanf(fp,"%s",temp)>0)
    {
        if(strcmp(temp,"INPUTVAR")==0 || strcmp(temp,"OUTPUTVAR")==0)
        {
            if(fscanf(fp,"%d",&num)>0)
            {
                if(strcmp(temp,"INPUTVAR")==0)
                    numin = num;
                else
                {
                    numout = num;
                    if(numin == 0)
                        reverse = 1;
                }
                for(int i = 0;i<num;i++)
                {
                    if(fscanf(fp,"%s",temp)>0)
                    {
                        node* var = malloc(sizeof(node));
                        var = initnode(var);
                        memcpy(var->name,temp,strlen(temp)+1);
                        vars = insert(vars, var);
                    }
                }
            }
        }
        else if(strcmp(temp,"MULTIPLEXER")==0||strcmp(temp,"DECODER")==0)
        {
            gate* cir = malloc(sizeof(gate));
            cir = initgate(cir);
            cir->order = counter;
            memcpy(cir->op, temp,strlen(temp)+1);
            if(fscanf(fp,"%d",&num)>0)
            {
                if(strcmp(temp,"MULTIPLEXER")==0)
                {
                    int select = log2(num);
                    for(int i = 0;i<num;i++)
                    {
                        node* in = malloc(sizeof(node));
                        if(fscanf(fp,"%s",temp)>0)
                        {
                            in->name = malloc((strlen(temp)+1)*sizeof(char));
                            memcpy(in->name,temp,strlen(temp)+1);
                            in->next = NULL;
                            cir->inputs = insert(cir->inputs,in);
                        }
                    }
                    for(int i = 0;i<select;i++)
                    {
                        node* sel = malloc(sizeof(node));
                        if(fscanf(fp,"%s",temp)>0)
                        {
                            sel->name = malloc((strlen(temp)+1)*sizeof(char));
                            memcpy(sel->name,temp,strlen(temp)+1);
                            sel->next = NULL;
                            cir->selectors = insert(cir->selectors,sel);
                        }
                    }
                    if(fscanf(fp,"%s",temp)>0);
                    node* out = malloc(sizeof(node));
                    out->name = malloc((strlen(temp)+1)*sizeof(char));
                    memcpy(out->name,temp,strlen(temp)+1);
                    out->next = NULL;
                    cir->outputs = insert(cir->outputs,out);
                }
                else
                {
                    int max = pow(2,num);
                    for(int i = 0;i<num;i++)
                    {
                        node* in = malloc(sizeof(node));
                        if(fscanf(fp,"%s",temp)>0)
                        {
                            in->name = malloc((strlen(temp)+1)*sizeof(char));
                            memcpy(in->name,temp,strlen(temp)+1);
                            in->next = NULL;
                            cir->inputs= insert(cir->inputs,in);
                        }
                    }
                    for(int i = 0;i<max;i++)
                    {
                        node* out = malloc(sizeof(node));
                        if(fscanf(fp,"%s",temp)>0)
                        {
                            out->name = malloc((strlen(temp)+1)*sizeof(char));
                            memcpy(out->name,temp,strlen(temp)+1);
                            out->next = NULL;
                            cir->outputs = insert(cir->outputs,out);
                        }
                    }
                }
            }
            circuit = append(circuit, cir);
            counter++;
        }
        else
        {
            gate* cir = malloc(sizeof(gate));
            cir = initgate(cir);
            cir->order=counter;
            memcpy(cir->op,temp,strlen(temp)+1);
            for(int i = 0;i<2;i++)
            {
                if(strcmp(temp, "NOT")==0)
                    i++;
                if(fscanf(fp,"%s",temp)>0)
                {
                    node* in = malloc(sizeof(node));
                    in->name = malloc((strlen(temp)+1)*sizeof(char));
                    memcpy(in->name,temp,strlen(temp)+1);
                    in->next = NULL;
                    cir->inputs = insert(cir->inputs, in);
                }
            }
            if(fscanf(fp,"%s",temp)>0);
            node* out = malloc(sizeof(node));
            out->name = malloc((strlen(temp)+1)*sizeof(char));
            memcpy(out->name,temp,strlen(temp)+1);
            out->next = NULL;
            cir->outputs = insert(cir->outputs,out);
            circuit = append(circuit,cir);
            counter++;
        }  
    }
    node* one = malloc(sizeof(node));
    one = initnode(one);
    one->name = "1";
    one->val = 1;
    vars = insert(vars, one);
    node* zero = malloc(sizeof(node));
    zero = initnode(zero);
    zero->name = "0";
    zero->val = 0;
    vars = insert(vars, zero);
}

int graycode(int i)
{
    return i ^ (i>>1);
}

void load(node* vars, int index, int v)
{
    node* ptr = vars;
    for(int i = 0;i<index;i++)
        ptr=ptr->next;
    ptr->val = v;
}

node* get(char* name, node* vars)
{
    node* ptr = vars;
    while(ptr!=NULL)
    {
        if(strcmp(ptr->name,name)==0)
            return ptr;
        ptr=ptr->next;
    }
    return NULL;
}

int valid(node* vars, gate* circuit)
{
    node* ins = circuit->inputs;
    node* temp;
    while(ins!=NULL)
    {
        temp = get(ins->name,vars);
        if(temp == NULL)
            return 0;
        else if(temp->val == -1)
            return 0;
        ins = ins->next;
    }
    return 1;
}

void cal(gate* circuit,node* vars)
{
    if(strcmp(circuit->op,"NOT")==0)
    {
        node* in = get(circuit->inputs->name,vars);
        node* out = get(circuit->outputs->name,vars);
        notgate(in,out);
    }
    else if(strcmp(circuit->op,"AND")==0)
    {
        node* in1 = get(circuit->inputs->name,vars);
        node* in2 = get(circuit->inputs->next->name,vars);
        node* out = get(circuit->outputs->name,vars);
        andgate(in1,in2,out);
    }
    else if(strcmp(circuit->op,"OR")==0)
    {
        node* in1 = get(circuit->inputs->name,vars);
        node* in2 = get(circuit->inputs->next->name,vars);
        node* out = get(circuit->outputs->name,vars);
        orgate(in1,in2,out);
    }
    else if(strcmp(circuit->op,"NAND")==0)
    {
        node* in1 = get(circuit->inputs->name,vars);
        node* in2 = get(circuit->inputs->next->name,vars);
        node* out = get(circuit->outputs->name,vars);
        nandgate(in1,in2,out);
    }
    else if(strcmp(circuit->op,"NOR")==0)
    {
        node* in1 = get(circuit->inputs->name,vars);
        node* in2 = get(circuit->inputs->next->name,vars);
        node* out = get(circuit->outputs->name,vars);
        norgate(in1,in2,out);
    }
    else if(strcmp(circuit->op,"XOR")==0)
    {
        node* in1 = get(circuit->inputs->name,vars);
        node* in2 = get(circuit->inputs->next->name,vars);
        node* out = get(circuit->outputs->name,vars);
        xorgate(in1,in2,out);
    }
    else if(strcmp(circuit->op,"XNOR")==0)
    {
        node* in1 = get(circuit->inputs->name,vars);
        node* in2 = get(circuit->inputs->next->name,vars);
        node* out = get(circuit->outputs->name,vars);
        xnorgate(in1,in2,out);
    }
    else if(strcmp(circuit->op, "DECODER")==0)
    {
        node* ins = circuit->inputs;
        node* temp;
        int num = 0;
        while(ins!=NULL)
        {
            num+=1;
            ins= ins->next;
        }
        ins = circuit->inputs;
        int index = 0;
        for(int i = 0;i<num;i++)
        {
            temp = get(ins->name,vars);
            index += (temp->val)<<(num-i-1);
            ins = ins->next;
        }
        int inv = 0;
        for(int i = index;i!=0;i=i>>1)
            inv ^= i;
        node* outs = circuit->outputs;
        num = 0;
        while(outs!=NULL)
        {
            temp = get(outs->name,vars);
            if(num == inv)
                temp->val = 1;
            else
                temp->val = 0;
            num++;
            outs=outs->next;
        }
    }
    else if(strcmp(circuit->op, "MULTIPLEXER")==0)
    {
        node* sels = circuit->selectors;
        node* temp;
        int num = 0;
        while(sels!=NULL)
        {
            num+=1;
            sels = sels->next;
        }
        sels = circuit->selectors;
        int index = 0;
        for(int i = 0;i<num;i++)
        {
            temp = get(sels->name,vars);
            index += (temp->val)<<(num-i-1);
            sels = sels->next;
        }
        int inv = 0;
        for(int i = index;i!=0;i=i>>1)
            inv ^= i;
        node* ins = circuit->inputs;
        node* temp2;
        for(int i = 0;i<inv;i++)
            ins=ins->next;
        temp = get(circuit->outputs->name,vars);
        temp2 = get(ins->name,vars);
        temp->val = temp2->val;
    }
}

gate* evaluate(node* vars,gate* circuit,int i)
{
    int g = graycode(i);
    int v;
    if(reverse)
    {
        int counter = 0;
        for(int i = numout;i<numin+numout;i++)
        {
            v = (g>>(numin-counter-1))%2;
            printf("%d ",v);
            load(vars,i,v);
            counter++;
        }
    }
    else{
        for(int i = 0;i<numin;i++)
        {
            v = (g>>(numin-i-1))%2;
            printf("%d ",v);
            load(vars,i,v);
        }
    }
    gate* ptr = circuit;
    gate* prev = NULL;
    while(ptr!=NULL)
    {
        if(!valid(vars,ptr))
        {
            circuit = eject(circuit,ptr);
            if(prev == NULL)
                ptr = circuit;
            else
                ptr = prev->next;
        }
        else
        {
            node* outs = ptr->outputs;
            while(outs!=NULL)
            {
                if(get(outs->name,vars)==NULL)
                {
                    node* t = malloc(sizeof(node));
                    memcpy(t,outs,sizeof(node));
                    t->val = -1;
                    t->next = NULL;
                    vars = insert(vars,t);
                }
                outs = outs->next;
            }
            cal(ptr,vars);
            prev = ptr;
            ptr=ptr->next;
        }
    }
    node* out = vars;
    if(reverse)
    {
        for(int i = 0;i<numout;i++)
        {
            printf("%d ", out->val);
            out=out->next;
        }
    }
    else
    {
        for(int i = 0;i<numin;i++)
            out=out->next;
        for(int i = 0;i<numout;i++)
        {
            printf("%d ", out->val);
            out=out->next;
        }
    }
    printf("\n");
    return circuit;
}

int main(int argc, char** argv)
{
    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL)
        return 0;
    node* vars = malloc(sizeof(node));
    gate* circuit = malloc(sizeof(gate));
    readFile(fp,vars,circuit);
    vars = vars->next;
    circuit = circuit->next;
    for(int i = 0;i<pow(2,numin);i++)
        circuit = evaluate(vars,circuit,i);
    fclose(fp);
    return 0;
}