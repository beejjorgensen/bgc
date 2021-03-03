<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<ctype.h>` Character Classification {#ctype}

This collection of macros is good for testing characters to see if
they're of a certain class, such as alphabetic, numeric, control
characters, etc.

Surprisingly, they take `int` arguments instead of some kind of `char`.
This is so you can feed `EOF` in for convenience if you have an integer
representation of that. If not `EOF`, the value passed in has to be
representable in an `unsigned char`. Otherwise it's (dun dun DUUNNNN)
undefined behavior. So you can forget about passing in your UTF-8
multibyte characters.

Also, the behavior of these functions varies based on locale.

In many of the pages in this section, I give some examples. These are
from the "C" locale, and might vary if you've set a different locale.

Note that wide characters have their own set of classification
functions, so don't try to use these on `wchar_t`s. Or _else_!

[[pagebreak]]
## `isalnum()` {#man-example}

Tests if a character is alphabetic or is a digit

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int isalnum(int c);
```

### Description {.unnumbered .unlisted}

Tests if a character is alphabetic (`A`-`Z` or `a`-`z`) or a digit
(`0`-`9`).

Is equivalent to:

``` {.c}
isalpha(c) || isdigit(c)
```

### Return Value {.unnumbered .unlisted}

Returns true if a character is alphabetic (`A`-`Z` or `a`-`z`) or a
digit (`0`-`9`).

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
//             testing this char
//                      v
printf("%s\n", isalnum('a')? "yes": "no");  // yes
printf("%s\n", isalnum('B')? "yes": "no");  // yes
printf("%s\n", isalnum('5')? "yes": "no");  // yes
printf("%s\n", isalnum('?')? "yes": "no");  // no
```

### See Also {.unnumbered .unlisted}

[`isalpha()`](#man-isalpha),
[`isdigit()`](#man-isdigit)


[[pagebreak]]
## `isalpha()` {#man-example}

Returns true if a character is alphabetic

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int isalpha(int c);
```

### Description {.unnumbered .unlisted}

Returns true for alphabetic characters (`A`-`Z` or `a`-`z`).

Technically (and in the "C" locale) equivalent to:

``` {.c}
isupper(c) || islower(c)
```

Extra super technically, because I know you're dying for this to be
extra unnecessarily complex, it can also include some locale-specific
characters for which this is true:

``` {.c}
!iscntrl(c) && !isdigit(c) && !ispunct(c) && !isspace(c)
```

and this is true:

``` {.c}
isupper(c) || islower(c)
```

### Return Value {.unnumbered .unlisted}

Returns true for alphabetic characters (`A`-`Z` or `a`-`z`).

Or for any of the other crazy stuff in the description, above.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
//             testing this char
//                      v
printf("%s\n", isalpha('a')? "yes": "no");  // yes
printf("%s\n", isalpha('B')? "yes": "no");  // yes
printf("%s\n", isalpha('5')? "yes": "no");  // no
printf("%s\n", isalpha('?')? "yes": "no");  // no
```

### See Also {.unnumbered .unlisted}

[`isalnum()`](#man-isalnum)

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