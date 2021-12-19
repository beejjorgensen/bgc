#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x;
    
    atomic_init(&x, 3490);

    printf("%d\n", x);
}
