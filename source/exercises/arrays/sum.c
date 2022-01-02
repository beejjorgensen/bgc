#include <stdio.h>

int sum(int a[], int len)
{
    int s = 0;

    for (int i = 0; i < len; i++) {
        s += a[i];
    }

    return s;
}

int main(void)
{
    int a[] = {10, 5, 2, 30, 97, 64};

    int s = sum(a, 6);

    printf("%d\n", s);
}
