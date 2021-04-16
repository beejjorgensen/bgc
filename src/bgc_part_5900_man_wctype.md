<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<wctype.h>` Wide Character Classification and Transformation {#wctype}

This is like [`<ctype.h>`](#ctype) except for wide characters.

[[pagebreak]]
## `example()` {#man-iswalnum}

Test if a wide character is alphanumeric.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswalnum(wint_t wc);
```

### Description {.unnumbered .unlisted}

Basically tests if a character is alphabetic (`A`-`Z` or `a`-`z`) or a digit
(`0`-`9`). But some other characters might also qualify based on the locale.

This is equivalent to testing if `iswalpha()` or `iswdigit()` is true.

### Return Value {.unnumbered .unlisted}

Returns true if the character is alphanumeric.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                  testing this char
    //                           v
    wprintf(L"%ls\n", iswalnum(L'a')? L"yes": L"no");  // yes
    wprintf(L"%ls\n", iswalnum(L'B')? L"yes": L"no");  // yes
    wprintf(L"%ls\n", iswalnum(L'5')? L"yes": L"no");  // yes
    wprintf(L"%ls\n", iswalnum(L'?')? L"yes": L"no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`iswalpha()`](#man-iswalpha),
[`iswdigit()`](#man-iswdigit),
[`isalnum()`](#man-isalnum)

[[pagebreak]]
## `iswalpha()` {#man-iswalpha}

Tests if a wide character is alphabetic

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswalpha(wint_t wc);
```

### Description {.unnumbered .unlisted}

Basically tests if a character is alphabetic (`A`-`Z` or `a`-`z`). But
some other characters might also qualify based on the locale. (If other
characters qualify, they won't be control characters, digits,
punctuation, or spaces.)

This is the same as testing for `iswupper()` or `iswlower()`.

### Return Value {.unnumbered .unlisted}

Returns true if the character is alphabetic.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                  testing this char
    //                           v
    wprintf(L"%ls\n", iswalpha(L'a')? L"yes": L"no");  // yes
    wprintf(L"%ls\n", iswalpha(L'B')? L"yes": L"no");  // yes
    wprintf(L"%ls\n", iswalpha(L'5')? L"yes": L"no");  // no
    wprintf(L"%ls\n", iswalpha(L'?')? L"yes": L"no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`iswalnum()`](#man-iswalnum),
[`isalpha()`](#man-isalpha)


[[pagebreak]]
## `iswblank()` {#man-iswblank}

Tests if this is a wide blank character

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswblank(wint_t wc);
```

### Description {.unnumbered .unlisted}

Blank characters are whitespace that are also used as word separators on
the same line. In the "C" locale, the only blank characters are space
and tab.

Other locales might define other blank characters.

### Return Value {.unnumbered .unlisted}

Returns true if this is a blank character.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                   testing this char
    //                           v
    wprintf(L"%ls\n", iswblank(L' ')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswblank(L'\t')? L"yes": L"no");  // yes
    wprintf(L"%ls\n", iswblank(L'\n')? L"yes": L"no");  // no
    wprintf(L"%ls\n", iswblank(L'a')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswblank(L'?')? L"yes": L"no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`iswspace()`](#man-iswspace),
[`isblank()`](#man-isblank)

[[pagebreak]]
## `iswcntrl()` {#man-iswcntrl}

Tests if this is a wide control character.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswcntrl(wint_t wc);
```

### Description {.unnumbered .unlisted}

The spec is pretty barren, here. But I'm just going to assume that it
works like the non-wide version. So let's look at that.

A _control character_ is a locale-specific non-printing character.

For the "C" locale, this means control characters are in the range 0x00
to 0x1F (the character right before SPACE) and 0x7F (the DEL character).

Basically if it's not an ASCII (or Unicode less than 128) printable
character, it's a control character in the "C" locale.

Probably.
 
### Return Value {.unnumbered .unlisted}

Returns true if this is a control character.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                   testing this char
    //                           v
    wprintf(L"%ls\n", iswcntrl(L'\t')? L"yes": L"no");  // yes (tab)
    wprintf(L"%ls\n", iswcntrl(L'\n')? L"yes": L"no");  // yes (newline)
    wprintf(L"%ls\n", iswcntrl(L'\r')? L"yes": L"no");  // yes (return)
    wprintf(L"%ls\n", iswcntrl(L'\a')? L"yes": L"no");  // yes (bell)
    wprintf(L"%ls\n", iswcntrl(L' ')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswcntrl(L'a')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswcntrl(L'?')? L"yes": L"no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`iscntrl()`](#man-iscntrl)

[[pagebreak]]
## `iswdigit()` {#man-iswdigit}

Test if this wide character is a digit

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswdigit(wint_t wc);
```

### Description {.unnumbered .unlisted}

Tests if the wide character is a digit (`0`-`9`).

### Return Value {.unnumbered .unlisted}

Returns true if the character is a digit.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                   testing this char
    //                           v
    wprintf(L"%ls\n", iswdigit(L'0')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswdigit(L'5')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswdigit(L'a')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswdigit(L'B')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswdigit(L'?')? L"yes": L"no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`iswalnum()`](#man-iswalnum),
[`isdigit()`](#man-isdigit)

[[pagebreak]]
## `example()` {#man-example}

Test to see if a wide character is a printable non-space

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswgraph(wint_t wc);
```

### Description {.unnumbered .unlisted}

Returns true if this is a printable (non-control) character and also not
a whitespace character.

Basically if `iswprint()` is true and `iswspace()` is false.

### Return Value {.unnumbered .unlisted}

Returns true if this is a printable non-space character.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                   testing this char
    //                           v
    wprintf(L"%ls\n", iswgraph(L'0')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswgraph(L'a')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswgraph(L'B')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswgraph(L'?')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswgraph(L' ')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswgraph(L'\n')? L"yes": L"no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`iswprint()`](#man-iswprint),
[`iswspace()`](#man-iswspace),
[`isgraph()`](#man-isgraph)

<!--
[[pagebreak]]
## `example()` {#man-example}

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