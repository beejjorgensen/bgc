#include <stdio.h>
#include <math.h>

int main(void)
{
    double cosx;
    long double ldcosx;

    cosx = cos(3490.0); // round and round we go!
    ldcosx = cosl(3.490L);

    printf("%f\n", cosx);
    printf("%Lf\n", ldcosx);
}

