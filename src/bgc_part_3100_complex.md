<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Complex Numbers

A tiny primer on [flw[Complex numbers|Complex_number]] stolen directly
from Wikipedia:

> A **complex number** is a number that can be expressed in the form
> $a+bi$, where $a$ and $b$ are real numbers [i.e. floating point types
> in C], and $i$ represents the imaginary unit, satisfying the equation
> $i^2=−1$. Because no real number satisfies this equation, $i$ is
> called an imaginary number. For the complex number $a+bi$, $a$ is
> called the **real part**, and $b$ is called the **imaginary part**.

But that's as far as I'm going to go. We'll assume that if you're
reading this chapter, you know what a complex number is and what you
want to do with them.

And all we need to cover is C's faculties for doing so.

Turns out, though, that complex number support in a compiler is an
_optional_ feature. Not all compliant compilers can do it. And the ones
that do, might do it to various degrees of completeness.

You can test if your system supports complex numbers with:

``` {.c}
#ifdef __STDC_NO_COMPLEX__
#error Complex numbers not supported!
#endif
```

Furthermore, there is a macro that indicates adherence to the ISO 60559
(IEEE 754) standard for floating point math with complex numbers, as
well as the presence of the `_Imaginary` type.

``` {.c}
#if __STDC_IEC_559_COMPLEX__ != 1
#error Need IEC 60559 complex support!
#endif
```

More details on that are spelled out in Annex G in the C11 spec.

## Complex Types

To use complex numbers, `#include <complex.h>`.

With that, you get at least two types:

``` {.c}
_Complex
complex
```

Those both mean the same thing, so you might as well use the prettier
`complex`.

You also get some types for imaginary numbers if you implementation is
IEC 60559-compliant:

``` {.c}
_Imaginary
imaginary
```

These also both mean the same thing, so you might as well use the
prettier `imaginary`.

You also get values for the imaginary number $i$, itself:

``` {.c}
I
_Complex_I
_Imaginary_I
```

The macro `I` is set to `_Imaginary_I` (if available), or `_Complex_I`.
So just use `I` for the imaginary number.

One aside: I've said that if a compiler has `__STDC_IEC_559_COMPLEX__` set to
`1`, it must support `_Imaginary` types to be compliant. That's my read
of the spec. However, I don't know of a single compiler that actually
supports `_Imaginary` even though they have `__STDC_IEC_559_COMPLEX__`
set. So I'm going to write some code with that type in here I have no
way of testing. Sorry!

OK, so now we know there's a `complex` type, how can we use it?

## Assigning Complex Numbers

Since the complex number has a real and imaginary part, but both of them
rely on floating point numbers to store values, we need to also tell C
what precision to use for those parts of the complex number.

We do that by just pinning a `float`, `double`, or `long double` to the
`complex`, either before or after it.

Let's define a complex number that uses `float` for its components:

``` {.c}
float complex c;   // Spec prefers this way
complex float c;   // Same thing--order doesn't matter
```

So that's great for declarations, but how do we initialize them or
assign to them?

Turns out we get to use some pretty natural notation. Example!

``` {.c}
double complex x = 5 + 2*I;
double complex y = 10 + 3*I;
```

For $5+2i$ and $10+3i$, respectively.

## Constructing, Deconstructing, and Printing

We're getting there...

We've already seen one way to write a complex number:

``` {.c}
double complex x = 5 + 2*I;
```

There's also no problem using other floating point numbers to build it:

``` {.c}
double a = 5;
double b = 2;
double complex x = a + b*I;
```

There is also a set of macros to help build these. The above code could
be written using the `CMPLX()` macro, like so:

``` {.c}
double complex x = CMPLX(5, 2);
```

As far as I can tell in my research, these are _almost_ equivalent:

``` {.c}
double complex x = 5 + 2*I;
double complex x = CMPLX(5, 2);
```

But the `CMPLX()` macro will handle negative zeros in the imaginary part
correctly every time, whereas the other way might convert them to
positive zeros. I _think_^[This was a harder one to research, and I'll
take any more information anyone can give me. `I` could be defined as
`_Complex_I` or `_Imaginary_I`, if the latter exists. `_Imaginary_I`
will handle signed zeros, but `_Complex_I` _might_ not. This has
implications with branch cuts and other complex-numbery-mathy things.
Maybe. Can you tell I'm really getting out of my element here? In any
case, the `CMPLX()` macros behave as if `I` were defined as
`_Imaginary_I`, with signed zeros, even if `_Imaginary_I` doesn't exist
on the system.] This seems to imply that if there's a chance the
imaginary part will be zero, you should use the macro... but someone
should correct me on this if I'm mistaken!

The `CMPLX()` macro works on `double` types. There are two other macros
for `float` and `long double`: `CMPLXF()` and `CMPLXL()`. (These "f" and
"l" suffixes appear in virtually all the complex-number-related
functions.)

Now let's try the reverse: if we have a complex number, how do we break
it apart into its real and imaginary parts?

Here we have a couple functions that will extract the real and imaginary
parts from the number: `creal()` and `cimag()`:

``` {.c}
double complex x = 5 + 2*I;
double complex y = 10 + 3*I;

printf("x = %f + %fi\n", creal(x), cimag(x));
printf("y = %f + %fi\n", creal(y), cimag(y));
```

for the output:

``` {.default}
x = 5.000000 + 2.000000i
y = 10.000000 + 3.000000i
```

Note that the `i` I have in the `printf()` format string is a literal
`i` that gets printed---it's not part of the format specifier. Both
return values from `creal()` and `cimag()` are `double`.

And as usual, there are `float` and `long double` variants of these
functions: `crealf()`, `cimagf()`, `creall()`, and `cimagl()`.

## Complex Arithmetic and Comparisons

Arithmetic can be performed on complex numbers, though how this works
mathematically is beyond the scope of the guide.

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2*I;
    double complex y = 3 + 4*I;
    double complex z;

    z = x + y;
    printf("x + y = %f + %fi\n", creal(z), cimag(z));

    z = x - y;
    printf("x - y = %f + %fi\n", creal(z), cimag(z));

    z = x * y;
    printf("x * y = %f + %fi\n", creal(z), cimag(z));

    z = x / y;
    printf("x / y = %f + %fi\n", creal(z), cimag(z));
}
```

for a result of:

``` {.default}
x + y = 4.000000 + 6.000000i
x - y = -2.000000 + -2.000000i
x * y = -5.000000 + 10.000000i
x / y = 0.440000 + 0.080000i
```

You can also compare two complex numbers for equality (or inequality):

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int main(void)
{
    double complex x = 1 + 2*I;
    double complex y = 3 + 4*I;

    printf("x == y = %d\n", x == y);  // 0
    printf("x != y = %d\n", x != y);  // 1
}
```

with the output:

``` {.default}
x == y = 0
x != y = 1
```

They are equal if both components test equal. Note that as with all
floating point, they could be equal if they're close enough due to
rounding error^[The simplicity of this statement doesn't do justice to
the incredible amount of work that goes into simply understanding how
floating point actually functions.
https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/].

## Complex Math

But wait! There's more than just simple complex arithmetic!

Here's a summary table of all the math functions available to you with
complex numbers.

I'm only going to list the `double` version of each function, but for
all of them there is a `float` version that you can get by appending `f`
to the function name, and a `long double` version that you can get by
appending `l`.

For example, the `cabs()` function for computing the absolute value of a
complex number also has `cabsf()` and `cabsl()` variants. I'm omitting
them for brevity.

### Trigonometry Functions

|Function|Description|
|-|-|
|`ccos()`|Cosine|
|`csin()`|Sine|
|`ctan()`|Tangent|
|`cacos()`|Arc cosine|
|`casin()`|Arc sine|
|`catan()`|Play _Settlers of Catan_|
|`ccosh()`|Hyperbolic cosine|
|`csinh()`|Hyperbolic sine|
|`ctanh()`|Hyperbolic tangent|
|`cacosh()`|Arc hyperbolic cosine|
|`casinh()`|Arc hyperbolic sine|
|`catanh()`|Arc hyperbolic tangent|

### Exponential and Logarithmic Functions

|Function|Description|
|-|-|
|`cexp()`|Base-$e$ exponential|
|`clog()`|Natural (base-$e$) logarithm|

### Power and Absolute Value Functions

|Function|Description|
|-|-|
|`cabs()`|Absolute value|
|`cpow()`|Power|
|`csqrt()`|Square root|

### Manipulation Functions

|Function|Description|
|-|-|
|`creal()`|Return real part|
|`cimag()`|Return imaginary part|
|`CMPLX()`|Construct a complex number|
|`carg()`|Argument/phase angle|
|`conj()`|Conjugate[^4a34]|
|`cproj()`|Projection on Riemann sphere|

[^4a34]: This is the only one that doesn't begin with an extra leading
`c`, strangely.