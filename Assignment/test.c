#include "strategy.h"
#include "List.h"
#include <stdio.h>
#include <windows.h>

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
    char state[BOARDSIZE][BOARDSIZE] = {"202222000000000",
                                        "001010000000000",
                                        "102010000000000",
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
    Node* choice = mcts_decide(node);
    for (int i = 0; i!= BOARDSIZE; i++) {
        for (int j = 0 ; j!= BOARDSIZE; j++) {
            printf("%c",choice->state[i][j]);
        }
        printf("\n");
    }
}