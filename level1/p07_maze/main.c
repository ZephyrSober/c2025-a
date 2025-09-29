#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HEIGHT 8
#define WIDTH 8
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

struct Point {
    int x;
    int y;
};
typedef struct Point Point;

typedef enum{wall, player, door,air} Object;
void creat_map(int h, int w,Object map[][w], char* shape, Point* player_p);
void draw(int h, int w,Object map[][w]);
int move(int dx, int dy,int h, int w,Object map[][w],Point* player_p);

int main() {
    Point player_p = {3,1};
    char* map_shape = {"********"
                       "*      *"
                       "*      *"
                       "*      *"
                       "*      *"
                       "*      *"
                       "*      0"
                       "********\0"};
    Object map[HEIGHT][WIDTH];
    creat_map(HEIGHT,WIDTH,map,map_shape,&player_p);

    char act;
    printf("enjoy it\npress any key to start");
    act = getchar();
    draw(HEIGHT,WIDTH,map);
    while ((act = getchar()) != 'q') {
        int result =0;
        switch (act) {
        case UP:
            result = move(0,-1,HEIGHT,WIDTH,map,&player_p);
            break;
        case DOWN:
            result = move(0,1,HEIGHT,WIDTH,map,&player_p);
            break;
        case LEFT:
            result = move(-1,0,HEIGHT,WIDTH,map,&player_p);
            break;
        case RIGHT:
            result = move(1,0,HEIGHT,WIDTH,map,&player_p);
            break;
        }
        draw(HEIGHT,WIDTH,map);
        if (result == 2) break;
    }
    printf("gg, you win");
    system("pause");
    return 0;
}

void creat_map(int h, int w,Object map[][w],char* shape,Point* Player_p) {
    if (strlen(shape)!=w*h) {
        printf("invalid map size");
        system("pause");
        return;
    }
    int cnt = 0;
    for (int i= 0;i!= h; i++) {
        for (int j = 0;j != w; j++) {
            switch (shape[cnt]) {
            case '*':
                map[i][j] = wall;
                break;
            case '0':
                map[i][j] = door;
                break;
            case ' ':
                map[i][j] = air;
                break;
            default:
                printf("invalid map sign");
                return;
            }
            cnt++;
        }
    }
    map[Player_p->y][Player_p->x] = player;
}

void draw(int h, int w,Object map[][w]) {
    system("cls");
    for (int i= 0;i!= h; i++) {
        for (int j = 0;j != w; j++) {
            switch (map[i][j]) {
            case wall:
                printf("*");
                break;
            case player:
                printf("1");
                break;
            case door:
                printf("0");
                break;
            case air:
                printf(" ");
                break;
            default:
                printf("invalid map");
                return;
            }
        }
        printf("\n");
    }
}

int move(int dx, int dy,int h, int w,Object map[][w],Point* player_p) {
    int new_x = player_p->x + dx, new_y = player_p->y + dy;
    switch (map[new_y][new_x]) {
    case wall:
        return 0;
    case air:
        map[player_p->y][player_p->x] = air;
        player_p->x += dx;
        player_p->y += dy;
        map[player_p->y][player_p->x] = player;
        return 1;
    case door:
        return 2;
    }
}