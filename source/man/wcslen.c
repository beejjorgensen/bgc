#include <wchar.h>

int main(void)
{
    wchar_t *s = L"Hello, world!"; // 13 characters

    // prints "The string is 13 characters long.":

    wprintf(L"The string is %zu characters long.\n", wcslen(s));
}

