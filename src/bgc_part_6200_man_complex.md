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
