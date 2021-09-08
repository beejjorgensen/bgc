<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<fenv.h>` Floating Point Exceptions and Environment {#fenv}

|Function|Description|
|--------|----------------------|

## Types and Macros

There are two types defined in this header:

|Type|Description|
|--------|----------------------|
|`fenv_t`|The entire floating point environment|
|`fexcept_t`|A set of floating point exceptions

The "environment" can be thought of as the status at this moment of the
floating point processing system. Things that have gone wrong,
bookkeeping, etc. It's an opaque type, so you won't be able to access it
directly, and it must be done through the proper functions.

If the functions in question exist on your system (they might not be!),
then you'll also have these macros defined to represent different
exceptions:

|Macro|Description|
|--------|----------------------|
|`FE_DIVBYZERO`|Division by zero|
|`FE_INEXACT`|Result was not exact, was rounded|
|`FE_INVALID`|Domain error|
|`FE_OVERFLOW`|Numeric overflow|
|`FE_UNDERFLOW`|Numeric underflow|
|`FE_ALL_EXCEPT`|All of the above combined|

The idea is that you can bitwise-OR these together to represent multiple
exceptions, e.g. `FE_INVALID|FE_OVERFLOW`.

See [`<math.h>`](#math) for which functions raise which exceptions and
when.

## Pragmas

Normally C is free to optimize all kinds of stuff that might cause the
flags to not look like you might expect. So if you're going to use this
stuff, be sure to set this pragma:

``` {.c}
#pragma STDC FENV_ACCESS ON
```

If you do this at global scope, it remains in effect until you turn it
off:

``` {.c}
#pragma STDC FENV_ACCESS OFF
```

If you do it in block scope, it has to come before any statements or
declarations. In this case, it has effect until the block ends (or until
it is explicitly turned off.)

[[manbreak]]
## `feclearexcept()` {#man-example}

Clear floating point exceptions

### Synopsis {.unnumbered .unlisted}

``` {.c}
int feclearexcept(int excepts);
```

### Description {.unnumbered .unlisted}




### Return Value {.unnumbered .unlisted}

Returns `0` on success and non-zero on failure.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    double f = sqrt(-1);

    int r = feclearexcept(FE_INVALID);

    printf("%d\n", r);
}
```

### See Also {.unnumbered .unlisted}

[`example()`](#man-example),

<!--
[[manbreak]]
## `example()` `example()` `example()` {#man-example}

### Synopsis {.unnumbered .unlisted}

``` {.c}
```

### Description {.unnumbered .unlisted}

### Return Value {.unnumbered .unlisted}

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
```

### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->