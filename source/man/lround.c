#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%ld\n", lround(3.14));   // 3
    printf("%ld\n", lround(3.5));    // 4

    printf("%ld\n", lround(-1.5));   // -2
    printf("%ld\n", lround(-1.14));  // -1
}
