<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<complex.h>` Complex Number Functionality {#complex}

The complex functions in this reference section come in three flavors
each: `double complex`, `float complex`, and `long double complex`.

The `float` variants end with `f` and the `long double` variants end
with `l`, e.g. for complex cosine:

``` {.c}
ccos()   double complex
ccosf()  float complex
ccosl()  long double complex
```

The table below only lists the `double complex` version for brevity.

|Function|Description|
|--------|----------------------|
|[`cabs()`](#man-cabs)|Compute the complex absolute value|
|[`cacos()`](#man-cacos)|Compute the complex arc-cosine|
|[`cacosh()`](#man-cacosh)|Compute the complex arc hyperbolic cosine|
|[`carg()`](#man-carg)|Compute the complex argument|
|[`casin()`](#man-casin)|Compute the complex arc-sine|
|[`casinh()`](#man-casinh)|Compute the complex arc hyperbolic sine|
|[`catan()`](#man-catan)|Compute the complex arc-tangent|
|[`catanh()`](#man-catanh)|Compute the complex arc hyperbolic tangent|
|[`ccos()`](#man-ccos)|Compute the complex cosine|
|[`ccosh()`](#man-ccosh)|Compute the complex hyperbolic cosine|
|[`cexp()`](#man-cexp)|Compute the complex base-$e$ exponential|
|[`cimag()`](#man-cimag)|Returns the imaginary part of a complex number|
|[`clog()`](#man-clog)|Compute the complex logarithm|
|[`CMPLX()`](#man-CMPLX)|Build a complex value from real and imaginary types|
|[`conj()`](#man-conj)|Compute the conjugate of a complex number|
|[`cproj()`](#man-cproj)|Compute the projection of a complex number|
|[`creal()`](#man-creal)|Returns the real part of a complex number|
|[`csin()`](#man-csin)|Compute the complex sine|
|[`csinh()`](#man-csinh)|Compute the complex hyperbolic sine|
|[`csqrt()`](#man-csqrt)|Compute the complex square root|
|[`ctan()`](#man-ctan)|Compute the complex tangent|
|[`ctanh()`](#man-ctanh)|Compute the complex hyperbolic tangent|

You can test for complex number support by looking at the
`__STDC_NO_COMPLEX__` macro. If it's defined, complex numbers aren't
available.

There are possibly two types of numbers defined: _complex_ and
_imaginary_. No system I'm currently aware of implements imaginary
types.

The complex types, which are a real value plus a multiple of $i$, are:

``` {.c}
float complex
double complex
long double complex
```

The imaginary types, which hold a multiple of $i$, are:

``` {.c}
float imaginary
double imaginary
long double imaginary
```

The mathematical value $i=\sqrt{-1}$ is represented by the symbol
`_Complex_I` or `_Imaginary_I`, if it exists.

The The macro `I` will be preferentially set to `_Imaginary_I` (if it
exists), or to `_Complex_I` otherwise.

You can write imaginary literals (if supported) using this notation:

``` {.c}
double imaginary x = 3.4 * I;
```

You can write complex literals using regular complex notation:

``` {.c}
double complex x = 1.2 + 3.4 * I;
```

or build them with the `CMPLX()` macro:

``` {.c}
double complex x = CMPLX(1.2, 3.4);  // Like 1.2 + 3.4 * I
```

The latter has the advantage of handing special cases of complex numbers
correctly (like those involving infinity or signed zeroes) as if
`_Imaginary_I` were present, even if it's not.

All angular values are in radians.

Some functions have discontinuities called _branch cuts_. Now, I'm no
mathematician so I can't really talk sensibly about this, but if you're
here, I like to think you know what you're doing when it comes to this
side of things.

If you system has signed zeroes, you can tell which side of the cut
you're on by the sign. And you can't if you don't. The spec elaborates:

> Implementations that do not support a signed zero [...] cannot
> distinguish the sides of branch cuts. These implementations shall map
> a cut so the function is continuous as the cut is approached coming
> around the finite endpoint of the cut in a counter clockwise
> direction. (Branch cuts for the functions specified here have just one
> finite endpoint.) For example, for the square root function, coming
> counter clockwise around the finite endpoint of the cut along the
> negative real axis approaches the cut from above, so the cut maps to
> the positive imaginary axis.

Finally, there's a pragma called `CX_LIMITED_RANGE` that can be turned
on and off (default is off). You can turn it on with:

``` {.c}
#pragma STDC CX_LIMITED_RANGE ON
```

It allows for certain intermediate operations to underflow, overflow, or
deal badly with infinity, presumably for a tradeoff in speed. If you're
sure these types of errors won't occur with the numbers you're using AND
you're trying to get as much speed out as you can, you could turn this
macro on.

The spec also elaborates here:

> The purpose of the pragma is to allow the implementation to use the
> formulas:
>
> $(x+iy)\times(u+iv) = (xu-yv)+i(yu+xv)$
>
> $(x+iy)/(u+iv) = [(xu+yv)+i(yu-xv)]/(u^2+v^2)$
>
> $|x+iy|=\sqrt{x^2+y^2}$
>
> where the programmer can determine they are safe.


[[manbreak]]
## `cacos()` `cacosf()` `cacosl()` {#man-cacos}

Compute the complex arc-cosine

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex cacos(double complex z);

float complex cacosf(float complex z);

long double complex cacosl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex arc-cosine of a complex number.

The complex number `z` will have an imaginary component in the range
$[0,\pi]$, and the real component is unbounded.

There are branch cuts outside the interval $[-1,+1]$ on the real axis.

### Return Value {.unnumbered .unlisted}

Returns the complex arc-cosine of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = cacos(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 0.195321 + -2.788006i
```

### See Also {.unnumbered .unlisted}

[`ccos()`](#man-ccos),
[`casin()`](#man-casin),
[`catan()`](#man-catan)

[[manbreak]]
## `casin()` `casinf()` `casinl()` {#man-casin}

Compute the complex arc-sine

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex casin(double complex z);

float complex casinf(float complex z);

long double complex casinl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex arc-sine of a complex number.

The complex number `z` will have an imaginary component in the range
$[-\pi/2,+\pi/2]$, and the real component is unbounded.

There are branch cuts outside the interval $[-1,+1]$ on the real axis.

### Return Value {.unnumbered .unlisted}

Returns the complex arc-sine of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = casin(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 1.375476 + 2.788006i
```

### See Also {.unnumbered .unlisted}

[`csin()`](#man-csin),
[`cacos()`](#man-cacos),
[`catan()`](#man-catan)


[[manbreak]]
## `catan()` `catanf()` `catanl()` {#man-catan}

Compute the complex arc-tangent

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex catan(double complex z);

float complex catanf(float complex z);

long double complex catanl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex arc-tangent of a complex number.

The complex number `z` will have an real component in the range
$[-\pi/2,+\pi/2]$, and the imaginary component is unbounded.

There are branch cuts outside the interval $[-i,+i]$ on the imaginary
axis.

### Return Value {.unnumbered .unlisted}

Returns the complex arc-tangent of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double wheat = 8;
    double sheep = 1.5708;

    double complex x = wheat + sheep * I;

    double complex y = catan(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 1.450947 + 0.023299i
```

### See Also {.unnumbered .unlisted}

[`ctan()`](#man-ctan),
[`cacos()`](#man-cacos),
[`casin()`](#man-casin)

[[manbreak]]
## `ccos()` `ccosf()` `ccosl()` {#man-ccos}

Compute the complex cosine

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex ccos(double complex z);

float complex ccosf(float complex z);

long double complex ccosl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex cosine of a complex number.

### Return Value {.unnumbered .unlisted}

Returns the complex cosine of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = ccos(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: -0.365087 + -2.276818i
```

### See Also {.unnumbered .unlisted}

[`csin()`](#man-csin),
[`ctan()`](#man-ctan),
[`cacos()`](#man-cacos)

[[manbreak]]
## `csin()` `csinf()` `csinl()` {#man-csin}

Compute the complex sine

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex csin(double complex z);

float complex csinf(float complex z);

long double complex csinl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex sine of a complex number.

### Return Value {.unnumbered .unlisted}

Returns the complex sine of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = csin(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 2.482485 + -0.334840i
```

### See Also {.unnumbered .unlisted}

[`ccos()`](#man-ccos),
[`ctan()`](#man-ctan),
[`casin()`](#man-casin)


[[manbreak]]
## `ctan()` `ctanf()` `ctanl()` {#man-ctan}

Compute the complex tangent

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex ctan(double complex z);

float complex ctanf(float complex z);

long double complex ctanl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex tangent of a complex number.

### Return Value {.unnumbered .unlisted}

Returns the complex tangent of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = ctan(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: -0.027073 + 1.085990i
```

### See Also {.unnumbered .unlisted}

[`ccos()`](#man-ccos),
[`csin()`](#man-csin),
[`catan()`](#man-catan)


[[manbreak]]
## `cacosh()` `cacoshf()` `cacoshl()` {#man-cacosh}

Compute the complex arc hyperbolic cosine

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex cacosh(double complex z);

float complex cacoshf(float complex z);

long double complex cacoshl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex arc hyperbolic cosine of a complex number.

There is a branch cut at values less than $1$ on the real axis.

The return value will be non-negative on the real number axis, and in
the range $[-i\pi,+i\pi]$ on the imaginary axis.

### Return Value {.unnumbered .unlisted}

Returns the complex arc hyperbolic cosine of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = cacosh(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 2.788006 + 0.195321i
```

### See Also {.unnumbered .unlisted}

[`casinh()`](#man-casinh),
[`catanh()`](#man-catanh),
[`acosh()`](#man-acosh)

[[manbreak]]
## `casinh()` `casinhf()` `casinhl()` {#man-casinh}

Compute the complex arc hyperbolic sine

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex casinh(double complex z);

float complex casinhf(float complex z);

long double complex casinhl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex arc hyperbolic sine of a complex number.

There are branch cuts outside $[-i,+i]$ on the imaginary axis.

The return value will be unbounded on the real number axis, and in
the range $[-i\pi/2,+i\pi/2]$ on the imaginary axis.

### Return Value {.unnumbered .unlisted}

Returns the complex arc hyperbolic sine of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = casinh(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 2.794970 + 0.192476i
```

### See Also {.unnumbered .unlisted}

[`cacosh()`](#man-cacosh),
[`catanh()`](#man-catanh),
[`asinh()`](#man-asinh)

[[manbreak]]
## `catanh()` `catanhf()` `catanhl()` {#man-catanh}

Compute the complex arc hyperbolic tangent

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex catanh(double complex z);

float complex catanhf(float complex z);

long double complex catanhl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex arc hyperbolic tangent of a complex number.

There are branch cuts outside $[-1,+1]$ on the real axis.

The return value will be unbounded on the real number axis, and in
the range $[-i\pi/2,+i\pi/2]$ on the imaginary axis.

### Return Value {.unnumbered .unlisted}

Returns the complex arc hyperbolic tangent of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = catanh(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 0.120877 + 1.546821i
```

### See Also {.unnumbered .unlisted}

[`cacosh()`](#man-cacosh),
[`casinh()`](#man-casinh),
[`atanh()`](#man-atanh)

[[manbreak]]
## `ccosh()` `ccoshf()` `ccoshl()` {#man-ccosh}

Compute the complex hyperbolic cosine

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex ccosh(double complex z);

float complex ccoshf(float complex z);

long double complex ccoshl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex hyperbolic cosine of a complex number.

### Return Value {.unnumbered .unlisted}

Returns the complex hyperbolic cosine of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = ccosh(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: -0.005475 + 1490.478826i
```

### See Also {.unnumbered .unlisted}

[`csinh()`](#man-csinh),
[`ctanh()`](#man-ctanh),
[`ccos()`](#man-ccos)

[[manbreak]]
## `csinh()` `csinhf()` `csinhl()` {#man-csinh}

Compute the complex hyperbolic sine

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex csinh(double complex z);

float complex csinhf(float complex z);

long double complex csinhl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex hyperbolic sine of a complex number.

### Return Value {.unnumbered .unlisted}

Returns the complex hyperbolic sine of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = csinh(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: -0.005475 + 1490.479161i
```

### See Also {.unnumbered .unlisted}

[`ccosh()`](#man-ccosh),
[`ctanh()`](#man-ctanh),
[`csin()`](#man-csin)


[[manbreak]]
## `ctanh()` `ctanhf()` `ctanhl()` {#man-ctanh}

Compute the complex hyperbolic tangent

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex ctanh(double complex z);

float complex ctanhf(float complex z);

long double complex ctanhl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex hyperbolic tangent of a complex number.

### Return Value {.unnumbered .unlisted}

Returns the complex hyperbolic tangent of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 8 + 1.5708 * I;

    double complex y = ctanh(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 1.000000 + -0.000000i
```

### See Also {.unnumbered .unlisted}

[`ccosh()`](#man-ccosh),
[`csinh()`](#man-csinh),
[`ctan()`](#man-ctan)

[[manbreak]]
## `cexp()` `cexpf()` `cexpl()` {#man-cexp}

Compute the complex base-$e$ exponential

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex cexp(double complex z);

float complex cexpf(float complex z);

long double complex cexpl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex base-$e$ exponential of `z`.

### Return Value {.unnumbered .unlisted}

Returns the complex base-$e$ exponential of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2 * I;

    double complex y = cexp(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: -1.131204 + 2.471727i
```

### See Also {.unnumbered .unlisted}

[`cpow()`](#man-cpow),
[`clog()`](#man-clog),
[`exp()`](#man-exp)

[[manbreak]]
## `clog()` `clogf()` `clogl()` {#man-clog}

Compute the complex logarithm

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex clog(double complex z);

float complex clogf(float complex z);

long double complex clogl(long double complex z);
```

### Description {.unnumbered .unlisted}

Compute the base-$e$ complex logarithm of `z`. There is a branch cut on
the negative real axis.

The returns value is unbounded on the real axis and in the range
$[-i\pi,+i\pi]$ on the imaginary axis.

### Return Value {.unnumbered .unlisted}

Returns the base-$e$ complex logarithm of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2 * I;

    double complex y = clog(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 0.804719 + 1.107149i
```

### See Also {.unnumbered .unlisted}

[`cexp()`](#man-cexp),
[`log()`](#man-log)

[[manbreak]]
## `cabs()` `cabsf()` `cabsl()` {#man-cabs}

Compute the complex absolute value

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double cabs(double complex z);

float cabsf(float complex z);

long double cabsl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex absolute value of `z`.

### Return Value {.unnumbered .unlisted}

Returns the complex absolute value of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2 * I;

    double complex y = cabs(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 2.236068 + 0.000000i
```

### See Also {.unnumbered .unlisted}

[`fabs()`](#man-fabs),
[`abs()`](#man-abs)

[[manbreak]]
## `cpow()` `cpowf()` `cpowl()` {#man-cpow}

Compute complex power

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex cpow(double complex x, double complex y);

float complex cpowf(float complex x, float complex y);

long double complex cpowl(long double complex x,
                          long double complex y);
```

### Description {.unnumbered .unlisted}

Computes the complex $x^y$.

There is a branch cut for `x` along the negative real axis.

### Return Value {.unnumbered .unlisted}

Returns the complex $x^y$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2 * I;
    double complex y = 3 + 4 * I;

    double r = cpow(x, y);

    printf("Result: %f + %fi\n", creal(r), cimag(r));
}
```

Result:

``` {.default}
Result: 0.129010 + 0.000000i
```

### See Also {.unnumbered .unlisted}

[`csqrt()`](#man-csqrt),
[`cexp()`](#man-cexp)

[[manbreak]]
## `csqrt()` `csqrtf()` `csqrtl()` {#man-csqrt}

Compute the complex square root

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex csqrt(double complex z);

float complex csqrtf(float complex z);

long double complex csqrtl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex square root of `z`.

There is a branch cut along the negative real axis.

The return value is in the right half of the complex plane and includes
the imaginary axis.

### Return Value {.unnumbered .unlisted}

Returns the complex square root of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2 * I;

    double complex y = csqrt(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 1.272020 + 0.786151i
```

### See Also {.unnumbered .unlisted}

[`cpow()`](#man-cpow),
[`sqrt()`](#man-sqrt)

[[manbreak]]
## `carg()` `cargf()` `cargl()` {#man-carg}

Compute the complex argument

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double carg(double complex z);

float cargf(float complex z);

long double cargl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the complex argument (AKA phase angle) of `z`.

There is a branch cut along the negative real axis.

Returns a value in the range $[-\pi,+\pi]$.

### Return Value {.unnumbered .unlisted}

Returns the complex argument of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2 * I;

    double y = carg(x);

    printf("Result: %f\n", y);
}
```

Output:

``` {.default}
Result: 1.107149
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `cimag()` `cimagf()` `cimagl()` {#man-cimag}

Returns the imaginary part of a complex number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double cimag(double complex z);

float cimagf(float complex z);

long double cimagl(long double complex z);
```

### Description {.unnumbered .unlisted}

Returns the imaginary part of `z`.

As a footnote, the spec points out that any complex number `x` is part
of the following equivalency:

``` {.c}
x == creal(x) + cimag(x) * I;
```

### Return Value {.unnumbered .unlisted}

Returns the imaginary part of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2 * I;

    double y = cimag(x);

    printf("Result: %f\n", y);
}
```

Output---just the imaginary part:

``` {.default}
Result: 2.000000
```

### See Also {.unnumbered .unlisted}

[`creal()`](#man-creal)

[[manbreak]]
## `CMPLX()` `CMPLXF()` `CMPLXL()` {#man-CMPLX}

Build a complex value from real and imaginary types

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex CMPLX(double x, double y);

float complex CMPLXF(float x, float y);

long double complex CMPLXL(long double x, long double y);
```

### Description {.unnumbered .unlisted}

These macros build a complex value from real and imaginary types.

Now I know what you're thinking. "But I can already build a complex
value from real and imaginary types using the `I` macro, like in the
example you're about to give us."

``` {.c}
double complex x = 1 + 2 * I;
```

And that's true.

But the reality of the matter is weird and complex.

Maybe `I` got undefined, or maybe you redefined it.

Or maybe `I` was defined as `_Complex_I` which doesn't necessarily
preserve the sign of a zero value.

As the spec points out, these macros build complex numbers as if
`_Imaginary_I` were defined (thus preserving your zero sign) even if
it's not. That is, they are defined equivalently to:

``` {.c}
#define CMPLX(x, y)  ((double complex)((double)(x) + \
                     _Imaginary_I * (double)(y)))

#define CMPLXF(x, y) ((float complex)((float)(x) + \
                     _Imaginary_I * (float)(y)))

#define CMPLXL(x, y) ((long double complex)((long double)(x) + \
                     _Imaginary_I * (long double)(y)))
```

### Return Value {.unnumbered .unlisted}

Returns the complex number for the given real `x` and imaginary `y`
components.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = CMPLX(1, 2);  // Like 1 + 2 * I

    printf("Result: %f + %fi\n", creal(x), cimag(x));
}
```

Output:

``` {.default}
Result: 1.000000 + 2.000000i
```

### See Also {.unnumbered .unlisted}

[`creal()`](#man-creal),
[`cimag()`](#man-cimag)

[[manbreak]]
## `conj()` `conjf()` `conjl()` {#man-conj}

Compute the conjugate of a complex number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex conj(double complex z);

float complex conjf(float complex z);

long double complex conjl(long double complex z);
```

### Description {.unnumbered .unlisted}

This function computes the [flw[complex conjugate|Complex_conjugate]] of
`z`. Apparently it does this by reversing the sign of the imaginary
part, but dammit, I'm a programmer not a mathematician, Jim!

### Return Value {.unnumbered .unlisted}

Returns the complex conjugate of `z`

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2 * I;

    double complex y = conj(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 1.000000 + -2.000000i
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `cproj()` `cproj()` `cproj()` {#man-cproj}

Compute the projection of a complex number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double complex cproj(double complex z);

float complex cprojf(float complex z);

long double complex cprojl(long double complex z);
```

### Description {.unnumbered .unlisted}

Computes the projection of `z` onto a [flw[Riemann
sphere|Riemann_sphere]].

Now we're _really_ outside my expertise. The spec has this to say, which
I'm quoting verbatim because I'm not knowledgable enough to rewrite it
sensibly. Hopefully it makes sense to anyone who would need to use this
function.

> `z` projects to `z` except that all complex infinities (even those with
> one infinite part and one `NaN` part) project to positive infinity on
> the real axis. If `z` has an infinite part, then `cproj(z)` is equivalent
> to
>
>     INFINITY + I * copysign(0.0, cimag(z))

So there you have it.

### Return Value {.unnumbered .unlisted}

Returns the projection of `z` onto a Riemann sphere.

### Example {.unnumbered .unlisted}

Fingers crossed this is a remotely sane example...

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>
#include <math.h>

int main(void)
{
    double complex x = 1 + 2 * I;

    double complex y = cproj(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));

    x = INFINITY + 2 * I;
    y = cproj(x);

    printf("Result: %f + %fi\n", creal(y), cimag(y));
}
```

Output:

``` {.default}
Result: 1.000000 + 2.000000i
Result: inf + 0.000000i
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `creal()` `crealf()` `creall()` {#man-creal}

Returns the real part of a complex number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <complex.h>

double creal(double complex z);

float crealf(float complex z);

long double creall(long double complex z);
```

### Description {.unnumbered .unlisted}

Returns the real part of `z`.

As a footnote, the spec points out that any complex number `x` is part
of the following equivalency:

``` {.c}
x == creal(x) + cimag(x) * I;
```

### Return Value {.unnumbered .unlisted}

Returns the real part of `z`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2 * I;

    double y = creal(x);

    printf("Result: %f\n", y);
}
```

Output---just the real part:

``` {.default}
Result: 1.000000
```

### See Also {.unnumbered .unlisted}

[`cimag()`](#man-cimag)

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
