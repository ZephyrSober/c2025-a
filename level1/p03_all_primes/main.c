
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

int main() {
    clock_t start,end;
    double clock_time;
    const int num = 100000;
    bool state[num+1] = {};
    int prime[num+1] = {};

    start = clock();
    int n_prime = 0;

    for (int i = 2; i <=num; ++i)
    {
        if (!state[i])
        {
            prime[n_prime++] = i;
            printf("%d ",i);
        }
        for (int j = 0; i*prime[j]<=num && j<=n_prime;++j)
        {
            state[i*prime[j]] = 1;
            if ( i%prime[j]==0) break;
        }
    }

    end = clock();
    clock_time = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("time used%f",clock_time);

    return 0;
}