<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<locale.h>` locale handling {#locale}

|Function|Description|
|--------|----------------------|
|[`setlocale()`](#man-setlocale)|Set the locale|

The "locale" is the details of how the program should run given its
physical location on the planet.

For example, in one locale, a unit of money might be printed as `$123`,
and in another `€123`.

Or one locale might use ASCII encoding and another UTF-8 encoding.

By default, the program runs in the "C" locale. It has a basic set of
characters with a single-byte encoding. If you try to print UTF-8
characters in the C locale, nothing will print. You have to switch to a
proper locale.

[[manbreak]]
## `setlocale()` {#man-setlocale}

Set the locale

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <locale.h>

char *setlocale(int category, const char *locale);
```

### Description {.unnumbered .unlisted}

Sets the `locale` for the given `category`.

Category is one of the following:

|Category|Description|
|-|-|
|`LC_ALL`|All of the following categories|
|`LC_COLLATE`|Affects the [`strcoll()`](#man-strcoll) and [`strxfrm()`](#man-strxfrm) functions|
|`LC_CTYPE`|Affects the functions in [`<ctype.h>`](#ctype)|
|`LC_MONETARY`|Affects the monetary information returned from [`localeconv()`](#man-localeconv)|
|`LC_NUMERIC`|Affects the decimal point for formatted I/O and formatted string functions, and the monetary information returned from [`localeconv()`](#man-localeconv)|
|`LC_TIME`|Affects the [`strftime()`](#man-strftime) and [`wcsftime()`](#man-wcsftime) functions|

And there are three portable things you can pass in for `locale`; any
other string passed in is implementation-defined and non-portable.

|Locale|Description|
|-|-|
|`"C"`|Set the program to the C locale|
|`""`|(Empty string) Set the program to the native locale of this system|
|`NULL`|Change nothing; just return the current locale|
|Other|Set the program to an implementation-defined locale|

The most common call, I'd wager, is this:

``` {.c}
// Set all locale settings to the local, native locale

setlocale(LC_ALL, "");
```

Handily, `setlocale()` returns the locale that was just set, so you
could see what the actual locale is on your system.

### Return Value {.unnumbered .unlisted}

On success, returns a pointer to the string representing the current
locale. You may not modify this string, and it might be changed by
subsequent calls to `setlocale()`.

On failure, returns `NULL`.

### Example {.unnumbered .unlisted}

Here we get the current locale. Then we set it to the native locale, and
print out what that is.

``` {.c .numberLines}
#include <stdio.h>
#include <locale.h>

int main(void)
{
    char *loc;

    // Get the current locale
    loc = setlocale(LC_ALL, NULL);

    printf("Starting locale: %s\n", loc);

    // Set (and get) the locale to native locale
    loc = setlocale(LC_ALL, "");
    
    printf("Native locale: %s\n", loc);
}
```

Output on my system:

```
Starting locale: C
Native locale: en_US.UTF-8
```

Note that my native locale (on a Linux box) might be different from what
you see.

Nevertheless, I can explicitly set it on my system without a problem, or
to any other locale I have installed:

``` {.c .numberLines startFrom="13"}
    loc = setlocale(LC_ALL, "en_US.UTF-8");   // Non-portable
```

But again, your system might have different locales defined.

### See Also {.unnumbered .unlisted}

[`localeconv()`](#man-localeconv),
[`strcoll()`](#man-strcoll),
[`strxfrm()`](#man-strxfrm),
[`strftime()`](#man-strftime),
[`wcsftime()`](#man-wcsftime),
[`printf()`](#man-printf),
[`scanf()`](#man-scanf),
[`<ctype.h>`](#ctype)

[[manbreak]]
## `localeconv()` {#man-localeconv}

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