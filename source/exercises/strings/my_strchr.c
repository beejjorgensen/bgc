#include <stdio.h>

char *my_strchr(char *s, char c)
{
    for (; *s != '\0'; s++)
        if (*s == c)
            return s;

    return NULL;
}

int main(void)
{
    printf("%s\n", my_strchr("ABCD", 'C'));  // "CD"
    printf("%s\n", my_strchr("ABCD", 'A'));  // "ABCD"
    printf("%s\n", my_strchr("D", 'D'));     // "D"

    char *p = my_strchr("ABCD", 'Z');

    printf("%s\n", p == NULL? "NULL": p);    // "NULL"
}
