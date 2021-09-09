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

The functions, below, that have an `excepts` parameter will take these
values.

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

**A caveat**: this program isn't supported on either of the compilers I
have (gcc and clang) as of this writing, so though I have built the
code, below, it's not particularly well-tested.

[[manbreak]]
## `feclearexcept()` {#man-example}

Clear floating point exceptions

### Synopsis {.unnumbered .unlisted}

``` {.c}
int feclearexcept(int excepts);
```

### Description {.unnumbered .unlisted}

If a floating point exception has occurred, this function can clear it.

Set `excepts` to a bitwise-OR list of exceptions to clear.

Passing `0` has no effect.

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

    printf("%d %f\n", r, f);
}
```

### See Also {.unnumbered .unlisted}

[`feraiseexcept()`](#man-feraiseexcept),
[`fetestexcept()`](#man-fertestxcept)

[[manbreak]]
## `fegetexceptflag()` `fesetexceptflag()` {#man-fegetexceptflag}

Save or restore the floating point exception flags

### Synopsis {.unnumbered .unlisted}

``` {.c}
int fegetexceptflag(fexcept_t *flagp, int excepts);

int fesetexceptflag(fexcept_t *flagp, int excepts);
```

### Description {.unnumbered .unlisted}

Use these functions to save or restore the current floating point
environment in a variable.

Set `excepts` to the set of exceptions you want to save or restore the
state of. Setting it to `FE_ALL_EXCEPT` will save or restore the entire
state.

Note that `fexcept_t` is an opaque type---you don't know what's in
it.

`excepts` can be set to zero for no effect.

### Return Value {.unnumbered .unlisted}

Returns `0` on success or if `excepts` is zero.

Returns non-zero on failure.

### Example {.unnumbered .unlisted}

This program saves the state (before any error has happened), then
deliberately causes a domain error by trying to take $\sqrt{-1}$.

After that, it restores the floating point state to before the error had
occurred, thereby clearing it.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    fexcept_t flag;

    fegetexceptflag(&flag, FE_ALL_EXCEPT);  // Save state

    double f = sqrt(-1);                    // I imagine this won't work
    printf("%f\n", f);                      // "nan"

    if (fetestexcept(FE_INVALID))
        printf("1: Domain error\n");        // This prints!
    else
        printf("1: No domain error\n");

    fesetexceptflag(&flag, FE_ALL_EXCEPT);  // Restore to before error

    if (fetestexcept(FE_INVALID))
        printf("2: Domain error\n");
    else
        printf("2: No domain error\n");     // This prints!
}
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `feraiseexcept()` {#man-example}

Raise a floating point exception through software

### Synopsis {.unnumbered .unlisted}

``` {.c}
int feraiseexcept(int excepts);
```

### Description {.unnumbered .unlisted}

This attempts to raise a floating point exception as if it had happened.

You can specify multiple exceptions to raise.

If either `FE_UNDERFLOW` or `FE_OVERFLOW` is raised, C _might_ also
raise `FE_INEXACT`.

If either `FE_UNDERFLOW` or `FE_OVERFLOW` is raised at the same time as
`FE_INEXACT`, then `FE_UNDERFLOW` or `FE_OVERFLOW` will be raised
_before_ `FE_INEXACT` behind the scenes.

The order the other exceptions are raised is undefined.

### Return Value {.unnumbered .unlisted}

Returns `0` if all the exceptions were raised or if `excepts` is `0`.

Returns non-zero otherwise.

### Example {.unnumbered .unlisted}

This code deliberately raises a division-by-zero exception and then
detects it.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    feraiseexcept(FE_DIVBYZERO);

    if (fetestexcept(FE_DIVBYZERO) == FE_DIVBYZERO)
        printf("Detected division by zero\n");  // This prints!!
    else
        printf("This is fine.\n");
}
```

### See Also {.unnumbered .unlisted}

[`feclearexcept()`](#man-feclearexcept),
[`fetestexcept()`](#man-fetestexcept)

[[manbreak]]
## `fetestexcept()` {#man-example}

Test to see if an exception has occurred

### Synopsis {.unnumbered .unlisted}

``` {.c}
int fetestexcept(int excepts);
```

### Description {.unnumbered .unlisted}

Put the exceptions you want to test in `excepts`, bitwise-ORing them
together.

### Return Value {.unnumbered .unlisted}

Returns the bitwise-OR of the exceptions that have been raised.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
This code deliberately raises a division-by-zero exception and then
detects it.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    feraiseexcept(FE_DIVBYZERO);

    if (fetestexcept(FE_DIVBYZERO) == FE_DIVBYZERO)
        printf("Detected division by zero\n");  // This prints!!
    else
        printf("This is fine.\n");
}
```

### See Also {.unnumbered .unlisted}

[`feclearexcept()`](#man-feclearexcept),
[`feraiseexcept()`](#man-feraiseexcept)

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