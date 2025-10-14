#include <stdio.h>
#include <unistd.h>
#include "Gobang.h"
#include "strategy.h"

go_Game* main_game;

void game_end(){
    go_SetMsg("Game ended!");
}

void ai_play() {
    go_SetMsg("AI is thinking...");
    // get position by your algorithm
    const go_CellState (*boardData)[15] = go_GetBoardCells(go_GetGameBoard(main_game));
    char state[15][15];
    for (int i = 0; i!=15 ; i++) {
        for (int j = 0; j!=15; j++) {
            switch (boardData[i][j]) {
            case CELL_BLACK:
                state[i][j] = '1';
                break;
            case CELL_WHITE:
                state[i][j] = '2';
                break;
            case CELL_EMPTY:
                state[i][j] = '0';
                break;
            }
        }
    }
    int valid_range[2][2] = {{7,7},{7,7}};
    Node* root = create_node(state,valid_range,create_list(0,NULL),NULL,NULL);
    Point* decision = mcts_decide(root);
    go_GameAutoPlaceStone(main_game,decision->x,decision->y);
}
void human_play(){
    go_SetMsg("Your turn.");
}

int main(){
    printf("Hello, from Gobang!\n");

    go_InitWindow("Gobang");

    main_game = go_NewGame();
    go_Player* player_black = go_GetPlayerBlack(main_game);
    go_Player* player_white = go_GetPlayerWhite(main_game);
    go_PlayerSetName(player_black, "YOU");
    go_PlayerSetName(player_white, "Foolish AI");
    go_PlayerSetType(player_white, ROBOT);
    SignalSetCallback(go_PlayerTurnStart(player_white), ai_play);
    SignalSetCallback(go_PlayerTurnStart(player_black), human_play);
    SignalSetCallback(go_GetEndSignal(main_game), game_end);

    go_ActivateGame(main_game);

    while (!go_ShouldTerminate())
    {
        go_Process();
    }

    go_CloseWindow();
    go_DeleteGame(main_game);
    
    return 0;
}
