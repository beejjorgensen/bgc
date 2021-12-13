#include <stdio.h>
#include <inttypes.h>

int main(void)
{
    intmax_t numer = INTMAX_C(3490);
    intmax_t denom = INTMAX_C(17);

    imaxdiv_t r = imaxdiv(numer, denom);

    printf("Quotient: %jd, remainder: %jd\n", r.quot, r.rem);
}
