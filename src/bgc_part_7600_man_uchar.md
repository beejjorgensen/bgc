<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<uchar.h>` Unicode utility functions {#uchar}

|Function|Description|
|--------|----------------------|
|[`c16rtomb()`](#man-c16rtomb)|Convert a `char16_t` to a multibyte character|
|[`c32rtomb()`](#man-c16rtomb)|Convert a `char32_t` to a multibyte character|
|[`mbrtoc16()`](#man-mbrtoc16)|Convert a multibyte character to a `char16_t`|
|[`mbrtoc32()`](#man-mbrtoc16)|Convert a multibyte character to a `char32_t`|


These functions are _restartable_, meaning multiple threads can safely
call them at once. They handle this by having their own conversion
state variable (of type `mbstate_t`) per call.

## Types

This header file defines four types.

|Type|Description|
|------|----------------------|
|`char16_t`|Type to hold 16-bit characters|
|`char32_t`|Type to hold 32-bit characters|
|`mbstate_t`|Holds the conversion state for restartable funcitons (also defined in [`<wchar.h>`](#wchar))|
|`size_t`|To hold various counts (also defined in [`<stddef.h>`](#stddef))|

String literals for the character types are `u` for `char16_t` and `U`
for `char32_t`.

``` {.c}
char16_t *str1 = u"Hello, world!";
char32_t *str2 = U"Hello, world!";

char16_t *chr1 = u'A';
char32_t *chr2 = U'B';
```

Note that `char16_t` and `char32_t` _might_ contain Unicode. Or not. If
`__STDC_UTF_16__` or `__STDC_UTF_32__` is defined as `1`, then
`char16_t` and `char32_t` use Unicode, respectively. Otherwise they
don't and the actual value stored depend on the locale. And if you're
not using Unicode, you have my commiserations.

## OS X issue

This header file doesn't exist on OS X---bummer. If you just want the
types, you can:

``` {.c}
#include <stdint.h>

typedef int_least16_t char16_t;
typedef int_least32_t char32_t;
```

But if you also want the functions, that's all on you.

[[manbreak]]
## `mbrtoc16()` `mbrtoc32()` {#man-mbrtoc16}

Convert a multibyte character to a `char16_t` or `char32_t` restartably

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <uchar.h>

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
|------|----------------------|
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

You can also pass `NULL` for `pc16` or `pc32`. This will cause no result
to be stored, but you can use it if you're only interested in the return
value from the functions.

Finally, if you pass `NULL` for `s`, the call is equivalent to:

``` {.c}
mbrtoc16(NULL, "", 1, ps)
```

Since the character is a NUL in that case, this has the effect of
setting the state in `ps` to the initial conversion state.

### Example {.unnumbered .unlisted}

Normal use case example where we get the first two character values from
the multibyte string `"€Zillion"`:

``` {.c}
#include <uchar.h>
#include <stdio.h>   // for printf()
#include <locale.h>  // for setlocale()
#include <string.h>  // for memset()

int main(void)
{
    char *s = "\u20acZillion";  // 20ac is "€"
    char16_t pc16;
    size_t r;
    mbstate_t mbs;

    setlocale(LC_ALL, "");
    memset(&mbs, 0, sizeof mbs);

    // Examine the next 8 bytes to see if there's a character in there
    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zu\n", r);     // Prints a value >= 1 (3 in UTF-8 locale)
    printf("%#x\n", pc16);  // Prints 0x20ac for "€"

    s += r;  // Move to next character

    // Examine the next 8 bytes to see if there's a character in there
    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zu\n", r);     // Prints 1
    printf("%#x\n", pc16);  // Prints 0x5a for "Z"
}
```

Example with a surrogate pair. In this case we read plenty to get the
entire character, but the result must be stored in two `char16_t`s,
requiring two calls to get them both.

``` {.c .numberLines}
#include <uchar.h>
#include <stdio.h>   // for printf()
#include <string.h>  // for memset()
#include <locale.h>  // for setlocale()

int main(void)
{
    char *s = "\U0001fbc5*";   // Stick figure glyph, more than 16 bits
    char16_t pc16;
    mbstate_t mbs;
    size_t r;

    setlocale(LC_ALL, "");
    memset(&mbs, 0, sizeof mbs);

    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zd\n", r);     // r is 4 bytes in UTF-8 locale
    printf("%#x\n", pc16);  // First value of surrogate pair

    s += r;  // Move to next character

    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zd\n", r);     // r is (size_t)(-3) here to indicate...
    printf("%#x\n", pc16);  // ...Second value of surrogate pair

    // Since r is -3, it means we're still processing the same
    // character, so DON'T move to the next character this time
    //s += r;  // Commented out

    r = mbrtoc16(&pc16, s, 8, &mbs);

    printf("%zd\n", r);     // 1 byte for "*"
    printf("%#x\n", pc16);  // 0x2a for "*"
}
```

Output on my system, indicating the first character is represented by
the pair `(0xd83e, 0xdfc5)` and the second character is represented by
`0x2a`:

``` {.default}
4
0xd83e
-3
0xdfc5
1
0x2a
```

### See Also {.unnumbered .unlisted}

[`c16rtomb()`](#man-c16rtomb),
[`c32rtomb()`](#man-c16rtomb)

[[manbreak]]
## `c16rtomb()` `c32rtomb()` {#man-c16rtomb}

Convert a `char16_t` or `char32_t` to a multibyte character restartably

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <uchar.h>

size_t c16rtomb(char * restrict s, char16_t c16, mbstate_t * restrict ps);

size_t c32rtomb(char * restrict s, char32_t c32, mbstate_t * restrict ps);
```

### Description {.unnumbered .unlisted}

If you have a character in a `char16_t` or `char32_t`, use these
functions to convert them into a multibyte character.

These functions figure out how many bytes are needed for the multibyte
character in the current locale and stores them in the buffer pointed to
by `s`.

But how big to make that buffer? Luckily there is a macro to help: it
needs be no larger than `MB_CUR_MAX`.

As a special case, if `s` is `NULL`, it's the same as calling

``` {.c}
c16rtomb(buf, L'\0', ps);  // or...
c32rtomb(buf, L'\0', ps);
```

where `buf` is a buffer maintained by the system that you don't have
access to.

This has the effect of setting the `ps` state to the initial state.

Finally for surrogate pairs (where the character has been split into two
`char16_t`s), you call this once with the first of the pair---at this
point, the function will return `0`. Then you call it again with the
second of the pair, and the function will return the number of bytes and
store the result in the array `s`.

### Return Value {.unnumbered .unlisted}

Returns the number of bytes stored in the array pointed to by `s`.

Returns 0 if processing is not yet complete for the current character,
as in the case of surrogate pairs.

If there is an encoding error, the functions return `(size_t)(-1)` and
`errno` is set to `EILSEQ`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <uchar.h>
#include <stdlib.h>  // for MB_CUR_MAX
#include <stdio.h>   // for printf()
#include <string.h>  // for memset()
#include <locale.h>  // for setlocale()

int main(void)
{
    char16_t c16 = 0x20ac;  // Unicode for Euro symbol
    char dest[MB_CUR_MAX];
    size_t r;
    mbstate_t mbs;

    setlocale(LC_ALL, "");
    memset(&mbs, 0, sizeof mbs);  // Reset conversion state

    // Convert
    r = c16rtomb(dest, c16, &mbs);

    printf("r == %zd\n", r);  // r == 3 on my system

    // And this should print a Euro symbol
    printf("dest == \"%s\"\n", dest);
}
```

Output on my system:

``` {.default}
r == 3
dest == "€"
```

This is a more complex example that converts a large-valued character in
a multibyte string into a surrogate pair (as in the `mbrtoc16()`
example, above) and then converts it back again into a multibyte string
to print.

``` {.c .numberLines}
#include <uchar.h>
#include <stdlib.h>  // for MB_CUR_MAX
#include <stdio.h>   // for printf()
#include <string.h>  // for memset()
#include <locale.h>  // for setlocale()

int main(void)
{
    char *src = "\U0001fbc5*";   // Stick figure glyph, more than 16 bits
    char dest[MB_CUR_MAX];
    char16_t surrogate0, surrogate1;
    mbstate_t mbs;
    size_t r;

    setlocale(LC_ALL, "");
    memset(&mbs, 0, sizeof mbs);  // Reset conversion state

    // Get first surrogate character
    r = mbrtoc16(&surrogate0, src, 8, &mbs);

    // Get next surrogate character
    src += r;  // Move to next character
    r = mbrtoc16(&surrogate1, src, 8, &mbs);

    printf("Surrogate pair: %#x, %#x\n", surrogate0, surrogate1);

    // Now reverse it
    memset(&mbs, 0, sizeof mbs);  // Reset conversion state

    // Process first surrogate character
    r = c16rtomb(dest, surrogate0, &mbs);

    // r should be 0 at this point, because the character hasn't been
    // processed yet. And dest won't have anything useful... yet!
    printf("r == %zd\n", r);   // r == 0

    // Process second surrogate character
    r = c16rtomb(dest, surrogate1, &mbs);

    // Now we should be in business. r should have the number of
    // bytes, and dest should hold the character.
    printf("r == %zd\n", r);  // r == 4 on my system

    // And this should print a stick figure, if your font supports it
    printf("dest == \"%s\"\n", dest);
}
```

### See Also {.unnumbered .unlisted}

[`mbrtoc16()`](#man-mbrtoc16),
[`mbrtoc32()`](#man-mbrtoc16)

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