// runner.c

#include <stdio.h>
#include "total.h"

int main(void)
{
    for (int i = 0; i < 10; i++)
        add(i);

    printf("Total is %d over %d calls\n", total(), count);
}
