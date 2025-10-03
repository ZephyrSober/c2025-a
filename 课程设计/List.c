#include "List.h"
#include "strategy.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
List* insert(List* list, int index, void* value) {
    if (index == 0) {
        ListNode* innode = (ListNode*)malloc(sizeof(ListNode));
        innode->value = value;
        innode->next = list->head;
        list->head = innode;
        list->lenth++;
        return list;
    }
    ListNode *current_node = to_index(list,index);
    ListNode* innode = (ListNode*)malloc(sizeof(ListNode));
    innode->value = value;
    innode->next = current_node->next;
    current_node->next = innode;
    list->lenth++;
    return list;
}

List* append(List* list, void* value) {
    return insert(list,list->lenth -1,value);
}

List* delete(List* list, int index){
    if (index == 0) {
        ListNode* p = list->head;
        list->head = to_index(list,1);
        list->lenth--;
        free(p);
        p=NULL;
        return list;
    }
    ListNode* p = to_index(list,index);
    to_index(list,index-1)->next = p->next;
    list->lenth--;
    free(p);
    p=NULL;
    return list;
}

ListNode* move(ListNode* start,int index) {
    ListNode* p = start;
    for (int i = 0; i!= index; i++) {
        p = p->next;
    }
    return p;
}

ListNode* to_index(List* start,int index) {
    ListNode* p = start->head;
    for (int i = 0; i!= index; i++) {
        p = p->next;
    }
    return p;
}

int find_value(List* list, void* target, int rank) {
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

List* create_list(int lenth, void** value) {
    if (lenth == 0) {
        List* list = (List*)malloc(sizeof(List));
        list->head = NULL;
        list->lenth = 0;
        return list;
    }
    List* list = (List*)malloc(sizeof(List));
    ListNode* p = (ListNode*)malloc(sizeof(ListNode));
    p->value = value[0];
    p->next = NULL;
    list->head = p;
    ListNode* pre = p;
    int i = 1;
    while (i!=lenth) {
        p = (ListNode*)malloc(sizeof(ListNode));
        p->value = value[i];
        pre->next = p;
        p->next = NULL;
        pre = pre->next;
        i++;
    }
    list->lenth = lenth;
    return list;
}

Point* pop_action(List* actions) {
    ListNode* pop_node = actions->head;
    actions->head = pop_node->next;
    actions->lenth--;
    Point* value = pop_node->value;
    free(pop_node);
    return value;
}