#include <stdio.h>

float pi(int n)
{
    float pi = 0;
    int sign = 1;

    for (int i = 0; i < n; i++) {
        pi += sign * (1.0 / (i * 2 + 1));
        sign = -sign;
    }

    return 4 * pi;
}

int main(void)
{
    for (int i = 1; i <= 5000; i++)
        printf("%d %f\n", i, pi(i));
}
