#include "strategy.h"
#include "List.h"
#include "test.h"
#include "utils.h"
#include <stdio.h>
#include <windows.h>

Board* create_empty_board() {
    Board* board = (Board*)malloc(sizeof(Board));
    board->latest_action = NULL;
    for (int i = 0; i!= BOARDSIZE;i++) {
        for (int j = 0; j!= BOARDSIZE;j++) {
            board->state[i][j] = '0';
        }
    }
    board->valid_range[0][0] = 7;
    board->valid_range[1][0] = 7;
    board->valid_range[0][1] = 7;
    board->valid_range[1][0] = 7;
    return board;
}

void draw(char state[BOARDSIZE][BOARDSIZE]) {
    system("cls");
    for (int i = 0; i!= BOARDSIZE; i++) {
        for (int j = 0 ; j!= BOARDSIZE; j++) {
            printf("%c",state[i][j]);
        }
        printf("\n");
    }
}

void input(Board* board) {
    printf("input your action");
    int x,y;
    scanf("%d,%d",&x,&y);
    update_state(board,x,y,'1');
}

void update_state(Board* board, int x, int y,char player) {
    board->state[x][y] = player;
    board->latest_action = create_point(x,y);
    board->valid_range[0][0] = (x<=board->valid_range[0][0]) ? x : board->valid_range[0][0];
    board->valid_range[0][1] = (y<=board->valid_range[0][1]) ? y : board->valid_range[0][1];
    board->valid_range[1][0] = (x>=board->valid_range[1][0]) ? x : board->valid_range[1][0];
    board->valid_range[1][1] = (y>=board->valid_range[1][1]) ? y : board->valid_range[1][1];
}

void test_is_terminal() {
    char state[BOARDSIZE][BOARDSIZE] = {"202222000000000",
                                        "000000000000000",
                                        "002000000000000",
                                        "000200000000000",
                                        "000020000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000"};
    Point point = {0,5};
    Point* latest_action = &point;

    if (is_terminal(state,latest_action,'2')) printf("all right");
}

void test_get_all_actions() {
    char state[BOARDSIZE][BOARDSIZE] = {"202222000000000",
                                        "000000000000000",
                                        "002000000000000",
                                        "000200000000000",
                                        "000020000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000"};
    int valid_range[2][2] = {{0,0},{4,5}};
    Node* node = create_node(state,valid_range,NULL,NULL,NULL);
    List* actions = get_all_actions(node);
    print_actions(actions);
}

void test_mcts(){
    char state[BOARDSIZE][BOARDSIZE] = {"200222000000000",
                                        "001010000000000",
                                        "100010000000000",
                                        "010210000000000",
                                        "101020000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000",
                                        "000000000000000"};
    int valid_range[2][2] = {{0,0},{4,5}};
    Node* node = create_node(state,valid_range,NULL,NULL,NULL);
    Point* choice = mcts_decide(node);
    state[choice->x][choice->y] = '2';
    for (int i = 0; i!= BOARDSIZE; i++) {
        for (int j = 0 ; j!= BOARDSIZE; j++) {
            printf("%c",state[i][j]);
        }
        printf("\n");
    }
}