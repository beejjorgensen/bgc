#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int a;
    int i = 10, x, y;

    atomic_store_explicit(&a, 3490, memory_order_release);

    i = atomic_load_explicit(&a, memory_order_consume);
    x = i;
    y = kill_dependency(i);

    printf("%d %d\n", x, y);  // 3490 and either 3490 or 10
}
