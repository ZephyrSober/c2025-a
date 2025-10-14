// #include <stdio.h>
// #include "Gobang.h"
//
// go_Game* main_game;
//
// void game_end(){
//     go_SetMsg("Game ended!");
// }
//
// void ai_play(){
//     go_SetMsg("AI is thinking...");
//     // get position by your algorithm
//     static int x = 0, y = 0, step = 0;
//     step++;
//     go_GameAutoPlaceStone(main_game, x + step, y + step);
// }
// void human_play(){
//     go_SetMsg("Your turn.");
// }
//
// int main(){
//     printf("Hello, from Gobang!\n");
//
//     go_InitWindow("Gobang");
//
//     main_game = go_NewGame();
//     go_PlayerSetName(go_GetPlayerBlack(main_game), "HJH");
//     go_PlayerSetName(go_GetPlayerWhite(main_game), "Foolish AI");
//     go_PlayerSetType(go_GetPlayerWhite(main_game), ROBOT);
//     SignalSetCallback(go_PlayerTurnStart(go_GetPlayerWhite(main_game)), ai_play);
//     SignalSetCallback(go_PlayerTurnStart(go_GetPlayerBlack(main_game)), human_play);
//     SignalSetCallback(go_GetEndSignal(main_game), game_end);
//
//     go_ActivateGame(main_game);
//
//     while (!go_ShouldTerminate())
//     {
//         go_Process();
//     }
//
//     go_CloseWindow();
//     go_TerminateGame(main_game);
//
//     return 0;
// }