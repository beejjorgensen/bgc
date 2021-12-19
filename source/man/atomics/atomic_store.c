#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = 0;
    atomic_int y = 0;

    atomic_store(&x, 10);

    atomic_store_explicit(&y, 20, memory_order_relaxed);

    // Will print either "10 20" or "10 0":
    printf("%d %d\n", x, y);
}
