#ifndef C2025_CHALLENGE_LIST_H
#define C2025_CHALLENGE_LIST_H
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

ListNode* to_index(List* list, int index);
ListNode* move(ListNode* start,int index);
List* insert(List* list, int index, void* value);
int find_value(List* list, void* target, int rank);
List* create_list(int lenth, void** value);
List* delete(List* list, int index);
Point* pop_action(List* actions);
List* append(List* list, void* value);
void print_actions(List* actions);
ListNode* random_choose(List* list);
void destroy_list(List* list);
List* join(List* a, List* b);

#endif //C2025_CHALLENGE_LIST_H