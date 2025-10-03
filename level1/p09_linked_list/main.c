#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct ListNode {
    int value;
    struct ListNode* next;
};
typedef struct ListNode ListNode;
struct List {
    int lenth;
    ListNode* head;
};
typedef struct List List;

ListNode* to_index(List* list, int index);
ListNode* move(ListNode* start,int index);
ListNode* insert(ListNode* node, int value);
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

ListNode* insert(ListNode* node, int value) {
    ListNode* innode = (ListNode*)malloc(sizeof(ListNode));
    innode->value = value;
    innode->next = node->next;
    node->next = innode;
    return innode;
}

ListNode* move(ListNode* start,int index) {
    ListNode* p = start;
    for (int i = 0; i!= index; i++) {
        p = p->next;
    }
    return p;
}

ListNode* to_index(List* list,int index) {
    ListNode* p = list->head;
    for (int i = 0; i!= index; i++) {
        p = p->next;
    }
    return p;
}

int find_value(List* list, int target, int rank) {
    int place = 0;
    int count = 0;
    ListNode* p = list->head;
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
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));
    p->value = value[0];
    p->next = NULL;
    list->head = p;
    ListNode* pre = p;
    for (int i = 1; i!=lenth; i++) {
        p = (ListNode*)malloc(sizeof(ListNode));
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