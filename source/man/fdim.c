#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%f\n", fdim(10.0, 3.0));   // 7.000000
    printf("%f\n", fdim(3.0, 10.0));   // 0.000000, clamped
}

