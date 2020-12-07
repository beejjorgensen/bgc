#include <stdio.h>
#include <string.h>

size_t strlen_u8(const char *s)
{
    const unsigned char *p = (const unsigned char *)s;
    size_t count = 0;

    while (*p != '\0') {
        const unsigned char t = (*p>>4) == 0xe;
        const unsigned char x = (*p>>4) == 0xf;
        const unsigned char y = ((*p>>5) == 0x6) | t;
        const unsigned char z = ((*p>>7) == 0x0) | t;

        p += x << 2 | y << 1 | z;
        count++;
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

    return 0;
}

/*
ABCD  XYZ
0xxx  001
110x  010
1110  011
1111  100
*/