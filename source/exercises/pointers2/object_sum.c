#include <stdio.h>

int object_sum(void *p, size_t count)
{
    int t = 0;
    
    // Save the pointer as a char* so we can
    // do ++ arithmetic on it later
    char *c = p;

    while (count-- > 0)
        t += *c++;

    /*
    // A less condensed version of the loop
    while (count > 0) {
        t += *c;
        c++;
        count--;
    }
    */

    return t;
}

int main(void)
{
    int t;

    // ints are likely 4 bytes.
    // And in this case, the bytes are 1, 2, 3, and 4.
    int x = 0x01020304;  // hex number, 16909060 in decimal

    t = object_sum(&x, sizeof x);
    printf("%d\n", t);  // 10 probably, 1 + 2 + 3 + 4

    float f = 3.14159;

    t = object_sum(&f, sizeof f);
    printf("%d\n", t);  // 104 on my system

    int y = 0;

    t = object_sum(&y, sizeof y);
    printf("%d\n", t);  // 0 very extremely probably
}
