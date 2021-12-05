<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<limits.h>` Numeric Limits {#limits}

Important note: the "minimum magnitude" in the table below is the
minimum allowed by the spec. It's very likely that the values on your
bad-ass system exceed those, below.

|Macro|Minimum Magnitude|Description|
|-----|----------|-----------------------|
|`CHAR_BIT`|`8`|Number of bits in a byte|
|`SCHAR_MIN`|`-127`|Minimum value of a `signed char`|
|`SCHAR_MAX`|`127`|Maximum value of a `signed char`|
|`UCHAR_MAX`|`255`|Maximum value of an `unsigned char`^[The minimum value of an `unsigned char` is `0`. Same fo an `unsigned short` and `unsigned long`. Or any unsigned type, for that matter.]|
|`CHAR_MIN`|`0` or `SCHAR_MIN`|More detail below|
|`CHAR_MAX`|`SCHAR_MAX` or `UCHAR_MAX`|More detail below|
|`MB_LEN_MAX`|`1`|Maximum number of bytes in a multibyte character on any locale|
|`SHRT_MIN`|`-32767`|Minimum value of a `short`|
|`SHRT_MAX`|`32767`|Maximum value of a `short`|
|`USHRT_MAX`|`65535`|Maximum value of an `unsigned short`|
|`INT_MIN`|`-32768`|Minimum vale of an `int`|
|`INT_MAX`|`32768`|Maximum value of an `int`|
|`UINT_MAX`|`65535`|Maximum value of an `unsigned int`|
|`LONG_MIN`|`-2147483647`|Minimum value of a `long`|
|`LONG_MAX`|`2147483647`|Maximum value of a `long`|
|`ULONG_MAX`|`4294967295`|Maximum value of an `unsigned long`|
|`LLONG_MIN`|`-9223372036854775807`|Minimum value of a `long long`|
|`LLONG_MAX`|`9223372036854775807`|Maximum value of a `long long`|
|`ULLONG_MAX`|`18446744073709551615`|Maximum value of an `unsigned long long`|

## `CHAR_MIN` and `CHAR_MAX`

When it comes to the `CHAR_MIN` and `CHAR_MAX` macros, it all depends on
if your `char` type is signed or unsigned by default. Remember that C
leaves that up to the implementation? No? Well, it does.

So if it's signed, the values of `CHAR_MIN` and `CHAR_MAX` are the same
as `SCHAR_MIN` and `SCHAR_MAX`.

And if it's unsigned, the values of `CHAR_MIN` and `CHAR_MAX` are the
same as `0` and `UCHAR_MAX`.

Side benefit: you can tell at runtime if the system has signed or
unsigned chars by checking to see if `CHAR_MIN` is `0`.

``` {.c .numberLines}
#include <stdio.h>
#include <limits.h>

int main(void)
{
    printf("chars are %ssigned\n", CHAR_MIN == 0? "un": "");
}
```

On my system, `char`s are signed.

## Choosing the Correct Type

If you want to be super portable, choose a type you know will be at
least as big as you need by the table, above.

That said, a lot of code, for better or (likely) worse, assumes `int`s
are 32-bits, when in actuality it's only guaranteed to be 16.

If you need a guaranteed bit size, check out the `int_leastN_t` types in
[`<stdint.h>`](#stdint).

## Whither Two's Complement?

If you were looking closely and have _a priori_ knowledge of the matter,
you might have thought I erred in the minimum values of the macros,
above.

"`short` goes from `32767` to `-32767`? Shouldn't it go to `-32768?`"

No, I have it right. The spec list the minimum magnitudes for those
macros, and some old-timey systems might have used a different encoding
for their signed values that could only go that far.

Virtually every modern system uses [flw[Two's
Complement|Two%27s_complement]] for signed numbers, and those would go
from `32767` to `-32768` for a `short`. Your system probably does, too.

## Demo Program

Here's a program to print out the values of the macros:

``` {.c .numberLines}
#include <stdio.h>
#include <limits.h>

int main(void)
{
    printf("CHAR_BIT = %d\n", CHAR_BIT);
    printf("SCHAR_MIN = %d\n", SCHAR_MIN);
    printf("SCHAR_MAX = %d\n", SCHAR_MAX);
    printf("UCHAR_MAX = %d\n", UCHAR_MAX);
    printf("CHAR_MIN = %d\n", CHAR_MIN);
    printf("CHAR_MAX = %d\n", CHAR_MAX);
    printf("MB_LEN_MAX = %d\n", MB_LEN_MAX);
    printf("SHRT_MIN = %d\n", SHRT_MIN);
    printf("SHRT_MAX = %d\n", SHRT_MAX);
    printf("USHRT_MAX = %u\n", USHRT_MAX);
    printf("INT_MIN = %d\n", INT_MIN);
    printf("INT_MAX = %d\n", INT_MAX);
    printf("UINT_MAX = %u\n", UINT_MAX);
    printf("LONG_MIN = %ld\n", LONG_MIN);
    printf("LONG_MAX = %ld\n", LONG_MAX);
    printf("ULONG_MAX = %lu\n", ULONG_MAX);
    printf("LLONG_MIN = %lld\n", LLONG_MIN);
    printf("LLONG_MAX = %lld\n", LLONG_MAX);
    printf("ULLONG_MAX = %llu\n", ULLONG_MAX);
}
```

On my 64-bit Intel system with clang, this outputs:

``` {.default}
CHAR_BIT = 8
SCHAR_MIN = -128
SCHAR_MAX = 127
UCHAR_MAX = 255
CHAR_MIN = -128
CHAR_MAX = 127
MB_LEN_MAX = 6
SHRT_MIN = -32768
SHRT_MAX = 32767
USHRT_MAX = 65535
INT_MIN = -2147483648
INT_MAX = 2147483647
UINT_MAX = 4294967295
LONG_MIN = -9223372036854775808
LONG_MAX = 9223372036854775807
ULONG_MAX = 18446744073709551615
LLONG_MIN = -9223372036854775808
LLONG_MAX = 9223372036854775807
ULLONG_MAX = 18446744073709551615
```

Looks like my system probably uses two's-complement encoding for signed
numbers, my `char`s are signed, and my `int`s are 32-bit.
