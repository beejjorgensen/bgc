<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<ctype.h>` Character Classification and Conversion {#ctype}

|Function|Description|
|--------|----------------------|
|[`isalnum()`](#man-isalnum)|Tests if a character is alphabetic or is a digit|
|[`isalpha()`](#man-isalpha)|Returns true if a character is alphabetic|
|[`isblank()`](#man-isblank)|Tests if a character is word-separating whitespace|
|[`iscntrl()`](#man-iscntrl)|Test if a character is a control character|
|[`isdigit()`](#man-isdigit)|Tests if a character is a digit|
|[`isgraph()`](#man-isgraph)|Tests if the character is printable and not a space|
|[`islower()`](#man-islower)|Tests if a character is lowercase|
|[`isprint()`](#man-isprint)|Tests if a character is printable|
|[`ispunct()`](#man-ispunct)|Test if a character is punctuation|
|[`isspace()`](#man-isspace)|Test if a character is whitespace|
|[`isupper()`](#man-isupper)|Tests if a character is uppercase|
|[`isxdigit()`](#man-isxdigit)|Tests if a character is a hexadecimal digit|
|[`tolower()`](#man-tolower)|Convert a letter to lowercase|
|[`toupper()`](#man-toupper)|Convert a letter to uppercase|


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

[[manbreak]]
## `isalnum()` {#man-isalnum}

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
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", isalnum('a')? "yes": "no");  // yes
    printf("%s\n", isalnum('B')? "yes": "no");  // yes
    printf("%s\n", isalnum('5')? "yes": "no");  // yes
    printf("%s\n", isalnum('?')? "yes": "no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`isalpha()`](#man-isalpha),
[`isdigit()`](#man-isdigit)


[[manbreak]]
## `isalpha()` {#man-isalpha}

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
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", isalpha('a')? "yes": "no");  // yes
    printf("%s\n", isalpha('B')? "yes": "no");  // yes
    printf("%s\n", isalpha('5')? "yes": "no");  // no
    printf("%s\n", isalpha('?')? "yes": "no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`isalnum()`](#man-isalnum)

[[manbreak]]
## `isblank()` {#man-isblank}

Tests if a character is word-separating whitespace

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int isblank(int c);
```

### Description {.unnumbered .unlisted}

True if the character is a whitespace character used to separate words
in a single line.

For example, space (`' '`) or horizontal tab (`'\t'`). Other locales
might define other blank characters.

### Return Value {.unnumbered .unlisted}

Returns true if the character is a whitespace character used to separate
words in a single line.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", isblank(' ')? "yes": "no");   // yes
    printf("%s\n", isblank('\t')? "yes": "no");  // yes
    printf("%s\n", isblank('\n')? "yes": "no");  // no
    printf("%s\n", isblank('a')? "yes": "no");   // no
    printf("%s\n", isblank('?')? "yes": "no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`isspace()`](#man-isspace)

[[manbreak]]
## `iscntrl()` {#man-iscntrl}

Test if a character is a control character

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int iscntrl(int c);
```

### Description {.unnumbered .unlisted}

A _control character_ is a locale-specific non-printing character.

For the "C" locale, this means control characters are in the range 0x00
to 0x1F (the character right before SPACE) and 0x7F (the DEL character).

Basically if it's not an ASCII (or Unicode less than 128) printable
character, it's a control character in the "C" locale.
 
### Return Value {.unnumbered .unlisted}

Returns true if `c` is a control character.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", iscntrl('\t')? "yes": "no");  // yes (tab)
    printf("%s\n", iscntrl('\n')? "yes": "no");  // yes (newline)
    printf("%s\n", iscntrl('\r')? "yes": "no");  // yes (return)
    printf("%s\n", iscntrl('\a')? "yes": "no");  // yes (bell)
    printf("%s\n", iscntrl(' ')? "yes": "no");   // no
    printf("%s\n", iscntrl('a')? "yes": "no");   // no
    printf("%s\n", iscntrl('?')? "yes": "no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`isgraph()`](#man-isgraph),
[`isprint()`](#man-isprint)


[[manbreak]]
## `isdigit()` {#man-isdigit}

Tests if a character is a digit

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int isdigit(int c);
```

### Description {.unnumbered .unlisted}

Tests if `c` is a digit in the range `0`-`9`.

### Return Value {.unnumbered .unlisted}

Returns true if the character is a digit, unsurprisingly.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", isdigit('0')? "yes": "no");   // yes
    printf("%s\n", isdigit('5')? "yes": "no");   // yes
    printf("%s\n", isdigit('a')? "yes": "no");   // no
    printf("%s\n", isdigit('B')? "yes": "no");   // no
    printf("%s\n", isdigit('?')? "yes": "no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`isalnum()`](#man-isalnum),
[`isxdigit()`](#man-isxdigit)

[[manbreak]]
## `isgraph()` {#man-isgraph}

Tests if the character is printable and not a space

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int isgraph(int c);
```

### Description {.unnumbered .unlisted}

Tests if `c` is any printable character that isn't a space (`' '`).

### Return Value {.unnumbered .unlisted}

Returns true if `c` is any printable character that isn't a space
(`' '`).

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", isgraph('0')? "yes": "no");   // yes
    printf("%s\n", isgraph('a')? "yes": "no");   // yes
    printf("%s\n", isgraph('B')? "yes": "no");   // yes
    printf("%s\n", isgraph('?')? "yes": "no");   // yes
    printf("%s\n", isgraph(' ')? "yes": "no");   // no
    printf("%s\n", isgraph('\n')? "yes": "no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`iscntrl()`](#man-iscntrl),
[`isprint()`](#man-isprint)

[[manbreak]]
## `islower()` {#man-islower}

Tests if a character is lowercase

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int islower(int c);
```

### Description {.unnumbered .unlisted}

Tests if a character is lowercase, in the range `a`-`z`.

In other locales, there could be other lowercase characters. In all
cases, to be lowercase, the following must be true:

``` {.c}
!iscntrl(c) && !isdigit(c) && !ispunct(c) && !isspace(c)
```

### Return Value {.unnumbered .unlisted}

Returns true if the character is lowercase.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", islower('c')? "yes": "no");   // yes
    printf("%s\n", islower('0')? "yes": "no");   // no
    printf("%s\n", islower('B')? "yes": "no");   // no
    printf("%s\n", islower('?')? "yes": "no");   // no
    printf("%s\n", islower(' ')? "yes": "no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`isupper()`](#man-isupper),
[`isalpha()`](#man-isalpha),
[`toupper()`](#man-toupper),
[`tolower()`](#man-tolower)

[[manbreak]]
## `isprint()` {#man-isprint}

Tests if a character is printable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int isprint(int c);
```

### Description {.unnumbered .unlisted}

Tests if a character is printable, including space (`' '`). So like
`isgraph()`, except space isn't left out in the cold.

### Return Value {.unnumbered .unlisted}

Returns true if the character is printable, including space (`' '`).

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", isprint('c')? "yes": "no");   // yes
    printf("%s\n", isprint('0')? "yes": "no");   // yes
    printf("%s\n", isprint(' ')? "yes": "no");   // yes
    printf("%s\n", isprint('\r')? "yes": "no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`isgraph()`](#man-isgraph),
[`iscntrl()`](#man-iscntrl)

[[manbreak]]
## `ispunct()` {#man-ispunct}

Test if a character is punctuation

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int ispunct(int c);
```

### Description {.unnumbered .unlisted}

Tests if a character is punctuation.

In the "C" locale, this means:

``` {.c}
!isspace(c) && !isalnum(c)
```

In other locales, there could be other punctuation characters (but they
also can't be space or alphanumeric).

### Return Value {.unnumbered .unlisted}

True if the character is punctuation.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", ispunct(',')? "yes": "no");   // yes
    printf("%s\n", ispunct('!')? "yes": "no");   // yes
    printf("%s\n", ispunct('c')? "yes": "no");   // no
    printf("%s\n", ispunct('0')? "yes": "no");   // no
    printf("%s\n", ispunct(' ')? "yes": "no");   // no
    printf("%s\n", ispunct('\n')? "yes": "no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`isspace()`](#man-isspace),
[`isalnum()`](#man-isalnum)

[[manbreak]]
## `isspace()` {#man-isspace}

Test if a character is whitespace

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int isspace(int c);
```

### Description {.unnumbered .unlisted}

Tests if `c` is a whitespace character. These are:

* Space (`' '`)
* Formfeed (`'\f'`)
* Newline (`'\n'`)
* Carriage Return (`'\r'`)
* Horizontal Tab (`'\t'`)
* Vertical Tab (`'\v'`)

Other locales might specify other whitespace characters. `isalnum()` is
false for all whitespace characters.

### Return Value {.unnumbered .unlisted}

True if the character is whitespace.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", isspace(' ')? "yes": "no");   // yes
    printf("%s\n", isspace('\n')? "yes": "no");  // yes
    printf("%s\n", isspace('\t')? "yes": "no");  // yes
    printf("%s\n", isspace(',')? "yes": "no");   // no
    printf("%s\n", isspace('!')? "yes": "no");   // no
    printf("%s\n", isspace('c')? "yes": "no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`isblank()`](#man-isblank)

[[manbreak]]
## `isupper()` {#man-isupper}

Tests if a character is uppercase

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int isupper(int c);
```

### Description {.unnumbered .unlisted}

Tests if a character is uppercase, in the range `A`-`Z`.

In other locales, there could be other uppercase characters. In all
cases, to be uppercase, the following must be true:

``` {.c}
!iscntrl(c) && !isdigit(c) && !ispunct(c) && !isspace(c)
```

### Return Value {.unnumbered .unlisted}

Returns true if the character is uppercase.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", isupper('B')? "yes": "no");   // yes
    printf("%s\n", isupper('c')? "yes": "no");   // no
    printf("%s\n", isupper('0')? "yes": "no");   // no
    printf("%s\n", isupper('?')? "yes": "no");   // no
    printf("%s\n", isupper(' ')? "yes": "no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`islower()`](#man-islower),
[`isalpha()`](#man-isalpha),
[`toupper()`](#man-toupper),
[`tolower()`](#man-tolower)

[[manbreak]]
## `isxdigit()` {#man-isxdigit}

Tests if a character is a hexadecimal digit

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int isxdigit(int c);
```

### Description {.unnumbered .unlisted}

Returns true if the character is a hexadecimal digit. Namely if it's
`0`-`9`, `a`-`f`, or `A`-`F`.

### Return Value {.unnumbered .unlisted}

True if the character is a hexadecimal digit.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             testing this char
    //                      v
    printf("%s\n", isxdigit('B')? "yes": "no");   // yes
    printf("%s\n", isxdigit('c')? "yes": "no");   // yes
    printf("%s\n", isxdigit('2')? "yes": "no");   // yes
    printf("%s\n", isxdigit('G')? "yes": "no");   // no
    printf("%s\n", isxdigit('?')? "yes": "no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`isdigit()`](#man-isdigit)

[[manbreak]]
## `tolower()` {#man-tolower}

Convert a letter to lowercase

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int tolower(int c);
```

### Description {.unnumbered .unlisted}

If the character is uppercase (i.e. `isupper(c)` is true), this function
returns the corresponding lowercase letter.

Different locales might have different upper- and lowercase letters.

### Return Value {.unnumbered .unlisted}

Returns the lowercase value for an uppercase letter. If the letter isn't
uppercase, returns it unchanged.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             changing this char
    //                      v
    printf("%c\n", tolower('B'));  // b (made lowercase!)
    printf("%c\n", tolower('e'));  // e (unchanged)
    printf("%c\n", tolower('!'));  // ! (unchanged)
}
```

### See Also {.unnumbered .unlisted}

[`toupper()`](#man-toupper),
[`islower()`](#man-islower),
[`isupper()`](#man-isupper)

[[manbreak]]
## `toupper()` {#man-toupper}

Convert a letter to uppercase

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <ctype.h>

int toupper(int c);
```

### Description {.unnumbered .unlisted}

If the character is lower (i.e. `islower(c)` is true), this function
returns the corresponding uppercase letter.

Different locales might have different upper- and lowercase letters.

### Return Value {.unnumbered .unlisted}

Returns the uppercase value for a lowercase letter. If the letter isn't
lowercase, returns it unchanged.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             changing this char
    //                      v
    printf("%c\n", toupper('B'));  // B (unchanged)
    printf("%c\n", toupper('e'));  // E (made uppercase!)
    printf("%c\n", toupper('!'));  // ! (unchanged)
}
```

### See Also {.unnumbered .unlisted}

[`tolower()`](#man-tolower),
[`islower()`](#man-islower),
[`isupper()`](#man-isupper)

<!--
[[manbreak]]
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
