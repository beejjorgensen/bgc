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
    char *src = "\U0001fbc5*";   // Stick figure glyph, more than 16 bits
    char dest[MB_CUR_MAX];
    char16_t surrogate0, surrogate1;
    mbstate_t mbs;
    size_t r;

    setlocale(LC_ALL, "");
    memset(&mbs, 0, sizeof mbs);  // Reset conversion state

    // Get first surrogate character
    r = mbrtoc16(&surrogate0, src, 8, &mbs);

    // Get next surrogate character
    src += r;  // Move to next character
    r = mbrtoc16(&surrogate1, src, 8, &mbs);

    printf("Surrogate pair: %#x, %#x\n", surrogate0, surrogate1);

    // Now reverse it
    memset(&mbs, 0, sizeof mbs);  // Reset conversion state

    // Process first surrogate character
    r = c16rtomb(dest, surrogate0, &mbs);

    // r should be 0 at this point, because the character hasn't been
    // processed yet. And dest won't have anything useful... yet!
    printf("r == %zd\n", r);   // r == 0

    // Process second surrogate character
    r = c16rtomb(dest, surrogate1, &mbs);

    // Now we should be in business. r should have the number of
    // bytes, and dest should hold the character.
    printf("r == %zd\n", r);  // r == 4 on my system

    // And this should print a stick figure, if your font supports it
    printf("dest == \"%s\"\n", dest);
#else
    printf("Not supported on OSX\n");
#endif
}

