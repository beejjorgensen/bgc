#include <wchar.h>

int main(void)
{
    // "Hello, world!"
    //       ^  ^   ^
    //       A  B   C

    wchar_t *str = L"Hello, world!";
    wchar_t *p;

    p = wcschr(str, ',');       // p now points at position A
    p = wcsrchr(str, 'o');      // p now points at position B

    p = wmemchr(str, '!', 13);   // p now points at position C

    // repeatedly find all occurrences of the letter 'B'
    str = L"A BIG BROWN BAT BIT BEEJ";

    for(p = wcschr(str, 'B'); p != NULL; p = wcschr(p + 1, 'B')) {
        wprintf(L"Found a 'B' here: %ls\n", p);
    }

    // output is:
    //
    // Found a 'B' here: BIG BROWN BAT BIT BEEJ
    // Found a 'B' here: BROWN BAT BIT BEEJ
    // Found a 'B' here: BAT BIT BEEJ
    // Found a 'B' here: BIT BEEJ
    // Found a 'B' here: BEEJ
}

