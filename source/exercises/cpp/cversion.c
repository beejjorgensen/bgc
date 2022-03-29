#include <stdio.h>

int main(void)
{
    printf("Your C version is: ");

#ifndef __STDC_VERSION__
    printf("C89 or earlier\n");
#else
    if (__STDC_VERSION__ >= 201112L)
        printf("C11 or higher\n");
    else if (__STDC_VERSION__ >= 199901L)
        printf("C99\n");
    else
        printf("C89\n");
#endif
}
