#include <stdio.h>

void swap(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

int main(void)
{
    int a = 10, b = 20;

    swap(&a, &b);

    printf("%d, %d\n", a, b); // Should print 20, 10
}
