#include <stdio.h>

int funcxy(int x, int y)
{
    return x * 20 + x * y;
}

int main(void)
{
    printf("%d\n", funcxy(1, 2));
}
