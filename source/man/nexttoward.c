#include <stdio.h>
#include <float.h>
#include <math.h>

int main(void)
{
    printf("%.*f\n", DBL_DECIMAL_DIG, nexttoward(0.5, 1.0));
    printf("%.*f\n", DBL_DECIMAL_DIG, nexttoward(0.349, 0.0));
}

