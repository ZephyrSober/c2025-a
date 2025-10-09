#ifndef C2025_CHALLENGE_STRATEGY_H
#define C2025_CHALLENGE_STRATEGY_H
#include <stdbool.h>
#include "List.h"
#define WINCOUNT 5
#define BOARDSIZE 15
struct Node {
    double visits;
    double value;
    char state[BOARDSIZE][BOARDSIZE];
    int valid_range[2][2];
    List* children;
    struct Node* parant;
    List* untryed_actions;
    Point* latest_action;
    bool is_player;
};
typedef struct Node Node;

Node* expand(Node* leaf);
Point* mcts_decide(Node* root);
Node* choose(Node* root,double time);
Node* create_node(char state[BOARDSIZE][BOARDSIZE],int valid_range[2][2],List* children,Node* parent,Point* latest_action);
List* get_all_actions(Node* node);
bool is_fully_expand(Node* node);
double ucb(double visits, double value, double time, double c);
Node* apply_action(Node* origin, Point* action);
double simulate(Node* simulate_base);
bool is_terminal(char state[BOARDSIZE][BOARDSIZE], Point* latest_action,char player);

#endif //C2025_CHALLENGE_STRATEGY_H