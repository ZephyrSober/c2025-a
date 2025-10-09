#include <stdio.h>
#include <windows.h>

#include "strategy.h"
#include "test.h"

int main() {
    Board* board = create_empty_board();
    draw(board->state);
    input(board);
    while (1) {//regardless of board is full
        Node* root = create_node(board->state,board->valid_range,create_list(0,NULL),NULL,NULL);
        Point* decision = mcts_decide(root);
        update_state(board,decision->x,decision->y,'2');
        free(root);
        root = NULL;
        free(decision);
        decision = NULL;
        draw(board->state);
        if (is_terminal(board->state,board->latest_action,'2')) {
            printf("ai win");
            system("pause");
            return 0;
        }
        input(board);
        draw(board->state);
        if (is_terminal(board->state,board->latest_action,'1')) {
            printf("you win");
            system("pause");
            return 0;
        }
    }
}
