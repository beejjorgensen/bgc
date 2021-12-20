#ifndef __APPLE__
#include <uchar.h>
#endif
#include <stdio.h>   // for printf()
#include <string.h>  // for memset()
#include <locale.h>  // for setlocale()

int main(void)
{
#ifndef __APPLE__
    char *s = "\U0001fbc5*";   // Stick figure glyph, more than 16 bits
    char16_t pc16;
    mbstate_t mbs;
    size_t r;

    setlocale(LC_ALL, "");
    memset(&mbs, 0, sizeof mbs);

    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zd\n", r);     // r is 4 bytes in UTF-8 locale
    printf("%#x\n", pc16);  // First value of surrogate pair

    s += r;  // Move to next character

    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zd\n", r);     // r is (size_t)(-3) here to indicate...
    printf("%#x\n", pc16);  // ...Second value of surrogate pair

    // Since r is -3, it means we're still processing the same
    // character, so DON'T move to the next character this time
    //s += r;  // Commented out

    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zd\n", r);     // 1 byte for "*"
    printf("%#x\n", pc16);  // 0x2a for "*"
#else
    printf("Not supported on OSX\n");
#endif
}

