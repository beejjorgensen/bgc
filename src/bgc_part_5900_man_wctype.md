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