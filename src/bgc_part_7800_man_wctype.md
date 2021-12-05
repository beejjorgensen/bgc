<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<wctype.h>` Wide Character Classification and Transformation {#wctype}

|Function|Description|
|--------|----------------------|
|[`iswalnum()`](#man-iswalnum)|Test if a wide character is alphanumeric.|
|[`iswalpha()`](#man-iswalpha)|Tests if a wide character is alphabetic|
|[`iswblank()`](#man-iswblank)|Tests if this is a wide blank character|
|[`iswcntrl()`](#man-iswcntrl)|Tests if this is a wide control character.|
|[`iswctype()`](#man-iswctype)|Determine wide character classification|
|[`iswdigit()`](#man-iswdigit)|Test if this wide character is a digit|
|[`iswgraph()`](#man-iswgraph)|Test to see if a wide character is a printable non-space|
|[`iswlower()`](#man-iswlower)|Tests if a wide character is lowercase|
|[`iswprint()`](#man-iswprint)|Tests if a wide character is printable|
|[`iswpunct()`](#man-iswpunct)|Test if a wide character is punctuation|
|[`iswspace()`](#man-iswspace)|Test if a wide character is whitespace|
|[`iswupper()`](#man-iswupper)|Tests if a wide character is uppercase|
|[`iswxdigit()`](#man-iswxdigit)|Tests if a wide character is a hexadecimal digit|
|[`towctrans()`](#man-towctrans)|Convert wide characters to upper or lowercase|
|[`towlower()`](#man-towlower)|Convert an uppercase wide character to lowercase|
|[`towupper()`](#man-towupper)|Convert a lowercase wide character to uppercase|
|[`wctrans()`](#man-wctrans)|Helper function for `towctrans()`|
|[`wctype()`](#man-wctype)|Helper function for `iswctype()`|

This is like [`<ctype.h>`](#ctype) except for wide characters.

With it you can test for character classifications (like "is this
character whitespace?") or do basic character conversions (like "force
this character to lowercase").

[[manbreak]]
## `iswalnum()` {#man-iswalnum}

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

[[manbreak]]
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


[[manbreak]]
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

[[manbreak]]
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

[[manbreak]]
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

[[manbreak]]
## `iswgraph()` {#man-iswgraph}

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

[[manbreak]]
## `iswlower()` {#man-iswlower}

Tests if a wide character is lowercase

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswlower(wint_t wc);
```

### Description {.unnumbered .unlisted}

Tests if a character is lowercase, in the range `a`-`z`.

In other locales, there could be other lowercase characters. In all
cases, to be lowercase, the following must be true:

``` {.c}
!iswcntrl(c) && !iswdigit(c) && !iswpunct(c) && !iswspace(c)
```

### Return Value {.unnumbered .unlisted}

Returns true if the wide character is lowercase.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                   testing this char
    //                           v
    wprintf(L"%ls\n", iswlower(L'c')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswlower(L'0')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswlower(L'B')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswlower(L'?')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswlower(L' ')? L"yes": L"no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`islower()`](#man-islower),
[`iswupper()`](#man-isupper),
[`iswalpha()`](#man-isalpha),
[`towupper()`](#man-toupper),
[`towlower()`](#man-tolower)

[[manbreak]]
## `iswprint()` {#man-iswprint}

Tests if a wide character is printable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswprint(wint_t wc);
```

### Description {.unnumbered .unlisted}

Tests if a wide character is printable, including space (`' '`). So like
`isgraph()`, except space isn't left out in the cold.

### Return Value {.unnumbered .unlisted}

Returns true if the wide character is printable, including space (`' '`).

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                   testing this char
    //                           v
    wprintf(L"%ls\n", iswprint(L'c')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswprint(L'0')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswprint(L' ')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswprint(L'\r')? L"yes": L"no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`isprint()`](#man-isprint),
[`iswgraph()`](#man-iswgraph),
[`iswcntrl()`](#man-iswcntrl)

[[manbreak]]
## `iswpunct()` {#man-iswpunct}

Test if a wide character is punctuation

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswpunct(wint_t wc);
```

### Description {.unnumbered .unlisted}

Tests if a wide character is punctuation.

This means for any given locale:

``` {.c}
!isspace(c) && !isalnum(c)
```

### Return Value {.unnumbered .unlisted}

True if the wide character is punctuation.

### Example {.unnumbered .unlisted}

Results may vary based on locale.

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                   testing this char
    //                           v
    wprintf(L"%ls\n", iswpunct(L',')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswpunct(L'!')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswpunct(L'c')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswpunct(L'0')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswpunct(L' ')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswpunct(L'\n')? L"yes": L"no");  // no
}
```

### See Also {.unnumbered .unlisted}

[`ispunct()`](#man-ispunct),
[`iswspace()`](#man-iswspace),
[`iswalnum()`](#man-iswalnum)

[[manbreak]]
## `iswspace()` {#man-iswspace}

Test if a wide character is whitespace

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswspace(wint_t wc);
```

### Description {.unnumbered .unlisted}

Tests if `c` is a whitespace character. These are probably:

* Space (`' '`)
* Formfeed (`'\f'`)
* Newline (`'\n'`)
* Carriage Return (`'\r'`)
* Horizontal Tab (`'\t'`)
* Vertical Tab (`'\v'`)

Other locales might specify other whitespace characters. `iswalnum()`,
`iswgraph()`, and `iswpunct()` are all false for all whitespace
characters.

### Return Value {.unnumbered .unlisted}

True if the character is whitespace.

### Example {.unnumbered .unlisted}

Results may vary based on locale.

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                   testing this char
    //                           v
    wprintf(L"%ls\n", iswspace(L' ')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswspace(L'\n')? L"yes": L"no");  // yes
    wprintf(L"%ls\n", iswspace(L'\t')? L"yes": L"no");  // yes
    wprintf(L"%ls\n", iswspace(L',')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswspace(L'!')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswspace(L'c')? L"yes": L"no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`isspace()`](#man-isspace),
[`iswblank()`](#man-iswblank)

[[manbreak]]
## `iswupper()` {#man-iswupper}

Tests if a wide character is uppercase

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswupper(wint_t wc);
```

### Description {.unnumbered .unlisted}

Tests if a character is uppercase in the current locale.

To be uppercase, the following must be true:

``` {.c}
!iscntrl(c) && !isdigit(c) && !ispunct(c) && !isspace(c)
```

### Return Value {.unnumbered .unlisted}

Returns true if the wide character is uppercase.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                   testing this char
    //                           v
    wprintf(L"%ls\n", iswupper(L'B')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswupper(L'c')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswupper(L'0')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswupper(L'?')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswupper(L' ')? L"yes": L"no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`isupper()`](#man-isupper),
[`iswlower()`](#man-iswlower),
[`iswalpha()`](#man-iswalpha),
[`towupper()`](#man-towupper),
[`towlower()`](#man-towlower)

[[manbreak]]
## `iswxdigit()` {#man-iswxdigit}

Tests if a wide character is a hexadecimal digit

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswxdigit(wint_t wc);
```

### Description {.unnumbered .unlisted}

Returns true if the wide character is a hexadecimal digit. Namely if
it's `0`-`9`, `a`-`f`, or `A`-`F`.

### Return Value {.unnumbered .unlisted}

True if the character is a hexadecimal digit.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                    testing this char
    //                            v
    wprintf(L"%ls\n", iswxdigit(L'B')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswxdigit(L'c')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswxdigit(L'2')? L"yes": L"no");   // yes
    wprintf(L"%ls\n", iswxdigit(L'G')? L"yes": L"no");   // no
    wprintf(L"%ls\n", iswxdigit(L'?')? L"yes": L"no");   // no
}
```

### See Also {.unnumbered .unlisted}

[`isxdigit()`](#man-isxdigit),
[`iswdigit()`](#man-iswdigit)

[[manbreak]]
## `iswctype()` {#man-iswctype}

Determine wide character classification

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

int iswctype(wint_t wc, wctype_t desc);
```

### Description {.unnumbered .unlisted}

This is the Swiss Army knife of classification functions; it's all the
other ones rolled into one.

You call it with something like this:

``` {.c}
if (iswctype(c, wctype("digit")))  // or "alpha" or "space" or...
```

and it behaves just like you'd called:

``` {.c}
if (iswdigit(c))
```

The difference is that you can specify the type of matching you want to
do as a string at runtime, which might be convenient.

`iswctype()` relies on the return value from the
[`wctype()`](#man-wctype) call to get its work done.

Stolen from the spec, here are the `iswctype()` calls and their
equivalents:

|`iswctype()` call|Hard-coded equivalent|
|-|-|
|`iswctype(c, wctype("alnum"))`|`iswalnum(c)`|
|`iswctype(c, wctype("alpha"))`|`iswalpha(c)`|
|`iswctype(c, wctype("blank"))`|`iswblank(c)`|
|`iswctype(c, wctype("cntrl"))`|`iswcntrl(c)`|
|`iswctype(c, wctype("digit"))`|`iswdigit(c)`|
|`iswctype(c, wctype("graph"))`|`iswgraph(c)`|
|`iswctype(c, wctype("lower"))`|`iswlower(c)`|
|`iswctype(c, wctype("print"))`|`iswprint(c)`|
|`iswctype(c, wctype("punct"))`|`iswpunct(c)`|
|`iswctype(c, wctype("space"))`|`iswspace(c)`|
|`iswctype(c, wctype("upper"))`|`iswupper(c)`|
|`iswctype(c, wctype("xdigit"))`|`iswxdigit(c)`|

See the [`wctype()` documentation](#man-wctype) for how that helper
function works.

### Return Value {.unnumbered .unlisted}

Returns true if the wide character `wc` matches the character class in
`desc`.

### Example {.unnumbered .unlisted}

Test for a given character classification at when the classification
isn't known at compile time:

``` {.c .numberLines}
#include <stdio.h>  // for fflush(stdout)
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    wchar_t c;        // Holds a single wide character (to test)
    char desc[128];   // Holds the character class

    // Get the character and classification from the user
    wprintf(L"Enter a character and character class: ");
    fflush(stdout);
    wscanf(L"%lc %s", &c, desc);

    // Compute the type from the given class
    wctype_t t = wctype(desc);

    if (t == 0)
        // If the type is 0, it's an unknown class
        wprintf(L"Unknown character class: \"%s\"\n", desc);
    else {
        // Otherwise, let's test the character and see if its that
        // classification
        if (iswctype(c, t))
            wprintf(L"Yes! '%lc' is %s!\n", c, desc);
        else
            wprintf(L"Nope! '%lc' is not %s.\n", c, desc);
    }
}
```

Output:

``` {.default}
Enter a character and character class: 5 digit
Yes! '5' is digit!

Enter a character and character class: b digit
Nope! 'b' is not digit.

Enter a character and character class: x alnum
Yes! 'x' is alnum!
```

### See Also {.unnumbered .unlisted}

[`wctype()`](#man-wctype)

[[manbreak]]
## `wctype()` {#man-wctype}

Helper function for `iswctype()`

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

wctype_t wctype(const char *property);
```

### Description {.unnumbered .unlisted}

This function returns an opaque value for the given `property` that is
meant to be passed as the second argument to
[`iswctype()`](#man-iswctype).

The returned value is of type `wctype_t`.

Valid properties in all locales are:

``` {.c}
"alnum"   "alpha"   "blank"   "cntrl"
"digit"   "graph"   "lower"   "print"
"punct"   "space"   "upper"   "xdigit"
```

Other properties might be defined as determined by the `LC_CTYPE`
category of the current locale.

See the [`iswctype()` reference page](#man-iswctype) for more usage
details.

### Return Value {.unnumbered .unlisted}

Returns the `wctype_t` value associated with the given `property`.

If an invalid value is passed for `property`, returns `0`.

### Example {.unnumbered .unlisted}

Test for a given character classification at when the classification
isn't known at compile time:

``` {.c .numberLines}
#include <stdio.h>  // for fflush(stdout)
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    wchar_t c;        // Holds a single wide character (to test)
    char desc[128];   // Holds the character class

    // Get the character and classification from the user
    wprintf(L"Enter a character and character class: ");
    fflush(stdout);
    wscanf(L"%lc %s", &c, desc);

    // Compute the type from the given class
    wctype_t t = wctype(desc);

    if (t == 0)
        // If the type is 0, it's an unknown class
        wprintf(L"Unknown character class: \"%s\"\n", desc);
    else {
        // Otherwise, let's test the character and see if its that
        // classification
        if (iswctype(c, t))
            wprintf(L"Yes! '%lc' is %s!\n", c, desc);
        else
            wprintf(L"Nope! '%lc' is not %s.\n", c, desc);
    }
}
```

Output:

``` {.default}
Enter a character and character class: 5 digit
Yes! '5' is digit!

Enter a character and character class: b digit
Nope! 'b' is not digit.

Enter a character and character class: x alnum
Yes! 'x' is alnum!
```

### See Also {.unnumbered .unlisted}

[`iswctype()`](#man-iswctype)

[[manbreak]]
## `towlower()` {#man-towlower}

Convert an uppercase wide character to lowercase

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

wint_t towlower(wint_t wc);
```

### Description {.unnumbered .unlisted}

If the character is upper (i.e. `iswupper(c)` is true), this function
returns the corresponding lowercase letter.

Different locales might have different upper and lowercase letters.

### Return Value {.unnumbered .unlisted}

If the letter `wc` is uppercase, a lowercase version of that letter will
be returned according to the current locale.

If the letter is not uppercase, `wc` is returned unchanged.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                  changing this char
    //                           v
    wprintf(L"%lc\n", towlower(L'B'));  // b (made lowercase!)
    wprintf(L"%lc\n", towlower(L'e'));  // e (unchanged)
    wprintf(L"%lc\n", towlower(L'!'));  // ! (unchanged)
}
```

### See Also {.unnumbered .unlisted}

[`tolower()`](#man-tolower),
[`towupper()`](#man-towupper),
[`iswlower()`](#man-iswlower),
[`iswupper()`](#man-iswupper)

[[manbreak]]
## `towupper()` {#man-towupper}

Convert a lowercase wide character to uppercase

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

wint_t towupper(wint_t wc);
```

### Description {.unnumbered .unlisted}

If the character is upper (i.e. `iswupper(c)` is true), this function
returns the corresponding lowercase letter.

Different locales might have different upper and lowercase letters.

### Return Value {.unnumbered .unlisted}

If the letter `wc` is lowercase, an uppercase version of that letter
will be returned according to the current locale.

If the letter is not lowercase, `wc` is returned unchanged.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    //                  changing this char
    //                           v
    wprintf(L"%lc\n", towupper(L'B'));  // B (unchanged)
    wprintf(L"%lc\n", towupper(L'e'));  // E (made uppercase!)
    wprintf(L"%lc\n", towupper(L'!'));  // ! (unchanged)
}
```

### See Also {.unnumbered .unlisted}

[`toupper()`](#man-toupper),
[`towlower()`](#man-towlower),
[`iswlower()`](#man-iswlower),
[`iswupper()`](#man-iswupper)

[[manbreak]]
## `towctrans()` {#man-towctrans}

Convert wide characters to upper or lowercase

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

wint_t towctrans(wint_t wc, wctrans_t desc);
```

### Description {.unnumbered .unlisted}

This is the Swiss Army knife of character conversion functions; it's all
the other ones rolled into one. And by "all the other ones" I mean
`towupper()` and `towlower()`, since those are the only ones there are.

You call it with something like this:

``` {.c}
if (towctrans(c, wctrans("toupper")))  // or "tolower"
```

and it behaves just like you'd called:

``` {.c}
towupper(c);
```

The difference is that you can specify the type of conversion you want
to do as a string at runtime, which might be convenient.

`towctrans()` relies on the return value from the
[`wctrans()`](#man-wctrans) call to get its work done.

|`towctrans()` call|Hard-coded equivalent|
|-|-|
|`towctrans(c, wctrans("toupper"))`|`towupper(c)`|
|`towctrans(c, wctrans("tolower"))`|`towlower(c)`|

See the [`wctrans()` documentation](#man-wctrans) for how that helper
function works.

### Return Value {.unnumbered .unlisted}

Returns the character `wc` as if run through `towupper()` or
`towlower()`, depending on the value of `desc`.

If the character already matches the classification, it is returned
as-is.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>  // for fflush(stdout)
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    wchar_t c;        // Holds a single wide character (to test)
    char desc[128];   // Holds the conversion type

    // Get the character and conversion type from the user
    wprintf(L"Enter a character and conversion type: ");
    fflush(stdout);
    wscanf(L"%lc %s", &c, desc);

    // Compute the type from the given conversion type
    wctrans_t t = wctrans(desc);

    if (t == 0)
        // If the type is 0, it's an unknown conversion type
        wprintf(L"Unknown conversion: \"%s\"\n", desc);
    else {
        // Otherwise, let's do the conversion
        wint_t result = towctrans(c, t);
        wprintf(L"'%lc' -> %s -> '%lc'\n", c, desc, result);
    }
}
```

Output on my system:

``` {.default}
Enter a character and conversion type: b toupper
'b' -> toupper -> 'B'

Enter a character and conversion type: B toupper
'B' -> toupper -> 'B'

Enter a character and conversion type: B tolower
'B' -> tolower -> 'b'

Enter a character and conversion type: ! toupper
'!' -> toupper -> '!'
```

### See Also {.unnumbered .unlisted}

[`wctrans()`](#man-wctrans),
[`towupper()`](#man-towupper),
[`towlower()`](#man-towlower)

[[manbreak]]
## `wctrans()` {#man-wctrans}

Helper function for `towctrans()`

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wctype.h>

wctrans_t wctrans(const char *property);
```

### Description {.unnumbered .unlisted}

This is a helper function for generating the second argument to
[`towctrans()`](#man-towctrans).

You can pass in one of two things for the `property`:

* `toupper` to make `towctrans()` behave like `towupper()`
* `tolower` to make `towctrans()` behave like `towlower()`

### Return Value {.unnumbered .unlisted}

On success, returns a value that can be used as the `desc` argument to
`towctrans()`.

Otherwise, if the `property` isn't recognized, returns `0`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>  // for fflush(stdout)
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    wchar_t c;        // Holds a single wide character (to test)
    char desc[128];   // Holds the conversion type

    // Get the character and conversion type from the user
    wprintf(L"Enter a character and conversion type: ");
    fflush(stdout);
    wscanf(L"%lc %s", &c, desc);

    // Compute the type from the given conversion type
    wctrans_t t = wctrans(desc);

    if (t == 0)
        // If the type is 0, it's an unknown conversion type
        wprintf(L"Unknown conversion: \"%s\"\n", desc);
    else {
        // Otherwise, let's do the conversion
        wint_t result = towctrans(c, t);
        wprintf(L"'%lc' -> %s -> '%lc'\n", c, desc, result);
    }
}
```

Output on my system:

``` {.default}
Enter a character and conversion type: b toupper
'b' -> toupper -> 'B'

Enter a character and conversion type: B toupper
'B' -> toupper -> 'B'

Enter a character and conversion type: B tolower
'B' -> tolower -> 'b'

Enter a character and conversion type: ! toupper
'!' -> toupper -> '!'
```

### See Also {.unnumbered .unlisted}

[`towctrans()`](#man-towctrans)

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