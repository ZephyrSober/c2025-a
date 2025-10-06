#include <stdio.h>
#include <windows.h>

#include "strategy.h"
#include "test.h"

int main() {
    // Board* board = create_empty_board();
    // draw(board);
    // input(board);
    // while (1) {//regardless of board is full
    //     Node* root = create_node(board->state,board->valid_range,create_list(0,NULL),NULL,NULL);
    //     update_state(board,mcts_decide(root)->state);
    //     free(root);
    //     root = NULL;
    //     draw(board);
    //     if (is_end(board)) {
    //         draw_end(board,true);
    //         return 0;
    //     }
    //     input(board);
    //     draw(board);
    //     if (is_end(board)) {
    //         draw_end(board,false);
    //         return 0;
    //     }
    // }
    test_mcts();
}
