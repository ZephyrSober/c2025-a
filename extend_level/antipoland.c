#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 100
struct Stack{
    int *data;
    int capacity;
    int rear;
};
typedef struct Stack Stack;

Stack* create_stack(int size);
void inStack(Stack* stack,int data);
int outStack(Stack* stack);
char* read_string();
bool is_num(char c);
double operate(int a, int b,char operation);

int main() {
    Stack* exp = create_stack(MAX);
    char* str = read_string();
    for (int i= 0; i!= strlen(str);i++) {
        if (str[i]==',') continue;
        if (is_num(str[i])) {
            int num = str[i] - 48;
            inStack(exp,num);
        }else {
            inStack(exp,operate(outStack(exp),outStack(exp),str[i]));
        }
    }
    printf("%d",outStack(exp));
}

double operate(int a, int b,char operation) {
    switch (operation) {
    case'+':
        return a+b;
    case '-':
        return a-b;
    case '*':
        return a*b;
    case '/':
        return a/b;
    default:
        printf("error: invalid openration");
        return 0;
    }
}

bool is_num(char c) {
    if (c >= 48 && c <= 57) return true;
    return false;
}

char* read_string() {
    const size_t INITIAL_BUFFER_SIZE = 128;
    const int GROWTH_FACTOR = 2;
    size_t buff_size = INITIAL_BUFFER_SIZE;

    char* buff = (char*)malloc(buff_size*sizeof(char));
    buff[0]='\0';
    size_t total_lenth = 0;
    char ch;

    printf("input string:");
    while ((ch = getchar()) != '\n') {
        if (total_lenth+1 >= buff_size) {
            buff_size *= GROWTH_FACTOR;
            char* new_buff = realloc(buff,buff_size*sizeof(char));
            buff = new_buff;
        }
        buff[total_lenth++]=ch;
    }
    buff[total_lenth]='\0';
    return buff;
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
