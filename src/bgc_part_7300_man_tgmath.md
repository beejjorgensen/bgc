<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<tgmath.h>` Type-Generic Math Functions {#tgmath}

These are type-generic macros that are wrappers around the math
functions in [`<math.h>`](#math) and [`<complex.h>`](#complex). This
header includes both of those.

Deep down, these are implemented using type generics as described in
the [Types Part V chapter](#type-generics).

But on the surface, you can think of them as being able to use, say, the
`sqrt()` function with any type without needed to think about if it's
`double` or `long double` or even `complex`.

These are the defined macros---some of them don't have a counterpart in
the real or complex space. Type suffixes are omitted in the table on the
Real and Complex columns. None of the generic macros have type suffixes.

|Real Function|Complex Function|Generic Macro|
|-|-|-|
|`acos`|`cacos`|`acos`|
|`asin`|`casin`|`asin`|
|`atan`|`catan`|`atan`|
|`acosh`|`cacosh`|`acosh`|
|`asinh`|`casinh`|`asinh`|
|`atanh`|`catanh`|`atanh`|
|`cos`|`ccos`|`cos`|
|`sin`|`csin`|`sin`|
|`tan`|`ctan`|`tan`|
|`cosh`|`ccosh`|`cosh`|
|`sinh`|`csinh`|`sinh`|
|`tanh`|`ctanh`|`tanh`|
|`exp`|`cexp`|`exp`|
|`log`|`clog`|`log`|
|`pow`|`cpow`|`pow`|
|`sqrt`|`csqrt`|`sqrt`|
|`fabs`|`cabs`|`fabs`|
|`atan2`|---|`atan2`|
|`fdim`|---|`fdim`|
|`cbrt`|---|`cbrt`|
|`floor`|---|`floor`|
|`ceil`|---|`ceil`|
|`fma`|---|`fma`|
|`copysign`|---|`copysign`|
|`fmax`|---|`fmax`|
|`erf`|---|`erf`|
|`fmin`|---|`fmin`|
|`erfc`|---|`erfc`|
|`fmod`|---|`fmod`|
|`exp2`|---|`exp2`|
|`frexp`|---|`frexp`|
|`expm1`|---|`expm1`|
|`hypot`|---|`hypot`|
|`ilogb`|---|`ilogb`|
|`ldexp`|---|`ldexp`|
|`lgamma`|---|`lgamma`|
|`llrint`|---|`llrint`|
|`llround`|---|`llround`|
|`log10`|---|`log10`|
|`log1p`|---|`log1p`|
|`log2`|---|`log2`|
|`logb`|---|`logb`|
|`lrint`|---|`lrint`|
|`lround`|---|`lround`|
|`nearbyint`|---|`nearbyint`|
|`nextafter`|---|`nextafter`|
|`nexttoward`|---|`nexttoward`|
|`remainder`|---|`remainder`|
|`remquo`|---|`remquo`|
|`rint`|---|`rint`|
|`round`|---|`round`|
|`scalbn`|---|`scalbn`|
|`scalbln`|---|`scalbln`|
|`tgamma`|---|`tgamma`|
|`trunc`|---|`trunc`|
|---|`carg`|`carg`|
|---|`cimag`|`cimag`|
|---|`conj`|`conj`|
|---|`cproj`|`cproj`|
|---|`creal`|`creal`|

## Example

Here's an example where we call the type-generic `sqrt()` function on a
variety of types.

``` {.c .numberLines}
#include <stdio.h>
#include <tgmath.h>

int main(void)
{
    double x = 12.8;
    long double y = 34.9;
    double complex z = 1 + 2 * I;

    double x_result;
    long double y_result;
    double complex z_result;

    // We call the same sqrt() function--it's type-generic!
    x_result = sqrt(x);
    y_result = sqrt(y);
    z_result = sqrt(z);

    printf("x_result: %f\n", x_result);
    printf("y_result: %Lf\n", y_result);
    printf("z_result: %f + %fi\n", creal(z_result), cimag(z_result));
}
```

Output:

``` {.default}
x_result: 3.577709
y_result: 5.907622
z_result: 1.272020 + 0.786151i
```