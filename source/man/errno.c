#include <stdio.h>
#include <math.h>
#include <errno.h>

int main(void)
{
    double x;

    errno = 0;       // Make sure this is clear before the call

    x = acos(2.0);   // Invalid argument to acos()

    if (errno == EDOM)
        perror("acos");
    else
        printf("Answer is %f\n", x);

    return 0;
}
