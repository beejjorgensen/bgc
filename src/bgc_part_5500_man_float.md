<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<float.h>` Floating Point Limits {#float}

|Macro|Minimum Magnitude|Description|
|--|:--:|--------|
|`FLT_ROUNDS`||Current rounding mode|
|`FLT_EVAL_METHOD`||Types used for evaluation|
|`FLT_HAS_SUBNORM`||Subnormal support for `float`|
|`DBL_HAS_SUBNORM`||Subnormal support for `double`|
|`LDBL_HAS_SUBNORM`||Subnormal support for `long double`|
|`FLT_RADIX`|`2`|Floating point radix (base)|
|`FLT_MANT_DIG`||Number of base `FLT_RADIX` digits in a `float`|
|`DBL_MANT_DIG`||Number of base `FLT_RADIX` digits in a `double`|
|`LDBL_MANT_DIG`||Number of base `FLT_RADIX` digits in a `long double`|
|`FLT_DECIMAL_DIG`|`6`|Number of decimal digits required to encode a `float`|
|`DBL_DECIMAL_DIG`|`10`|Number of decimal digits required to encode a `double`|
|`LDBL_DECIMAL_DIG`|`10`|Number of decimal digits required to encode a `long double`|
|`DECIMAL_DIG`|`10`|Number of decimal digits required to encode the the widest floating point number supported|
|`FLT_DIG`|`6`|Number of decimal digits that can be safely stored in a `float`|
|`DBL_DIG`|`10`|Number of decimal digits that can be safely stored in a `double`|
|`LDBL_DIG`|`10`|Number of decimal digits that can be safely stored in a `long double`|
|`FLT_MIN_EXP`||`FLT_RADIX` to the `FLT_MIN_EXP-1` power is the smallest normalized `float`|
|`DBL_MIN_EXP`||`FLT_RADIX` to the `DBL_MIN_EXP-1` power is the smallest normalized `double`|
|`LDBL_MIN_EXP`||`FLT_RADIX` to the `LDBL_MIN_EXP-1` power is the smallest normalized `long double`|
|`FLT_MIN_10_EXP`|`-37`|Minimum exponent such that `10` to this number is a normalized `float`|
|`DBL_MIN_10_EXP`|`-37`|Minimum exponent such that `10` to this number is a normalized `double`
|`LDBL_MIN_10_EXP`|`-37`|Minimum exponent such that `10` to this number is a normalized `long_double`
|`FLT_MAX_EXP`||`FLT_RADIX` to the `FLT_MAX_EXP-1` power is the largest finite `float`|
|`DBL_MAX_EXP`||`FLT_RADIX` to the `DBL_MAX_EXP-1` power is the largest finite `double`|
|`LDBL_MAX_EXP`||`FLT_RADIX` to the `LDBL_MAX_EXP-1` power is the largest finite `long double`|
|`FLT_MAX_10_EXP`|`-37`|Minimum exponent such that `10` to this number is a finite `float`|
|`DBL_MAX_10_EXP`|`-37`|Minimum exponent such that `10` to this number is a finite `double`
|`LDBL_MAX_10_EXP`|`-37`|Minimum exponent such that `10` to this number is a finite `long_double`
|`FLT_MAX`|`1E+37`|Largest finite `float`|
|`DBL_MAX`|`1E+37`|Largest finite `double`|
|`LDBL_MAX`|`1E+37`|Largest finite `long double`|

|Macro|Maximum Value|Description|
|--|:--:|--------|
|`FLT_EPSILON`|`1E-5`|Difference between 1 and the next biggest representable `float`|
|`DBL_EPSILON`|`1E-9`|Difference between 1 and the next biggest representable `double`|
|`LDBL_EPSILON`|`1E-9`|Difference between 1 and the next biggest representable `long double`|
|`FLT_MIN`|`1E-37`|Minimum positive normalized `float`|
|`DBL_MIN`|`1E-37`|Minimum positive normalized `double`|
|`LDBL_MIN`|`1E-37`|Minimum positive normalized `long double`|
|`FLT_TRUE_MIN`|`1E-37`|Minimum positive `float`|
|`DBL_TRUE_MIN`|`1E-37`|Minimum positive `double`|
|`LDBL_TRUE_MIN`|`1E-37`|Minimum positive `long double`|

The minimum and maximum values here are from the spec---they should what
you can at least expect across all platforms. Your super dooper machine
might do better, still!

## Background

The spec allows a lot of leeway when it comes to how C represents
floating point numbers. This header file spells out the limits on those
numbers.

It gives a model that can describe any floating point number that I
_know_ you're going to absolutely love. It looks like this:

$\displaystyle x=sb^e\sum_{k=1}^p f_k b^{-k}, e_{min} \le e \le e_{max}$

where:

|Variable|Meaning|
|:-:|-|
|$s$|Sign, $-1$ or $1$|
|$b$|Base (radix), probably $2$ on your system|
|$e$|Exponent|
|$p$|Precision: how many base-$b$ digits in the number|
|$f_k$|The individual digits of the number, the significand|

But let's blissfully ignore all that for a second.

Let's assume your computer uses base 2 for it's floating point (it
probably does). And that in the example below the 1s-and-0s numbers are
in binary, and the rest are in decimal.

The short of it is you could have floating point numbers like shown in
this example:

$-0.10100101 \times 2^5 = -10100.101 = -20.625$

That's your fractional part multiplied by the base to the exponent's
power. The exponent controls where the decimal point is. It "floats"
around!

<!--
Or, more specifically, that example uses:

$s=-1$ \
$b=2$ \
$e=5$ \
$p=8$ \
$f_n=10100101$ 
-->

## `FLT_ROUNDS` Details

This tells you the rounding mode. It can be changed with a call to
[`fesetround()`](#man-fegetround).

|Mode|Description|
|:-:|-|
|`-1`|Indeterminable|
|`0`|Toward zero|
|`1`|To nearest|
|`2`|Toward positive infinity|
|`3`|Toward negative infinity... and beyond!|

Unlike every other macro in this here header, `FLT_ROUNDS` might not be
a constant expression.

## `FLT_EVAL_METHOD` Details

This basically tells you how floating point values are promoted to
different types in expressions.

|Method|Description|
|:--:|-----------------------|
|`-1`|Indeterminable|
|`0`|Evaluate all operations and constants to the precision of their respective types|
|`1`|Evaluate `float` and `double` operations as `double` and `long double` ops as `long double`|
|`2`|Evaluate all operations and constants as `long double`|

## Subnormal Numbers

The macros `FLT_HAS_SUBNORM`, `DBL_HAS_SUBNORM`, and `LDBL_HAS_SUBNORM`
all let you know if those types support [flw[subnormal
numbers|Subnormal_number]].

|Value|Description|
|:-:|-|
|`-1`|Indeterminable|
|`0`|Subnormals not supported for this type|
|`1`|Subnormals supported for this type|

## How Many Decimal Places Can I Use?

It depends on what you want to do.

The safe thing is if you never use more than `FLT_DIG` base-10 digits in
your `float`, you're good. (Same for `DBL_DIG` and `LDBL_DIG` for their
types.)

And by "use" I mean print out, have in code, read from the keyboard,
etc.

You can print out that many decimal places with `printf()` and the `%g`
format specifier:

``` {.c .numberLines}
#include <stdio.h>
#include <float.h>

int main(void)
{
    float pi = 3.1415926535897932384626433832795028841971;

    // With %g or %G, the precision refers to the number of significant
    // digits:

    printf("%.*g\n", FLT_DIG, pi);  // For me: 3.14159

    // But %f prints too many, since the precision is the number of
    // digits to the right of the decimal--it doesn't count the digits
    // to the left of it:

    printf("%.*f\n", FLT_DIG, pi);  // For me: 3.14159... 3 ???
}
```

That's the end, but stay tuned for the exciting conclusion of "How Many
Decimal Places Can I Use?"

Because base 10 and base 2 (your typical `FLT_RADIX`) don't mix very
well, you can actually have more than `FLT_DIG` in your
`float`; the bits of storage go out a little farther. But these might
round in a way you don't expect.

But if you want to convert a floating point number to base 10 and then
be able to convert it back again to the exact same floating point
number, you'll need `FLT_DECIMAL_DIG` digits from your `float` to make
sure you get those extra bits of storage represented.

Here's some example output that shows how the value stored might have
some extra decimal places at the end.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

int main(void)
{
    printf("FLT_DIG = %d\n", FLT_DIG);
    printf("FLT_DECIMAL_DIG = %d\n\n", FLT_DECIMAL_DIG);

    assert(FLT_DIG == 6);  // Code below assumes this

    for (float x = 0.123456; x < 0.12346; x += 0.000001) {
        printf("As written: %.*g\n", FLT_DIG, x);
        printf("As stored:  %.*g\n\n", FLT_DECIMAL_DIG, x);
    }
}
```

And the output on my machine, starting at `0.123456` and incrementing by
`0.000001` each time:

``` {.default}
FLT_DIG = 6
FLT_DECIMAL_DIG = 9

As written: 0.123456
As stored:  0.123456001

As written: 0.123457
As stored:  0.123457

As written: 0.123458
As stored:  0.123457998

As written: 0.123459
As stored:  0.123458996

As written: 0.12346
As stored:  0.123459995
```

You can see that the value stored isn't always the value we're expecting
since base-2 can't represent all base-10 fractions exactly. The best it
can do is store more places and then round.

Also notice that even though we tried to stop the `for` loop _before_
`0.123460`, it actually ran including that value since the stored
version of that number was `0.123459995`, which is still less than
`0.123460`.

Aren't floating point numbers fun?

## Comprehensive Example

Here's a program that prints out the details for a particular machine:

``` {.c .numberLines}
#include <stdio.h>
#include <float.h>

int main(void)
{
    printf("FLT_RADIX: %d\n", FLT_RADIX);
    printf("FLT_ROUNDS: %d\n", FLT_ROUNDS);
    printf("FLT_EVAL_METHOD: %d\n", FLT_EVAL_METHOD);
    printf("DECIMAL_DIG: %d\n\n", DECIMAL_DIG);

    printf("FLT_HAS_SUBNORM: %d\n", FLT_HAS_SUBNORM);
    printf("FLT_MANT_DIG: %d\n", FLT_MANT_DIG);
    printf("FLT_DECIMAL_DIG: %d\n", FLT_DECIMAL_DIG);
    printf("FLT_DIG: %d\n", FLT_DIG);
    printf("FLT_MIN_EXP: %d\n", FLT_MIN_EXP);
    printf("FLT_MIN_10_EXP: %d\n", FLT_MIN_10_EXP);
    printf("FLT_MAX_EXP: %d\n", FLT_MAX_EXP);
    printf("FLT_MAX_10_EXP: %d\n", FLT_MAX_10_EXP);
    printf("FLT_MIN: %.*e\n", FLT_DECIMAL_DIG, FLT_MIN);
    printf("FLT_MAX: %.*e\n", FLT_DECIMAL_DIG, FLT_MAX);
    printf("FLT_EPSILON: %.*e\n", FLT_DECIMAL_DIG, FLT_EPSILON);
    printf("FLT_TRUE_MIN: %.*e\n\n", FLT_DECIMAL_DIG, FLT_TRUE_MIN);

    printf("DBL_HAS_SUBNORM: %d\n", DBL_HAS_SUBNORM);
    printf("DBL_MANT_DIG: %d\n", DBL_MANT_DIG);
    printf("DBL_DECIMAL_DIG: %d\n", DBL_DECIMAL_DIG);
    printf("DBL_DIG: %d\n", DBL_DIG);
    printf("DBL_MIN_EXP: %d\n", DBL_MIN_EXP);
    printf("DBL_MIN_10_EXP: %d\n", DBL_MIN_10_EXP);
    printf("DBL_MAX_EXP: %d\n", DBL_MAX_EXP);
    printf("DBL_MAX_10_EXP: %d\n", DBL_MAX_10_EXP);
    printf("DBL_MIN: %.*e\n", DBL_DECIMAL_DIG, DBL_MIN);
    printf("DBL_MAX: %.*e\n", DBL_DECIMAL_DIG, DBL_MAX);
    printf("DBL_EPSILON: %.*e\n", DBL_DECIMAL_DIG, DBL_EPSILON);
    printf("DBL_TRUE_MIN: %.*e\n\n", DBL_DECIMAL_DIG, DBL_TRUE_MIN);

    printf("LDBL_HAS_SUBNORM: %d\n", LDBL_HAS_SUBNORM);
    printf("LDBL_MANT_DIG: %d\n", LDBL_MANT_DIG);
    printf("LDBL_DECIMAL_DIG: %d\n", LDBL_DECIMAL_DIG);
    printf("LDBL_DIG: %d\n", LDBL_DIG);
    printf("LDBL_MIN_EXP: %d\n", LDBL_MIN_EXP);
    printf("LDBL_MIN_10_EXP: %d\n", LDBL_MIN_10_EXP);
    printf("LDBL_MAX_EXP: %d\n", LDBL_MAX_EXP);
    printf("LDBL_MAX_10_EXP: %d\n", LDBL_MAX_10_EXP);
    printf("LDBL_MIN: %.*Le\n", LDBL_DECIMAL_DIG, LDBL_MIN);
    printf("LDBL_MAX: %.*Le\n", LDBL_DECIMAL_DIG, LDBL_MAX);
    printf("LDBL_EPSILON: %.*Le\n", LDBL_DECIMAL_DIG, LDBL_EPSILON);
    printf("LDBL_TRUE_MIN: %.*Le\n\n", LDBL_DECIMAL_DIG, LDBL_TRUE_MIN);
    
    printf("sizeof(float): %zu\n", sizeof(float));
    printf("sizeof(double): %zu\n", sizeof(double));
    printf("sizeof(long double): %zu\n", sizeof(long double));
}
```

And here's the output on my machine:

``` {.default}
FLT_RADIX: 2
FLT_ROUNDS: 1
FLT_EVAL_METHOD: 0
DECIMAL_DIG: 21

FLT_HAS_SUBNORM: 1
FLT_MANT_DIG: 24
FLT_DECIMAL_DIG: 9
FLT_DIG: 6
FLT_MIN_EXP: -125
FLT_MIN_10_EXP: -37
FLT_MAX_EXP: 128
FLT_MAX_10_EXP: 38
FLT_MIN: 1.175494351e-38
FLT_MAX: 3.402823466e+38
FLT_EPSILON: 1.192092896e-07
FLT_TRUE_MIN: 1.401298464e-45

DBL_HAS_SUBNORM: 1
DBL_MANT_DIG: 53
DBL_DECIMAL_DIG: 17
DBL_DIG: 15
DBL_MIN_EXP: -1021
DBL_MIN_10_EXP: -307
DBL_MAX_EXP: 1024
DBL_MAX_10_EXP: 308
DBL_MIN: 2.22507385850720138e-308
DBL_MAX: 1.79769313486231571e+308
DBL_EPSILON: 2.22044604925031308e-16
DBL_TRUE_MIN: 4.94065645841246544e-324

LDBL_HAS_SUBNORM: 1
LDBL_MANT_DIG: 64
LDBL_DECIMAL_DIG: 21
LDBL_DIG: 18
LDBL_MIN_EXP: -16381
LDBL_MIN_10_EXP: -4931
LDBL_MAX_EXP: 16384
LDBL_MAX_10_EXP: 4932
LDBL_MIN: 3.362103143112093506263e-4932
LDBL_MAX: 1.189731495357231765021e+4932
LDBL_EPSILON: 1.084202172485504434007e-19
LDBL_TRUE_MIN: 3.645199531882474602528e-4951

sizeof(float): 4
sizeof(double): 8
sizeof(long double): 16
```
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