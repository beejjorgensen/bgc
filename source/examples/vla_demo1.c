#include <stdio.h>

int main(void)
{
    int n;

    printf("Enter a number: "); fflush(stdout);
    scanf(" %d", &n);

    int v[n];

    for (int i = 0; i < n; i++)
        v[i] = i * 10;

    for (int i = 0; i < n; i++)
        printf("v[%d] = %d\n", i, v[i]);
}
