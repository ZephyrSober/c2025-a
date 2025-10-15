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
#define EXPLORERANGE 1

Point* mcts_decide(Node* root) {
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
    return p->latest_action;
}

Node* create_node(char state[BOARDSIZE][BOARDSIZE],int valid_range[2][2],List* children,Node* parent,Point* latest_action) {
    Node* node = (Node*)malloc(sizeof(Node));
    for (int i = 0;i != BOARDSIZE ; i++) {
        for (int j = 0; j!= BOARDSIZE; j++) {
            node->state[i][j] = state[i][j];
        }
    }
    node->valid_range[0][0]=valid_range[0][0];
    node->valid_range[0][1]=valid_range[0][1];
    node->valid_range[1][0]=valid_range[1][0];
    node->valid_range[1][1]=valid_range[1][1];
    node->parant = parent;
    node->latest_action = latest_action;
    node->is_player = (parent == NULL)?true:!parent->is_player;
    node->untryed_actions = get_all_actions(node);
    node->children = children;
    if (children == NULL) node->children = create_list(0,NULL);
    return node;
}

void get_valid_range(char state[BOARDSIZE][BOARDSIZE],int valid_range[2][2]) {
    for (int i = 0; i!=BOARDSIZE; i++) {
        for (int j = 0; j!=BOARDSIZE; j++) {
            if (state[i][j] == '1'||state[i][j] == '2') {
                valid_range[0][0] = (i<=valid_range[0][0]) ? i : valid_range[0][0];
                valid_range[0][1] = (j<=valid_range[0][1]) ? j : valid_range[0][1];
                valid_range[1][0] = (i>=valid_range[1][0]) ? i : valid_range[1][0];
                valid_range[1][1] = (j>=valid_range[1][1]) ? j : valid_range[1][1];
            }

        }
    }
}

List* get_all_actions(Node* node) {
    //简单策略：
    //第一优先级：己方活四（活四单活或者双活均可）
    //第二优先级：对方活四
    //第三优先级：己方双活三（连三返回两个位置，跳三返回中间位置）
    //第四优先级：对方活三（同理）
    //其他：搜索所有空位即可
    int expand_valid_range[2][2] = {{node->valid_range[0][0]-EXPLORERANGE,node->valid_range[0][1]-EXPLORERANGE},{node->valid_range[1][0]+EXPLORERANGE,node->valid_range[1][1]+EXPLORERANGE}};
    List* actions = create_list(0,NULL);
    //first priority:
    actions = find_pattern(node->state, expand_valid_range,6,"322224");
    actions = join(actions,find_pattern(node->state, expand_valid_range,6,"322223"));
    actions = join(actions,find_pattern(node->state, expand_valid_range,6,"422223"));
    if (actions->lenth != 0) return actions;
    //second priority:
    actions = find_pattern(node->state, expand_valid_range,6,"311114");
    actions = join(actions,find_pattern(node->state, expand_valid_range,6,"411113"));
    actions = join(actions,find_pattern(node->state, expand_valid_range,6,"311113"));
    if (actions->lenth != 0) return actions;
    //third priority:
    actions = find_pattern(node->state, expand_valid_range,5,"32223");
    actions = join(actions,find_pattern(node->state, expand_valid_range,6,"022320"));
    actions = join(actions,find_pattern(node->state, expand_valid_range,6,"023220"));
    if (actions->lenth != 0) return actions;
    //fourth priority:
    actions = find_pattern(node->state, expand_valid_range,5,"31113");
    actions = join(actions,find_pattern(node->state, expand_valid_range,6,"322323"));
    actions = join(actions,find_pattern(node->state, expand_valid_range,6,"323223"));
    if (actions->lenth != 0) return actions;
    //fifth priority
    for (int i = expand_valid_range[0][0] ; i<=expand_valid_range[1][0] ; i++) {
        if (i<0||i>=BOARDSIZE) continue;
        for (int j = expand_valid_range[0][1] ; j<=expand_valid_range[1][1] ; j++) {
            if (j<0||j>=BOARDSIZE) continue;
            if (node->state[i][j]!='0') continue;
            Point* action = (Point*)malloc(sizeof(Point));
            action->x = i;action->y = j;
            append(actions,action);
        }
    }
    return actions;
}

List* find_pattern(char state[BOARDSIZE][BOARDSIZE], int valid_range[2][2], int pattern_lenth, char pattern[pattern_lenth]) {
    //'3' in pattern means target, '4' means null
    //为了避免对称pattern重复问题，这里只检查180°范围
    List* result = create_list(0,NULL);
    int directions[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (int x = valid_range[0][0]; x<=valid_range[1][0]; x++) {
        for (int y = valid_range[0][1]; y<=valid_range[1][1]; y++) {
            for (int d = 0; d!=4; d++) {
                int dx = directions[d][0], dy = directions[d][1];
                if (is_match(state,x,y,dx,dy,pattern_lenth,pattern)) {
                    for (int i = 0; i!=pattern_lenth; i++) {
                        if (pattern[i] == '3') {
                            Point* p = create_point(x+i*dx,y+i*dy);
                            append(result,p);
                        }
                    }
                }
            }
        }
    }
    return result;
}

bool is_match(char state[BOARDSIZE][BOARDSIZE], int x, int y, int dx, int dy, int pattern_lenth, char _pattern[pattern_lenth]) {
    //'3' in pattern means target, '4' means null
    char pattern[pattern_lenth] = {};
    for (int i = 0; i!=pattern_lenth; i++) {
        pattern[i] = _pattern[i];
    }
    for (int i = 0; i!= pattern_lenth;i++) {
        if (pattern[i] == '3') pattern[i] = '0';
        if (pattern[i] == '4') continue;
        if (x+(pattern_lenth-1)*dx>=BOARDSIZE || x-(pattern_lenth-1)*dx>=BOARDSIZE || x+(pattern_lenth-1)*dx<0 || x+(pattern_lenth-1)*dx<0) return false;
        if (y+(pattern_lenth-1)*dy>=BOARDSIZE || y-(pattern_lenth-1)*dy>=BOARDSIZE || y+(pattern_lenth-1)*dy<0 || y+(pattern_lenth-1)*dy<0) return false;
        if (pattern[i] != state[x+i*dx][y+i*dy]) return false;
    }
    return true;
}

Node* apply_action(Node* origin, Point* action) {
    Node* new_node = create_node(origin->state,origin->valid_range,NULL,origin,action);
    new_node->state[action->x][action->y] = origin->is_player?'2':'1';
    new_node->valid_range[0][0] = (action->x<=new_node->valid_range[0][0]) ? action->x : new_node->valid_range[0][0];
    new_node->valid_range[0][1] = (action->y<=new_node->valid_range[0][1]) ? action->y : new_node->valid_range[0][1];
    new_node->valid_range[1][0] = (action->x>=new_node->valid_range[1][0]) ? action->x : new_node->valid_range[1][0];
    new_node->valid_range[1][1] = (action->y>=new_node->valid_range[1][1]) ? action->y : new_node->valid_range[1][1];
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
    Node* iter;
    iter = root;
    while (is_fully_expand(iter)) {
        Node* p = NULL;
        double max=0.;
        for (int i = 0; i!=iter->children->lenth; i++) {
            Node *current_node = to_index(iter->children,i)->value;
            double current_ucb = ucb(current_node->visits,current_node->value,time,sqrt(2.));
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