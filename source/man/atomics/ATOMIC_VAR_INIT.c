#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = ATOMIC_VAR_INIT(3490);  // Deprecated
    printf("%d\n", x);
}
