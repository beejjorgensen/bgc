#include <stdio.h>
#include <assert.h>

#define ARRAY_SIZE 48  // OK
//#define ARRAY_SIZE 16  // Causes assertion to fire

int main(void)
{
    static_assert(ARRAY_SIZE > 32, "ARRAY_SIZE too small");

    int a[ARRAY_SIZE];

    a[32] = 10;

    printf("%d\n", a[32]);
}

