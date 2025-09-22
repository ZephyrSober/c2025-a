#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Node {
    int value;
    struct Node* next;
};
typedef struct Node Node;
struct List {
    int lenth;
    Node* head;
};
typedef struct List List;

Node* to_index(List* start, int num);
Node* move(Node* start,int num);
Node* insert(Node* node, int value);
int find_value(List* list, int target, int rank);
List* create_list(int lenth, int* value);
List* invert(List* list);
void print(List* list);

int main() {
    int value_list[10] = {1,2,3,4,5,6,7,5,9,10};
    List* list = create_list(sizeof(value_list)/4,value_list);
    print(list);
    list = invert(list);
    int x = find_value(list,5,1);
    assert(x==2);
    int y = find_value(list,5,2);
    assert(y==5);
    return 0;
}

Node* insert(Node* node, int value) {
    Node* innode = (Node*)malloc(sizeof(Node));
    innode->value = value;
    innode->next = node->next;
    node->next = innode;
    return innode;
}

Node* move(Node* start,int num) {
    Node* p = start;
    for (int i = 0; i!= num; i++) {
        p = p->next;
    }
    return p;
}

Node* to_index(List* start,int num) {
    Node* p = start->head;
    for (int i = 0; i!= num; i++) {
        p = p->next;
    }
    return p;
}

int find_value(List* list, int target, int rank) {
    int place = 0;
    int count = 0;
    Node* p = list->head;
    while (p->next != NULL) {
        if (p->value == target) {
            count++;
            if (count == rank) return place;
        }
        place++;
        p = p->next;
    }
    if (p->value == target) {
        count++;
        if (count == rank) return place;
    }
    return -1;
}

List* create_list(int lenth, int* value) {
    List* list = (List*)malloc(sizeof(List));
    Node* p = (Node*)malloc(sizeof(Node));
    p->value = value[0];
    p->next = NULL;
    list->head = p;
    Node* pre = p;
    for (int i = 1; i!=lenth; i++) {
        p = (Node*)malloc(sizeof(Node));
        p->value = value[i];
        pre->next = p;
        p->next = NULL;
        pre = pre->next;
    }
    list->lenth = lenth;
    return list;
}

List* invert(List* list) {
    int invert_value[list->lenth]={};
    for (int i = 0; i!=list->lenth;i++) {
        invert_value[list->lenth-i-1] = to_index(list,i)->value;
    }
    return create_list(list->lenth,invert_value);
}

void print(List* list) {
    for (int i = 0; i!=list->lenth;i++) {
        printf("%d ",to_index(list,i)->value);
    }
}