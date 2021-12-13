#include <stdio.h>
#include <math.h>
#include <fenv.h>

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    feraiseexcept(FE_DIVBYZERO);

    if (fetestexcept(FE_DIVBYZERO) == FE_DIVBYZERO)
        printf("Detected division by zero\n");  // This prints!!
    else
        printf("This is fine.\n");
}
