#include <stdio.h>

int main(void)
{
    int a[5] = {11, 22, 33, 44, 55};
    int *p;

    //p = &a[0];  // p points to the array
                  // Well, to the first element, actually

    p = a;      // p points to the array, but much nicer-looking!

    printf("%d\n", *p);  // Prints "11"
}
