<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<complex.h>` Complex Number Functionality {#complex}

Math functions for complex numbers.

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
$[0,π]$, and the real component is unbounded.

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

```
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
$[-π/2,+π/2]$, and the real component is unbounded.

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

```
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
$[-π/2,+π/2]$, and the imaginary component is unbounded.

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

```
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

```
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

```
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

```
Result: -0.027073 + 1.085990i
```

### See Also {.unnumbered .unlisted}

[`ccos()`](#man-ccos),
[`csin()`](#man-csin),
[`catan()`](#man-catan)

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
