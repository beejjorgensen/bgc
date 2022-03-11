#include <stdio.h>

int main(void)
{
    unsigned char x = 0;

    for (int i = 0; i < 260; i++) {
        printf("%d\n", x++);
    }
}
