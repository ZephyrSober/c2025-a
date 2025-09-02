#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int get_sqrt(const int n)  //get the
{
    double d_n = n;
    int target = (int) sqrt(d_n);
    return target;
}

bool is_prime(int n)
{
    for (int i = 2;i !=get_sqrt(n)+1;i++)
    {
        if (n%i == 0)
        {
            printf("%d is not prime",n);
            return false;
        }
    }
    printf("%d is prime",n);
    return true;
}

int main() {
    int n;
    printf("input number");
    scanf("%d",&n);
    is_prime(n);
    return 0;
}