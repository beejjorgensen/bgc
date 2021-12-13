#include <wchar.h>
#include <inttypes.h>

int main(void)
{
    intmax_t r;
    wchar_t *endptr;

    // Valid base-10 number
    r = wcstoimax(L"123456789012345", &endptr, 10);

    if (*endptr != '\0')
        wprintf(L"Invalid digit: %lc\n", *endptr);
    else
        wprintf(L"Value is %jd\n", r);
    
    // The following binary number contains an invalid digit
    r = wcstoimax(L"0100102010101101", &endptr, 2);

    if (*endptr != '\0')
        wprintf(L"Invalid digit: %lc\n", *endptr);
    else
        wprintf(L"Value is %jd\n", r);
}
