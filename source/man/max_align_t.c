#include <stddef.h>
#include <stdio.h>      // For printf()
#include <stdalign.h>   // For alignof

int main(void)
{
    int max = alignof(max_align_t);

    printf("Maximum fundamental alignment: %d\n", max);
}

