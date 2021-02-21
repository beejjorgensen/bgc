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
and (optionally) a decimal point into a floating point value.

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

[`strtod()`](#man-strtod)

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