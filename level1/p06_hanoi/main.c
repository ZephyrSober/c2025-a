#include <assert.h>
#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define MAX 64
char all_stack[3]= {'A','B','C'};

struct Stack{
    int *data;
    int capacity;
    int rear; // after the last element
    char name;
};
typedef struct Stack Stack;

Stack* create_stack(char name,int size);
void inStack(Stack* stack,int data);
int outStack(Stack* stack);
void move(Stack* origin, Stack* target, Stack* help, int lenth);
int spy_top(Stack* pole);

int main() {
    Stack* A = create_stack('A',MAX);
    Stack* B = create_stack('B',MAX);
    Stack* C = create_stack('C',MAX);

    for (int i=0; i!=MAX; i++) {
        inStack(A,MAX-i); //1 ~ MAX , the min is on the top
    }
    assert(spy_top(A)==1);
    move(A,C,B,MAX);
}

void move(Stack* origin, Stack* target, Stack* help, int lenth) {
    if (lenth==1) {
        inStack(target,outStack(origin));
        printf("%c -> %c\n",origin->name,target->name);
        return;
    }
    move(origin,help,target,lenth-1);
    inStack(target,outStack(origin));
    printf("%c -> %c\n",origin->name,target->name);
    move(help,target,origin,lenth-1);
}

Stack* create_stack(char name,const int size) {
    Stack *p = (Stack*)malloc(sizeof(Stack));
    p->capacity = size;
    p->data = (int *)malloc(size * sizeof(int));
    p->rear = 0;
    p->name = name;
    return p;
}

void inStack(Stack* stack,const int data) {
    if (stack->rear>=stack->capacity) {
        printf("out of size");
        return;
    }
    stack->data[stack->rear++] = data;
}

int outStack(Stack* stack) {
    if (stack->rear == 0) {
        printf("empty");
        return 0;
    }
    const int data = stack->data[--stack->rear];
    return data;
}

int spy_top(Stack* pole) {
    return pole->data[pole->rear-1];
}