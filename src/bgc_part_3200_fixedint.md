<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Fixed Width Integer Types

C has all those small, bigger, and biggest integer types like `int` and
`long` and all that. And you can look in [the section on
limits](#limits-macros) to see what the largest int is with `INT_MAX`
and so on.

How big are those types? That is, how many bytes do they take up? We
could use `sizeof` to get that answer.

But what if I wanted to go the other way? What if I needed a type that
was exactly 32 bits (4 bytes) or at least 16 bits or somesuch?

How can we declare a type that's a certain size?

The header `<stdint.h>` gives us a way.

## The Bit-Sized Types

For both signed and unsigned integers, we can specify a type that is a
certain number of bits, with some caveats, of course.

And there are three main classes of these types (in these examples, the
`N` would be replaced by a certain number of bits):

* Integers of exactly a certain size (`intN_t`)
* Integers that are at least a certain size (`int_leastN_t`)
* Integers that are at least a certain size and are as fast as possible
  (`int_fastN_t`)[^4582]

[^4582]: Some architectures have different sized data that the CPU and
RAM can operate with at a faster rate than others. In those cases, if
you need the fastest 8-bit number, it might give you have a 16- or
32-bit type instead because that's just faster. So with this, you won't
know how big the type is, but it will be least as big as you say.

How much faster is `fast`? Definitely maybe some amount faster.
Probably. The spec doesn't say how much faster, just that they'll be the
fastest on this architecture. Most C compilers are pretty good, though,
so you'll probably only see this used in places where the most possible
speed needs to be guaranteed (rather than just hoping the compiler is
producing pretty-dang-fast code, which it is).

Finally, these unsigned number types have a leading `u` to differentiate
them.

For example, these types have the corresponding listed meaning:

``` {.c}
int32_t w;        // x is exactly 32 bits, signed
uint16_t x;       // y is exactly 16 bits, unsigned

int_least8_t y;   // y is at least 8 bits, signed

uint_fast64_t z;  // z is the fastest representation at least 64 bits, unsigned
```

The following types are guaranteed to be defined:

``` {.c}
int_least8_t      uint_least8_t
int_least16_t     uint_least16_t
int_least32_t     uint_least32_t
int_least64_t     uint_least64_t

int_fast8_t       uint_fast8_t
int_fast16_t      uint_fast16_t
int_fast32_t      uint_fast32_t
int_fast64_t      uint_fast64_t
```

There might be others of different widths, as well, but those are
optional.

Hey! Where are the fixed types like `int16_t`? Turns out those are
entirely optional...unless certain conditions are met^[Namely, the
system has 8, 16, 32, or 64 bit integers with no padding that use two's
complement representation, in which case the `intN_t` variant for that
particular number of bits _must_ be defined.]. And if you have an
average run-of-the-mill modern computer system, those conditions
probably are met. And if they are, you'll have these types:

``` {.c}
int8_t      uint8_t
int16_t     uint16_t
int32_t     uint32_t
int64_t     uint64_t
```

Other variants with different widths might be defined, but they're
optional.

## Maximum Integer Size Type

There's a type you can use that holds the largest representable integers
available on the system, both signed and unsigned:

``` {.c}
intmax_t
uintmax_t
```

Use these types when you want to go as big as possible.

Obviously values from any other integer types of the same sign will fit
in this type, necessarily.

## Using Fixed Size Constants

If you have a constant that you want to have fit in a certain number of
bits, you can use these macros to automatically append the proper suffix
onto the number (e.g. `22L` or `3490ULL`).

``` {.c}
INT8_C(x)     UINT8_C(x)
INT16_C(x)    UINT16_C(x)
INT32_C(x)    UINT32_C(x)
INT64_C(x)    UINT64_C(x)
INTMAX_C(x)   UINTMAX_C(x)
```

Again, these work only with constant integer values.

For example, we can use one of these to assign constant values like so:

``` {.c}
uint16_t x = UINT16_C(12);
intmax_t y = INTMAX_C(3490);
```

## Limits of Fixed Size Integers

We also have some limits defined so you can get the maximum and minimum
values for these types:

``` {.c}
INT8_MAX           INT8_MIN           UINT8_MAX
INT16_MAX          INT16_MIN          UINT16_MAX
INT32_MAX          INT32_MIN          UINT32_MAX
INT64_MAX          INT64_MIN          UINT64_MAX

INT_LEAST8_MAX     INT_LEAST8_MIN     UINT_LEAST8_MAX
INT_LEAST16_MAX    INT_LEAST16_MIN    UINT_LEAST16_MAX
INT_LEAST32_MAX    INT_LEAST32_MIN    UINT_LEAST32_MAX
INT_LEAST64_MAX    INT_LEAST64_MIN    UINT_LEAST64_MAX

INT_FAST8_MAX      INT_FAST8_MIN      UINT_FAST8_MAX
INT_FAST16_MAX     INT_FAST16_MIN     UINT_FAST16_MAX
INT_FAST32_MAX     INT_FAST32_MIN     UINT_FAST32_MAX
INT_FAST64_MAX     INT_FAST64_MIN     UINT_FAST64_MAX

INTMAX_MAX         INTMAX_MIN         UINTMAX_MAX
```

Note the MIN for all the unsigned types is `0`, so, as such, there's no
macro for it.

## Format Specifiers

In order to print these types, you need to send the right format
specifier to `printf()`. (And the same issue for getting input with
`scanf()`.)

But how are you going to know what size the types are under the hood?
Luckily, once again, C provides some macros to help with this.

All this can be found in `<inttypes.h>`.

Now, we have a bunch of macros. Like a complexity explosion of macros.
So I'm going to stop listing out every one and just put the lowercase
letter `n` in the place where you should put `8`, `16`, `32`, or `64`
depending on your needs.

Let's look at the macros for printing signed integers:

``` {.c}
PRIdn    PRIdLEASTn    PRIdFASTn    PRIdMAX
PRIin    PRIiLEASTn    PRIiFASTn    PRIiMAX
```

Look for the patterns there. You can see there are variants for the
fixed, least, fast, and max types.

And you also have a lowercase `d` and a lowercase `i`. Those correspond
to the `printf()` format specifiers `%d` and `%i`.

So if I have something of type:

``` {.c}
int_least16_t x = 3490;
```

I can print that with the equivalent format specifier for `%d` by
using `PRId16`.

But how? How do we use that macro?

First of all, that macro specifies a string containing the letter or
letters `printf()` needs to use to print that type. Like, for example, it could
be `"d"` or `"ld"`.

So all we need to do is embed that in our format string to the
`printf()` call.

To do this, we can take advantage of a fact about C that you might have
forgotten: adjacent string literals are automatically concatenated to a
single string. E.g.:

``` {.c}
printf("Hello, " "world!\n");   // Prints "Hello, world!"
```

And since these macros are string literals, we can use them like so:

``` {.c .numberLines}
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(void)
{
    int_least16_t x = 3490;

    printf("The value is %" PRIdLEAST16 "!\n", x);
}
```

We also have a pile of macros for printing unsigned types:

``` {.c}
PRIon    PRIoLEASTn    PRIoFASTn    PRIoMAX
PRIun    PRIuLEASTn    PRIuFASTn    PRIuMAX
PRIxn    PRIxLEASTn    PRIxFASTn    PRIxMAX
PRIXn    PRIXLEASTn    PRIXFASTn    PRIXMAX
```

In this case, `o`, `u`, `x`, and `X` correspond to the documented format
specifiers in `printf()`.

And, as before, the lowercase `n` should be substituted with `8`, `16`,
`32`, or `64`.

But just when you think you had enough of the macros, it turns out we
have a complete complementary set of them for `scanf()`!

``` {.c}
SCNdn    SCNdLEASTn    SCNdFASTn    SCNdMAX
SCNin    SCNiLEASTn    SCNiFASTn    SCNiMAX
SCNon    SCNoLEASTn    SCNoFASTn    SCNoMAX
SCNun    SCNuLEASTn    SCNuFASTn    SCNuMAX
SCNxn    SCNxLEASTn    SCNxFASTn    SCNxMAX
```

Remember: when you want to print out a fixed size integer type with
`printf()` or `scanf()`, grab the correct corresponding format specifer
from `<inttypes.h>`.
