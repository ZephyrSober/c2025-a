#ifndef C2025_CHALLENGE_LIST_H
#define C2025_CHALLENGE_LIST_H
#include "strategy.h"
#include "utils.h"

struct ListNode {
    void* value;
    struct ListNode* next;
};
typedef struct ListNode ListNode;
struct List {
    int lenth;
    ListNode* head;
};
typedef struct List List;

ListNode* to_index(List* start, int index);
ListNode* move(ListNode* start,int index);
List* insert(List* list, int index, void* value);
int find_value(List* list, void* target, int rank);
List* create_list(int lenth, void** value);
List* delete(List* list, int index);
Point* pop_action(List* actions);
List* append(List* list, void* value);

#endif //C2025_CHALLENGE_LIST_H