#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

size_t mbslen(const char *s)
{
    size_t count = 0;
    mbstate_t mbs;

    memset(&mbs, 0, sizeof mbs);

    while (*s != '\0') {
        int t = mbrlen(s, 10, &mbs);
        s += t;
        count++;
    }

    return count;
}

int main(void)
{
    setlocale(LC_ALL, "");
    char *s = "§¶°±π€•";

    printf("%zu bytes, %zu characters\n", strlen(s), mbslen(s));
}