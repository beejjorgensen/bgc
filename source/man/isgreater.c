#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%d\n", isgreater(10.0, 3.0));        // 1
    printf("%d\n", isgreaterequal(10.0, 10.0));  // 1
    printf("%d\n", isless(10.0, 3.0));           // 0
    printf("%d\n", islessequal(10.0, 3.0));      // 0
}

