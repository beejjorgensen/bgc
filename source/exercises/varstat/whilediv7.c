#include <stdio.h>

int main(void)
{
    int i = 20754371;
    int count = 0;

    while (i != 0) {
        i /= 7;
        count++;
    }

    printf("%d\n", count);  // 9
}
