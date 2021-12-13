#include <stdio.h>
#include <math.h>
#include <fenv.h>

void show_status(void)
{
    printf("Rounding is FE_TOWARDZERO: %d\n",
           fegetround() == FE_TOWARDZERO);

    printf("FE_DIVBYZERO is set: %d\n",
           fetestexcept(FE_DIVBYZERO) != 0);
}

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    fenv_t env;

    fegetenv(&env);  // Save the environment

    fesetround(FE_TOWARDZERO);    // Change rounding
    feraiseexcept(FE_DIVBYZERO);  // Raise an exception

    show_status();

    fesetenv(&env);  // Restore the environment

    show_status();
}
