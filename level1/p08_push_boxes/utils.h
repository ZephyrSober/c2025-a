#ifndef C2025_CHALLENGE_UTILS_H
#define C2025_CHALLENGE_UTILS_H
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

#endif //C2025_CHALLENGE_UTILS_H