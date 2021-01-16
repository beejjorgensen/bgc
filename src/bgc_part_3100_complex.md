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

```
#if __STDC_IEC_559_COMPLEX__ != 1
#error Need IEC 60559 complex support!
#endif
```

More details on that are spelled out in Annex G in the C11 spec.

## Complex Basics

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



## TODO

Equal if both components are equal