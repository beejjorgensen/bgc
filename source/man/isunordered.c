#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%d\n", isunordered(1.0, 2.0));       // 0
    printf("%d\n", isunordered(1.0, sqrt(-1)));  // 1
    printf("%d\n", isunordered(NAN, 30.0));      // 1
    printf("%d\n", isunordered(NAN, NAN));       // 1
}

