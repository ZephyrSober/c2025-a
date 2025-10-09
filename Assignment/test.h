#ifndef C2025_CHALLENGE_TEST_H
#define C2025_CHALLENGE_TEST_H
#include "strategy.h"
struct Board {
    char state[BOARDSIZE][BOARDSIZE];
    Point* latest_action;
    int valid_range[2][2];
};
typedef struct Board Board;
Board* create_empty_board();
void draw(char state[BOARDSIZE][BOARDSIZE]);
void input(Board* board);
void update_state(Board* board, int x ,int y,char player);

void test_get_all_actions();
void test_is_terminal();
void test_mcts();
#endif //C2025_CHALLENGE_TEST_H