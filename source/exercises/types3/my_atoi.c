#include <stdio.h>
#include <ctype.h>

int my_atoi(char *s)
{
    // Skip leading spaces
    while (isspace((unsigned char)*s))
        s++;

    // Look for negative
    int sign;

    if (*s == '-') {
        s++;
        sign = -1;
    } else
        sign = 1;

    int result = 0;

    // Loop through digits in the string
    while (isdigit((unsigned char)*s)) {
        result *= 10;
        result += *s - '0';
        s++;
    }

    // Return signed result
    return result * sign;
}

int main(void)
{
    printf("%d\n", my_atoi("3490"));
    printf("%d\n", my_atoi("-3490"));
    printf("%d\n", my_atoi("   3490"));
    printf("%d\n", my_atoi("   -3490"));
    printf("%d\n", my_atoi("3490xyz"));
    printf("%d\n", my_atoi("-3490a"));
    printf("%d\n", my_atoi("   3490a"));
    printf("%d\n", my_atoi("   -3490-"));
    printf("%d\n", my_atoi("--3490"));  // 0
    printf("%d\n", my_atoi("a3490"));  // 0
    printf("%d\n", my_atoi("a3490"));  // 0
}
