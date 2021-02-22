<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdlib.h>` Standard Library Functions {#stdlib}

The `<stdlib.h>` header has all kinds of---dare I say---miscellaneous
functions bundled into it. This functionality includes:

* Conversions from numbers to strings
* Conversions from strings to numbers
* Pseudorandom number generation
* Dynamic memory allocation
* Various ways to exit the program
* Ability to run external programs
* Binary search (or some fast search)
* Quicksort (or some fast sort)
* Integer arithmetic functions
* Multibyte and wide character and string conversions

So, you know... a little of everything.

## `<stdlib.h>` Types and Macros

A couple new types and macros are introduced, though some of these might
also be defined elsewhere:

|Type|Description|
|-|-|
|`size_t`|Returned from `sizeof` and used elsewhere
|`wchar_t`|For wide character operations
|`div_t`|For the `div()` function|
|`ldiv_t`|For the `ldiv()` function|
|`lldiv_t`|for the `lldiv()` function|

And some macros:

|Type|Description|
|-|-|
|`NULL`|Our good pointer friend|
|`EXIT_SUCCESS`|Good exit status when things go well|
|`EXIT_FAILURE`|Good exit status when things go poorly|
|`RAND_MAX`|The maximum value that can be returned by the `rand()` function|
|`MB_CUR_MAX`|Maximum number of bytes in a multibyte character in the current locale|

And there you have it. Just a lot of fun, useful functions in here.
Let's check 'em out!


[[pagebreak]]
## `atof()` {#man-atof}

Convert a string to a floating point value

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

double atof(const char *nptr);
```

### Description {.unnumbered .unlisted}

This stood for [fl["ASCII-To-Floating" back in the
day|http://man.cat-v.org/unix-1st/3/atof]], but no one would dare to use
such coarse language now.

But the gist is the same: we're going to convert a string with numbers
and (optionally) a decimal point into a floating point value. Leading
whitespace is ignored, and translation stops at the first invalid
character.

If the result doesn't fit in a `double`, behavior is undefined.

It generally works as if you'd called [`strtod()`](#man-strtod):

``` {.c}
strtod(nptr, NULL)
```

So check out [that reference page](#man-strtod) for more info.

In fact, `strtod()` is just better and you should probably use that.

### Return Value {.unnumbered .unlisted}

Returns the string converted to a `double`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
    double x = atof("3.141593");
    printf("%f\n", x);  // 3.141593
```

### See Also {.unnumbered .unlisted}

[`atoi()`](#man-atoi),
[`strtod()`](#man-strtod)

[[pagebreak]]
## `atoi()`, `atol()`, `atoll()` {#man-atoi}

Convert an integer in a string into a integer type

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

int atoi(const char *nptr);

long int atol(const char *nptr);

long long int atoll(const char *nptr);
```

### Description {.unnumbered .unlisted}

Back in the day, `atoi()` stood for
[fl["ASCII-To_Integer"|http://man.cat-v.org/unix-1st/3/atoi]] but now
the spec makes no mention of that.

These functions take a string with a number in them and convert it to an
integer of the specified return type. Leading whitespace is ignored.
Translation stops at the first invalid character.

If the result doesn't fit in the return type, behavior is undefined.

It generally works as if you'd called [`strtol()`](#man-strtol) family
of functions:

``` {.c}
atoi(nptr)                 // is basically the same as...
(int)strtol(nptr, NULL, 10)

atol(nptr)                 // is basically the same as...
strtol(nptr, NULL, 10)

atoll(nptr)                // is basically the same as...
strtoll(nptr, NULL, 10)
```

Again, the [`strtol()`](#man-strtol) functions are generally better, so
I recommend them instead of these.

### Return Value {.unnumbered .unlisted}

Returns an integer result corresponding to the return type.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
int x = atof("3490");
printf("%d\n", x);  // 3490
```

### See Also {.unnumbered .unlisted}

[`atof()`](#man-atof),
[`strtol()`](#man-strtol)

[[pagebreak]]
## `strtod()`, `strtof()`, `strtold()` {#man-strtod}

Convert a string to a floating point number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

double strtod(const char * restrict nptr, char ** restrict endptr);

float strtof(const char * restrict nptr, char ** restrict endptr);

long double strtold(const char * restrict nptr, char ** restrict endptr);
```

### Description {.unnumbered .unlisted}

These are some neat functions that convert strings to floating point
numbers (or even NaN or Infinity) and provide some error checking,
besides.

Firstly, leading whitespace is skipped.

Then the functions attempt to convert characters into the floating point
result. Finally, when an invalid character (or NUL character) is
reached, they set `endptr` to point to the invalid character.

Set `endptr` to `NULL` if you don't care about where the first invalid
character is.

If you didn't set `endptr` to `NULL`, it will point to a NUL character
if the translation didn't find any bad characters. That is:

``` {.c}
if (*endptr == '\0') {
    printf("What a perfectly-formed number!\n");
} else {
    printf("I found badness in your number: \"%s\"\n", endptr);
}
```

But guess what! You can also translate strings into special values, like
NaN and Infinity!

If `nptr` points to a string containing `INF` or `INFINITY` (upper or
lowercase), the value for Infinity will be returned.

If `nptr` points to a string containing `NAN`, then (a quiet,
non-signalling) NaN will be returned. You can tag the `NAN` with a
sequence of characters from the set `0`-`9`, `a`-`z`, `A`-`Z`, and `_`
by enclosing them in parens:

``` {.c}
NAN(foobar_3490)
```

What your compiler does with this is implementation-defined, but it can
be used to specify different kinds of NaN.

You can also specify a number in hexadecimal with a power-of-two
exponent ($2^x$) if you lead with `0x` (or `0X`). For the exponent, use
a `p` followed by a base 10 exponent. (You can't use `e` because that's
a valid hex digit!)

Example:

``` {.c}
0xabc.123p15
```

Which computes to $0xabc.123\times2^{15}$.

You can put in `FLT_DECIMAL_DIG`, `DBL_DECIMAL_DIG`, or
`LDBL_DECIMAL_DIG` digits and get a correctly-rounded result for the
type.

### Return Value {.unnumbered .unlisted}

Returns the converted number. If there was no number, returns `0`.
`endptr` is set to point to the first invalid character, or the NUL
terminator if all characters were consumed.

If there's an overflow, `HUGE_VAL`, `HUGE_VALF`, or `HUGE_VALL` is
returned, signed like the input, and `errno` is set to `ERANGE`.

If there's an underflow, it returns the smallest number closest to zero
with the input sign. `errno` may be set to `ERANGE`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
char *inp = "   123.4567beej";
char *badchar;

double val = strtod(inp, &badchar);

printf("Converted string to %f\n", val);
printf("Encountered bad characters: %s\n", badchar);

val = strtod("987.654321beej", NULL);
printf("Ignoring bad chars: %f\n", val);

val = strtod("11.2233", &badchar);

if (*badchar == '\0')
    printf("No bad chars: %f\n", val);
else
    printf("Found bad chars: %f, %s\n", val, badchar);
```

Output:

```
Converted string to 123.456700
Encountered bad characters: beej
Ignoring bad chars: 987.654321
No bad chars: 11.223300
```

### See Also {.unnumbered .unlisted}

[`atof()`](#man-atof),
[`strtol()`](#man-strtol)
<!--
[[pagebreak]]
## `example()`, `example()`, `example()` {#man-example}

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