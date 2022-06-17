<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<tgmath.h>` Type-Generic Math Functions {#tgmath}

[i[`tgmath.h` header file]i]

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
|`acos`|`cacos`|[i[`acos()` function]i]`acos`|
|`asin`|`casin`|[i[`asin()` function]i]`asin`|
|`atan`|`catan`|[i[`atan()` function]i]`atan`|
|`acosh`|`cacosh`|[i[`acosh()` function]i]`acosh`|
|`asinh`|`casinh`|[i[`asinh()` function]i]`asinh`|
|`atanh`|`catanh`|[i[`atanh()` function]i]`atanh`|
|`cos`|`ccos`|[i[`cos()` function]i]`cos`|
|`sin`|`csin`|[i[`sin()` function]i]`sin`|
|`tan`|`ctan`|[i[`tan()` function]i]`tan`|
|`cosh`|`ccosh`|[i[`cosh()` function]i]`cosh`|
|`sinh`|`csinh`|[i[`sinh()` function]i]`sinh`|
|`tanh`|`ctanh`|[i[`tanh()` function]i]`tanh`|
|`exp`|`cexp`|[i[`exp()` function]i]`exp`|
|`log`|`clog`|[i[`log()` function]i]`log`|
|`pow`|`cpow`|[i[`pow()` function]i]`pow`|
|`sqrt`|`csqrt`|[i[`sqrt()` function]i]`sqrt`|
|`fabs`|`cabs`|[i[`fabs()` function]i]`fabs`|
|`atan2`|---|[i[`atan2()` function]i]`atan2`|
|`fdim`|---|[i[`fdim()` function]i]`fdim`|
|`cbrt`|---|[i[`cbrt()` function]i]`cbrt`|
|`floor`|---|[i[`floor()` function]i]`floor`|
|`ceil`|---|[i[`ceil()` function]i]`ceil`|
|`fma`|---|[i[`fma()` function]i]`fma`|
|`copysign`|---|[i[`copysign()` function]i]`copysign`|
|`fmax`|---|[i[`fmax()` function]i]`fmax`|
|`erf`|---|[i[`erf()` function]i]`erf`|
|`fmin`|---|[i[`fmin()` function]i]`fmin`|
|`erfc`|---|[i[`erfc()` function]i]`erfc`|
|`fmod`|---|[i[`fmod()` function]i]`fmod`|
|`exp2`|---|[i[`exp2()` function]i]`exp2`|
|`frexp`|---|[i[`frexp()` function]i]`frexp`|
|`expm1`|---|[i[`expm1()` function]i]`expm1`|
|`hypot`|---|[i[`hypot()` function]i]`hypot`|
|`ilogb`|---|[i[`ilogb()` function]i]`ilogb`|
|`ldexp`|---|[i[`ldexp()` function]i]`ldexp`|
|`lgamma`|---|[i[`lgamma()` function]i]`lgamma`|
|`llrint`|---|[i[`llrint()` function]i]`llrint`|
|`llround`|---|[i[`llround()` function]i]`llround`|
|`log10`|---|[i[`log10()` function]i]`log10`|
|`log1p`|---|[i[`log1p()` function]i]`log1p`|
|`log2`|---|[i[`log2()` function]i]`log2`|
|`logb`|---|[i[`logb()` function]i]`logb`|
|`lrint`|---|[i[`lrint()` function]i]`lrint`|
|`lround`|---|[i[`lround()` function]i]`lround`|
|`nearbyint`|---|[i[`nearbyint()` function]i]`nearbyint`|
|`nextafter`|---|[i[`nextafter()` function]i]`nextafter`|
|`nexttoward`|---|[i[`nexttoward()` function]i]`nexttoward`|
|`remainder`|---|[i[`remainder()` function]i]`remainder`|
|`remquo`|---|[i[`remquo()` function]i]`remquo`|
|`rint`|---|[i[`rint()` function]i]`rint`|
|`round`|---|[i[`round()` function]i]`round`|
|`scalbn`|---|[i[`scalbn()` function]i]`scalbn`|
|`scalbln`|---|[i[`scalbln()` function]i]`scalbln`|
|`tgamma`|---|[i[`tgamma()` function]i]`tgamma`|
|`trunc`|---|[i[`trunc()` function]i]`trunc`|
|---|`carg`|[i[`carg()` function]i]`carg`|
|---|`cimag`|[i[`cimag()` function]i]`cimag`|
|---|`conj`|[i[`conj()` function]i]`conj`|
|---|`cproj`|[i[`cproj()` function]i]`cproj`|
|---|`creal`|[i[`creal()` function]i]`creal`|

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
