#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack{
    int *data;
    int capacity;
    int rear;
};
typedef struct Stack Stack;

Stack* create_stack(int size);
void inStack(Stack* stack,int data);
int outStack(Stack* stack);

int main() {
    Stack* q1 = create_stack(10);

    for (int i = 0; i!=11; i++) {
        inStack(q1,i+1);
    }
    for (int i = 0 ; i != 12; i++) {
        printf("%d ",outStack(q1));
    }
}

Stack* create_stack(const int size) {
    Stack *p = (Stack*)malloc(sizeof(Stack));
    p->capacity = size;
    p->data = (int *)malloc(size * sizeof(int));
    p->rear = 0;
    return p;
}


void inStack(Stack* stack,const int data) {
    if (stack->rear>=stack->capacity) {
        printf("out of size");
        return;
    }
    stack->data[stack->rear] = data;
    stack->rear++;
}

int outStack(Stack* stack) {
    if (stack->rear == 0) {
        printf("empty");
        return 0;
    }
    const int data = stack->data[--stack->rear];
    return data;
}
