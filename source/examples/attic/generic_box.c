#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// malloc space for a value, storing the value there
#define BOX(x) _Generic((x), \
                    int: box_int, \
                    float: box_float, \
                    double: box_double, \
                    char *: box_string \
                )(x)

// Builder for the box_* functions
#define BOX_ANY_FUNC(T) T *box_ ## T(T n) { \
    T *p = malloc(sizeof(T)); \
    if (p) *p = n; \
    return p; \
}

BOX_ANY_FUNC(int)
BOX_ANY_FUNC(float)
BOX_ANY_FUNC(double)

char *box_string(const char *s)
{
    char *p = malloc(strlen(s) + 1);
    if (p) strcpy(p, s);
    return p;
}

int main(void)
{
    int *x = BOX(2 + 9);
    float *y = BOX(2.3f);
    char *s = BOX("Hello, world!");

    printf("%d %f %s\n", *x, *y, s);

    free(s);
    free(y);
    free(x);
}
