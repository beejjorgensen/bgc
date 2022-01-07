#include <stdio.h>
#include <string.h>

char *reverse(char *s)
{
    char *p0, *p1;

    p0 = s;
    p1 = s + strlen(s) - 1;

    while (p1 > p0) {
        char t = *p0;
        *p0++ = *p1;
        *p1-- = t;
    }

    return s;
}

int main(void)
{
    char s[] = "Hello, world!";

    printf("%s\n", s);

    reverse(s);

    printf("%s\n", s);
}
