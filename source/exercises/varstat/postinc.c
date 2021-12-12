#include <stdio.h>

int main(void)
{
    int i = 2;
    int j = i++ + 7;  // post-increment

    printf("%d %d\n", i, j);  // 3 9
}
