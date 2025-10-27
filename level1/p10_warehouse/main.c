#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DATAPATH "D:/1FILE/Program/cpp/c2025-a/level1/p10_warehouse/data.txt"
#define MENU "=== Inventory Management System ===\n1. Show inventory\n2. Add item\n3. Delete item\n4. Quit\ninput commond:"

int main() {
    printf(MENU);
    char c;
    c = _getch();
    while (c!='4') {
        system("cls");
        switch (c) {
        case '1': {
            printf("show\n");
            FILE *file = fopen(DATAPATH, "r");
            int id;
            int num;
            while (fscanf(file, "%d %d\n",
                  &id,&num) != EOF) {
                printf("ID: %d, Num: %d\n",id,num);
                  }
            fclose(file);
            break;
        }
        case '2': {
            printf("add things\n");
            FILE *file = fopen(DATAPATH, "a");
            int id;
            int num;
            printf("input ID and num");
            scanf("%d %d", &id, &num);
            fprintf(file, "\n%d %d", id, num);
            fclose(file);
            break;
        }
        case '3': {
            printf("delete things\n");
            FILE *origin = fopen(DATAPATH, "r");
            FILE *temp = fopen("temp.txt", "w");
            char line[256];
            int currentId;
            int num;
            int found = 0;
            int targetId;
            printf("input ID\n");
            scanf("%d", &targetId);
            while (fscanf(origin, "%d %d\n",&currentId,&num) != EOF) {
                if (currentId != targetId) {
                    fprintf(temp, "%d %d\n",currentId,num);
                }else {
                    found = 1;
                    break;
                }

            }
            fclose(origin);
            fclose(temp);
            if (!found) {
                printf("did not find id=%d 的记录\n", targetId);
                remove("temp.txt"); // 删除临时文件
                break;
            }
            if (remove(DATAPATH) != 0) {
                printf("无法删除原文件\n");
                break;
            }
            if (rename("temp.txt", DATAPATH) != 0) {
                printf("无法重命名文件\n");
                break;
            }
            printf("successfully delete id=%d\n", targetId);
            break;
            }
        case '4':
            printf("quit");
            return 0;
        default:
            printf("error");
        }
        system("pause");
        system("cls");
        printf(MENU);
        c=_getch();
    }
    return 0;
}
