#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p;

    if ((p = malloc(10 * sizeof(int))) == NULL) {
        printf("Error allocating memory\n");
        return 1;
    }

    for (int i = 0; i < 10; i++)
        p[i] = i * 10;

    for (int i = 0; i < 10; i++)
        printf("%d: %d\n", i, p[i]);

    free(p);
}