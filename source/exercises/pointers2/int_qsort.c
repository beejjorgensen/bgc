#include <stdio.h>
#include <stdlib.h>

int compar_ascending(const void *a, const void *b)
{
    const int *ai = a;
    const int *bi = b;

    if (*ai > *bi) return 1;
    if (*ai < *bi) return -1;
    return 0;
}

int compar_descending(const void *a, const void *b)
{
    const int *ai = a;
    const int *bi = b;

    if (*ai > *bi) return -1;
    if (*ai < *bi) return 1;
    return 0;
}

void print_array(int a[], int count)
{
    for (int i = 0; i < count; i++)
        printf("%d ", a[i]);

    printf("\n");
}

int main(void) {
    // Some random numbers
    int x[15] = {12, 69, 9, 46, 49, 45, 33, 41, 97, 88, 26, 61, 53, 27, 68};
    
    qsort(x, 15, sizeof(int), compar_ascending);
    print_array(x, 15);

    qsort(x, 15, sizeof(int), compar_descending);
    print_array(x, 15);
}
