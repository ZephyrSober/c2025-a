#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define HEIGHT 8
#define WIDTH 8
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

typedef enum{wall, player, target,air,box,shadow_wrong,shadow_right} Object;
struct Point {
    int x;
    int y;
};
typedef struct Point Point;

void creat_map(int h, int w,Object map[][w], char* shape, Point* player_p, Point* boxes, int box_size);
void draw(int h, int w,Object map[][w]);
int move(int dx, int dy,int h, int w,Object map[][w],Point* entity, Point* boxes, int box_size);
void step(int dx, int dy,int h, int w,Object map[][w],Point* entity,Object origin, Object destination);


void creat_map(int h, int w,Object map[][w],char* shape,Point* Player_p, Point* boxes, int box_size) {
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
                map[i][j] = target;
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
    for (int i = 0; i != box_size; i++) {
        map[boxes[i].y][boxes[i].x] = box;
    }
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
            case target:
                printf("0");
                break;
            case air:
                printf(" ");
                break;
            case box:
                printf("b");
                break;
            case shadow_wrong:
                printf("1");
                break;
            case shadow_right:
                printf("b");
                break;
            default:
                printf("invalid map");
                return;
            }
        }
        printf("\n");
    }
}

int move(int dx, int dy,int h, int w,Object map[][w],Point* entity, Point* boxes, int box_size) {
    int new_x = entity->x + dx, new_y = entity->y + dy;
    switch (map[new_y][new_x]) {
    case wall:
        return 0;
    case air:
        Object origin_entity_sign = map[entity->y][entity->x];
        if (origin_entity_sign == player||origin_entity_sign == box) {
            step(dx,dy,h,w,map,entity,air,origin_entity_sign);
            return 1;
        }
        if (origin_entity_sign == shadow_wrong) {
            step(dx,dy,h,w,map,entity,target,player);
            return 1;
        }
        if (origin_entity_sign == shadow_right) {
            step(dx,dy,h,w,map,entity,target,box);
            return 3;
        }
    case box:
        Point* target_box = NULL;
        for (int i = 0; i != box_size; i++) {
            if (boxes[i].x==new_x&&boxes[i].y==new_y) {
                target_box = &boxes[i];
            }
        }
        switch (move(dx, dy,h, w,map,target_box,boxes,box_size)) {
        case 0:
            return 0;
        case 1:
            return move(dx, dy,h, w,map,entity,boxes,box_size);
        case 2:
            move(dx, dy,h, w,map,entity,boxes,box_size);
            return 2;
        default:
            return -1;
        }
    case target:
        if (map[entity->y][entity->x]==box) {
            step(dx,dy,h,w,map,entity,air,shadow_right);
            return 2;
        }
        if (map[entity->y][entity->x]==player) {
            step(dx,dy,h,w,map,entity,air,shadow_wrong);
            return 1;
        }
    default:
        return -1;
    }
}

void step(int dx, int dy,int h, int w,Object map[][w],Point* entity,Object origin, Object destination) {
    map[entity->y][entity->x] = origin;
    entity->x += dx;
    entity->y += dy;
    map[entity->y][entity->x] = destination;
}