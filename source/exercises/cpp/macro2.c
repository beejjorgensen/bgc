#include <stdio.h>

#define M(x, y) ((x) * (y) + __LINE__)

int main(void)
{
    printf("%d\n", M(1, 2));   // 9
    printf("%d\n", M(1, 2));   // 10
    printf("%d\n", M(34, 90)); // 3069
    printf("%d\n", M(34, 90)); // 3070
}
