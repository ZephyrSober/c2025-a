#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

int get_sqrt(const int n)  //get the
{
    double d_n = n;
    int target = (int) sqrt(d_n);
    return target;
}

bool is_prime(const int n)
{
    for (int i = 2;i !=get_sqrt(n)+1;i++)
    {
        if (n%i == 0)
        {
            return false;
        }
    }
    return true;
}

int main() {
    clock_t start,end;
    double clock_time;
    const int num = 100000;

    start = clock();
    for (int n = 2; n <= num; n++)
    {
        if (is_prime(n))
        {
            printf("%d ",n);
        }
    }
    end = clock();
    clock_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("time used%f",clock_time);

    return 0;
}
