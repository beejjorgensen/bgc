#include <stdio.h>
#include <stddef.h>

int main(void)
{
    int cats[100];

    int *f = cats + 20;
    int *g = cats + 60;

    ptrdiff_t d = g - f;  // difference is 40

    printf("%td\n", d);  // Print decimal: 40
    printf("%tX\n", d);  // Print hex:     28
}
