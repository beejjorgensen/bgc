#include <stdio.h>
#include <inttypes.h>

int main(void)
{
    intmax_t j = -3490;

    printf("%jd\n", imaxabs(j));    // 3490
}
