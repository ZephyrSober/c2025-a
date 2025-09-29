#include <stdio.h>
#include <windows.h>
#include "utils.h"
#define WIDTH 50
#define FORWARD 1
#define BACKWARD -1


int main() {
    while (1)
    {
        run_letter(FORWARD);
        run_letter(BACKWARD);
    }
    return 0;
}