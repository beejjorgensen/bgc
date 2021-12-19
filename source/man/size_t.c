#include <stdio.h>
#include <wchar.h>
#include <stddef.h>

int main(void)
{
    size_t x;

    x = sizeof(int);

    printf("%zu\n", x);

#ifndef __APPLE__
    x = mbrtoc16(&a, "b", 8, &ps);

    printf("%zd\n", x);
#endif
}
