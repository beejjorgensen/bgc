#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = 10;

    int previous = atomic_exchange(&x, 20);

    printf("x is  %d\n", x);
    printf("x was %d\n", previous);
}
