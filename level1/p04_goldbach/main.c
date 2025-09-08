#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void get_primes(const int num_primes, int *prime)
{
    bool state[num_primes+1] = {};

    int known_primes = 0;
    for (int i = 2; i <=num_primes; ++i)
    {
        if (!state[i])
        {
            prime[known_primes++] = i;
        }
        for (int p = 0; i*prime[p]<=num_primes && p<=known_primes;++p)
        {
            state[i*prime[p]] = 1;
            if (i%prime[p] == 0) break;
        }
    }
}

int get_sqrt(const int n)  //get the
{
    double d_n = n;
    int target = (int) sqrt(d_n);
    return target;
}

bool is_prime(const int n)
{
    if (n<=1)
    {
        printf("n is a invalid number");
        return false;
    }
    for (int i = 2;i != get_sqrt(n)+1;i++)
    {
        if (n%i == 0)
        {
            return false;
        }
    }
    return true;
}

int main() {
    const int NUM_PRIMES = 100;
    int primes[NUM_PRIMES] = {};
    get_primes(NUM_PRIMES,primes);
    for (int i = 4; i<=100;i+=2)
    {
        for (int j = 0;j!=100;++j)
        {
            if (is_prime(i-primes[j]))
            {
                printf("%d = %d + %d\n",i,primes[j],i-primes[j]);
                break;
            }
        }
    }

    return 0;
}