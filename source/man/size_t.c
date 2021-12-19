#include <stdio.h>
#ifndef __APPLE__
#include <uchar.h>
#endif
#include <string.h>
#include <stddef.h>

int main(void)
{
    size_t x;

    x = sizeof(int);

    printf("%zu\n", x);

#ifndef __APPLE__
    char16_t a;
    mbstate_t mbs;
    memset(&mbs, 0, sizeof mbs);

    x = mbrtoc16(&a, "b", 8, &mbs);

    printf("%zd\n", x);
#endif
}
