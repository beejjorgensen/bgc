#include <stdalign.h>
#include <stdio.h>     // for printf()
#include <stddef.h>    // for max_align_t

int main(void)
{
    int i, j;
    char alignas(max_align_t) a, b;
    char alignas(int) c, d;
    char e, f;

    printf("i: %p\n", (void *)&i);
    printf("j: %p\n\n", (void *)&j);
    printf("a: %p\n", (void *)&a);
    printf("b: %p\n\n", (void *)&b);
    printf("c: %p\n", (void *)&c);
    printf("d: %p\n\n", (void *)&d);
    printf("e: %p\n", (void *)&e);
    printf("f: %p\n", (void *)&f);
}
