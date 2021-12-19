#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%d\n", islessgreater(10.0, 3.0));   // 1
    printf("%d\n", islessgreater(10.0, 30.0));  // 1
    printf("%d\n", islessgreater(10.0, 10.0));  // 0
}

