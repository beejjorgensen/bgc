#ifndef __APPLE__
#include <uchar.h>
#endif
#include <stdio.h>   // for printf()
#include <locale.h>  // for setlocale()
#include <string.h>  // for memset()

int main(void)
{
#ifndef __APPLE__
    char *s = "\u20acZillion";  // 20ac is "€"
    char16_t pc16;
    size_t r;
    mbstate_t mbs;

    setlocale(LC_ALL, "");
    memset(&mbs, 0, sizeof mbs);

    // Examine the next 8 bytes to see if there's a character in there
    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zu\n", r);     // Prints a value >= 1 (3 in UTF-8 locale)
    printf("%#x\n", pc16);  // Prints 0x20ac for "€"

    s += r;  // Move to next character

    // Examine the next 8 bytes to see if there's a character in there
    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zu\n", r);     // Prints 1
    printf("%#x\n", pc16);  // Prints 0x5a for "Z"
#else
    printf("Not supported on OSX\n");
#endif
}

