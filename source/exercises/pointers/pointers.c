#include <stdio.h>

void func(int *a, float *b)
{
    int old_a = *a;

    *a -= *b;
    *b += old_a;
}

int main(void)
{
    int x = 5;
    float y = 3.2;

    func(&x, &y);

    printf("%d %f\n", x, y);
}
