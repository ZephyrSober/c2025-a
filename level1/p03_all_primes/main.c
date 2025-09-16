#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

void find_su(const int num, int *prime)
{
    bool is_composite[num+1] = {};

    int n_prime = 0;
    for (int i = 2; i <=num; ++i)
    {
        if (!is_composite[i])
        {
            prime[n_prime++] = i;
        }
        for (int j = 0; i*prime[j]<=num && j<=n_prime;++j)
        {
            is_composite[i*prime[j]] = 1;
            if (i%prime[j] == 0) break;
        }
    }
}

int main() {
    const int num = 1000;
    int su_list[num] = {};

    clock_t start = clock();
    find_su(num,su_list);
    for (int i = 0; i != num;++i)
    {
        if (su_list[i]!=0)
        {
            printf("%d ",su_list[i]);
        }else
        {
            break;
        }

    }
    clock_t end = clock();

    double clock_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("time used%f",clock_time);

    return 0;
}
