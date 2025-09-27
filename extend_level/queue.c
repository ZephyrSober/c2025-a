#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Queue{
    int front,rear,count;
    int *data;
    int capacity;
};
typedef struct Queue Queue;

Queue* create_queue(int size);
void inQueue(Queue* queue,int data);
int outQueue(Queue* queue);

int main() {
    Queue* q1 = create_queue(10);

    for (int i = 0; i!=11; i++) {
        inQueue(q1,i+1);
    }
    for (int i = 0 ; i != 12; i++) {
        printf("%d ",outQueue(q1));
    }
}

Queue* create_queue(const int size) {
    Queue *p = (Queue*)malloc(sizeof(Queue));
    p->capacity = size;
    p->data = (int *)malloc(size * sizeof(int));
    p->front = p->rear = 0;
    p->count = 0;
    return p;
}


void inQueue(Queue* queue,const int data) {
    if (queue->count>=queue->capacity) {
        printf("out of size");
        return;
    }
    queue->data[queue->rear] = data;
    if (queue->rear < queue->capacity - 1) {
        queue->rear++;
    }
    else {
        queue->rear = 0;
    }
    queue->count++;
}

int outQueue(Queue* queue) {
    if (queue->count==0) {
        printf("empty");
        return 0;
    }
    const int data = queue->data[queue->front];
    if (queue->front < queue->capacity - 1) {
        queue->front++;
    }
    else {
        queue->front = 0;
    }
    queue->count--;
    return data;
}
