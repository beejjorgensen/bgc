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

In short, you're a fricking MATHEMATICAL GOD!

Oh wait, before then, I should tell you that the trig functions have
three variants with different suffixes. The "f" suffix (e.g.
`sinf()`) returns a `float`, while the "l" suffix
(e.g. `sinl()`) returns a massive and nicely accurate
`long double`. Normal `sin()` just returns a
`double`. These are extensions to ANSI C, but they should be
supported by modern compilers.

Also, there are several values that are defined in the
`math.h` header file.

**`M_E`**
e
**`M_LOG2E`**
log_2 e
**`M_LOG10E`**
log_10 e
**`M_LN2`**
log_e 2
**`M_LN10`**
log_e 10
**`M_PI`**
pi
**`M_PI_2`**
pi/2
**`M_PI_4`**
pi/4
**`M_1_PI`**
1/pi
**`M_2_PI`**
2/pi
**`M_2_SQRTPI`**
2/sqrt(pi)
**`M_SQRT2`**
sqrt(2)
**`M_SQRT1_2`**
1/sqrt(2)

## `sin()`, `sinf()`, `sinl()` {#man-sin}

Calculate the sine of a number.

#### Synopsis

``` {.c}
#include <math.h>

double sin(double x);
float sinf(float x);
long double sinl(long double x);
```

#### Description 

Calculates the sine of the value `x`, where `x` is
in radians.

For those of you who don't remember, radians are another way of
measuring an angle, just like degrees. To convert from degrees to
radians or the other way around, use the following code:

``` {.c}
degrees = radians * 180.0f / M_PI;
radians = degrees * M_PI / 180;
```

#### Return Value

Returns the sine of `x`. The variants return different
types.

#### Example

``` {.c}
double sinx;
long double ldsinx;

sinx = sin(3490.0); // round and round we go!
ldsinx = sinl((long double)3.490);
```

#### See Also

[`cos()`](#cos),
[`tan()`](#tan),
[`asin()`](#asin)

## `cos()`, `cosf()`, `cosl()` {#man-cos}

Calculate the cosine of a number.

#### Synopsis

``` {.c}
#include <math.h>

double cos(double x)
float cosf(float x)
long double cosl(long double x)
```

#### Description 

Calculates the cosine of the value `x`, where `x` is
in radians.

For those of you who don't remember, radians are another way of
measuring an angle, just like degrees. To convert from degrees to
radians or the other way around, use the following code:

``` {.c}
degrees = radians * 180.0f / M_PI;
radians = degrees * M_PI / 180;
```

#### Return Value

Returns the cosine of `x`. The variants return different
types.

#### Example

``` {.c}
double sinx;
long double ldsinx;

sinx = sin(3490.0); // round and round we go!
ldsinx = sinl((long double)3.490);
```

#### See Also

[`sin()`](#sin),
[`tan()`](#tan),
[`acos()`](#acos)

## `tan()`, `tanf()`, `tanl()` {#man-tan}

Calculate the tangent of a number.

#### Synopsis

``` {.c}
#include <math.h>

double tan(double x)
float tanf(float x)
long double tanl(long double x)
```

#### Description 

Calculates the tangent of the value `x`, where `x` is
in radians.

For those of you who don't remember, radians are another way of
measuring an angle, just like degrees. To convert from degrees to
radians or the other way around, use the following code:

``` {.c}
degrees = radians * 180.0f / M_PI;
radians = degrees * M_PI / 180;
```

#### Return Value

Returns the tangent of `x`. The variants return different
types.

#### Example

``` {.c}
double tanx;
long double ldtanx;

tanx = tan(3490.0); // round and round we go!
ldtanx = tanl((long double)3.490);
```

#### See Also

[`sin()`](#sin),
[`cos()`](#cos),
[`atan()`](#atan),
[`atan2()`](#atan)

## `asin()`, `asinf()`, `asinl()` {#man-asin}

Calculate the arc sine of a number.

#### Synopsis

``` {.c}
#include <math.h>

double asin(double x);
float asinf(float x);
long double asinl(long double x);
```

#### Description 

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

#### Return Value

Returns the arc sine of `x`, unless `x` is out of
range. In that case, `errno` will be set to EDOM and the
return value will be NaN. The variants return different types.

#### Example

``` {.c}
double asinx;
long double ldasinx;

asinx = asin(0.2);
ldasinx = asinl((long double)0.3);
```

#### See Also

[`acos()`](#acos),
[`atan()`](#atan),
[`atan2()`](#atan),
[`sin()`](#sin)

## `acos()`, `acosf()`, `acosl()` {#man-acos}

Calculate the arc cosine of a number.

#### Synopsis

``` {.c}
#include <math.h>

double acos(double x);
float acosf(float x);
long double acosl(long double x);
```

#### Description 

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

#### Return Value

Returns the arc cosine of `x`, unless `x` is out of
range. In that case, `errno` will be set to EDOM and the
return value will be NaN. The variants return different types.

#### Example

``` {.c}
double acosx;
long double ldacosx;

acosx = acos(0.2);
ldacosx = acosl((long double)0.3);
```

#### See Also

[`asin()`](#asin),
[`atan()`](#atan),
[`atan2()`](#atan),
[`cos()`](#cos)

## `atan()`, `atanf()`, `atanl()`, {#man-atan}
`atan2()`, `atan2f()`, `atan2l()`

Calculate the arc tangent of a number.

#### Synopsis

``` {.c}
#include <math.h>

double atan(double x);
float atanf(float x);
long double atanl(long double x);

double atan2(double y, double x);
float atan2f(float y, float x);
long double atan2l(long double y, long double x);
```

#### Description 

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

#### Return Value

The `atan()` functions return the arc tangent of
`x`, which will be between PI/2 and -PI/2. The
`atan2()` functions return an angle between PI and -PI.

#### Example

``` {.c}
double atanx;
long double ldatanx;

atanx = atan(0.2);
ldatanx = atanl((long double)0.3);

atanx = atan2(0.2);
ldatanx = atan2l((long double)0.3);
```

#### See Also

[`tan()`](#tan),
[`asin()`](#asin),
[`atan()`](#acos)

## `sqrt()` {#man-sqrt}

Calculate the square root of a number

#### Synopsis

``` {.c}
#include <math.h>

double sqrt(double x);
float sqrtf(float x);
long double sqrtl(long double x);
```

#### Description 

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

#### Return Value

Returns (and I know this must be something of a surprise to you) the
square root of `x`. If you try to be smart and pass a negative
number in for `x`, the global variable `errno` will be
set to `EDOM` (which stands for DOMain Error, not some kind of
cheese.)

#### Example

``` {.c}
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

#### See Also

[`hypot()`](#tan)


