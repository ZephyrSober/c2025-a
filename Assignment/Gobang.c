#include "Gobang.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <raylib.h>

// go_ 前缀是模拟命名空间

// 多线程开关（修改后需重编译）
// #define MULTI_THREAD
//
// #ifdef MULTI_THREAD
// #include <pthread.h>
// pthread_t logic_thread_id;
// #endif

// 这些本来是写在后面的，但是后来修改代码发现前面也需要，就加个声明（懒得改位置）
extern go_Game* activated_game;
void* _process_logic();

// 关于界面的参数
int player1y = 110;
int player2y = 210;
int gap = 50;
int chess_radius = 12;
int text_font = 28;
int msgy = 400;
int msggap = 30;
char msg[1001];
int board_area = 600;
int board_size = 570;
int cell_size = 38;
int board_color = 0xfedcbdff;
int normal_thick = 2;
float preview_alpha = 0.6;
Color current_player_color = DARKPURPLE;
Rectangle cell_areas[15][15];

// 初始化界面与资源参数
void go_InitWindow(const char* title) {
	InitWindow(900, 600, title);
    SetTargetFPS(60);
    SetTraceLogLevel(LOG_ERROR);
    activated_game = NULL;
    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 15; j++){
            cell_areas[i][j] = (Rectangle){
                (board_area - board_size) / 2 + i * cell_size,
                (board_area - board_size) / 2 + j * cell_size,
                cell_size, cell_size,
            };
        }
    }
#ifdef MULTI_THREAD
    if(pthread_create(&logic_thread_id, NULL, _process_logic, NULL)){
        puts("ERROR: Logic thread failed to create");
    }else{
        puts("INFO: Logic thread started");
    }
#endif
}
// 打包，不对外展示 raylib 以防命名冲突
bool go_ShouldTerminate() {
	return WindowShouldClose();
}
void go_CloseWindow() {
#ifdef MULTI_THREAD
// 多线程时需要额外清理
    puts("INFO: Killing logic thread");
    pthread_cancel(logic_thread_id);
    pthread_join(logic_thread_id, NULL);
    puts("INFO: Logic thread killed");
#endif
	CloseWindow();
}
// 右下角的消息展示
void go_SetMsg(const char* txt) {
    if(sizeof(msg) > 1001)
        return;
    strcpy(msg, txt);
}


// 信号类（实用工具）
struct Signal {
    VoidFunc callback_func;
#ifdef MULTI_THREAD
    pthread_mutex_t rs_mutex;
#endif
};
// 伪构造函数
void _signal_init(Signal* _signal, VoidFunc _func) {
    _signal->callback_func = _func;
#ifdef MULTI_THREAD
    _signal->rs_mutex = pthread_mutex_init(&_signal->rs_mutex, NULL);
#endif
}
// 伪析构函数
void _signal_destroy(Signal* _signal) {
    return;
}
#ifdef MULTI_THREAD
// 多线程再包装
void* _signal_emit(void* _signal){
    ((Signal*)_signal)->callback_func();
    return 0;
}
#endif
void SignalSetCallback(Signal* signal, VoidFunc func) {
	signal->callback_func = func;
}
void SignalEmit(Signal* signal) {
	if(signal->callback_func) {
#ifdef MULTI_THREAD
        pthread_t id;
        pthread_create(&id, NULL, _signal_emit, signal);
        pthread_detach(id);
#else
        (*signal).callback_func();
#endif
    }
}


// 棋盘类
struct go_Board {
    go_CellState cell[15][15];
};
// ...
void _board_init(go_Board* _board) {
    go_ClearBoard(_board);
}
// ...
void _board_destroy(go_Board* _board) {
    return;
}
go_CellState go_GetCellState(go_Board* board, unsigned x, unsigned y) {
	if(x < 15 && y < 15)
		return board->cell[x][y];
	else
		return CELL_ERROR;
}
void go_SetCellState(go_Board* board, unsigned x, unsigned y, go_CellState state) {
	if(x < 15 && y < 15)
		board->cell[x][y] = state;
}
const go_CellState (*go_GetBoardCells(go_Board* board))[15] {
    return board->cell;
}
void go_ClearBoard(go_Board* board) {
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            board->cell[i][j] = CELL_EMPTY;
}


// 玩家类
struct go_Player {
	go_Game* in_game;
    char* name;
    Signal turn_start, turn_end;
    go_PlayerType type;
    go_PlayerSide side;
    // Something else...
};
// ...
void _player_init(go_Player* _player, go_Game* _in_game, go_PlayerType _type, go_PlayerSide _side) {
    _player->in_game = _in_game;
    _player->name = MemAlloc(sizeof("Unknown"));
    strcpy(_player->name, "Unknown");
    _player->type = _type;
    _player->side = _side;
    _signal_init(&_player->turn_start, NULL);
    _signal_init(&_player->turn_end, NULL);
}
// ...
void _player_destroy(go_Player* _player) {
    _signal_destroy(&_player->turn_start);
    _signal_destroy(&_player->turn_end);
    if(_player->name){
        MemFree(_player->name);
    }
}
void go_PlayerSetName(go_Player* player, const char* name) {
    if(player->name){
        MemFree(player->name);
    }
    player->name = MemAlloc(sizeof(name));
	strcpy(player->name, name);
}
const char* go_PlayerGetName(go_Player* player) {
	return player->name;
}
void go_PlayerSetType(go_Player *player, go_PlayerType type) {
    player->type = type;
}
go_PlayerType go_PlayerGetType(go_Player *player) {
    return player->type;
}
Signal *go_PlayerTurnStart(go_Player *player)
{
    return &((*player).turn_start);
}
Signal* go_PlayerTurnEnd(go_Player* player) {
	return &player->turn_end;
}
// 玩家落子。落子前检查落子行为的合法性。
bool go_PlayerPlaceStone(go_Player* player, unsigned x, unsigned y) {
	if(go_GetCurrentPlayer(player->in_game) != player)
		return false;
	return go_GameAutoPlaceStone(player->in_game, x, y);
}


// 游戏类
struct go_Game {
    go_Board board;
    go_Player players[2];
    go_Player* winner;
    go_GameState state;
    Signal game_start, game_end;
    go_Pos last_pos;
} *activated_game = NULL;
// ...
void _game_init(go_Game* _game) {
    _board_init(&_game->board);
	for(int i = 0; i < 2; i++){
        _player_init(_game->players + i, _game, HUMAN, i);
    }
    _game->winner = NULL;
    _game->state = GAME_BLACK_TURN;
    _signal_init(&_game->game_start, NULL);
    _signal_init(&_game->game_end, NULL);
    _game->last_pos = (go_Pos){-1,-1};
}
// ...
void _game_destroy(go_Game* _game){
    _board_destroy(&_game->board);
    for(int i = 0; i < 2; i++){
        _player_destroy(_game->players + i);
    }
    _signal_destroy(&_game->game_start);
    _signal_destroy(&_game->game_end);
}
go_Game* go_NewGame() {
	go_Game* game = MemAlloc(sizeof(go_Game));
	_game_init(game);
    return game;
}
void go_StartGame(go_Game* game) {
    game->state = GAME_BLACK_TURN;
    game->winner = NULL;
    game->last_pos = (go_Pos){-1,-1};
    SignalEmit(&game->game_start);
}
void go_ActivateGame(go_Game* game) {
    activated_game = game;
}
bool go_IsGameActivated(go_Game* game) {
    return activated_game == game;
}
go_Game *go_GetActiveGame() {
    return activated_game;
}
void go_GameRestart(go_Game *game) {
    SignalEmit(&(game->game_end));
    go_ClearBoard(&(game->board));
    game->winner = NULL;
    game->state = GAME_BLACK_TURN;
    game->last_pos = (go_Pos){-1,-1};
    SignalEmit(&(game->game_start));
    SignalEmit(&((*go_GetPlayerBlack(game)).turn_start));
}
Signal *go_GetStartSignal(go_Game *game) {
    return &(game->game_start);
}
Signal *go_GetEndSignal(go_Game *game) {
    return &(game->game_end);
}
go_GameState go_GetGameState(go_Game *game) {
    return game->state;
}
go_Board *go_GetGameBoard(go_Game *game) {
    return &(game->board);
}
go_Player *go_GetPlayerBlack(go_Game *game) {
    return &(game->players[0]);
}
go_Player *go_GetPlayerWhite(go_Game *game) {
    return &(game->players[1]);
}
go_Player *go_GetCurrentPlayer(go_Game *game) {
    if(game->state < 2)
        return &(game->players[game->state]);
    else
        return NULL;
}
go_Player *go_GetWinner(go_Game *game) {
    return game->winner;
}
bool check_if_game_has_ended(go_Game* game, unsigned x, unsigned y);    // 后面补的声明
// 获取指定游戏的当前行棋玩家，并检验给定步的合法性（唯一真实改变棋盘状态的函数（为了统一管理））
bool go_GameAutoPlaceStone(go_Game *game, unsigned x, unsigned y) {
    if(game->state >= GAME_ENDED) // if(game->state != GAME_BLACK_TURN && game->state != GAME_WHITE_TURN)
        {return false;}
    if(x >= 15 || y >= 15)
        {return false;}
    go_Board* game_board = &(game->board);
    go_Player* current_player = &(game->players[game->state]);
    if((*game_board).cell[x][y] == CELL_EMPTY) {
        (*game_board).cell[x][y] = (*current_player).side;
        game->last_pos = (go_Pos){x, y};
        SignalEmit(&((*current_player).turn_end));
        if(!check_if_game_has_ended(game, x, y)){
            game->state = (game->state + 1) & 1;    // 切换游戏状态
            current_player = &(game->players[game->state]);
            SignalEmit(&((*current_player).turn_start));
        } else {
            game->state = GAME_ENDED;
            game->winner = current_player;
            SignalEmit(&(game->game_end));
        }
    } else {
        return false;
    }
    return true;
}
go_Pos go_GetLastMove(go_Game *game){
    return game->last_pos;
}
void go_DeleteGame(go_Game *game) {
    SignalEmit(&game->game_end);
    _game_destroy(game);    // 先析构再释放
    MemFree(game);
}



// 画棋盘本体
void DrawBoard() {
    DrawRectangle((board_area - board_size) / 2, (board_area - board_size) / 2, board_size, board_size, GetColor(board_color));
    DrawRectangleLinesEx((Rectangle){
                            (board_area - board_size + cell_size) / 2 - normal_thick,
                            (board_area - board_size + cell_size) / 2 - normal_thick,
                            board_size - cell_size + 2 * normal_thick,
                            board_size - cell_size + 2 * normal_thick,
                        }, normal_thick * 2, BLACK);
    for(int i = (board_area - board_size + cell_size) / 2 + cell_size; i < (board_area - board_size - cell_size) / 2 + board_size; i += cell_size){
        DrawLineEx(
                    (Vector2){i, (board_area - board_size + cell_size) / 2},
                    (Vector2){i, (board_area - board_size - cell_size) / 2 + board_size},
                    normal_thick, BLACK);
        DrawLineEx(
                    (Vector2){(board_area - board_size + cell_size) / 2, i},
                    (Vector2){(board_area - board_size - cell_size) / 2 + board_size, i},
                    normal_thick, BLACK);
    }
    DrawCircle(board_area / 2, board_area / 2, normal_thick * 2, BLACK);
    DrawCircle(board_area / 2 + 4 * cell_size, board_area / 2 - 4 * cell_size, normal_thick * 2, BLACK);
    DrawCircle(board_area / 2 - 4 * cell_size, board_area / 2 + 4 * cell_size, normal_thick * 2, BLACK);
    DrawCircle(board_area / 2 + 4 * cell_size, board_area / 2 + 4 * cell_size, normal_thick * 2, BLACK);
    DrawCircle(board_area / 2 - 4 * cell_size, board_area / 2 - 4 * cell_size, normal_thick * 2, BLACK);
}

// 绘制玩家信息
void DrawPlayer() {
    if(activated_game){
        go_Player* pb = go_GetPlayerBlack(activated_game);
        go_Player* pw = go_GetPlayerWhite(activated_game);
        DrawText((*pb).name, board_area + gap * 2, player1y - text_font / 2, text_font, go_GetCurrentPlayer(activated_game) == pb ? current_player_color : BLACK);
        DrawText((*pw).name, board_area + gap * 2, player2y - text_font / 2, text_font, go_GetCurrentPlayer(activated_game) == pw ? current_player_color : BLACK);
    }
}

// 非公开，用函数指针实现绘制方法的统一管理
void draw_chess_b(unsigned x, unsigned y, float alpha) {
    DrawCircle(
        (board_area - board_size + cell_size) / 2 + x * cell_size,
        (board_area - board_size + cell_size) / 2 + y * cell_size,
        chess_radius, ColorAlpha(BLACK, alpha)
    );
}
void draw_chess_w(unsigned x, unsigned y, float alpha) {
    DrawCircle(
        (board_area - board_size + cell_size) / 2 + x * cell_size,
        (board_area - board_size + cell_size) / 2 + y * cell_size,
        chess_radius, ColorAlpha(WHITE, alpha)
    );
}
// 实际入口（指针）
void (*draw_chess[2])(unsigned x, unsigned y, float alpha) = {draw_chess_b, draw_chess_w};

// 交互逻辑与绘制逻辑的共享信息
bool should_draw_preview;
go_Pos mouse_board_pos;

//交互逻辑
void* _process_logic() {
    Vector2 mousepos;
#ifdef MULTI_THREAD
    while(!WindowShouldClose()){
#endif
        mousepos = GetMousePosition();
        should_draw_preview = false;
        if((*activated_game).state < GAME_ENDED && (*go_GetCurrentPlayer(activated_game)).type == HUMAN){
            if(CheckCollisionPointRec(mousepos, (Rectangle){(board_area - board_size) / 2, (board_area - board_size) / 2, board_size, board_size})){
                for(int i = 0; i < 15; i++){
                    for(int j = 0; j < 15; j++){
                        if(CheckCollisionPointRec(mousepos, cell_areas[i][j])){
                            if(should_draw_preview = (go_GetCellState(go_GetGameBoard(activated_game), i, j) == CELL_EMPTY)){
                                mouse_board_pos = (go_Pos){i, j};
                            }
                            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                                go_GameAutoPlaceStone(activated_game, i, j);
                            }
                            // 跳出两层循环。理由是鼠标只有一个。
                            goto END;
                        }
                    }
                }
                END:
            }
        }
#ifdef MULTI_THREAD
        usleep(16667);
    }
#endif
    return 0;
}

void go_Process() {
	BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawBoard();
        DrawCircle(board_area + gap, player1y, chess_radius, BLACK);
        DrawCircle(board_area + gap, player2y, chess_radius, BLACK);
        DrawCircle(board_area + gap, player2y, chess_radius - normal_thick, WHITE);
        DrawText(msg, board_area + msggap, msgy, text_font, BLACK);
        if(activated_game){
            DrawPlayer();
            go_Board* board = &((*activated_game).board);
            for(int i = 0; i < 15; i++){
                for(int j = 0; j < 15; j++){
                    if((*board).cell[i][j] < CELL_EMPTY) {
                        draw_chess[(*board).cell[i][j]](i, j, 1.0);
                    }
                }
            }
            if((*activated_game).state < GAME_ENDED && should_draw_preview){
                draw_chess[(*go_GetCurrentPlayer(activated_game)).side](mouse_board_pos.x, mouse_board_pos.y, preview_alpha);
                HideCursor();
            }else{
                ShowCursor();
            }
            go_Player* playerB = (*activated_game).players+0;
            go_Player* playerW = (*activated_game).players+1;
        }
    }
    EndDrawing();
#ifndef MULTI_THREAD
        _process_logic();
#endif
}

// 判断对局是否结束。假设了调用前对局没有结束，则仅当前步会造成影响。
// 直接数的。封装好的状态获取函数规避了越界问题，因此无需再判断。
bool check_if_game_has_ended(go_Game* game, unsigned x, unsigned y){
    go_Board* board = &(game->board);
    go_CellState current = go_GetCellState(board, x, y);
    static Vector2 directions[4] = {
        {1,0},
        {1,1},
        {0,1},
        {-1,1},
    };
    int line_cnt = 0;
    for(int i = 0; i < 4; i++){
        Vector2 drct = directions[i];
        int cnt = 0;
        for(cnt = 0; go_GetCellState(board, x + drct.x * cnt, y + drct.y * cnt) == current; cnt++);
        line_cnt += cnt - 1;
        for(cnt = 0; go_GetCellState(board, x - drct.x * cnt, y - drct.y * cnt) == current; cnt++);
        line_cnt += cnt;
        if(line_cnt >= 5) return true;
        line_cnt = 0;
    }
    return false;
}