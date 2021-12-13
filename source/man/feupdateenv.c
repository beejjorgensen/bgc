#include <stdio.h>
#include <math.h>
#include <fenv.h>

void show_status(void)
{
    printf("FE_DIVBYZERO: %d\n", fetestexcept(FE_DIVBYZERO) != 0);
    printf("FE_INVALID  : %d\n", fetestexcept(FE_INVALID) != 0);
    printf("FE_OVERFLOW : %d\n\n", fetestexcept(FE_OVERFLOW) != 0);
}

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    fenv_t env;

    feholdexcept(&env);  // Save the environment

    // Pretend some bad math happened here:
    feraiseexcept(FE_DIVBYZERO);  // Raise an exception
    feraiseexcept(FE_INVALID);    // Raise an exception
    feraiseexcept(FE_OVERFLOW);   // Raise an exception

    show_status();

    feclearexcept(FE_INVALID);

    feupdateenv(&env);  // Restore the environment

    show_status();
}
