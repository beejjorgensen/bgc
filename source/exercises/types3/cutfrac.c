#include <stdio.h>

int main(void)
{
    float f = 3.14159;

    int whole = f;   // implicit conversion to int drops fractional part
    float fraction = f - whole;  // 3.14159 - 3 = 0.14159

    printf("%d+%f\n", whole, fraction);
}
