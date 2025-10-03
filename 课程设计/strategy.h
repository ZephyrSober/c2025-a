#ifndef C2025_CHALLENGE_STRATEGY_H
#define C2025_CHALLENGE_STRATEGY_H
#include <stdbool.h>
#include "List.h"

#define SIZE 15
struct Node {
    double visits;
    double value;
    char state[SIZE][SIZE];
    int valid_ranage[2][2];
    List* children;
    struct Node* parant;
    List* untryed_actions;
};
typedef struct Node Node;

Node* expand(Node* leaf);
Node* mcts_decide(Node* root);
Node* choose(Node* root,double time);
Node* create_node(char state[SIZE][SIZE],int valid_range[2][2],List* children,Node* parent);
List* get_all_actions(Node* node);
bool is_fully_expand(Node* node);
double ucb(double visits, double value, double time, double c);
Node* apply_action(Node* origin, Point* action);
double simulate(Node* simulate_base);

#endif //C2025_CHALLENGE_STRATEGY_H