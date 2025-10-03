#include "strategy.h"
#include <stdlib.h>
#include "List.h"
#include <math.h>
#include <stdbool.h>
#include <windows.h>
#include "utils.h"
#define INF 9999.
#define ROUNDS 1000
#define MAXSTEP 100

Node* mcts_decide(Node* root) {
    for (int i = 0; i!= ROUNDS ;i++) {
        Node* leaf = choose(root,(double)i);
        Node* simulate_base = expand(leaf);
        double result = simulate(simulate_base);
        for (Node* p = leaf; leaf != NULL; p = p->parant) {
            p->visits++;
            p->value+=result;
        }
    }
    Node* p = NULL;
    double max=0.;
    for (int i = 0; i!=root->children->lenth; i++) {
        Node *current_node = to_index(root->children,i)->value;
        double current_visit = current_node->visits;
        if (current_visit>=max) {
            max = current_visit;
            p = current_node;
        }
    }
    return p;
}

Node* create_node(char state[SIZE][SIZE],int valid_range[2][2],List* children,Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    for (int i = 0;i != SIZE ; i++) {
        for (int j = 0; j!= SIZE; j++) {
            node->state[i][j] = state[i][j];
        }
    }
    node->children = children;
    node->untryed_actions = get_all_actions(node);
    node->valid_ranage[0][0]=valid_range[0][0];
    node->valid_ranage[0][1]=valid_range[0][1];
    node->valid_ranage[1][0]=valid_range[1][0];
    node->valid_ranage[1][1]=valid_range[1][1];
    node->parant = parent;
    return node;
}

List* get_all_actions(Node* node) {
    //todo
}

Node* apply_action(Node* origin, Point* action) {
    Node* new_node = create_node(origin->state,origin->valid_ranage,NULL,origin);
    new_node->state[action->x][action->y] = '2';
    new_node->valid_ranage[0][0] = (action->x<=new_node->valid_ranage[0][0]) ? action->x : new_node->valid_ranage[0][0];
    new_node->valid_ranage[0][1] = (action->y<=new_node->valid_ranage[0][1]) ? action->y : new_node->valid_ranage[0][1];
    new_node->valid_ranage[1][0] = (action->x>=new_node->valid_ranage[1][0]) ? action->x : new_node->valid_ranage[1][0];
    new_node->valid_ranage[1][1] = (action->y>=new_node->valid_ranage[1][1]) ? action->y : new_node->valid_ranage[1][1];
    return new_node;
}

bool is_fully_expand(Node* node) {
    return node->untryed_actions->lenth == 0;
}

double ucb(double visits, double value, double time, double c) {
    if (visits == 0) {
        return INF;
    }
    return value/visits + c*sqrt(log(time)/visits);
}

Node* choose(Node* root, double time) {
    Node* iter = root;
    while (is_fully_expand(iter)) {
        Node* p = NULL;
        double max=0.;
        for (int i = 0; i!=iter->children->lenth; i++) {
            Node *current_node = to_index(iter->children,i)->value;
            double current_ucb = ucb(current_node->visits,current_node->value,time,sqrt(2));
            if (current_ucb>=max) {
                max = current_ucb;
                p = current_node;
            }
        }
        iter = p;
    }
    return iter;
}

Node* expand(Node* leaf) {
    Point* action = pop_action(leaf->untryed_actions);
    Node* child = apply_action(leaf,action);
    append(leaf->children,child);
    return child;
}

double simulate(Node* simulate_base) {
    for (int i = 0; i!=MAXSTEP;i++) {

    }
}