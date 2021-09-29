<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<fenv.h>` Floating Point Exceptions and Environment {#fenv}

|Function|Description|
|--------|----------------------|
|[`feclearexcept()`](#man-feclearexcept)|Clear floating point exceptions|
|[`fegetexceptflag()`](#man-fegetexceptflag)|Save the floating point exception flags|
|[`fesetexceptflag()`](#man-fegetexceptflag)|Restore the floating point exception flags|
|[`feraiseexcept()`](#man-feraiseexcept)|Raise a floating point exception through software|
|[`fetestexcept()`](#man-fetestexcept)|Test to see if an exception has occurred|
|[`fegetround()`](#man-fegetround)|Get the rounding direction|
|[`fesetround()`](#man-fegetround)|Set the rounding direction|
|[`fegetenv()`](#man-fegetenv)|Save the entire floating point environment|
|[`fesetenv()`](#man-fegetenv)|Restore the entire floating point environment|
|[`feholdexcept()`](#man-feholdexcept)|Save floating point state and install non-stop mode|
|[`feupdateenv()`](#man-feupdateenv)|Restore floating point environment and apply recent exceptions|

## Types and Macros

There are two types defined in this header:

|Type|Description|
|--------|----------------------|
|`fenv_t`|The entire floating point environment|
|`fexcept_t`|A set of floating point exceptions

The "environment" can be thought of as the status at this moment of the
floating point processing system: this includes the exceptions,
rounding, etc. It's an opaque type, so you won't be able to access it
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
## `feclearexcept()` {#man-feclearexcept}

Clear floating point exceptions

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <fenv.h>

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
[`fetestexcept()`](#man-fetestexcept)

[[manbreak]]
## `fegetexceptflag()` `fesetexceptflag()` {#man-fegetexceptflag}

Save or restore the floating point exception flags

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <fenv.h>

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
## `feraiseexcept()` {#man-feraiseexcept}

Raise a floating point exception through software

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <fenv.h>

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
## `fetestexcept()` {#man-fetestexcept}

Test to see if an exception has occurred

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <fenv.h>

int fetestexcept(int excepts);
```

### Description {.unnumbered .unlisted}

Put the exceptions you want to test in `excepts`, bitwise-ORing them
together.

### Return Value {.unnumbered .unlisted}

Returns the bitwise-OR of the exceptions that have been raised.

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
[`feraiseexcept()`](#man-feraiseexcept)

[[manbreak]]
## `fegetround()` `fesetround()` {#man-fegetround}

Get or set the rounding direction

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <fenv.h>

int fegetround(void);

int fesetround(int round);
```

### Description {.unnumbered .unlisted}

Use these to get or set the rounding direction used by a variety of math
functions.

Basically when a function "rounds" a number, it wants to know how to do
it. By default, it does it how we tend to expect: if the fractional part
is less than 0.5, it rounds down closer to zero, otherwise up farther
from zero.

|Macro|Description|
|--------|----------------------|
|`FE_TONEAREST`|Round to the nearest whole number, the default|
|`FE_TOWARDZERO`|Round toward zero always|
|`FE_DOWNWARD`|Round toward the next lesser whole number|
|`FE_UPWARD`|Round toward the next greater whole number|

Some implementations don't support rounding. If it does, the above
macros will be defined.

Note that the `round()` function is always "to-nearest" and doesn't pay
attention to the rounding mode.

### Return Value {.unnumbered .unlisted}

`fegetround()` returns the current rounding direction, or a negative
value on error.

`fesetround()` returns zero on success, or non-zero on failure.

### Example {.unnumbered .unlisted}

This rounds some numbers 

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

// Helper function to print the rounding mode
const char *rounding_mode_str(int mode)
{
    switch (mode) {
        case FE_TONEAREST:  return "FE_TONEAREST";
        case FE_TOWARDZERO: return "FE_TOWARDZERO";
        case FE_DOWNWARD:   return "FE_DOWNWARD";
        case FE_UPWARD:     return "FE_UPWARD";
    }

    return "Unknown";
}

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    int rm;

    rm = fegetround();

    printf("%s\n", rounding_mode_str(rm));    // Print current mode
    printf("%f %f\n", rint(2.1), rint(2.7));  // Try rounding

    fesetround(FE_TOWARDZERO);                // Set the mode

    rm = fegetround();

    printf("%s\n", rounding_mode_str(rm));    // Print it
    printf("%f %f\n", rint(2.1), rint(2.7));  // Try it now!
}
```

Output:

``` {.default}
FE_TONEAREST
2.000000 3.000000
FE_TOWARDZERO
2.000000 2.000000
```

### See Also {.unnumbered .unlisted}

[`nearbyint()`](#man-nearbyint),
[`nearbyintf()`](#man-nearbyint),
[`nearbyintl()`](#man-nearbyint),
[`rint()`](#man-rint),
[`rintf()`](#man-rint),
[`rintl()`](#man-rint),
[`lrint()`](#man-lrint),
[`lrintf()`](#man-lrint),
[`lrintl()`](#man-lrint),
[`llrint()`](#man-lrint),
[`llrintf()`](#man-lrint),
[`llrintl()`](#man-lrint)

[[manbreak]]
## `fegetenv()` `fesetenv()` {#man-fegetenv}

Save or restore the entire floating point environment

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <fenv.h>

int fegetenv(fenv_t *envp);
int fesetenv(const fenv_t *envp);
```

### Description {.unnumbered .unlisted}

You can save the environment (exceptions, rounding direction, etc.) by
calling `fegetenv()` and restore it with `fesetenv()`.

Use this if you want to restore the state after a function call, i.e.
hide from the caller that some floating point exceptions or changes
occurred.

### Return Value {.unnumbered .unlisted}

`fegetenv()` and `fesetenv()` return `0` on success, and non-zero
otherwise.

### Example {.unnumbered .unlisted}

This example saves the environment, messes with the rounding and
exceptions, then restores it. After the environment is restored, we see
that the rounding is back to default and the exception is cleared.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

void show_status(void)
{
    printf("Rounding is FE_TOWARDZERO: %d\n",
           fegetround() == FE_TOWARDZERO);

    printf("FE_DIVBYZERO is set: %d\n",
           fetestexcept(FE_DIVBYZERO) != 0);
}

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    fenv_t env;

    fegetenv(&env);  // Save the environment

    fesetround(FE_TOWARDZERO);    // Change rounding
    feraiseexcept(FE_DIVBYZERO);  // Raise an exception

    show_status();

    fesetenv(&env);  // Restore the environment

    show_status();
}
```

Output:

``` {.default}
Rounding is FE_TOWARDZERO: 1
FE_DIVBYZERO is set: 1
Rounding is FE_TOWARDZERO: 0
FE_DIVBYZERO is set: 0
```

### See Also {.unnumbered .unlisted}

[`feholdexcept()`](#man-feholdexcept),
[`feupdateenv()`](#man-feupdateenv)

[[manbreak]]
## `feholdexcept()` {#man-feholdexcept}

Save floating point state and install non-stop mode

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <fenv.h>

int feholdexcept(fenv_t *envp);
```

### Description {.unnumbered .unlisted}

This is just like `fegetenv()` except that it updates the current
environment to be in _non-stop_ mode, namely it won't halt on any
exceptions.

It remains in this state until you restore the state with `fesetenv()`
or `feupdateenv()`.


### Return Value {.unnumbered .unlisted}

### Example {.unnumbered .unlisted}

This example saves the environment and goes into non-stop mode, messes
with the rounding and exceptions, then restores it. After the
environment is restored, we see that the rounding is back to default and
the exception is cleared. We'll also be out of non-stop mode.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

void show_status(void)
{
    printf("Rounding is FE_TOWARDZERO: %d\n",
           fegetround() == FE_TOWARDZERO);

    printf("FE_DIVBYZERO is set: %d\n",
           fetestexcept(FE_DIVBYZERO) != 0);
}

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    fenv_t env;

    // Save the environment and don't stop on exceptions
    feholdexcept(&env);

    fesetround(FE_TOWARDZERO);    // Change rounding
    feraiseexcept(FE_DIVBYZERO);  // Raise an exception

    show_status();

    fesetenv(&env);  // Restore the environment

    show_status();
}
```

### See Also {.unnumbered .unlisted}

[`fegetenv()`](#man-fegetenv),
[`fesetenv()`](#man-fegetenv),
[`feupdateenv()`](#man-feupdateenv)

[[manbreak]]
## `feupdateenv()` {#man-feupdateenv}

Restore floating point environment and apply recent exceptions

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <fenv.h>

int feupdateenv(const fenv_t *envp);
```

### Description {.unnumbered .unlisted}

This is like `fesetenv()` except that it modifies the passed-in
environment so that it is updated with exceptions that have happened in
the meantime.

So let's say you had a function that might raise exceptions, but you
wanted to hide those in the caller. One option might be to:

1. Save the environment with `fegetenv()` or `feholdexcept()`.
2. Do whatever you do that might raise exceptions.
3. Restore the environment with `fesetenv()`, thereby hiding the
   exceptions that happened in step 2.

But that hides _all_ exceptions. What if you just wanted to hide some of
them? You could use `feupdateenv()` like this:

1. Save the environment with `fegetenv()` or `feholdexcept()`.
2. Do whatever you do that might raise exceptions.
3. Call `feclearexcept()` to clear the exceptions you want to hide from
   the caller.
4. Call `feupdateenv()` to restore the previous environment and update
   it with the other exceptions that have occurred.

So it's like a more capable way of restoring the environment than simply
`fegetenv()`/`fesetenv()`.

### Return Value {.unnumbered .unlisted}

Returns `0` on success, non-zero otherwise.

### Example {.unnumbered .unlisted}

This program saves state, raises some exceptions, then clears one of the
exceptions, then restores and updates the state.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

void show_status(void)
{
    printf("FE_DIVBYZERO: %d\n", fetestexcept(FE_DIVBYZERO) != 0);
    printf("FE_INVALID  : %d\n", fetestexcept(FE_INVALID) != 0);
    printf("FE_OVERFLOW : %d\n\n", fetestexcept(FE_OVERFLOW) != 0);
}

int main(void)
{
    #pragma STDC FENV_ACCESS ON 

    fenv_t env;

    feholdexcept(&env);  // Save the environment

    // Pretend some bad math happened here:
    feraiseexcept(FE_DIVBYZERO);  // Raise an exception
    feraiseexcept(FE_INVALID);    // Raise an exception
    feraiseexcept(FE_OVERFLOW);   // Raise an exception

    show_status();

    feclearexcept(FE_INVALID);

    feupdateenv(&env);  // Restore the environment

    show_status();
}
```

In the output, at first we have no exceptions. Then we have the three we
raised. Then after we restore/update the environment, we see the one we
cleared (`FE_INVALID`) hasn't been applied:

``` {.default}
FE_DIVBYZERO: 0
FE_INVALID  : 0
FE_OVERFLOW : 0

FE_DIVBYZERO: 1
FE_INVALID  : 1
FE_OVERFLOW : 1

FE_DIVBYZERO: 1
FE_INVALID  : 0
FE_OVERFLOW : 1
```

### See Also {.unnumbered .unlisted}

[`fegetenv()`](#man-fegetenv),
[`fesetenv()`](#man-fegetenv),
[`feholdexcept()`](#man-feholdexcept),
[`feclearexcept()`](#man-feclearexcept)

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