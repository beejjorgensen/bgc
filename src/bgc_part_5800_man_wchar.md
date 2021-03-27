<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<wchar.h>` Wide Character Handling {#wchar}

These are the wide character variants of the functions found in
[`<stdio.h>`](#man-stdio).

Remember that you can't mix-and-match multibyte output functions (like
[`printf()`](#man-printf)) with wide character output functions (like
[`wprintf()`](#man-wprintf)). The output stream has an
[_orientation_](#io-stream-orientation) to either multibyte or wide that
gets set on the first I/O call to that stream. (Or it can be set with
[`fwide()`](#man-fwide).)

So choose one or the other and stick with it.

And you can specify wide character constants and string literals by
prefixing `L` to the front of it:

``` {.c}
wchar_t *s = L"Hello, world!";
wchar_t c = L'B';
```

[[pagebreak]]
## `wprintf()`, `fwprintf()`, `swprintf()` {#man-wprintf}

Formatted output with a wide string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>
#include <wchar.h>

int wprintf(const wchar_t * restrict format, ...);

int fwprintf(FILE * restrict stream, const wchar_t * restrict format, ...);

int swprintf(wchar_t * restrict s, size_t n,
             const wchar_t * restrict format, ...);
```

### Description {.unnumbered .unlisted}

These are the wide versions of [`printf()`](#man-printf),
[`fprintf()](#man-printf), and [`sprintf()`](#man-printf).

See those pages for exact substantial usage.

These are the same except the `format` string is a wide character
string instead of a multibyte string.

And one more thing: the precision specified for a `%s` specifier
corresponds to the number of wide characters printed, not the number of
bytes. If you know of other difference, let me know.

### Return Value {.unnumbered .unlisted}

Returns the number of wide characters outputted, or `-1` if there's an
error.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    char *mbs = "multibyte";
    wchar_t *ws = L"wide";

    wprintf(L"We're all wide for %s and %ls!\n", mbs, ws);

    double pi = 3.14159265358979;
    wprintf(L"pi = %f\n", pi);
}
```

Output:

```
We're all wide for multibyte and wide!
pi = 3.141593
```

### See Also {.unnumbered .unlisted}

[`printf()`](#man-printf),
[`fprintf()`](#man-printf),
[`sprintf()`](#man-printf),
[`snprintf()`](#man-printf)

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
