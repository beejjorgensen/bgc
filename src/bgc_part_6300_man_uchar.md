<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<uchar.h>` Unicode utility functions {#uchar}

These functions are _restartable_, meaning multiple threads can safely
call them at once. They handle this by having their own conversion
state variable (of type `mbstate_t`) per call.

## Types

This header file defines four types.

|Type|Description|
|-|-|
|`char16_t`|Type to hold 16-bit characters|
|`char32_t`|Type to hold 32-bit characters|
|`mbstate_t`|Holds the conversion state for restartable funcitons (also defined in [`<wchar.h>`](#wchar))|
|`size_t`|To hold various counts (also defined in [`<stddef.h>`](#stddef))|

String literals for the character types are `u` for `char16_t` and `U`
for `char32_t`.

``` {.c}
char16_t *str1 = u"Hello, world!";
char32_t *str2 = U"Hello, world!";
```

Note that `char16_t` and `char32_t` _might_ be Unicode. Or not. If
`__STDC_UTF_16__` or `__STDC_UTF_32__` is defined as `1`, then
`char16_t` and `char32_t` use Unicode, respectively. Otherwise they
don't and the actual value stored depend on the locale. Sorry!

[[manbreak]]
## `mbrtoc16()` `mbrtoc32()` {#man-mbrtoc}

Convert a multibyte character to a `char16_t` or `char32_t` restartably

### Synopsis {.unnumbered .unlisted}

``` {.c}
size_t mbrtoc16(char16_t * restrict pc16, const char * restrict s, size_t n,
                mbstate_t * restrict ps);

size_t mbrtoc32(char32_t * restrict pc32, const char * restrict s, size_t n,
                mbstate_t * restrict ps);
```

### Description {.unnumbered .unlisted}

Given a source string `s` and a destination buffer `pc16` (or `pc32` for
`mbrtoc32()`), convert the first character of the source to `char16_t`s
(or `char32_t`s for `mbrtoc32()`).

Basically you have a regular character and you want it as `char16_t` or
`char32_t`. Use these functions to do it. Note that only one character
is converted no matter how many characters in `s`.

As the functions scan `s`, you don't want them to overrun the end. So
you pass in `n` as the maximum number of bytes to inspect. The functions
will quit after that many bytes or when they have a complete multibyte
character, whichever comes first.

Since they're restartable, pass in a conversion state variable for the
functions to do their work.

And the result will be placed in `pc16` (or `pc32` for `mbrtoc32()`).

### Return Value {.unnumbered .unlisted}

When successful this function returns a number between `1` and `n`
inclusive representing the number of bytes that made up the multibyte
character.

Or, also in the success category, they can return `0` if the source
character is the NUL character (value `0`).

When not entirely successful, they can return a variety of codes. These
are all of type `size_t`, but negative values cast to that type.

|Return Value|Description|
|-|-|
|`(size_t)(-1)`|Encoding error---this isn't a valid sequence of bytes. `errno` is set to `EILSEQ`.|
|`(size_t)(-2)`|`n` bytes were examined and were a _partial_ valid character, but not a complete one.|
|`(size_t)(-3)`|A subsequent value of a character that can't be represented as a single value. See below.|

Case `(size_t)(-3)` is an odd one. Basically there are some characters
that can't be represented with 16 bits and so can't be stored in a
`char16_t`. These characters are store in something called (in the
Unicode world) _surrogate pairs_. That is, there are _two_ 16-bit values
back to back that represent a larger Unicode value.

For example, if you want to read the Unicode character `\U0001fbc5`
(which is a [flw[stick figure|Symbols_for_Legacy_Computing]]---I'm just
not putting it in the text because my font doesn't render it) that's
more than 16 bits. But each call to `mbrtoc16()` only returns a single
`char16_t`!

So subsequent calls to `mbrtoc16()` resolves the _next_ value in the
surrogate pair and returns `(size_t)(-3)` to let you know this has
happened.


### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>  // for fflush(stdout)
#include <string.h>
#include <uchar.h>
#include <locale.h>

char *size_t_str(size_t v, char *result)
{
    for (int i = -1; i > -20; i--) {
        if (v == (size_t)i) {
            sprintf(result, "(size_t)(%d)", i);
            return result;
        }
    }

    sprintf(result, "%zu", v);

    return result;
}

int main(void)
{
    //char *s = "\U0001fbc5€abc•";
    char *s = "🯅€abc•";
    char16_t pc16;
    mbstate_t mbs;
    size_t r;
    char tmp[128];

    setlocale(LC_ALL, "");
    memset(&mbs, 0, sizeof mbs);

    for (int i = 0; i < 10; i++) {
        r = mbrtoc16(&pc16, s+i, 1, &mbs);
        printf("%d %s %d\n", i, size_t_str(r, tmp), pc16);
    }
}
```

### See Also {.unnumbered .unlisted}

[`example()`](#man-example),

```
size_t mbrtoc16(char16_t * restrict pc16, const char * restrict s, size_t n,
                mbstate_t * restrict ps);

size_t c16rtomb(char * restrict s, char16_t c16, mbstate_t * restrict ps);

size_t mbrtoc32(char32_t * restrict pc32, const char * restrict s, size_t n,
                mbstate_t * restrict ps);

size_t c32rtomb(char * restrict s, char32_t c32, mbstate_t * restrict
ps);
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