#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = 0;
    int prev;

    atomic_fetch_add(&x, 3490);
    atomic_fetch_sub(&x, 12);
    atomic_fetch_xor(&x, 444);
    atomic_fetch_or(&x, 12);
    prev = atomic_fetch_and(&x, 42);

    printf("%d %d\n", prev, x);   // 3118 42
}
