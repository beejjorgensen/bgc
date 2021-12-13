#include <stdio.h>
#include <math.h>
#include <fenv.h>

// Helper function to print the rounding mode
const char *rounding_mode_str(int mode)
{
    switch (mode) {
        case FE_TONEAREST:  return "FE_TONEAREST";
        case FE_TOWARDZERO: return "FE_TOWARDZERO";
        case FE_DOWNWARD:   return "FE_DOWNWARD";
        case FE_UPWARD:     return "FE_UPWARD";
    }

    return "Unknown";
}

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    int rm;

    rm = fegetround();

    printf("%s\n", rounding_mode_str(rm));    // Print current mode
    printf("%f %f\n", rint(2.1), rint(2.7));  // Try rounding

    fesetround(FE_TOWARDZERO);                // Set the mode

    rm = fegetround();

    printf("%s\n", rounding_mode_str(rm));    // Print it
    printf("%f %f\n", rint(2.1), rint(2.7));  // Try it now!
}
