// runner.c
#include <stdio.h>

// Normally these would be found in a .h file
extern int count;
extern int add(int x);
extern int total(void);

// ^^ file scope function prototypes are extern by default,
// so this is redundantly being extra explicit that they're
// coming from elsewhere.

int main(void)
{
    for (int i = 0; i < 10; i++)
        add(i);

    printf("Total is %d over %d calls\n", total(), count);
}
