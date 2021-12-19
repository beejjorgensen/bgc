#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = 10;

    int v = atomic_load(&x);

    printf("%d\n", v);  // 10
}
