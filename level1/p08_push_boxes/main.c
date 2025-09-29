#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "utils.h"
#define HEIGHT 8
#define WIDTH 8
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'


int main() {
    Point player_p = {3,1};
    const int box_size = 1;
    Point boxes[box_size] = {};
    for (int i = 0; i!=box_size;i++) {
        boxes[i].x = 3;
        boxes[i].y = 2;
    }
    char* map_shape = {"********"
                       "*      *"
                       "*      *"
                       "*      *"
                       "*      *"
                       "*   0  *"
                       "*      *"
                       "********\0"};
    Object map[HEIGHT][WIDTH];
    creat_map(HEIGHT,WIDTH,map,map_shape,&player_p,boxes,box_size);

    char act;
    printf("enjoy it\npress any key to start");
    act = _getch();
    draw(HEIGHT,WIDTH,map);
    int num_right = 0;
    while ((act = _getch()) != 'q') {
        int step_result =0;
        switch (act) {
        case UP:
            step_result = move(0,-1,HEIGHT,WIDTH,map,&player_p,boxes,box_size);
            break;
        case DOWN:
            step_result = move(0,1,HEIGHT,WIDTH,map,&player_p,boxes,box_size);
            break;
        case LEFT:
            step_result = move(-1,0,HEIGHT,WIDTH,map,&player_p,boxes,box_size);
            break;
        case RIGHT:
            step_result = move(1,0,HEIGHT,WIDTH,map,&player_p,boxes,box_size);
            break;
        }
        draw(HEIGHT,WIDTH,map);
        if (step_result == 2) num_right++;
        if (step_result == 3) num_right--;
        if (num_right == box_size) break;
    }
    printf("gg, you win");
    system("pause");
    return 0;
}


