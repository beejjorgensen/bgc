#include <stdio.h>

#define X 30

int main(void)
{
#if X > 30
    printf("X is greater than 30\n");
#else
    for (int i = 0; i < X; i++)
        printf("%d\n", i);
#endif
}
