#include <stdio.h>

char *my_strchr(char *s, int c)
{
    // Scan the string looking for the character, or the end
    while (*s != c && *s != '\0')
        s++;

    // If we're at the end, return NULL
    // Otherwise, return the pointer to the character
    return *s == '\0'? NULL: s;
}

int main(void)
{
    char *s = "0abcdefedcba1";
    char *p;

    p = my_strchr(s, 'c');
    printf("%s\n", p);   // "cdefedcba1"

    p = my_strchr(s, '0');
    printf("%s\n", p);   // "0abcdefedcba1"

    p = my_strchr(s, '1');
    printf("%s\n", p);   // "1"

    p = my_strchr(s, 'Z');
    printf("%d\n", p == NULL);  // true
}
