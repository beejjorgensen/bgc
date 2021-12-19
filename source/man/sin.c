#include <stdio.h>
#include <math.h>

int main(void)
{
    double sinx;
    long double ldsinx;

    sinx = sin(3490.0); // round and round we go!
    ldsinx = sinl(3.490L);

    printf("%f\n", sinx);
    printf("%Lf\n", ldsinx);
}

