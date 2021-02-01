<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Mathematics {#math}

It's your favorite subject: Mathematics! Hello, I'm Doctor Math, and
I'll be making math FUN and EASY!

_[vomiting sounds]_

Ok, I know math isn't the grandest thing for some of you out there,
but these are merely functions that quickly and easily do math you
either know, want, or just don't care about. That pretty much covers
it.

For you trig fans out there, we've got all manner of things,
including
[sine](#sin),
[cosine](#cos),
[tangent](#tan), and, conversely, 
[arc sine](#asin),
[arc cosine](#acos), and
[arc tangent](#atan). That's very exciting.

And for normal people, there is a slurry of your run-of-the-mill
functions that will serve your general purpose mathematical needs,
including
[absolute value](#abs),
[hypotenuse length](#hypot),
[square root](#sqrt),
[cube root](#cbrt), and
[power](#pow).

In short, you're a fricking MATHEMATICAL DEITY!

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
## `vprintf()`, `vfprintf()`, `vsprintf()`, `vsnprintf()` {#man-vprintf}

### Synopsis {.unnumbered .unlisted}
### Description {.unnumbered .unlisted}
### Return Value {.unnumbered .unlisted}
### Example {.unnumbered .unlisted}
### See Also {.unnumbered .unlisted}
[`sprintf()`](#man-sprintf),
-->

