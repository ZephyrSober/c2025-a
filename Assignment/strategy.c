#include "strategy.h"
#include <complex.h>
#include <stdlib.h>
#include "List.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils.h"
#define INF 9999.
#define ROUNDS 1000
#define MAXSTEP 5
#define WINCOUNT 5

Node* mcts_decide(Node* root) {
    for (int i = 0; i!= ROUNDS ;i++) {
        Node* leaf = choose(root,(double)i);
        Node* simulate_base = expand(leaf);
        double result = 0;
        if (is_terminal(simulate_base->state,simulate_base->latest_action,simulate_base->is_player?'1':'2')) {
            result = simulate_base->is_player?0:1;
        }else {
            result = simulate(simulate_base);
        }
        for (Node* p = leaf; p != NULL; p = p->parant) {
            p->visits++;
            p->value+=p->is_player?1-result:result;
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

Node* create_node(char state[BOARDSIZE][BOARDSIZE],int valid_range[2][2],List* children,Node* parent,Point* latest_action) {
    Node* node = (Node*)malloc(sizeof(Node));
    for (int i = 0;i != BOARDSIZE ; i++) {
        for (int j = 0; j!= BOARDSIZE; j++) {
            node->state[i][j] = state[i][j];
        }
    }
    node->valid_ranage[0][0]=valid_range[0][0];
    node->valid_ranage[0][1]=valid_range[0][1];
    node->valid_ranage[1][0]=valid_range[1][0];
    node->valid_ranage[1][1]=valid_range[1][1];
    node->parant = parent;
    node->latest_action = latest_action;
    node->is_player = (parent == NULL)?true:!parent->is_player;
    node->untryed_actions = get_all_actions(node);
    node->children = children;
    if (children == NULL) node->children = create_list(0,NULL);
    return node;
}

List* get_all_actions(Node* node) {
    List* actions = create_list(0,NULL);
    for (int i = node->valid_ranage[0][0]-2 ; i<=node->valid_ranage[1][0]+2 ; i++) {
        if (i<0||i>=BOARDSIZE) continue;
        for (int j = node->valid_ranage[0][1]-2 ; j<=node->valid_ranage[1][1]+2 ; j++) {
            if (j<0||j>=BOARDSIZE) continue;
            if (node->state[i][j]!='0') continue;
            Point* action = (Point*)malloc(sizeof(Point));
            action->x = i;action->y = j;
            append(actions,action);
        }
    }
    return actions;
}

Node* apply_action(Node* origin, Point* action) {
    Node* new_node = create_node(origin->state,origin->valid_ranage,NULL,origin,action);
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

bool is_terminal(char state[BOARDSIZE][BOARDSIZE], Point* latest_action,char player) {
    int directions[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    int x=latest_action->x,y=latest_action->y;
    for (int i=0;i!=4;i++) {
        int count = 1;
        int dx=directions[i][0],dy=directions[i][1];
        for (int j=-WINCOUNT+1;j<WINCOUNT;j++) {
            int xn = x+dx*j,yn = y+dy*j;
            if (xn<0||xn>=BOARDSIZE||yn<0||yn>=BOARDSIZE) continue;
            if (xn==x&&yn==y) continue;
            if (state[xn][yn]!=player) continue;
            count++;
        }
        if (count >= WINCOUNT) return true;
    }
    return false;
}

double ucb(double visits, double value, double time, double c) {
    if (visits <= 0.001&&visits>=-0.001) {
        return INF;
    }
    // printf("%lf\n",value/visits + c*sqrt(log(time)/visits));
    return value/visits + c*sqrt(log(time)/visits);
}

Node* choose(Node* root, double time) {
    Node* iter = root;
    while (is_fully_expand(iter)) {
        Node* p = NULL;
        double max=0.;
        for (int i = 0; i!=iter->children->lenth; i++) {
            Node *current_node = to_index(iter->children,i)->value;
            double current_ucb = ucb(current_node->visits,current_node->value,time,0.5);
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
    Node* current_node = simulate_base;
    Node* parent = NULL;
    Node* grand_parent = NULL;
    double result = 0;
    int i = 0;
    for (; i!=MAXSTEP;i++) {
        parent = current_node;
        current_node = apply_action(parent,(Point*)(random_choose(parent->untryed_actions)->value));
        if (i>=2) {
            grand_parent = parent->parant;
            free(grand_parent);
            grand_parent = NULL;
        }
        if (is_terminal(current_node->state,current_node->latest_action,current_node->is_player?'1':'2')) {
            result = current_node->is_player?0:1;
            if (i==1) {
                free(current_node);
                current_node = NULL;
            }if(i>=2) {
                free(current_node);
                free(parent);
                current_node = NULL;
                parent = NULL;
            }
            return result;
        }
    }
    return 0;

}