#ifndef __APPLE__
#include <uchar.h>
#endif
#include <stdlib.h>  // for MB_CUR_MAX
#include <stdio.h>   // for printf()
#include <string.h>  // for memset()
#include <locale.h>  // for setlocale()

int main(void)
{
#ifndef __APPLE__
    char16_t c16 = 0x20ac;  // Unicode for Euro symbol
    char dest[MB_CUR_MAX];
    size_t r;
    mbstate_t mbs;

    setlocale(LC_ALL, "");
    memset(&mbs, 0, sizeof mbs);  // Reset conversion state

    // Convert
    r = c16rtomb(dest, c16, &mbs);

    printf("r == %zd\n", r);  // r == 3 on my system

    // And this should print a Euro symbol
    printf("dest == \"%s\"\n", dest);
#else
    printf("Not supported on OSX\n");
#endif
}

