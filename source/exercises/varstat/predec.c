#include <stdio.h>

int main(void)
{
    int i = 2;
    int j = --i + 7;  // pre-decrement

    printf("%d %d\n", i, j);  // 1 8
}
