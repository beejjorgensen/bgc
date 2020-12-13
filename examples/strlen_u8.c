#include <stdio.h>
#include <string.h>

size_t strlen_u8(const char *s)
{
    size_t count = 0;

    for (const char *p = s; *p != '\0'; p++) {
        if ((*p & 0xC0) != 0x80) count++;
    }

    return count;
}

int main(void)
{
    char *s = "abc𒀣efg";
    char *t = "abc☠efg";
    char *u = "abcdefg";
    char *v = u8"abc\u2620efg";

    for (char *p = s; *p != '\0'; p++) {
        printf("%02x\n", (*p)&0xff);
    }

    printf("%s\n", s);
    printf("%zu\n", strlen_u8(s));
    printf("%zu\n", strlen_u8(t));
    printf("%zu\n", strlen_u8(u));
    printf("%zu\n", strlen_u8(v));
}
