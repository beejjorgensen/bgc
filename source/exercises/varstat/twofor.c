#include <stdio.h>

int main(void)
{
    for (int i = 0, j = 0; i < 100; i++, j += i)
        printf("%d %d\n", i, j);
}
