<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<errno.h>` Error Information {#errno}
|Variable|Description|
|-|-|
|[`errno`](#man-errno)|Holds the error status of the last call|

This header defines a single variable^[Really it's just required to be
a modifiable lvalue, so not necessarily a variable. But you can treat it
as such.], `errno`, that can be checked to see if an error has occurred.

`errno` is set to `0` on startup, but no library function sets it to
`0`. If you're going to use solely it to check for errors, set it to `0`
before the call and then check it after. Not only that, but if there's
no error, all library functions will leave the value of `errno`
unchanged.

Often, though, you'll get some error indication from the function you're
calling then check `errno` to see what went wrong.

This is commonly used in conjunction with [`perror()`](#man-perror) to
get a human-readable error message that corresponds to the specific
error.

Important Safety Tip: You should never make your own variable called
`errno`---that's undefined behavior.

Note that the C Spec defines less than a handful of values `errno` can
take on. [fl[Unix defines a bunch
more|https://man.archlinux.org/man/errno.3.en]], [fl[as does
Windows|https://docs.microsoft.com/en-us/cpp/c-runtime-library/errno-constants?view=msvc-160]].

[[manbreak]]
## `errno` {#man-errno}

Holds the error status of the last call

### Synopsis {.unnumbered .unlisted}

``` {.c}
errno   // Type is undefined, but it's assignable
```

### Description {.unnumbered .unlisted}

Indicates the error status of the last call (note that not all calls
will set this value).

|Value|Description|
|-|-|
|`0`|No error|
|`EDOM`|Domain error (from math)|
|`EILSEQ`|Encoding error (from character conversion)|
|`ERANGE`|Range error (from math)|

If you're doing a number of math functions, you might come across `EDOM`
or `ERANGE`.

With multibyte/wide character conversion functions, you might see
`EILSEQ`.

And your system might define any other number of values that `errno`
could be set to, all of which will begin with the letter `E`.

Fun Fact: you can use `EDOM`, `EILSEQ`, and `ERANGE` with preprocessor
directives such as `#ifdef`. But, frankly, I'm not sure why you'd do
that other than to test their existence.

<!--
### Return Value {.unnumbered .unlisted}
-->

### Example {.unnumbered .unlisted}

The following prints an error message, since passing `2.0` to `acos()`
is outside the function's domain.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <errno.h>

int main(void)
{
    double x;

    errno = 0;       // Make sure this is clear before the call

    x = acos(2.0);   // Invalid argument to acos()

    if (errno == EDOM)
        perror("acos");
    else
        printf("Answer is %f\n", x);

    return 0;
}
```

Output:

``` {.default}
acos: Numerical argument out of domain
```

The following prints an error message (on my system), since passing
`1e+30` to `exp()` produces a result that's outside the range of a
`double`.

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>
#include <errno.h>

int main(void)
{
    double x;

    errno = 0;       // Make sure this is clear before the call

    x = exp(1e+30);  // Pass in some too-huge number

    if (errno == ERANGE)
        perror("exp");
    else
        printf("Answer is %f\n", x);

    return 0;
}
```

Output:

``` {.default}
exp: Numerical result out of range
```

This example tries to convert an invalid character into a wide
character, failing. This sets `errno` to `EILSEQ`. We then use
`perror()` to print an error message.

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

int main(void)
{
	setlocale(LC_ALL, "");

    char *bad_str = "\xff";  // Probably invalid char in this locale
    wchar_t wc;
    size_t result;
    mbstate_t ps;

    memset(&ps, 0, sizeof ps);

    result = mbrtowc(&wc, bad_str, 1, &ps);

    if (result == (size_t)(-1))
        perror("mbrtowc");  // mbrtowc: Illegal byte sequence
    else
        printf("Converted to L'%lc'\n", wc);

    return 0;
}
```

Output:

``` {.default}
mbrtowc: Invalid or incomplete multibyte or wide character
```

### See Also {.unnumbered .unlisted}

[`perror()`](#man-perror),
[`mbrtoc16()`](#man-mbrtoc16),
[`c16rtomb()`](#man-c16rtomb),
[`mbrtoc32()`](#man-mbrtoc16),
[`c32rtomb()`](#man-c16rtomb),
[`fgetwc()`](#man-getwc),
[`fputwc()`](#man-putwc),
[`mbrtowc()`](#man-mbrtowc),
[`wcrtomb()`](#man-wcrtomb),
[`mbsrtowcs()`](#man-mbsrtowcs),
[`wcsrtombs()`](#man-wcsrtombs),
[`<math.h>`](#math),
