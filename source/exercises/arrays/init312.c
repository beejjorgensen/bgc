#include <stdio.h>

int main(void)
{
    int a[2048] = {[312]=3490};

    printf("%d\n", a[311]);
    printf("%d\n", a[312]);
    printf("%d\n", a[313]);
}
