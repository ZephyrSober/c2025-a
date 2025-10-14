#ifndef GOBANG_H
#define GOBANG_H

// 英文注释是写代码的时候一起敲的（懒得切输入法），中文的是写完补的

#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false = 0, true = !false } bool;
    #define RL_BOOL_TYPE
#endif

typedef struct go_Pos{
    int x, y;
} go_Pos;

// 程序窗口部分
void go_InitWindow(const char* title);   // Start user interface.
bool go_ShouldTerminate();  // Return true only if window is closed.
void go_CloseWindow();
void go_Process();  //Should be called in each loop in order to update and react to user input.
void go_SetMsg(const char* txt);

// 定义状态机
typedef enum go_CellState {
    CELL_BLACK,
    CELL_WHITE,
    CELL_EMPTY,
    CELL_ERROR, // Reserved for invalid visits.
} go_CellState;

typedef enum go_GameState {
    GAME_BLACK_TURN,
    GAME_WHITE_TURN,
    GAME_ENDED,
    GAME_ERROR, // Reserved for invalid visits.
} go_GameState;

typedef enum go_PlayerType {
    ROBOT,
    HUMAN,
    UNKNOWN_PLAYER_TYPE,
} go_PlayerType;

typedef enum go_PlayerSide {
    PLAYER_BLACK_SIDE,
    PLAYER_WHITE_SIDE,
    PLAYER_UNKONWN_SIDE,
} go_PlayerSide;

// 信号与回调
typedef void (*VoidFunc)(); // 函数指针的一层包装，基本等价于std::function<void(void)>
typedef struct Signal Signal; // 信号机制，可以为事件注册回调（不要问我哪里学的，问就是Godot）
Signal* NewSignal();
void SignalSetCallback(Signal* signal, VoidFunc func);
void SignalEmit(Signal* signal);

// 数据类型：棋盘
// 不允许用户直接构造。
typedef struct go_Board go_Board;
go_CellState go_GetCellState(go_Board* board, unsigned x, unsigned y);   // Get cell state by coordinate(xy).
void go_SetCellState(go_Board* board, unsigned x, unsigned y, go_CellState state);  // Not recommended during a game. Use "go_PlaceStone" instead.
const go_CellState (*go_GetBoardCells(go_Board* board))[15];
void go_ClearBoard(go_Board* board);

// 数据类型：玩家
// 不允许用户直接构造。
typedef struct go_Player go_Player;
void go_PlayerSetName(go_Player* player, const char* name);
const char* go_PlayerGetName(go_Player* player);
void go_PlayerSetType(go_Player* player, go_PlayerType type);
go_PlayerType go_PlayerGetType(go_Player* player);
Signal* go_PlayerTurnStart(go_Player* player);
Signal* go_PlayerTurnEnd(go_Player* player);
bool go_PlayerPlaceStone(go_Player* player, unsigned x, unsigned y);  // Return true while move is valid, and false while invalid. Invalid move won't be recorded.

// 数据类型：游戏
// 一局游戏包含一个棋盘两名玩家，自动创建。你可以通过获取它们的指针以进行动作。
// 你可以在一个进程中同时创建多局游戏。但是同一时间只能在屏幕上展示一局。
typedef struct go_Game go_Game;
go_Game* go_NewGame();    // Start a new game. Can be called at any time.
void go_StartGame(go_Game* game);   // This function simply makes signal emit, while game can still be processed without this.
void go_ActivateGame(go_Game* game);   // Activated game will be shown on screen.
bool go_IsGameActivated(go_Game* game);
go_Game* go_GetActiveGame();
void go_GameRestart(go_Game* game); // Restart a game without creating a new one.
Signal* go_GetStartSignal(go_Game* game);
Signal* go_GetEndSignal(go_Game* game);
go_GameState go_GetGameState(go_Game* game);
go_Board* go_GetGameBoard(go_Game* game);
go_Player* go_GetPlayerBlack(go_Game* game);
go_Player* go_GetPlayerWhite(go_Game* game);
go_Player* go_GetCurrentPlayer(go_Game* game);
go_Player* go_GetWinner(go_Game* game);
bool go_GameAutoPlaceStone(go_Game* game, unsigned x, unsigned y);  // Current player moves. Return if placed successfully.
go_Pos go_GetLastMove(go_Game* game);
void go_DeleteGame(go_Game* game); // This will release all resource.

#endif
