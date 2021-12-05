<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdnoreturn.h>` Macros for Non-Returning Functions {#stdnoreturn}

This header provides a macro `noreturn` that is a handy alias for
[`_Noreturn`](#noreturn).

Use this macro to indicate to the compiler that a function will never
return to the caller. It's undefined behavior if the so-marked function
does return.

Here's a usage example:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

noreturn void foo(void) // This function should never return!
{
    printf("Happy days\n");

    exit(1);            // And it doesn't return--it exits here!
}

int main(void)
{
    foo();
}
```

That's all there is to it.