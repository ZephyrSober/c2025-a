#include "utils.h"
#include <stdlib.h>

Point* create_point(int x,int y) {
    Point* point = (Point*)malloc(sizeof(Point));
    point->x = x;
    point->y = y;
    return point;
}
