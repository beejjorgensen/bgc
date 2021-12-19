#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%f\n", nan(""));       // nan
    printf("%f\n", nan("goats"));  // nan
    printf("%f\n", nan("!"));      // nan
}

