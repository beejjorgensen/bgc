#include <stdio.h>
#include <stdarg.h>

// Add all the numbers together, but then add on all the numbers
// past the second one again.
int contrived_adder(int count, ...)
{
    if (count < 3) return 0; // OK, I'm being lazy. You got me.

    int total = 0;

    va_list args, mid_args;

    va_start(args, count);

    for (int i = 0; i < count; i++) {

        // If we're at the second number, save our place in
        // mid_args:

        if (i == 2)
            va_copy(mid_args, args);

        total += va_arg(args, int);
    }

    va_end(args); // Done with this

    // But now let's start with mid_args and add all those on:
    for (int i = 0; i < count - 2; i++)
        total += va_arg(mid_args, int);

    va_end(mid_args); // Done with this, too

    return total;
}

int main(void)
{
    // 10+20+30 + 30 == 90
    printf("%d\n", contrived_adder(3, 10, 20, 30));

    // 10+20+30+40+50 + 30+40+50 == 270
    printf("%d\n", contrived_adder(5, 10, 20, 30, 40, 50));
}
