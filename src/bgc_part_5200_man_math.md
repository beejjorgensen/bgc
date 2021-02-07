<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<math.h>` Mathematics {#math}

It's your favorite subject: Mathematics! Hello, I'm Doctor Math, and
I'll be making math FUN and EASY!

_[vomiting sounds]_

Ok, I know math isn't the grandest thing for some of you out there,
but these are merely functions that quickly and easily do math you
either know, want, or just don't care about. That pretty much covers
it.

## Math Function Idioms

Many of these math functions exist in three forms, each corresponding
to the argument and/or return types the function uses, `float`,
`double`, or `long double`.

The alternate form for `float` is made by appending `f` to the end of
the function name.

The alternate form for `long double` is made by appending `l` to the end
of the function name.

For example, the `pow()` function, which computes $x^y$, exists in these
forms:

``` {.c}
double      pow(double x, double y);             // double
float       powf(float x, float y);              // float
long double powl(long double x, long double y);  // long double
```

Remember that parameters are given values as if you assigned into them.
So if you pass a `double` into `powf()`, it'll choose the closest
`float` it can to hold the double. If the `double` doesn't fit,
undefined behavior happens.

## Math Types

We have two exciting new types in `<math.h>`:

* `float_t`
* `double_t`

The `float_t` type is at least as accurate as a `float`, and the
`double_t` type is at least as accurate as a `double`.

The idea with these types is they can represent the most efficient way
of storing numbers for maximum speed.

Their actual types vary by implementation, but can be determined by the
value of the `FLT_EVAL_METHOD` macro.

|`FLT_EVAL_METHOD`|`float_t` type|`double_t` type|
|-|-|-|
|`0`|`float`|`double`|
|`1`|`double`|`double`|
|`2`|`long double`|`long double`|
|Other|Implementation-defined|Implementation-defined|

For all defined values of `FLT_EVAL_METHOD`, `float_t` is the
least-precise type used for all floating calculations.

## Math Macros

There are actually a number of these defined, but we'll cover most of
them in their relevant reference sections, below.

Except for `NAN`, which represents Not-A-Number.

## Math Errors

As we know, nothing can ever go wrong with math... except _everything_!

So there are just a couple errors that might occur when using some of
these functions.

* **Range errors** mean that some result is beyond what can be stored in
  the result type.

* **Domain errors** mean that you've passed in an argument that doesn't
  have a defined result for this function.

* **Pole errors** mean that the limit of the function as $x$ approaches
  the given argument is infinite.

* **Overflow errors** are when the result is really large, but can't be
  stored without incurring large roundoff error.

* **Underflow errors** are like overflow errors, except with very small
  numbers.

Now, the C math library can do a couple things when these errors occur:

* Set `errno` to some value, or...
* Raise a floating point exception.

Your system might vary on what happens. You can check it by looking at
the value of the variable `math_errhandling`. It will be equivalent to
one of the following^[Though the system defines `MATH_ERRNO` as `1` and
`MATH_ERREXCEPT` as `2`, it's best to always use their symbolic names.
Just in case.]:

|`math_errhandling`|Description|
|-|-|
|`MATH_ERRNO`|The system uses `errno` for math errors.|
|`MATH_ERREXCEPT`|The system uses exceptions for math errors.|
|`MATH_ERRNO | MATH_ERREXCEPT`|The system does both! (That's a bitwise-OR!)|

You are not allowed to change `math_errhandling`.

For a fuller description on how exceptions work and their meanings, see
the [`<fenv.h>`](#fenv) section.

## Math Pragmas

In case you don't remember, you can brush up on [pragmas back in the C
Preprocessor section](#pragma).

But in a nutshell, they offer various ways to control the compiler's
behavior.

In this case, we have a pragma `FP_CONTRACT` that can be turned off and
on.

What does it mean?

First of all, keep in mind that any operation in an expression can cause
rounding error. So each step of the expression can introduce more
rounding error.

But what if the compiler knows a _double secret_ way of taking the
expression you wrote and converting it to a single instruction that
reduced the number of steps such that the intermediate rounding error
didn't occur?

Could it use it? I mean, the results would be different than if you let
the rounding error settle each step of the way...

Because the results would be different, you can tell the compiler if you
want to allow it to do this or not.

If you want to allow it:

``` {.c}
#pragma STDC FP_CONTRACT ON
```

and to disallow it:

``` {.c}
#pragma STDC FP_CONTRACT OFF
```

If you do this at global scope, it stays at whatever state you set it to
until you change it.

If you do it at block scope, it reverts to the value outside the block
when the block ends.

The initial value of the `FP_CONTRACT` pragma varies from system to
system.


[[pagebreak]]
## `fpclassify()`, {#man-fpclassify}

Return the classification of a given floating point number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

int fpclassify(any_floating_type x);
```

### Description {.unnumbered .unlisted}

What kind of entity does this floating point number represent? What are
the options?

We're used to floating point numbers being regular old things like
`3.14` or `3490.0001`.

But floating point numbers can also represent things like infinity. Or
Not-A-Number (NAN). This function will let you know which type of
floating point number the argument is.

This is a macro, so you can use it with `float`, `double`, `long double`
or anything similar.

### Return Value {.unnumbered .unlisted}

Returns one of these macros depending on the argument's classification:

|Classification|Description|
|-|-|
|`FP_INFINITE`|Number is infinite.|
|`FP_NAN`|Number is Not-A-Number (NAN).|
|`FP_NORMAL`|Just a regular number.|
|`FP_SUBNORMAL`|Number is a sub-normal number.|
|`FP_ZERO`|Number is zero.|

A discussion of subnormal numbers is beyond the scope of the guide, and
is something that most devs go their whole lives without dealing with.
In a nutshell, it's a way to represent really small numbers that might
normally round down to zero. If you want to know more, see the Wikipedia
page on [flw[denormal numbers|Denormal_number]].

### Example {.unnumbered .unlisted}

Print various number classifications.

```{.c .numberLines}
#include <stdio.h>
#include <math.h>

const char *get_classification(double n)
{
    switch (fpclassify(n)) {
        case FP_INFINITE: return "infinity";
        case FP_NAN: return "not a number";
        case FP_NORMAL: return "normal";
        case FP_SUBNORMAL: return "subnormal";
        case FP_ZERO: return "zero";
    }

    return "unknown";
}
 
int main(void)
{
    printf("    1.23: %s\n", get_classification(1.23));
    printf("     0.0: %s\n", get_classification(0.0));
    printf("sqrt(-1): %s\n", get_classification(sqrt(-1)));
    printf("1/tan(0): %s\n", get_classification(1/tan(0)));
    printf("  1e-310: %s\n", get_classification(1e-310));  // very small!
}
```

Output^[This is on my system. Some systems will have different points at
which numbers become subnormal, or they might not support subnormal
values at all.]:

```
    1.23: normal
     0.0: zero
sqrt(-1): not a number
1/tan(0): infinity
  1e-310: subnormal
```

### See Also {.unnumbered .unlisted}
[`isfinite()`](#man-isnan),
[`isinf()`](#man-isnan),
[`isnan()`](#man-isnan),
[`isnormal()`](#man-isnan),
[`signbit()`](#man-signbit)

[[pagebreak]]
## `isfinite()`, `isinf()`, `isnan()`, `isnormal()` {#man-isnan}

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

int isfinite(any_floating_type x);

int isinf(any_floating_type x);

int isnan(any_floating_type x);

int isnormal(any_floating_type x);
```

### Description {.unnumbered .unlisted}

These are helper macros to `fpclassify()`. Bring macros, they work on
any floating point type.

|Macro|Description|
|-|-|
|`isfinite()`|True if the number is not infinite or NaN.|
|`isinf()`|True if the number is infinite.|
|`isnan()`|True if the number is Not-a-Number.|
|`isnormal()`|True if the number is normal.|

For more superficial discussion on normal and subnormal numbers, see
[`fpclassify()`](#man-fpclassify).

### Return Value {.unnumbered .unlisted}

Returns non-zero for true, and zero for false.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("  isfinite(1.23): %d\n", isfinite(1.23));    // 1
    printf(" isinf(1/tan(0)): %d\n", isinf(1/tan(0)));   // 1
    printf(" isnan(sqrt(-1)): %d\n", isnan(sqrt(-1)));   // 1
    printf("isnormal(1e-310): %d\n", isnormal(1e-310));  // 0
}
```

### See Also {.unnumbered .unlisted}

[`fpclassify()`](#man-fpclassify),
[`signbit()`](#man-signbit),


[[pagebreak]]
## `signbit()` {#man-signbit}

Return the sign of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

int signbit(any_floating_type x);
```

### Description {.unnumbered .unlisted}

This macro takes any floating point number and returns a value
indicating the sign of the number, positive or negative.

### Return Value {.unnumbered .unlisted}

Returns `1` if the sign is negative, otherwise `0`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("%d\n", signbit(3490.0));  // 0
printf("%d\n", signbit(-37.0));   // 1
```

### See Also {.unnumbered .unlisted}

[`fpclassify()`](#man-fpclassify),
[`isfinite()`](#man-isnan),
[`isinf()`](#man-isnan),
[`isnan()`](#man-isnan),
[`isnormal()`](#man-isnan)


[[pagebreak]]
## `acos()`, `acosf()`, `acosl()` {#man-acos}

Calculate the arc cosine of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double acos(double x);
float acosf(float x);
long double acosl(long double x);
```

### Description {.unnumbered .unlisted}

Calculates the arc cosine of a number in radians. (That is, the value
whose cosine is `x`.)  The number must be in the range -1.0 to
1.0.

For those of you who don't remember, radians are another way of
measuring an angle, just like degrees. To convert from degrees to
radians or the other way around, use the following code:

``` {.c}
degrees = radians * 180.0f / M_PI;
radians = degrees * M_PI / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the arc cosine of `x`, unless `x` is out of
range. In that case, `errno` will be set to EDOM and the
return value will be NaN. The variants return different types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
double acosx;
long double ldacosx;

acosx = acos(0.2);
ldacosx = acosl((long double)0.3);
```

### See Also {.unnumbered .unlisted}

[`asin()`](#man-asin),
[`atan()`](#man-atan),
[`atan2()`](#man-atan),
[`cos()`](#man-cos)


[[pagebreak]]
## `asin()`, `asinf()`, `asinl()` {#man-asin}

Calculate the arc sine of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double asin(double x);
float asinf(float x);
long double asinl(long double x);
```

### Description {.unnumbered .unlisted}

Calculates the arc sine of a number in radians. (That is, the value
whose sine is `x`.)  The number must be in the range -1.0 to
1.0.

For those of you who don't remember, radians are another way of
measuring an angle, just like degrees. To convert from degrees to
radians or the other way around, use the following code:

``` {.c}
degrees = radians * 180.0f / M_PI;
radians = degrees * M_PI / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the arc sine of `x`, unless `x` is out of
range. In that case, `errno` will be set to EDOM and the
return value will be NaN. The variants return different types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
double asinx;
long double ldasinx;

asinx = asin(0.2);
ldasinx = asinl((long double)0.3);
```

### See Also {.unnumbered .unlisted}

[`acos()`](#man-acos),
[`atan()`](#man-atan),
[`atan2()`](#man-atan),
[`sin()`](#man-sin)


[[pagebreak]]
## `atan()`, `atanf()`, `atanl()`, `atan2()`, `atan2f()`, `atan2l()` {#man-atan}

Calculate the arc tangent of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double atan(double x);
float atanf(float x);
long double atanl(long double x);

double atan2(double y, double x);
float atan2f(float y, float x);
long double atan2l(long double y, long double x);
```

### Description {.unnumbered .unlisted}

Calculates the arc tangent of a number in radians. (That is, the value
whose tangent is `x`.)

The `atan2()` variants are pretty much the same as using
`atan()` with `y`/`x` as the
argument...except that `atan2()` will use those values to
determine the correct quadrant of the result.

For those of you who don't remember, radians are another way of
measuring an angle, just like degrees. To convert from degrees to
radians or the other way around, use the following code:

``` {.c}
degrees = radians * 180.0f / M_PI;
radians = degrees * M_PI / 180;
```

### Return Value {.unnumbered .unlisted}

The `atan()` functions return the arc tangent of
`x`, which will be between PI/2 and -PI/2. The
`atan2()` functions return an angle between PI and -PI.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
double atanx;
long double ldatanx;

atanx = atan(0.2);
ldatanx = atanl((long double)0.3);

atanx = atan2(0.2);
ldatanx = atan2l((long double)0.3);
```

### See Also {.unnumbered .unlisted}

[`tan()`](#man-tan),
[`asin()`](#man-asin),
[`atan()`](#man-acos)

[[pagebreak]]
## `cos()`, `cosf()`, `cosl()` {#man-cos}

Calculate the cosine of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double cos(double x)
float cosf(float x)
long double cosl(long double x)
```

### Description {.unnumbered .unlisted}

Calculates the cosine of the value `x`, where `x` is
in radians.

For those of you who don't remember, radians are another way of
measuring an angle, just like degrees. To convert from degrees to
radians or the other way around, use the following code:

``` {.c}
degrees = radians * 180.0f / M_PI;
radians = degrees * M_PI / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the cosine of `x`. The variants return different
types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
double sinx;
long double ldsinx;

sinx = sin(3490.0); // round and round we go!
ldsinx = sinl((long double)3.490);
```

### See Also {.unnumbered .unlisted}

[`sin()`](#man-sin),
[`tan()`](#man-tan),
[`acos()`](#man-acos)

[[pagebreak]]
## `sin()`, `sinf()`, `sinl()` {#man-sin}

Calculate the sine of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double sin(double x);
float sinf(float x);
long double sinl(long double x);
```

### Description {.unnumbered .unlisted}

Calculates the sine of the value `x`, where `x` is
in radians.

For those of you who don't remember, radians are another way of
measuring an angle, just like degrees. To convert from degrees to
radians or the other way around, use the following code:

``` {.c}
degrees = radians * 180.0f / M_PI;
radians = degrees * M_PI / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the sine of `x`. The variants return different
types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
double sinx;
long double ldsinx;

sinx = sin(3490.0); // round and round we go!
ldsinx = sinl((long double)3.490);
```

### See Also {.unnumbered .unlisted}

[`cos()`](#man-cos),
[`tan()`](#man-tan),
[`asin()`](#man-asin)

[[pagebreak]]
## `tan()`, `tanf()`, `tanl()` {#man-tan}

Calculate the tangent of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double tan(double x)
float tanf(float x)
long double tanl(long double x)
```

### Description {.unnumbered .unlisted}

Calculates the tangent of the value `x`, where `x` is
in radians.

For those of you who don't remember, radians are another way of
measuring an angle, just like degrees. To convert from degrees to
radians or the other way around, use the following code:

``` {.c}
degrees = radians * 180.0f / M_PI;
radians = degrees * M_PI / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the tangent of `x`. The variants return different
types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
double tanx;
long double ldtanx;

tanx = tan(3490.0); // round and round we go!
ldtanx = tanl((long double)3.490);
```

### See Also {.unnumbered .unlisted}

[`sin()`](#man-sin),
[`cos()`](#man-cos),
[`atan()`](#man-atan),
[`atan2()`](#man-atan)

[[pagebreak]]
## `acosh()`, `acoshf()`, `acoshl()` {#man-acosh}

Compute arc hyperbolic cosine.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double acosh(double x);

float acoshf(float x);

long double acoshl(long double x);
```

### Description {.unnumbered .unlisted}

Trig lovers can rejoice! C has arc hyperbolic cosine!

These functions return the nonnegative acosh of `x`, which must be
greater than or equal to `1`.

### Return Value {.unnumbered .unlisted}

Returns the arc hyperbolic cosince in the range $[0,+\infty]$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("acosh 1.8 = %f\n", acosh(1.8));  // 1.192911
```

### See Also {.unnumbered .unlisted}

[`asinh()`](#man-asinh)

[[pagebreak]]
## `asinh()`, `asinhf()`, `asinhl()` {#man-asinh}

Compute arc hyperbolic sine.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double asinh(double x);

float asinhf(float x);

long double asinhl(long double x);
```

### Description {.unnumbered .unlisted}

Trig lovers can rejoice! C has arc hyperbolic sine!

These functions return the asinh of `x`.

### Return Value {.unnumbered .unlisted}

Returns the arc hyperbolic sine.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("acosh 1.8 = %f\n", asinh(1.8));  // 1.350441
```

### See Also {.unnumbered .unlisted}

[`acosh()`](#man-acosh)

[[pagebreak]]
## `atanh()`, `atanhf()`, `atanhl()` {#man-atanh}

Compute the arc hyperbolic tangent.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double atanh(double x);

float atanhf(float x);

long double atanhl(long double x);
```

### Description {.unnumbered .unlisted}

These functions compute the arc hyperbolic tangent of `x`, which must be
in the range $[-1,+1]$. Passing exactly $-1$ or $+1$ might result in a
pole error.

### Return Value {.unnumbered .unlisted}

Returns the arc hyperbolic tangent of `x`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("atanh 0.5 = %f\n", atanh(0.5));  // 0.549306
```

### See Also {.unnumbered .unlisted}
[`acosh()`](#man-acosh),
[`asinh()`](#man-asinh)


[[pagebreak]]
## `cosh()`, `coshf()`, `coshl()` {#man-cosh}

Compute the hyperbolic cosine.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double cosh(double x);

float coshf(float x);

long double coshl(long double x);
```

### Description {.unnumbered .unlisted}

These functions predictably compute the hyperbolic cosine of `x`. A
range error might occur if `x` is too large.

### Return Value {.unnumbered .unlisted}

Returns the hyperbolic cosine of `x`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("cosh 0.5 = %f\n", cosh(0.5));  // 1.127626
```

### See Also {.unnumbered .unlisted}

[`sinh()`](#man-sinh),
[`tanh()`](#man-tanh)

[[pagebreak]]
## `tanh()`, `tanhf()`, `tanhl()` {#man-tanh}

Compute the hyperbolic tangent.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double tanh(double x);

float tanhf(float x);

long double tanhl(long double x);
```

### Description {.unnumbered .unlisted}

These functions predictably compute the hyperbolic tangent of `x`.

Mercifully, this is the last trig-related man page I'm going to write.

### Return Value {.unnumbered .unlisted}

Returns the hyperbolic tangent of `x`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("tanh 0.5 = %f\n", tanh(0.5));  // 0.462117
```

### See Also {.unnumbered .unlisted}

[`cosh()`](#man-cosh),
[`sinh()`](#man-sinh)

[[pagebreak]]
## `exp()`, `expf()`, `expl()` {#man-exp}

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double exp(double x);

float expf(float x);

long double expl(long double x);
```

### Description {.unnumbered .unlisted}

Compute $e^x$ where $e$ is [flw[Euler's
number|E_(mathematical_constant)]].

The number $e$ is named after Leonard Euler, born April 15, 1707, who is
responsible, among other things, for making this reference page longer
than it needed to be.

### Return Value {.unnumbered .unlisted}

Returns $e^x$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("exp(1) = %f\n", exp(1));  // 2.718282
printf("exp(2) = %f\n", exp(2));  // 7.389056
```

### See Also {.unnumbered .unlisted}

[`exp2()`](#man-exp2),
[`expm1()`](#man-expm1),
[`pow()`](#man-pow),
[`log()`](#man-log)

[[pagebreak]]
## `exp2()`, `exp2f()`, `exp2l()` {#man-exp2}

Compute 2 to a power.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double exp2(double x);

float exp2f(float x);

long double exp2l(long double x);
```

### Description {.unnumbered .unlisted}

These functions raise 2 to a power. Very exciting, since computers are
all about twos-to-powers!

These are likely to be faster than using `pow()` to do the same thing.

They support fractional exponents, as well.

A range error occurs if `x` is too large.

### Return Value {.unnumbered .unlisted}

`exp2()` returns $2^x$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("2^3 = %f\n", exp2(3));      // 2^3 = 8.000000
printf("2^8 = %f\n", exp2(8));      // 2^8 = 256.000000
printf("2^0.5 = %f\n", exp2(0.5));  // 2^0.5 = 1.414214
```

### See Also {.unnumbered .unlisted}
[`exp()`](#man-exp),
[`pow()`](#man-pow)


[[pagebreak]]
## `expm1()`, `expm1f()`, `expm1l()` {#man-expm1}

Compute $e^x-1$.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double expm1(double x);

float expm1f(float x);

long double expm1l(long double x);
```

### Description {.unnumbered .unlisted}

This is just like `exp()` except---_plot twist!_--it computes that
result minus one.

For more discussion about what $e$ is, see [the `exp()` man
page](#man-exp).

If `x` is giant, a range error might occur.

For small values of `x` near zero, `expm1(x)` might be more accurate
than computing `exp(x)-1`.

### Return Value {.unnumbered .unlisted}

Returns $e^x-1$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("%f\n", expm1(2.34));  // 9.381237
```
### See Also {.unnumbered .unlisted}

[`exp()`](#man-exp)


[[pagebreak]]
## `frexp()`, `frexpf()`, `frexpl()` {#man-frexp}

Break a number into its fraction part and exponent (as a power of 2)

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double frexp(double value, int *exp);

float frexpf(float value, int *exp);

long double frexpl(long double value, int *exp);
```

### Description {.unnumbered .unlisted}

If you have a floating point number, you can break it into its
fractional part and exponent part (as a power of 2).

For example, if you have the number $1234.56$, this can be represented
as a multiple of a power of 2 like so:

$1234.56=0.6028125\times2^{11}$

And you can use this function to get the $0.6028125$ and $11$ parts of
that equation.

As for why, I have a simple answer: I don't know. I can't find a use.
K&R2 and everyone else I can find just says _how_ to use it, but not
_why_ you might want to.

The C99 Rationale document says:

> The functions `frexp`, `ldexp`, and `modf` are primitives used by the
> remainder of the library.
>
> There was some sentiment for dropping them for the same reasons that
> `ecvt`, `fcvt`, and `gcvt` were dropped, but their adherents rescued
> them for general use. Their use is problematic: on non-binary
> architectures, ldexp may lose precision and frexp may be inefficient. 

So there you have it. If you need it.

### Return Value {.unnumbered .unlisted}

`frexp()` returns the fractional part of `value` in the range 0.5
(inclusive) to 1 (exclusive), or 0. And it stores the exponent
power-of-2 in the variable pointed to by `exp`.

If you pass in zero, the return value and the variable `exp` points to
are both zero.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
double frac;
int expt;

frac = frexp(1234.56, &expt);
printf("1234.56 = %.7f x 2^%d\n", frac, expt);
```

Output:

```
1234.56 = 0.6028125 x 2^11
```

### See Also {.unnumbered .unlisted}

[`ldexp()`](#man-ldexp),
[`ilogb()`](#man-ldexp),
[`modf()`](#man-modf)


[[pagebreak]]
## `ilogb()`, `ilogbf()`, `ilogbl()` {#man-ilogb}

Return the base-2 exponent of a floating point number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

int ilogb(double x);

int ilogbf(float x);

int ilogbl(long double x);
```

### Description {.unnumbered .unlisted}

This returns the integer base-2 exponent of the given number. Basically,
if you pass in $x$, it will return $y$ as an integer where:

$x=2^y$

roughly.

Or, if you're more mathy, it returns the integer portion of:

$\log_2x$

The value of FP_ILOGB0 shall be either INT_MIN or-INT_MAX . The value of
FP_ILOGBNAN shall be either INT_MAX or INT_MIN.

Domain or range errors might occur for invalid values of `x`, or for
return values that are outside the range of the return type.

### Return Value {.unnumbered .unlisted}

A truncated integer value $y$ where $x=2^y$. In other words, a truncated
$\log_2x$.

If you pass in `0`, it'll return `FP_ILOGB0`.

If you pass in infinity, it'll return `INT_MAX`.

If you pass in NaN, it'll return `FP_ILOGBNAN`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("%d\n", ilogb(257));  // 8
printf("%d\n", ilogb(256));  // 8
printf("%d\n", ilogb(255));  // 7
```

### See Also {.unnumbered .unlisted}

[`frexp()`](#man-frexp),
[`logb()`](#man-frexp)


[[pagebreak]]
## `ldexp()`, `ldexpf()`, `ldexpl()` {#man-ldexp}

Multiply a number by an integral power of 2.

### Synopsis {.unnumbered .unlisted}

```
#include <math.h>

double ldexp(double x, int exp);

float ldexpf(float x, int exp);

long double ldexpl(long double x, int exp);
```

### Description {.unnumbered .unlisted}

These functions multiply the given number `x` by 2 raised to the `exp`
power.

### Return Value {.unnumbered .unlisted}

Returns $x\times2^{exp}$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("1 x 2^10 = %f\n", ldexp(1, 10));
printf("5.67 x 2^7 = %f\n", ldexp(5.67, 7));
```

Output:

```
1 x 2^10 = 1024.000000
5.67 x 2^7 = 725.760000
```

### See Also {.unnumbered .unlisted}

[`exp()`](#man-exp)


[[pagebreak]]
## `log()`, `logf()`, `logl()` {#man-log}

Compute the natural logarithm

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double log(double x);

float logf(float x);

long double logl(long double x);
```

### Description {.unnumbered .unlisted}

Natural logarithms! And there was much rejoycing. 

These compute the base-$e$ logarithm of a number, $\log_ex$, $\ln x$.

In other words, for a given $x$, solves $x=e^y$ for $y$.

### Return Value {.unnumbered .unlisted}

The base-$e$ logarithm of the given value, $\log_ex$, $\ln x$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
const double e = 2.718281828459045;

printf("%f\n", log(3490.2));  // 8.157714
printf("%f\n", log(e));       // 1.000000
```

### See Also {.unnumbered .unlisted}
[`exp()`](#man-exp),
[`log10()`](#man-log10),
[`logp1()`](#man-log10)

[[pagebreak]]
## `log10()`, `log10f()`, `log10l()` {#man-log10}

Compute the log-base-10 of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double log10(double x);

float log10f(float x);

long double log10l(long double x);
```

### Description {.unnumbered .unlisted}

Just when you thought you might have to use Laws of Logarithms to
compute this, here's a function coming out of the blue to save you.

These compute the base-$10$ logarithm of a number, $\log_{10}x$.

In other words, for a given $x$, solves $x=10^y$ for $y$.

### Return Value {.unnumbered .unlisted}

Returns the log base-10 of `x`, $\log_{10}x$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("%f\n", log10(3490.2));   // 3.542850
printf("%f\n", log10(10));       // 1.000000
```

### See Also {.unnumbered .unlisted}

[`pow()`](#man-pow),
[`log()`](#man-log)

[[pagebreak]]
## `logp1()`, `logp1f()`, `logp1l()` {#man-logp1}

Compute the natural logarithm of a number plus 1.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double log1p(double x);

float log1pf(float x);

long double log1pl(long double x);
```

### Description {.unnumbered .unlisted}

This computes $\log_e(1 + x)$, $\ln(1+x)$.

This works just like calling:

``` {.c}
log(1 + x)
```

except it could be more accurate for small values of `x`.

So if your `x` is small magnitude, use this.

### Return Value {.unnumbered .unlisted}

Returns $\log_e(1 + x)$, $\ln(1+x)$.

### Example {.unnumbered .unlisted}

Compute some big and small logarithm values to see the difference
between `log1p()` and `log()`:

``` {.c .numberLines}
printf("Big log1p()  : %.*Lf\n", LDBL_DECIMAL_DIG-1, log1pl(9));
printf("Big log()    : %.*Lf\n", LDBL_DECIMAL_DIG-1, logl(1 + 9));

printf("Small log1p(): %.*Lf\n", LDBL_DECIMAL_DIG-1, log1pl(0.01));
printf("Small log()  : %.*Lf\n", LDBL_DECIMAL_DIG-1, logl(1 + 0.01));
```

Output on my system:

```
Big log1p()  : 2.30258509299404568403
Big log()    : 2.30258509299404568403
Small log1p(): 0.00995033085316808305
Small log()  : 0.00995033085316809164
```

### See Also {.unnumbered .unlisted}

[`log()`](#man-log)


[[pagebreak]]
## `log2()`, `log2f()`, `log2l()` {#man-log2}

Compute the base-2 logarithm of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double log2(double x);

float log2f(float x);

long double log2l(long double x);
```

### Description {.unnumbered .unlisted}

Wow! Were you thinking we were done with the logarithm functions? We're
only getting started!

This one computes $\log_2 x$. That is, computes $y$ that satisfies
$x=2^y$.

Love me those powers of 2!

### Return Value {.unnumbered .unlisted}

Returns the base-2 logarithm of the given value, $\log_2 x$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("%f\n", log2(3490.2));  // 11.769094
printf("%f\n", log2(256));     // 8.000000
```

### See Also {.unnumbered .unlisted}

[`log()`](#man-log)

<!-- MARKER -->

[[pagebreak]]
## `sqrt()` {#man-sqrt}

Calculate the square root of a number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double sqrt(double x);
float sqrtf(float x);
long double sqrtl(long double x);
```

### Description {.unnumbered .unlisted}

Computes the square root of a number. To those of you who don't know
what a square root is, I'm not going to explain. Suffice it to say, the
square root of a number delivers a value that when squared (multiplied
by itself) results in the original number.

Ok, fine---I did explain it after all, but only because I wanted
to show off. It's not like I'm giving you examples or anything, such as
the square root of nine is three, because when you multiply three by
three you get nine, or anything like that. No examples. I hate
examples!

And I suppose you wanted some actual practical information here as
well. You can see the usual trio of functions here---they all
compute square root, but they take different types as arguments. Pretty
straightforward, really.

### Return Value {.unnumbered .unlisted}

Returns (and I know this must be something of a surprise to you) the
square root of `x`. If you try to be smart and pass a negative
number in for `x`, the global variable `errno` will be
set to `EDOM` (which stands for DOMain Error, not some kind of
cheese.)

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
// example usage of sqrt()

float something = 10;

double x1 = 8.2, y1 = -5.4;
double x2 = 3.8, y2 = 34.9;
double dx, dy;

printf("square root of 10 is %.2f\n", sqrtf(something));

dx = x2 - x1;
dy = y2 - y1;
printf("distance between points (x1, y1) and (x2, y2): %.2f\n",
    sqrt(dx*dx + dy*dy));
```

And the output is:

```shell
square root of 10 is 3.16
distance between points (x1, y1) and (x2, y2): 40.54
```

### See Also {.unnumbered .unlisted}

[`hypot()`](#man-tan)

<!--
[[pagebreak]]
## `example()`, `example()`, `example()` {#man-example}

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

