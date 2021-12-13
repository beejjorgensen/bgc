#include <stdio.h>
#include <inttypes.h>

int main(void)
{
    intmax_t r;
    char *endptr;

    // Valid base-10 number
    r = strtoimax("123456789012345", &endptr, 10);

    if (*endptr != '\0')
        printf("Invalid digit: %c\n", *endptr);
    else
        printf("Value is %jd\n", r);
    
    // The following binary number contains an invalid digit
    r = strtoimax("0100102010101101", &endptr, 2);

    if (*endptr != '\0')
        printf("Invalid digit: %c\n", *endptr);
    else
        printf("Value is %jd\n", r);
}
