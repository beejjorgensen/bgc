#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

int main(void)
{
	setlocale(LC_ALL, "");

    char *bad_str = "\xff";  // Probably invalid char in this locale
    wchar_t wc;
    size_t result;
    mbstate_t ps;

    memset(&ps, 0, sizeof ps);

    result = mbrtowc(&wc, bad_str, 1, &ps);

    if (result == (size_t)(-1))
        perror("mbrtowc");  // mbrtowc: Illegal byte sequence
    else
        printf("Converted to L'%lc'\n", wc);

    return 0;
}
