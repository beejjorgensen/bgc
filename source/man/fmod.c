#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%f\n", fmod(-9.2, 5.1));  // -4.100000
    printf("%f\n", fmod(9.2, 5.1));   //  4.100000
}
