#include <stdio.h>
#include <windows.h>
#define WIDTH 50
#define FORWARD 1
#define BACKWARD -1

void run_letter(const int direction)
{
    for (int position = (direction>0)?0:WIDTH; position != (WIDTH - ((direction>0)?0:WIDTH)); position+=direction)
    {
        system("cls");
        for (int i = 0; i != WIDTH; i++)
        {
            if (i == position)
            {
                printf("a");
            }else
            {
                printf(" ");
            }
        }
        Sleep(100);
    }
}
