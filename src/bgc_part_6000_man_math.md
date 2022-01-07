<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<math.h>` Mathematics {#math}

Many of the following functions have `float` and `long double` variants
as described [below](#func-idioms) (e.g. `pow()`, `powf()`, `powl()`).
The `float` and `long double` variants are omitted from the following
table to keep your eyeballs from melting out.

|Function|Description|
|--------|----------------------|
|[`acos()`](#man-acos)|Calculate the arc cosine of a number.|
|[`acosh()`](#man-acosh)|Compute arc hyperbolic cosine.|
|[`asin()`](#man-asin)|Calculate the arc sine of a number.|
|[`asinh()`](#man-asinh)|Compute arc hyperbolic sine.|
|[`atan()`](#man-atan), [`atan2()`](#man-atan)|Calculate the arc tangent of a number.|
|[`atanh()`](#man-atanh)|Compute the arc hyperbolic tangent.|
|[`cbrt()`](#man-cbrt)|Compute the cube root.|
|[`ceil()`](#man-ceil)|Ceiling---return the next whole number not smaller than the given number.|
|[`copysign()`](#man-copysign)|Copy the sign of one value into another.|
|[`cos()`](#man-cos)|Calculate the cosine of a number.|
|[`cosh()`](#man-cosh)|Compute the hyperbolic cosine.|
|[`erf()`](#man-erf)|Compute the error function of the given value.|
|[`erfc()`](#man-erfc)|Compute the complementary error function of a value.|
|[`exp()`](#man-exp)|Compute $e$ raised to a power.|
|[`exp2()`](#man-exp2)|Compute 2 to a power.|
|[`expm1()`](#man-expm1)|Compute $e^x-1$.|
|[`fabs()`](#man-fabs)|Compute the absolute value.|
|[`fdim()`](#man-fdim)|Return the positive difference between two numbers clamped at 0.|
|[`floor()`](#man-floor)|Compute the largest whole number not larger than the given value.|
|[`fma()`](#man-fma)|Floating (AKA "Fast") multiply and add.|
|[`fmax()`](#man-fmax), [`fmin()`](#man-fmax)|Return the maximum or minimum of two numbers.|
|[`fmod()`](#man-fmod)|Compute the floating point remainder.|
|[`fpclassify()`](#man-fpclassify)|Return the classification of a given floating point number.|
|[`frexp()`](#man-frexp)|Break a number into its fraction part and exponent (as a power of 2).|
|[`hypot()`](#man-hypot)|Compute the length of the hypotenuse of a triangle.|
|[`ilogb()`](#man-ilogb)|Return the exponent of a floating point number.|
|[`isfinite()`](#man-isnan)|True if the number is not infinite or NaN.|
|[`isgreater()`](#man-isgreater)|True if one argument is greater than another.|
|[`isgreatereequal()`](#man-isgreater)|True if one argument is greater than or equal to another.|
|[`isinf()`](#man-isnan)|True if the number is infinite.|
|[`isless()`](#man-isgreater)|True if one argument is less than another.|
|[`islesseequal()`](#man-isgreater)|True if one argument is less than or equal to another.|
|[`islessgreater()`](#man-islessgreater)|Test if a floating point number is less than or greater than another.|
|[`isnan()`](#man-isnan)|True if the number is Not-a-Number.|
|[`isnormal()`](#man-isnan)|True if the number is normal.|
|[`isunordered()`](#man-isunordered)|Macro returns true if either floating point argument is NaN.|
|[`ldexp()`](#man-ldexp)|Multiply a number by an integral power of 2.|
|[`lgamma()`](#man-lgamma)|Compute the natural logarithm of the absolute value of $\Gamma(x)$.|
|[`log()`](#man-log)|Compute the natural logarithm.|
|[`log10()`](#man-log10)|Compute the log-base-10 of a number.|
|[`log2()`](#man-log2)|Compute the base-2 logarithm of a number.|
|[`logb()`](#man-logb)|Extract the exponent of a number given `FLT_RADIX`.|
|[`log1p()`](#man-log1p)|Compute the natural logarithm of a number plus 1.|
|[`lrint()`](#man-lrint)|Returns `x` rounded in the current rounding direction as an integer.|
|[`lround()`](#man-lround), [`llround()`](#man-lround)|Round a number in the good old-fashioned way, returning an integer.|
|[`modf()`](#man-modf)|Extract the integral and fractional parts of a number.|
|[`nan()`](#man-nan)|Return `NAN`.|
|[`nearbyint()`](#man-nearbyint)|Rounds a value in the current rounding direction.|
|[`nextafter()`](#man-nextafter)|Get the next (or previous) representable floating point value.|
|[`nexttoward()`](#man-nexttoward)|Get the next (or previous) representable floating point value.|
|[`pow()`](#man-pow)|Compute a value raised to a power.|
|[`remainder()`](#man-remainder)|Compute the remainder IEC 60559-style.|
|[`remquo()`](#man-remquo)|Compute the remainder and (some of the) quotient.|
|[`rint()`](#man-rint)|Rounds a value in the current rounding direction.|
|[`round()`](#man-round)|Round a number in the good old-fashioned way.|
|[`scalbn()`](#man-scalb), [`scalbln()`](#man-scalb)|Efficiently compute $x\times r^n$, where $r$ is `FLT_RADIX`.|
|[`signbit()`](#man-signbit)|Return the sign of a number.|
|[`sin()`](#man-sin)|Calculate the sine of a number.|
|[`sqrt()`](#man-sqrt)|Calculate the square root of a number.|
|[`tan()`](#man-tan)|Calculate the tangent of a number.|
|[`tanh()`](#man-tanh)|Compute the hyperbolic tangent.|
|[`tgamma()`](#man-tgamma)|Compute the gamma function, $\Gamma(x)$.|
|[`trunc()`](#man-trunc)|Truncate the fractional part off a floating point value.|


It's your favorite subject: Mathematics! Hello, I'm Doctor Math, and
I'll be making math FUN and EASY!

_[vomiting sounds]_

Ok, I know math isn't the grandest thing for some of you out there,
but these are merely functions that quickly and easily do math you
either know, want, or just don't care about. That pretty much covers
it.

## Math Function Idioms {#func-idioms}

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
So if you pass a `double` to `powf()`, it'll choose the closest
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

But here are a couple:

`NAN` represents Not-A-Number.

Defined in `<float.h>` is `FLT_RADIX`: the number base used by floating
point numbers. This is commonly `2`, but could be anything.

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


[[manbreak]]
## `fpclassify()` {#man-fpclassify}

Return the classification of a given floating point number.

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

``` {.default}
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

[[manbreak]]
## `isfinite()`, `isinf()`, `isnan()`, `isnormal()` {#man-isnan}

Return true if a number matches a classification.

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


[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%d\n", signbit(3490.0));  // 0
	printf("%d\n", signbit(-37.0));   // 1
}
```

### See Also {.unnumbered .unlisted}

[`fpclassify()`](#man-fpclassify),
[`isfinite()`](#man-isnan),
[`isinf()`](#man-isnan),
[`isnan()`](#man-isnan),
[`isnormal()`](#man-isnan),
[`copysign()`](#man-copysign)


[[manbreak]]
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
pi = 3.14159265358979;
degrees = radians * 180 / pi;
radians = degrees * pi / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the arc cosine of `x`, unless `x` is out of
range. In that case, `errno` will be set to EDOM and the
return value will be NaN. The variants return different types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	double acosx;
	long double ldacosx;

	acosx = acos(0.2);
	ldacosx = acosl(0.3L);

	printf("%f\n", acosx);
	printf("%Lf\n", ldacosx);
}
```

### See Also {.unnumbered .unlisted}

[`asin()`](#man-asin),
[`atan()`](#man-atan),
[`atan2()`](#man-atan),
[`cos()`](#man-cos)


[[manbreak]]
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
pi = 3.14159265358979;
degrees = radians * 180 / pi;
radians = degrees * pi / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the arc sine of `x`, unless `x` is out of
range. In that case, `errno` will be set to EDOM and the
return value will be NaN. The variants return different types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	double asinx;
	long double ldasinx;

	asinx = asin(0.2);
	ldasinx = asinl(0.3L);

	printf("%f\n", asinx);
	printf("%Lf\n", ldasinx);
}
```

### See Also {.unnumbered .unlisted}

[`acos()`](#man-acos),
[`atan()`](#man-atan),
[`atan2()`](#man-atan),
[`sin()`](#man-sin)


[[manbreak]]
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
pi = 3.14159265358979;
degrees = radians * 180 / pi;
radians = degrees * pi / 180;
```

### Return Value {.unnumbered .unlisted}

The `atan()` functions return the arc tangent of
`x`, which will be between PI/2 and -PI/2. The
`atan2()` functions return an angle between PI and -PI.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	double atanx;
	long double ldatanx;

	atanx = atan(0.7);
	ldatanx = atanl(0.3L);

	printf("%f\n", atanx);
	printf("%Lf\n", ldatanx);

	atanx = atan2(7, 10);
	ldatanx = atan2l(3L, 10L);

	printf("%f\n", atanx);
	printf("%Lf\n", ldatanx);
}
```

### See Also {.unnumbered .unlisted}

[`tan()`](#man-tan),
[`asin()`](#man-asin),
[`atan()`](#man-acos)

[[manbreak]]
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
pi = 3.14159265358979;
degrees = radians * 180 / pi;
radians = degrees * pi / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the cosine of `x`. The variants return different
types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	double cosx;
	long double ldcosx;

	cosx = cos(3490.0); // round and round we go!
	ldcosx = cosl(3.490L);

	printf("%f\n", cosx);
	printf("%Lf\n", ldcosx);
}
```

### See Also {.unnumbered .unlisted}

[`sin()`](#man-sin),
[`tan()`](#man-tan),
[`acos()`](#man-acos)

[[manbreak]]
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
pi = 3.14159265358979;
degrees = radians * 180 / pi;
radians = degrees * pi / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the sine of `x`. The variants return different
types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	double sinx;
	long double ldsinx;

	sinx = sin(3490.0); // round and round we go!
	ldsinx = sinl(3.490L);

	printf("%f\n", sinx);
	printf("%Lf\n", ldsinx);
}
```

### See Also {.unnumbered .unlisted}

[`cos()`](#man-cos),
[`tan()`](#man-tan),
[`asin()`](#man-asin)

[[manbreak]]
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
pi = 3.14159265358979;
degrees = radians * 180 / pi;
radians = degrees * pi / 180;
```

### Return Value {.unnumbered .unlisted}

Returns the tangent of `x`. The variants return different
types.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	double tanx;
	long double ldtanx;

	tanx = tan(3490.0); // round and round we go!
	ldtanx = tanl(3.490L);

	printf("%f\n", tanx);
	printf("%Lf\n", ldtanx);
}
```

### See Also {.unnumbered .unlisted}

[`sin()`](#man-sin),
[`cos()`](#man-cos),
[`atan()`](#man-atan),
[`atan2()`](#man-atan)

[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("acosh 1.8 = %f\n", acosh(1.8));  // 1.192911
}
```

### See Also {.unnumbered .unlisted}

[`asinh()`](#man-asinh)

[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("asinh 1.8 = %f\n", asinh(1.8));  // 1.350441
}
```

### See Also {.unnumbered .unlisted}

[`acosh()`](#man-acosh)

[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("atanh 0.5 = %f\n", atanh(0.5));  // 0.549306
}
```

### See Also {.unnumbered .unlisted}
[`acosh()`](#man-acosh),
[`asinh()`](#man-asinh)


[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("cosh 0.5 = %f\n", cosh(0.5));  // 1.127626
}
```

### See Also {.unnumbered .unlisted}

[`sinh()`](#man-sinh),
[`tanh()`](#man-tanh)

[[manbreak]]
## `sinh()`, `sinhf()`, `sinhl()` {#man-sinh}

Compute the hyperbolic sine.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double sinh(double x);

float sinhf(float x);

long double sinhl(long double x);
```

### Description {.unnumbered .unlisted}

These functions predictably compute the hyperbolic sine of `x`. A
range error might occur if `x` is too large.

### Return Value {.unnumbered .unlisted}

Returns the hyperbolic sine of `x`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("sinh 0.5 = %f\n", sinh(0.5));  // 0.521095
}
```

### See Also {.unnumbered .unlisted}

[`sinh()`](#man-cosh),
[`tanh()`](#man-tanh)

[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("tanh 0.5 = %f\n", tanh(0.5));  // 0.462117
}
```

### See Also {.unnumbered .unlisted}

[`cosh()`](#man-cosh),
[`sinh()`](#man-sinh)

[[manbreak]]
## `exp()`, `expf()`, `expl()` {#man-exp}

Compute $e$ raised to a power.

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
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("exp(1) = %f\n", exp(1));  // 2.718282
    printf("exp(2) = %f\n", exp(2));  // 7.389056
}
```

### See Also {.unnumbered .unlisted}

[`exp2()`](#man-exp2),
[`expm1()`](#man-expm1),
[`pow()`](#man-pow),
[`log()`](#man-log)

[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("2^3 = %f\n", exp2(3));      // 2^3 = 8.000000
    printf("2^8 = %f\n", exp2(8));      // 2^8 = 256.000000
    printf("2^0.5 = %f\n", exp2(0.5));  // 2^0.5 = 1.414214    
}
```

### See Also {.unnumbered .unlisted}
[`exp()`](#man-exp),
[`pow()`](#man-pow)


[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%f\n", expm1(2.34));  // 9.381237
}
```

### See Also {.unnumbered .unlisted}

[`exp()`](#man-exp)


[[manbreak]]
## `frexp()`, `frexpf()`, `frexpl()` {#man-frexp}

Break a number into its fraction part and exponent (as a power of 2).

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
#include <stdio.h>
#include <math.h>

int main(void)
{
    double frac;
    int expt;

    frac = frexp(1234.56, &expt);
    printf("1234.56 = %.7f x 2^%d\n", frac, expt);  
}
```

Output:

``` {.default}
1234.56 = 0.6028125 x 2^11
```

### See Also {.unnumbered .unlisted}

[`ldexp()`](#man-ldexp),
[`ilogb()`](#man-ldexp),
[`modf()`](#man-modf)


[[manbreak]]
## `ilogb()`, `ilogbf()`, `ilogbl()` {#man-ilogb}

Return the exponent of a floating point number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

int ilogb(double x);

int ilogbf(float x);

int ilogbl(long double x);
```

### Description {.unnumbered .unlisted}

This gives you the exponent of the given number... it's a little weird,
because the exponent depends on the value of `FLT_RADIX`. Now, this is
very often `2`---but no guarantees!

It actually returns $\log_r|x|$ where $r$ is `FLT_RADIX`.

Domain or range errors might occur for invalid values of `x`, or for
return values that are outside the range of the return type.

### Return Value {.unnumbered .unlisted}

The exponent of the absolute value of the given number, depending on
`FLT_RADIX`. 

Specifically $\log_r|x|$ where $r$ is `FLT_RADIX`.

If you pass in `0`, it'll return `FP_ILOGB0`.

If you pass in infinity, it'll return `INT_MAX`.

If you pass in NaN, it'll return `FP_ILOGBNAN`.

The spec goes on to say that the value of `FP_ILOGB0` will be either
`INT_MIN` or `-INT_MAX`. And the value of `FP_ILOGBNAN` shall be either
`INT_MAX` or `INT_MIN`, if that's useful in any way.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%d\n", ilogb(257));  // 8
    printf("%d\n", ilogb(256));  // 8
    printf("%d\n", ilogb(255));  // 7
}
```

### See Also {.unnumbered .unlisted}

[`frexp()`](#man-frexp),
[`logb()`](#man-logb)


[[manbreak]]
## `ldexp()`, `ldexpf()`, `ldexpl()` {#man-ldexp}

Multiply a number by an integral power of 2.

### Synopsis {.unnumbered .unlisted}

``` {.c}
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
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("1 x 2^10 = %f\n", ldexp(1, 10));
    printf("5.67 x 2^7 = %f\n", ldexp(5.67, 7));
}
```

Output:

``` {.default}
1 x 2^10 = 1024.000000
5.67 x 2^7 = 725.760000
```

### See Also {.unnumbered .unlisted}

[`exp()`](#man-exp)


[[manbreak]]
## `log()`, `logf()`, `logl()` {#man-log}

Compute the natural logarithm.

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
#include <stdio.h>
#include <math.h>

int main(void)
{
    const double e = 2.718281828459045;

    printf("%f\n", log(3490.2));  // 8.157714
    printf("%f\n", log(e));       // 1.000000
}
```

### See Also {.unnumbered .unlisted}
[`exp()`](#man-exp),
[`log10()`](#man-log10),
[`log1p()`](#man-log10)

[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%f\n", log10(3490.2));   // 3.542850
    printf("%f\n", log10(10));       // 1.000000
}
```

### See Also {.unnumbered .unlisted}

[`pow()`](#man-pow),
[`log()`](#man-log)

[[manbreak]]
## `log1p()`, `log1pf()`, `log1pl()` {#man-log1p}

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
#include <stdio.h>
#include <float.h> // for LDBL_DECIMAL_DIG
#include <math.h>

int main(void)
{
    printf("Big log1p()  : %.*Lf\n", LDBL_DECIMAL_DIG-1, log1pl(9));
    printf("Big log()    : %.*Lf\n", LDBL_DECIMAL_DIG-1, logl(1 + 9));

    printf("Small log1p(): %.*Lf\n", LDBL_DECIMAL_DIG-1, log1pl(0.01));
    printf("Small log()  : %.*Lf\n", LDBL_DECIMAL_DIG-1, logl(1 + 0.01));
}
```

Output on my system:

``` {.default}
Big log1p()  : 2.30258509299404568403
Big log()    : 2.30258509299404568403
Small log1p(): 0.00995033085316808305
Small log()  : 0.00995033085316809164
```

### See Also {.unnumbered .unlisted}

[`log()`](#man-log)


[[manbreak]]
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
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("%f\n", log2(3490.2));  // 11.769094
    printf("%f\n", log2(256));     // 8.000000
}
```

### See Also {.unnumbered .unlisted}

[`log()`](#man-log)

[[manbreak]]
## `logb()`, `logbf()`, `logbl()` {#man-logb}

Extract the exponent of a number given `FLT_RADIX`.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double logb(double x);

float logbf(float x);

long double logbl(long double x);
```

### Description {.unnumbered .unlisted}

This function returns the whole number portion of the exponent of the
number with radix `FLT_RADIX`, namely the whole number portion
$\log_r|x|$ where $r$ is `FLT_RADIX`. Fractional numbers are truncated.

If the number is [flw[subnormal|Denormal_number]], `logb()` treats it as
if it were normalized.

If `x` is `0`, there could be a domain error or pole error.

### Return Value {.unnumbered .unlisted}

This function returns the whole number portion of $\log_r|x|$ where $r$
is `FLT_RADIX`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <float.h>  // For FLT_RADIX
#include <math.h>

int main(void)
{
    printf("FLT_RADIX = %d\n", FLT_RADIX);
    printf("%f\n", logb(3490.2));
    printf("%f\n", logb(256));
}
```

Output:

``` {.default}
FLT_RADIX = 2
11.000000
8.000000
```

### See Also {.unnumbered .unlisted}

[`ilogb()`](#man-ilogb)


[[manbreak]]
## `modf()`, `modff()`, `modfl()` {#man-modf}

Extract the integral and fractional parts of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double modf(double value, double *iptr);

float modff(float value, float *iptr);

long double modfl(long double value, long double *iptr);
```

### Description {.unnumbered .unlisted}

If you have a floating point number, like `123.456`, this function will
extract the integral part (`123.0`) and the fractional part (`0.456`).
It's total coincidence that this is exactly the plot for the latest
Jason Statham action spectacular.

Both the integral part and fractional parts keep the sign of the passed
in `value`.

The integral part is stored in the address pointed to by `iptr`.

See the note in [`frexp()`](#man-frexp) regarding why this is in the
library.

### Return Value {.unnumbered .unlisted}

These functions return the fractional part of the number. The integral
part is stored in the address pointed to by `iptr`. Both the integral
and fractional parts preserve the sign of the passed-in `value`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

void print_parts(double x)
{
    double i, f;

    f = modf(x, &i);

    printf("Entire number  : %f\n", x);
    printf("Integral part  : %f\n", i);
    printf("Fractional part: %f\n\n", f);
}

int main(void)
{
    print_parts(123.456);
    print_parts(-123.456);
}
```

Output:

``` {.default}
Entire number  : 123.456000
Integral part  : 123.000000
Fractional part: 0.456000

Entire number  : -123.456000
Integral part  : -123.000000
Fractional part: -0.456000
```

### See Also {.unnumbered .unlisted}

[`frexp()`](#man-frexp)

[[manbreak]]
## `scalbn()`, `scalbnf()`, `scalbnl()` `scalbln()`, `scalblnf()`, `scalblnl()` {#man-scalb}

Efficiently compute $x\times r^n$, where $r$ is `FLT_RADIX`.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double scalbn(double x, int n);

float scalbnf(float x, int n);

long double scalbnl(long double x, int n);

double scalbln(double x, long int n);

float scalblnf(float x, long int n);

long double scalblnl(long double x, long int n);
```

### Description {.unnumbered .unlisted}
These functions efficiently compute $x\times r^n$, where $r$ is
`FLT_RADIX`.

If `FLT_RADIX` happens to be `2` (no guarantees!), then this works like
[`exp2()`](#man-exp2).

The name of this function should have an obvious meaning to you. Clearly
they all start with the prefix "scalb" which means...

...OK, I confess! I have no idea what it means. My searches are futile!

But let's look at the suffixes:

|Suffix|Meaning|
|-|-|
|`n`|`scalbn()`---exponent `n` is an `int`|
|`nf`|`scalbnf()`---`float` version of `scalbn()`|
|`nl`|`scalbnl()`---`long double` version of `scalbn()`|
|`ln`|`scalbln()`---exponent `n` is a `long int`|
|`lnf`|`scalblnf()`---`float` version of `scalbln()`|
|`lnl`|`scalblnl()`---`long double` version of `scalbln()`|

So while I'm still in the dark about "scalb", at least I have that part
down.

A range error might occur for large values.

### Return Value {.unnumbered .unlisted}

Returns $x\times r^n$, where $r$ is `FLT_RADIX`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <float.h>

int main(void)
{
    printf("FLT_RADIX = %d\n\n", FLT_RADIX);
    printf("scalbn(3, 8)      = %f\n", scalbn(2, 8));
    printf("scalbnf(10.2, 20) = %f\n", scalbnf(10.2, 20));
}
```

Output on my system:

``` {.default}
FLT_RADIX = 2

scalbn(3, 8)       = 512.000000
scalbn(10.2, 20.7) = 10695475.200000
```

### See Also {.unnumbered .unlisted}

[`exp2()`](#man-exp2),
[`pow()`](#man-pow)


[[manbreak]]
## `cbrt()`, `cbrtf()`, `cbrtl()` {#man-cbrt}

Compute the cube root.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double cbrt(double x);

float cbrtf(float x);

long double cbrtl(long double x);
```

### Description {.unnumbered .unlisted}

Computes the cube root of `x`, $x^{1/3}$, $\sqrt[3]{x}$.

### Return Value {.unnumbered .unlisted}

Returns the cube root of `x`, $x^{1/3}$, $\sqrt[3]{x}$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("cbrt(1729.03) = %f\n", cbrt(1729.03));
}
```

Output:

``` {.default}
cbrt(1729.03) = 12.002384
```

### See Also {.unnumbered .unlisted}

[`sqrt()`](#man-sqrt),
[`pow()`](#man-pow)

[[manbreak]]
## `fabs()`, `fabsf()`, `fabsl()` {#man-fabs}

Compute the absolute value.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double fabs(double x);

float fabsf(float x);

long double fabsl(long double x);
```

### Description {.unnumbered .unlisted}

These functions straightforwardly return the absolute value of `x`, that
is $|x|$.

If you're rusty on your absolute values, all it means is that the result
will be positive, even if `x` is negative. It's just strips negative
signs off.

### Return Value {.unnumbered .unlisted}

Returns the absolute value of `x`, $|x|$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("fabs(3490.0)  = %f\n", fabs(3490.0));  // 3490.000000
    printf("fabs(-3490.0) = %f\n", fabs(3490.0));  // 3490.000000
}
```

### See Also {.unnumbered .unlisted}

[`abs()`](#man-abs),
[`copysign()`](#man-copysign),
[`imaxabs()`](#man-imaxabs)

[[manbreak]]
## `hypot()`, `hypotf()`, `hypotl()` {#man-hypot}

Compute the length of the hypotenuse of a triangle.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double hypot(double x, double y);

float hypotf(float x, float y);

long double hypotl(long double x, long double y);
```

### Description {.unnumbered .unlisted}

[flw[Pythagorean Theorem|Pythagorean_theorem]] fans rejoice! This is the
function you've been waiting for!

If you know the lengths of the two sides of a right triangle, `x` and
`y`, you can compute the length of the hypotenuse (the longest, diagonal
side) with this function.

In particular, it computes the square root of the sum of the squares of
the sides: $\sqrt{x^2 + y^2}$.

### Return Value {.unnumbered .unlisted}

Returns the lenght of the hypotenuse of a right triangle with side
lengths `x` and `y`: $\sqrt{x^2 + y^2}$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("%f\n", hypot(3, 4));  // 5.000000
```

### See Also {.unnumbered .unlisted}

[`sqrt()`](#man-sqrt)

[[manbreak]]
## `pow()`, `powf()`, `powl()` {#man-pow}

Compute a value raised to a power.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double pow(double x, double y);

float powf(float x, float y);

long double powl(long double x, long double y);
```

### Description {.unnumbered .unlisted}

Computes `x` raised to the `y`th power: $x^y$.

These arguments can be fractional.

### Return Value {.unnumbered .unlisted}

Returns `x` raised to the `y`th power: $x^y$.

A domain error can occur if:

* `x` is a finite negative number and `y` is a finite non-integer
* `x` is zero and `y` is zero.

A domain error or pole error can occur if `x` is zero and `y` is
negative.

A range error can occur for large values.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("%f\n", pow(3, 4));    // 3^4    = 81.000000
printf("%f\n", pow(2, 0.5));  // sqrt 2 = 1.414214
```

### See Also {.unnumbered .unlisted}

[`exp()`](#man-exp),
[`exp2()`](#man-exp2),
[`sqrt()`](#man-sqrt),
[`cbrt()`](#man-cbrt)

[[manbreak]]
## `sqrt()` {#man-sqrt}

Calculate the square root of a number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double sqrt(double x);

float sqrtf(float x);

long double sqrtl(long double x);
```

### Description {.unnumbered .unlisted}

Computes the square root of a number: $\sqrt{x}$. To those of you who
don't know what a square root is, I'm not going to explain. Suffice it
to say, the square root of a number delivers a value that when squared
(multiplied by itself) results in the original number.

Ok, fine---I did explain it after all, but only because I wanted
to show off. It's not like I'm giving you examples or anything, such as
the square root of nine is three, because when you multiply three by
three you get nine, or anything like that. No examples. I hate
examples!

And I suppose you wanted some actual practical information here as
well. You can see the usual trio of functions here---they all
compute square root, but they take different types as arguments. Pretty
straightforward, really.

A domain error occurs if `x` is negative.

### Return Value {.unnumbered .unlisted}

Returns (and I know this must be something of a surprise to you) the
square root of `x`: $\sqrt{x}$.

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

``` {.default}
square root of 10 is 3.16
distance between points (x1, y1) and (x2, y2): 40.54
```

### See Also {.unnumbered .unlisted}

[`hypot()`](#man-hypot),
[`pow()`](#man-pow)

[[manbreak]]
## `erf()`, `erff()`, `erfl()` {#man-erf}

Compute the error function of the given value.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double erfc(double x);

float erfcf(float x);

long double erfcl(long double x);
```

### Description {.unnumbered .unlisted}

These functions compute the [flw[error function|Error_function]] of a
value.

### Return Value {.unnumbered .unlisted}

Returns the error function of `x`:

${\displaystyle \frac{2}{\sqrt\pi} \int_0^x e^{-t^2}\,dt}$

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
for (float i = -2; i <= 2; i += 0.5)
    printf("% .1f: %f\n", i, erf(i));
```

Output:

``` {.default}
-2.0: -0.995322
-1.5: -0.966105
-1.0: -0.842701
-0.5: -0.520500
 0.0: 0.000000
 0.5: 0.520500
 1.0: 0.842701
 1.5: 0.966105
 2.0: 0.995322
```

### See Also {.unnumbered .unlisted}

[`erfc()`](#man-erfc)

[[manbreak]]
## `erfc()`, `erfcf()`, `erfcl()` {#man-erfc}

Compute the complementary error function of a value.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double erfc(double x);

float erfcf(float x);

long double erfcl(long double x);
```

### Description {.unnumbered .unlisted}

These functions compute the [flw[complementary error
function|Error_function]] of a value.

This is the same as:

``` {.c}
1 - erf(x)
```

A range error can occur if `x` is too large.

### Return Value {.unnumbered .unlisted}

Returns `1 - erf(x)`, namely:

${\displaystyle \frac{2}{\sqrt\pi} \int_x^{\infty} e^{-t^2}\,dt}$

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
for (float i = -2; i <= 2; i += 0.5)
    printf("% .1f: %f\n", i, erfc(i));
```

Output:

``` {.default}
-2.0: 1.995322
-1.5: 1.966105
-1.0: 1.842701
-0.5: 1.520500
 0.0: 1.000000
 0.5: 0.479500
 1.0: 0.157299
 1.5: 0.033895
 2.0: 0.004678
```

### See Also {.unnumbered .unlisted}

[`erf()`](#man-erf)

[[manbreak]]
## `lgamma()`, `lgammaf()`, `lgammal()` {#man-lgamma}

Compute the natural logarithm of the absolute value of $\Gamma(x)$.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double lgamma(double x);

float lgammaf(float x);

long double lgammal(long double x);
```

### Description {.unnumbered .unlisted}

Compute the natural log of the absolute value of
[flw[gamma|Gamma_function]] `x`, $\log_e|\Gamma(x)|$.

A range error can occur if `x` is too large.

A pole error can occur is `x` is non-positive.

### Return Value {.unnumbered .unlisted}

Returns $\log_e|\Gamma(x)|$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
for (float i = 0.5; i <= 4; i += 0.5)
    printf("%.1f: %f\n", i, lgamma(i));
```

Output:

``` {.default}
0.5: 0.572365
1.0: 0.000000
1.5: -0.120782
2.0: 0.000000
2.5: 0.284683
3.0: 0.693147
3.5: 1.200974
4.0: 1.791759
```

### See Also {.unnumbered .unlisted}

[`tgamma()`](#man-tgamma)

[[manbreak]]
## `tgamma()`, `tgammaf()`, `tgammal()` {#man-tgamma}

Compute the gamma function, $\Gamma(x)$.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double tgamma(double x);

float tgammaf(float x);

long double tgammal(long double x);
```

### Description {.unnumbered .unlisted}

Computes the [flw[gamma function|Gamma_function]] of `x`, $\Gamma(x)$.

A domain or pole error might occur if `x` is non-positive.

A range error might occur if `x` is too large or too small.

### Return Value {.unnumbered .unlisted}

Returns the gamma function of `x`, $\Gamma(x)$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
for (float i = 0.5; i <= 4; i += 0.5)
    printf("%.1f: %f\n", i, tgamma(i));
```

Output:

``` {.default}
0.5: 1.772454
1.0: 1.000000
1.5: 0.886227
2.0: 1.000000
2.5: 1.329340
3.0: 2.000000
3.5: 3.323351
4.0: 6.000000
```

### See Also {.unnumbered .unlisted}

[`lgamma()`](#man-lgamma)

[[manbreak]]
## `ceil()`, `ceilf()`, `ceill()` {#man-ceil}

Ceiling---return the next whole number not smaller than the given number.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double ceil(double x);

float ceilf(float x);

long double ceill(long double x);
```

### Description {.unnumbered .unlisted}

Returns the ceiling of the `x`: $\lceil{x}\rceil$.

This is the next whole number not smaller than `x`.

Beware this minor dragon: it's not just "rounding up". Well, it is for
positive numbers, but negative numbers effectively round toward zero.
(Because the ceiling function is headed for the next largest whole
number and $-4$ is larger than $-5$.)

### Return Value {.unnumbered .unlisted}

Returns the next largest whole number larger than `x`.

### Example {.unnumbered .unlisted}

Notice for the negative numbers it heads toward zero, i.e. toward the
next largest whole number---just like the positives head toward the next
largest whole number.

``` {.c .numberLines}
printf("%f\n", ceil(4.0));   //  4.000000
printf("%f\n", ceil(4.1));   //  5.000000
printf("%f\n", ceil(-2.0));  // -2.000000
printf("%f\n", ceil(-2.1));  // -2.000000
printf("%f\n", ceil(-3.1));  // -3.000000
```

### See Also {.unnumbered .unlisted}

[`floor()`](#man-floor),
[`round()`](#man-round)

[[manbreak]]
## `floor()`, `floorf()`, `floorl()` {#man-floor}

Compute the largest whole number not larger than the given value.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>
double floor(double x);
float floorf(float x);
long double floorl(long double x);
```

### Description {.unnumbered .unlisted}

Returns the floor of the value: $\lfloor{x}\rfloor$. This is the
opposite of `ceil()`.

This is the largest whole number that is not greater than `x`.

For positive numbers, this is like rounding down: `4.5` becomes `4.0`.

For negative numbers, it's like rounding up: `-3.6` becomes `-4.0`.

In both cases, those results are the largest whole number not bigger
than the given number.

### Return Value {.unnumbered .unlisted}

Returns the largest whole number not greater than `x`:
$\lfloor{x}\rfloor$.

### Example {.unnumbered .unlisted}

Note how the negative numbers effectively round away from zero, unlike
the positives.

``` {.c .numberLines}
printf("%f\n", floor(4.0));   //  4.000000
printf("%f\n", floor(4.1));   //  4.000000
printf("%f\n", floor(-2.0));  // -2.000000
printf("%f\n", floor(-2.1));  // -3.000000
printf("%f\n", floor(-3.1));  // -4.000000
```

### See Also {.unnumbered .unlisted}

[`ceil()`](#man-ceil),
[`round()`](#man-round)


[[manbreak]]
## `nearbyint()`, `nearbyintf()`, `nearbyintl()` {#man-nearbyint}

Rounds a value in the current rounding direction.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double nearbyint(double x);

float nearbyintf(float x);

long double nearbyintl(long double x);
```

### Description {.unnumbered .unlisted}

This function rounds `x` to the nearest integer in the current rounding
direction.

The rounding direction can be set with [`fesetround()`](#man-fegetround)
in `<fenv.h>`.

`nearbyint()` won't raise the "inexact" floating point exception.

### Return Value {.unnumbered .unlisted}

Returns `x` rounded in the current rounding direction.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

int main(void)
{
    #pragma STDC FENV_ACCESS ON        // If supported

    fesetround(FE_TONEAREST);          // round to nearest

    printf("%f\n", nearbyint(3.14));   // 3.000000
    printf("%f\n", nearbyint(3.74));   // 4.000000

    fesetround(FE_TOWARDZERO);         // round toward zero

    printf("%f\n", nearbyint(1.99));   // 1.000000
    printf("%f\n", nearbyint(-1.99));  // -1.000000
}
```

### See Also {.unnumbered .unlisted}

[`rint()`](#man-rint),
[`lrint()`](#man-lrint),
[`round()`](#man-round),
[`fesetround()`](#man-fegetround),
[`fegetround()`](#man-fegetround)

[[manbreak]]
## `rint()`, `rintf()`, `rintl()` {#man-rint}

Rounds a value in the current rounding direction.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double rint(double x);

float rintf(float x);

long double rintl(long double x);
```

### Description {.unnumbered .unlisted}

This works just like [`nearbyint()`](#man-nearbyint) except that is can
raise the "inexact" floating point exception.

### Return Value {.unnumbered .unlisted}

Returns `x` rounded in the current rounding direction.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

int main(void)
{
    #pragma STDC FENV_ACCESS ON

    fesetround(FE_TONEAREST);

    printf("%f\n", rint(3.14));   // 3.000000
    printf("%f\n", rint(3.74));   // 4.000000

    fesetround(FE_TOWARDZERO);

    printf("%f\n", rint(1.99));   // 1.000000
    printf("%f\n", rint(-1.99));  // -1.000000
}
```

### See Also {.unnumbered .unlisted}

[`nearbyint()`](#man-nearbyint),
[`lrint()`](#man-lrint),
[`round()`](#man-round),
[`fesetround()`](#man-fegetround),
[`fegetround()`](#man-fegetround)

[[manbreak]]
## `lrint()`, `lrintf()`, `lrintl()`, `llrint()`, `llrintf()`, `llrintl()` {#man-lrint}

Returns `x` rounded in the current rounding direction as an integer.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

long int lrint(double x);
long int lrintf(float x);
long int lrintl(long double x);

long long int llrint(double x);
long long int llrintf(float x);
long long int llrintl(long double x);
```

### Description {.unnumbered .unlisted}

Round a floating point number in the current rounding direction, but
this time return an integer intead of a float. You know, just to mix it
up.

These come in two variants:

* `lrint()`---returns `long int`
* `llrint()`---returns `long long int`

If the result doesn't fit in the return type, a domain or range error
might occur.

### Return Value {.unnumbered .unlisted}

The value of `x` rounded to an integer in the current rounding
direction.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <fenv.h>

int main(void)
{
    #pragma STDC FENV_ACCESS ON

    fesetround(FE_TONEAREST);

    printf("%ld\n", lrint(3.14));   // 3
    printf("%ld\n", lrint(3.74));   // 4

    fesetround(FE_TOWARDZERO);

    printf("%ld\n", lrint(1.99));   // 1
    printf("%ld\n", lrint(-1.99));  // -1
}
```

### See Also {.unnumbered .unlisted}

[`nearbyint()`](#man-nearbyint),
[`rint()`](#man-lrint),
[`round()`](#man-round),
[`fesetround()`](#man-fegetround),
[`fegetround()`](#man-fegetround)

[[manbreak]]
## `round()`, `roundf()`, `roundl()` {#man-round}

Round a number in the good old-fashioned way.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double round(double x);

float roundf(float x);

long double roundl(long double x);
```

### Description {.unnumbered .unlisted}

Rounds a number to the nearest whole value.

In case of halfsies, rounds away from zero (i.e. "round up" in
magnitude).

The current rounding direction's Jedi mind tricks don't work on this
function.

### Return Value {.unnumbered .unlisted}

The rounded value of `x`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%f\n", round(3.14));   // 3.000000
	printf("%f\n", round(3.5));    // 4.000000

	printf("%f\n", round(-1.5));   // -2.000000
	printf("%f\n", round(-1.14));  // -1.000000
}
```

### See Also {.unnumbered .unlisted}

[`lround()`](#man-lround),
[`nearbyint()`](#man-nearbyint),
[`rint()`](#man-lrint),
[`lrint()`](#man-lrint),
[`trunc()`](#man-trunc)

[[manbreak]]
## `lround()`, `lroundf()`, `lroundl()` `llround()`, `llroundf()`, `llroundl()` {#man-lround}

Round a number in the good old-fashioned way, returning an integer.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

long int lround(double x);
long int lroundf(float x);
long int lroundl(long double x);

long long int llround(double x);
long long int llroundf(float x);
long long int llroundl(long double x);
```

### Description {.unnumbered .unlisted}

These are just like [`round()`](#man-round) except they return integers.

Halfway values round away from zero, e.g. $1.5$ rounds to $2$ and $-1.5$
rounds to $-2$.

The functions are grouped by return type:

* `lround()`---returns a `long int`
* `llround()`---returns a `long long int`

If the rounded value can't find in the return time, a domain or range
error can occur.

### Return Value {.unnumbered .unlisted}

Returns the rounded value of `x` as an integer.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%ld\n", lround(3.14));   // 3
	printf("%ld\n", lround(3.5));    // 4

	printf("%ld\n", lround(-1.5));   // -2
	printf("%ld\n", lround(-1.14));  // -1
}
```

### See Also {.unnumbered .unlisted}

[`round()`](#man-lround),
[`nearbyint()`](#man-nearbyint),
[`rint()`](#man-lrint),
[`lrint()`](#man-lrint),
[`trunc()`](#man-trunc)

[[manbreak]]
## `trunc()`, `truncf()`, `truncl()` {#man-trunc}

Truncate the fractional part off a floating point value.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double trunc(double x);

float truncf(float x);

long double truncl(long double x);
```

### Description {.unnumbered .unlisted}

These functions just drop the fractional part of a floating point
number. Boom.

In other words, they always round toward zero.

### Return Value {.unnumbered .unlisted}

Returns the truncated floating point number.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%f\n", trunc(3.14));   // 3.000000
	printf("%f\n", trunc(3.8));    // 3.000000

	printf("%f\n", trunc(-1.5));   // -1.000000
	printf("%f\n", trunc(-1.14));  // -1.000000
}
```

### See Also {.unnumbered .unlisted}
[`round()`](#man-lround),
[`lround()`](#man-lround),
[`nearbyint()`](#man-nearbyint),
[`rint()`](#man-lrint),
[`lrint()`](#man-lrint)

[[manbreak]]
## `fmod()`, `fmodf()`, `fmodl()` {#man-fmod}

Compute the floating point remainder.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double fmod(double x, double y);

float fmodf(float x, float y);

long double fmodl(long double x, long double y);
```

### Description {.unnumbered .unlisted}

Returns the remainder of $\frac{x}{y}$. The result will have the same
sign as `x`.

Under the hood, the computation performed is:

``` {.c}
x - trunc(x / y) * y
```

But it might be easier just to think of the remainder.

### Return Value {.unnumbered .unlisted}

Returns the remainder of $\frac{x}{y}$ with the same sign as `x`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%f\n", fmod(-9.2, 5.1));  // -4.100000
	printf("%f\n", fmod(9.2, 5.1));   //  4.100000
}
```

### See Also {.unnumbered .unlisted}

[`remainder()`](#man-remainder)

[[manbreak]]
## `remainder()`, `remainderf()`, `remainderl()` {#man-remainder}

Compute the remainder IEC 60559-style.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double remainder(double x, double y);

float remainderf(float x, float y);

long double remainderl(long double x, long double y);
```

### Description {.unnumbered .unlisted}

This is similar to `fmod()`, but not quite the same. `fmod()` is
probably what you're after if you're expecting remainders to wrap around
like an odometer.

The C spec quotes IEC 60559 on how this works:

> When $y\neq0$, the remainder $r=x$ REM $y$ is defined regardless of
> the rounding mode by the mathematical relation $r=x-ny$, where $n$ is
> the integer nearest the exact value of $x/y$; whenever $|n-x/y|=1/2$,
> then $n$ is even. If $r=0$, its sign shall be that of $x$.

Hope that clears it up!

OK, maybe not. Here's the upshot:

You know how if you `fmod()` something by, say `2.0` you get a result
that is somewhere between `0.0` and `2.0`? And how if you just increase
the number that you're modding by `2.0`, you can see the result climb up
to `2.0` and then wrap around to `0.0` like your car's odometer?

`remainder()` works just like that, except if `y` is `2.0`, it wraps
from `-1.0` to `1.0` instead of from `0.0` to `2.0`.

In other words, the range of the function runs from `-y/2` to `y/2`.
Contrasted to `fmod()` that runs from `0.0` to `y`, `remainder()`'s
output is just shifted down half a `y`.

And zero-remainder-anything is `0`.

Except if `y` is zero, the function might return zero or a domain error
might occur.

### Return Value {.unnumbered .unlisted}

The IEC 60559 result of `x`-remainder-`y`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%f\n", remainder(3.7, 4));  // -0.300000
	printf("%f\n", remainder(4.3, 4));  //  0.300000
}
```

### See Also {.unnumbered .unlisted}

[`fmod()`](#man-fmod),
[`remquo()`](#man-remquo)

[[manbreak]]
## `remquo()`, `remquof()`, `remquol()` {#man-remquo}

Compute the remainder and (some of the) quotient.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double remquo(double x, double y, int *quo);

float remquof(float x, float y, int *quo);

long double remquol(long double x, long double y, int *quo);
```

### Description {.unnumbered .unlisted}

This is a funky little thing.

First of all, the return value is the remainder, the same as the
[`remainder()`](#man-remainder) function, so check that out.

And the quotient comes back in the `quo` pointer.

Or at least _some of it_ does. You'll get at least 3 bits worth of the
quotient.

But _why_?

So a couple things.

One is that the quotient of some very large floating point numbers can
easily be far too gigantic to fit in even a `long long unsigned int`. So
some of it might very well need to be lopped off, anyway.

But at 3 bits? How's that even useful? That only gets you from 0 to 7!

The C99 Rationale document states:

> The `remquo` functions are intended for implementing argument
> reductions which can exploit a few low-order bits of the quotient.
> Note that $x$ may be so large in magnitude relative to $y$ that an
> exact representation of the quotient is not practical.

So... implementing argument reductions... which can exploit a few
low-order bits... Ooookay.

[fl[CPPReference has this to
say|https://en.cppreference.com/w/c/numeric/math/remquo]] on the matter,
which is spoken so well, I will quote wholesale:

> This function is useful when implementing periodic functions with the
> period exactly representable as a floating-point value: when
> calculating $\sin(πx)$ for a very large `x`, calling `sin` directly
> may result in a large error, but if the function argument is first
> reduced with `remquo`, the low-order bits of the quotient may be used
> to determine the sign and the octant of the result within the period,
> while the remainder may be used to calculate the value with high
> precision.

And there you have it. If you have another example that works for you...
congratulations! :)

### Return Value {.unnumbered .unlisted}

Returns the same as [`remainder`](#man-remainder): The IEC 60559 result
of `x`-remainder-`y`.

In addition, at least the lowest 3 bits of the quotient will be stored
in `quo` with the same sign as `x/y`.

### Example {.unnumbered .unlisted}

There's a [fl[great `cos()` example at
CPPReference|https://en.cppreference.com/w/c/numeric/math/remquo]] that
covers a genuine use case.

But instead of stealing it, I'll just post a simple example here and you
can visit their site for a real one.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	int quo;
	double rem;

	rem = remquo(12.75, 2.25, &quo);

	printf("%d remainder %f\n", quo, rem);  // 6 remainder -0.750000
}
```

### See Also {.unnumbered .unlisted}

[`remainder()`](#man-remainder),
[`imaxdiv()`](#man-imaxdiv)

[[manbreak]]
## `copysign()`, `copysignf()`, `copysignl()` {#man-copysign}

Copy the sign of one value into another.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double copysign(double x, double y);

float copysignf(float x, float y);

long double copysignl(long double x, long double y);
```

### Description {.unnumbered .unlisted}

These functions return a number that has the magnitude of `x` and the
sign of `y`. You can use them to coerce the sign to that of another
value.

Neither `x` nor `y` are modified, of course. The return value holds the
result.

### Return Value {.unnumbered .unlisted}

Returns a value with the magnitude of `x` and the sign of `y`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	double x = 34.9;
	double y = -999.9;
	double z = 123.4;

	printf("%f\n", copysign(x, y)); // -34.900000
	printf("%f\n", copysign(x, z)); //  34.900000
}
```

### See Also {.unnumbered .unlisted}

[`signbit()`](#man-signbit)

[[manbreak]]
## `nan()`, `nanf()`, `nanl()` {#man-nan}

Return `NAN`.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double nan(const char *tagp);

float nanf(const char *tagp);

long double nanl(const char *tagp);
```

### Description {.unnumbered .unlisted}

These functions return a quiet NaN^[A _quiet NaN_ is one that doesn't
raise any exceptions.]. It is produced as if calling
[`strtod()`](#man-strtod) with `"NAN"` (or a variant thereof) as an
argument.

`tagp` points to a string which could be several things, including
empty. The contents of the string determine which variant of NaN might
get returned depending on the implementation.

Which _version_ of NaN? Did you even know it was possible to get this
far into the weeds with something that wasn't a number?

Case 1 in which you pass in an empty string, in which case these are the
same:

``` {.c}
nan("");

strtod("NAN()", NULL);
```

Case 2 in which the string contains only digits 0-9, letters a-z,
letters A-Z, and/or underscore:

``` {.c}
nan("goats");

strtod("NAN(goats)", NULL);
```

And Case 3, in which the string contains anything else and is ignored:

``` {.c}
nan("!");

strtod("NAN", NULL);
```

As for what `strtod()` does with those values in parens, see the
[`strtod()`] reference page. Spoiler: it's implementation-defined.

### Return Value {.unnumbered .unlisted}

Returns the requested quiet NaN, or 0 if such things aren't supported by
your system.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%f\n", nan(""));       // nan
	printf("%f\n", nan("goats"));  // nan
	printf("%f\n", nan("!"));      // nan
}
```

### See Also {.unnumbered .unlisted}

[`strtod()`](#man-strtod)

[[manbreak]]
## `nextafter()`, `nextafterf()`, `nextafterl()` {#man-nextafter}

Get the next (or previous) representable floating point value.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double nextafter(double x, double y);

float nextafterf(float x, float y);

long double nextafterl(long double x, long double y);
```

### Description {.unnumbered .unlisted}

As you probably know, floating point numbers can't represent _every_
possible real number. There are limits.

And, as such, there exists a "next" and "previous" number after or
before any floating point number.

These functions return the next (or previous) representable number. That
is, no floating point numbers exist between the given number and the
next one.

The way it figures it out is it works from `x` in the direction of `y`,
answering the question of "what is the next representable number from
`x` as we head toward `y`.

### Return Value {.unnumbered .unlisted}

Returns the next representable floating point value from `x` in the
direction of `y`.

If `x` equals `y`, returns `y`. And also `x`, I suppose.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%.*f\n", DBL_DECIMAL_DIG, nextafter(0.5, 1.0));
	printf("%.*f\n", DBL_DECIMAL_DIG, nextafter(0.349, 0.0));
}
```

Output on my system:

``` {.default}
0.50000000000000011
0.34899999999999992
```

### See Also {.unnumbered .unlisted}

[`nexttoward()`](#man-nexttoward)

[[manbreak]]
## `nexttoward()`, `nexttowardf()`, `nexttowardl()` {#man-nexttoward}

Get the next (or previous) representable floating point value.

### Synopsis {.unnumbered .unlisted}

``` {.c}
include <math.h>

double nexttoward(double x, long double y);

float nexttowardf(float x, long double y);

long double nexttowardl(long double x, long double y);
```

### Description {.unnumbered .unlisted}

These functions are the same as [`nextafter()`](#man-nextafter) except
the second parameter is always `long double`.

### Return Value {.unnumbered .unlisted}

Returns the same as [`nextafter()`](#man-nextafter) except if `x` equals
`y`, returns `y` cast to the function's return type.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <float.h>
#include <math.h>

int main(void)
{
	printf("%.*f\n", DBL_DECIMAL_DIG, nexttoward(0.5, 1.0));
	printf("%.*f\n", DBL_DECIMAL_DIG, nexttoward(0.349, 0.0));
}
```

Output on my system:

``` {.default}
0.50000000000000011
0.34899999999999992
```

### See Also {.unnumbered .unlisted}

[`nextafter()`](#man-nextafter)

[[manbreak]]
## `fdim()`, `fdimf()`, `fdiml()` {#man-fdim}

Return the positive difference between two numbers clamped at 0.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double fdim(double x, double y);

float fdimf(float x, float y);

long double fdiml(long double x, long double y);
```

### Description {.unnumbered .unlisted}

The positive difference between `x` and `y` is the difference... except
if the difference is less than `0`, it's clamped to `0`.

These functions might throw a range error.

### Return Value {.unnumbered .unlisted}

Returns the difference of `x-y` if the difference is greater than `0`.
Otherwise it returns `0`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%f\n", fdim(10.0, 3.0));   // 7.000000
	printf("%f\n", fdim(3.0, 10.0));   // 0.000000, clamped
}
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `fmax()`, `fmaxf()`, `fmaxl()`, `fmin()`, `fminf()`, `fminl()` {#man-fmax}

Return the maximum or minimum of two numbers.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double fmax(double x, double y);

float fmaxf(float x, float y);

long double fmaxl(long double x, long double y);

double fmin(double x, double y);

float fminf(float x, float y);

long double fminl(long double x, long double y);
```

### Description {.unnumbered .unlisted}

Straightforwardly, these functions return the minimum or maximum of two
given numbers.

If one of the numbers is NaN, the functions return the non-NaN number.
If both arguments are NaN, the functions return NaN.

### Return Value {.unnumbered .unlisted}

Returns the minimum or maximum values, with NaN handled as mentioned
above.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%f\n", fmin(10.0, 3.0));   //  3.000000
	printf("%f\n", fmax(3.0, 10.0));   // 10.000000
}
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `fma()`, `fmaf()`, `fmal()` {#man-fma}

Floating (AKA "Fast") multiply and add.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

double fma(double x, double y, double z);

float fmaf(float x, float y, float z);

long double fmal(long double x, long double y, long double z);
```

### Description {.unnumbered .unlisted}

This performs the operation $(x\times{y})+z$, but does so in a nifty
way. It does the computation as if it had infinite precision, and then
rounds the final result to the final data type according to the current
rounding mode.

Contrast to if you'd do the math yourself, where it would have rounded
each step of the way, potentially.

Also some architectures have a CPU instruction to do exactly this
calculation, so it can do it super quick. (If it doesn't, it's
considerably slower.)

You can tell if your CPU supports the fast version by checking that the
macro `FP_FAST_FMA` is set to `1`. (The `float` and `long` variants of
`fma()` can be tested with `FP_FAST_FMAF` and `FP_FAST_FMAL`,
respectively.)

These functions might cause a range error to occur.

### Return Value {.unnumbered .unlisted}

Returns `(x * y) + z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
printf("%f\n", fma(1.0, 2.0, 3.0));  // 5.000000
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `isgreater()`, `isgreaterequal()`, `isless()`, `islessequal()` {#man-isgreater}

Floating point comparison macros.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

int isgreater(any_floating_type x, any_floating_type y);

int isgreaterequal(any_floating_type x, any_floating_type y);

int isless(any_floating_type x, any_floating_type y);

int islessequal(any_floating_type x, any_floating_type y);
```

### Description {.unnumbered .unlisted}

These macros compare floating point numbers. Being macros, we can pass
in any floating point type.

You might think you can already do that with just regular comparison
operators---and you'd be right!

One one exception: the comparison operators raise the "invalid" floating
exception if one or more of the operands is NaN. These macros do not.

Note that you must only pass floating point types into these functions.
Passing an integer or any other type is undefined behavior.

### Return Value {.unnumbered .unlisted}

`isgreater()` returns the result of `x > y`.

`isgreaterequal()` returns the result of `x >= y`.

`isless()` returns the result of `x < y`.

`islessequal()` returns the result of `x <= y`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%d\n", isgreater(10.0, 3.0));        // 1
	printf("%d\n", isgreaterequal(10.0, 10.0));  // 1
	printf("%d\n", isless(10.0, 3.0));           // 0
	printf("%d\n", islessequal(10.0, 3.0));      // 0
}
```

### See Also {.unnumbered .unlisted}

[`islessgreater()`](#man-islessgreater),
[`isunordered()`](#man-isunordered)

[[manbreak]]
## `islessgreater()` {#man-islessgreater}

Test if a floating point number is less than or greater than another.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

int islessgreater(any_floating_type x, any_floating_type y);
```

### Description {.unnumbered .unlisted}

This macro is similar to `isgreater()` and all those, except it made the
section name too long if I included it up there. So it gets its own
spot.

This returns true if $x < y$ or $x > y$.

Even though it's a macro, we can rest assured that `x` and `y` are only
evaluated once.

And even if `x` or `y` are NaN, this will not throw an "invalid"
exception, unlike the normal comparison operators.

If you pass in a non-floating type, the behavior is undefined.

### Return Value {.unnumbered .unlisted}

Returns `(x < y) || (x > y)`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%d\n", islessgreater(10.0, 3.0));   // 1
	printf("%d\n", islessgreater(10.0, 30.0));  // 1
	printf("%d\n", islessgreater(10.0, 10.0));  // 0
}
```

### See Also {.unnumbered .unlisted}

[`isgreater()`](#man-isgreater),
[`isgreaterequal()`](#man-isgreater),
[`isless()`](#man-isgreater),
[`islessequal()`](#man-isgreater),
[`isunordered()`](#man-isunordered)

[[manbreak]]
## `isunordered()` {#man-isunordered}

Macro returns true if either floating point argument is NaN.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <math.h>

int isunordered(any_floating_type x, any_floating_type y);
```

### Description {.unnumbered .unlisted}

The spec writes:

> The isunordered macro determines whether its arguments are unordered.

See? Told you C was easy!

It does also elaborate that the arguments are unordered if one or both
of them are NaN.

### Return Value {.unnumbered .unlisted}

This macro returns true if one or both of the arguments are NaN.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("%d\n", isunordered(1.0, 2.0));       // 0
	printf("%d\n", isunordered(1.0, sqrt(-1)));  // 1
	printf("%d\n", isunordered(NAN, 30.0));      // 1
	printf("%d\n", isunordered(NAN, NAN));       // 1
}
```

### See Also {.unnumbered .unlisted}

[`isgreater()`](#man-isgreater),
[`isgreaterequal()`](#man-isgreater),
[`isless()`](#man-isgreater),
[`islessequal()`](#man-isgreater),
[`islessgreater()`](#man-islessgreater)

<!--
[[manbreak]]
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

