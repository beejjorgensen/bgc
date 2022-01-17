#include <stdio.h>

char *my_strrchr(char *s, int c)
{
    char *end = s;

    // Scan for the end of the string
    while (*end != '\0')
        end++;

    // Scan backward from the end looking for the letter
    // but don't fall off the front of the string
    while (*end != c && end > s)
        end--;

    // If this is the char we're looking for, return that
    // pointer. Otherwise, we didn't find it.
    return *end == c? end: NULL;
}

int main(void)
{
    char *s = "0abcdefedcba1";
    char *p;

    p = my_strrchr(s, 'c');
    printf("%s\n", p);   // "cba1"

    p = my_strrchr(s, '0');
    printf("%s\n", p);   // "0abcdefedcba1"

    p = my_strrchr(s, '1');
    printf("%s\n", p);   // "1"

    p = my_strrchr(s, 'Z');
    printf("%d\n", p == NULL);  // true
}
