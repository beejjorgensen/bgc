#include <stdio.h>
#include <string.h>

int main(void)
{
    char *s1 = "Muffin";
    char *s2 = "Muffin Sandwich";
    char *s3 = "Muffin";

    strcmp("Biscuits", "Kittens"); // returns < 0 since 'B' < 'K'
    strcmp("Kittens", "Biscuits"); // returns > 0 since 'K' > 'B'

    if (strcmp(s1, s2) == 0)
        printf("This won't get printed because the strings differ");

    if (strcmp(s1, s3) == 0)
        printf("This will print because s1 and s3 are the same");

    // this is a little weird...but if the strings are the same, it'll
    // return zero, which can also be thought of as "false". Not-false
    // is "true", so (!strcmp()) will be true if the strings are the
    // same. yes, it's odd, but you see this all the time in the wild
    // so you might as well get used to it:

    if (!strcmp(s1, s3))
        printf("The strings are the same!");

    if (!strncmp(s1, s2, 6))
        printf("The first 6 characters of s1 and s2 are the same");
}
