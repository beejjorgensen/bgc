<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<inttypes.h>` More Integer Conversions {#inttypes}

|Function|Description|
|--------|----------------------|
|[`imaxabs()`](#man-imaxabs)|Compute the absolute value of an `intmax_t`|
|[`imaxdiv()`](#man-imaxdiv)|Compute the quotient and remainder of `intmax_t`s|
|[`strtoimax()`](#man-strtoimax)|Convert strings to type `intmax_t`|
|[`strtoumax()`](#man-strtoimax)|Convert strings to type `uintmax_t`|
|[`wcstoimax()`](#man-wcstoimax)|Convert wide strings to type `intmax_t`|
|[`wcstoumax()`](#man-wcstoimax)|Convert wide strings to type `uintmax_t`|

This header does conversions to maximum sized integers, division with
maximum sized integers, and also provides format specifiers for
[`printf()`](#man-printf) and [`scanf()`](#man-scanf) for a variety of
types defined in [`<stdint.h>`](#stdint).

The header [`<stdint.h>`](#stdint) is included by this one.

## Macros

These are to help with 
[`printf()`](#man-printf) and [`scanf()`](#man-scanf) when you use a
type such as `int_least16_t`... what format specifiers do you use?

Let's start with `printf()`---all these macros start with `PRI` and then
are followed by the format specifier you'd typically use for that type.
Lastly, the number of bits is added on.

For example, the format specifier for a 64-bit integer is `PRId64`---the
`d` is because you usually print integers with `"%d"`.

An unsigned 16-bit integer could be printed with `PRIu16`.

These macros expand to string literals. We can take advantage of the
fact that C automatically concatenates neighboring string literals and
use these specifiers like this:

``` {.c .numberLines}
#include <stdio.h>     // for printf()
#include <inttypes.h>

int main(void)
{
    int16_t x = 32;

    printf("The value is %" PRId16 "!\n", x);
}
```

There's nothing magical happening on line 8, above. Indeed, if I print
out the value of the macro:

``` {.c}
printf("%s\n", PRId16);
```

we get this on my system:

``` {.default}
hd
```

which is a `printf()` format specifier meaning "short signed integer" .

So back to line 8, after string literal concatenation, it's just as if
I'd typed:

``` {.c .numberLines startFrom="8"}
    printf("The value is %hd!\n", x);
```

Here's a table of all the macros you can use for `printf()` format
specifiers... substitute the number of bits for _N_, usually 8, 16, 32,
or 64.

--------- -------------- ------------- --------- ---------
`PRId`_N_ `PRIdLEAST`_N_ `PRIdFAST`_N_ `PRIdMAX` `PRIdPTR`
`PRIi`_N_ `PRIiLEAST`_N_ `PRIiFAST`_N_ `PRIiMAX` `PRIiPTR`
`PRIo`_N_ `PRIoLEAST`_N_ `PRIoFAST`_N_ `PRIoMAX` `PRIoPTR`
`PRIu`_N_ `PRIuLEAST`_N_ `PRIuFAST`_N_ `PRIuMAX` `PRIuPTR`
`PRIx`_N_ `PRIxLEAST`_N_ `PRIxFAST`_N_ `PRIxMAX` `PRIxPTR`
`PRIX`_N_ `PRIXLEAST`_N_ `PRIXFAST`_N_ `PRIXMAX` `PRIXPTR`
--------- -------------- ------------- --------- ---------

Note again how the lowercase center letter represents the usual format
specifiers you'd pass to `printf()`: `d`, `i`, `o`, `u`, `x`, and `X`.

And we have a similar set of macros for `scanf()` for reading in these
various types:

--------- -------------- ------------- --------- ---------
`SCNd`_N_ `SCNdLEAST`_N_ `SCNdFAST`_N_ `SCNdMAX` `SCNdPTR`
`SCNi`_N_ `SCNiLEAST`_N_ `SCNiFAST`_N_ `SCNiMAX` `SCNiPTR`
`SCNo`_N_ `SCNoLEAST`_N_ `SCNoFAST`_N_ `SCNoMAX` `SCNoPTR`
`SCNu`_N_ `SCNuLEAST`_N_ `SCNuFAST`_N_ `SCNuMAX` `SCNuPTR`
`SCNx`_N_ `SCNxLEAST`_N_ `SCNxFAST`_N_ `SCNxMAX` `SCNxPTR`
--------- -------------- ------------- --------- ---------

The rule is that for each type defined in `<stdint.h>` there will be
corresponding `printf()` and `scanf()` macros defined here.

[[manbreak]]
## `imaxabs()` {#man-imaxabs}

Compute the absolute value of an `intmax_t`

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <inttypes.h>

intmax_t imaxabs(intmax_t j);
```

### Description {.unnumbered .unlisted}

When you need the absolute value of the biggest integer type on the
system, this is the function for you.

The spec notes that if the absolute value of the number cannot be
represented, the behavior is undefined. This would happen if you tried
to take the absolute value of the smallest possible negative number in a
two's-complement system.

### Return Value {.unnumbered .unlisted}

Returns the absolute value of the input, $|j|$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <inttypes.h>

int main(void)
{
    intmax_t j = -3490;

    printf("%jd\n", imaxabs(j));    // 3490
}
```

### See Also {.unnumbered .unlisted}

[`fabs()`](#man-fabs)


[[manbreak]]
## `imaxdiv()` {#man-imaxdiv}

Compute the quotient and remainder of `intmax_t`s

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <inttypes.h>

imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom);
```

### Description {.unnumbered .unlisted}

When you want to do integer division and remainder in a single
operation, this function will do it for you.

It computes `numer/denom` and `numer%denom` and returns the result in a
structure of type `imaxdiv_t`.

This structure has two `imaxdiv_t` fields, `quot` and `rem`, that you
use to retrieve the sought-after values.

### Return Value {.unnumbered .unlisted}

Returns an `imaxdiv_t` containing the quotient and remainder of the
operation.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <inttypes.h>

int main(void)
{
    intmax_t numer = INTMAX_C(3490);
    intmax_t denom = INTMAX_C(17);

    imaxdiv_t r = imaxdiv(numer, denom);

    printf("Quotient: %jd, remainder: %jd\n", r.quot, r.rem);
}
```

Output:

``` {.default}
Quotient: 205, remainder: 5
```

### See Also {.unnumbered .unlisted}

[`remquo()`](#man-remquo)

[[manbreak]]
## `strtoimax()` `strtoumax()` {#man-strtoimax}

Convert strings to types `intmax_t` and `uintmax_t`

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <inttypes.h>

intmax_t strtoimax(const char * restrict nptr, char ** restrict endptr,
                   int base);

uintmax_t strtoumax(const char * restrict nptr, char ** restrict endptr,
                   int base);
```

### Description {.unnumbered .unlisted}

These work just like the [`strtol()`](#man-strtol) family of functions,
except they return an `intmax_t` or `uintmax_t`.

See the [`strtol()`](#man-strtol) reference page for details.

### Return Value {.unnumbered .unlisted}

Returns the converted string as an `intmax_t` or `uintmax_t`.

If the result is out of range, the returned value will be `INTMAX_MAX`,
`INTMAX_MIN`, or `UINTMAX_MAX`, as appropriate. And the `errno` variable
will be set to `ERANGE`.

### Example {.unnumbered .unlisted}

The following example converts a base-10 number to an `intmax_t`.  Then
it attempts to convert an invalid base-2 number, catching the error.

``` {.c .numberLines}
#include <stdio.h>
#include <inttypes.h>

int main(void)
{
    intmax_t r;
    char *endptr;

    // Valid base-10 number
    r = strtoimax("123456789012345", &endptr, 10);

    if (*endptr != '\0')
        printf("Invalid digit: %c\n", *endptr);
    else
        printf("Value is %jd\n", r);
    
    // The following binary number contains an invalid digit
    r = strtoimax("0100102010101101", &endptr, 2);

    if (*endptr != '\0')
        printf("Invalid digit: %c\n", *endptr);
    else
        printf("Value is %jd\n", r);
}
```

Output:

``` {.default}
Value is 123456789012345
Invalid digit: 2
```

### See Also {.unnumbered .unlisted}

[`strtol()`](#man-strtol),
[`errno`](#errno)


[[manbreak]]
## `wcstoimax()` `wcstoumax()` {#man-wcstoimax}

Convert wide strings to types `intmax_t` and `uintmax_t`

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stddef.h> // for wchar_t
#include <inttypes.h>

intmax_t wcstoimax(const wchar_t * restrict nptr,
                   wchar_t ** restrict endptr, int base);

uintmax_t wcstoumax(const wchar_t * restrict nptr,
                    wchar_t ** restrict endptr, int base);
```

### Description {.unnumbered .unlisted}

These work just like the [`wcstol()`](#man-wcstol) family of functions,
except they return an `intmax_t` or `uintmax_t`.

See the [`wcstol()`](#man-wcstol) reference page for details.

### Return Value {.unnumbered .unlisted}

Returns the converted wide string as an `intmax_t` or `uintmax_t`.

If the result is out of range, the returned value will be `INTMAX_MAX`,
`INTMAX_MIN`, or `UINTMAX_MAX`, as appropriate. And the `errno` variable
will be set to `ERANGE`.

### Example {.unnumbered .unlisted}

The following example converts a base-10 number to an `intmax_t`.  Then
it attempts to convert an invalid base-2 number, catching the error.

``` {.c .numberLines}
#include <wchar.h>
#include <inttypes.h>

int main(void)
{
    intmax_t r;
    wchar_t *endptr;

    // Valid base-10 number
    r = wcstoimax(L"123456789012345", &endptr, 10);

    if (*endptr != '\0')
        wprintf(L"Invalid digit: %lc\n", *endptr);
    else
        wprintf(L"Value is %jd\n", r);
    
    // The following binary number contains an invalid digit
    r = wcstoimax(L"0100102010101101", &endptr, 2);

    if (*endptr != '\0')
        wprintf(L"Invalid digit: %lc\n", *endptr);
    else
        wprintf(L"Value is %jd\n", r);
}
```

``` {.default}
Value is 123456789012345
Invalid digit: 2
```

### See Also {.unnumbered .unlisted}

[`wcstol()`](#man-wcstol),
[`errno`](#errno)

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