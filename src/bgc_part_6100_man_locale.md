<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<locale.h>` locale handling {#locale}

|Function|Description|
|--------|----------------------|
|[`setlocale()`](#man-setlocale)|Set the locale|
|[`localeconv()`](#man-localeconv)|Get information about the current locale|

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

``` {.default}
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

Get information about the current locale

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <locale.h>

struct lconv *localeconv(void);
```

### Description {.unnumbered .unlisted}

This function just returns a pointer to a `struct lconv`, but is still
a bit of a powerhouse.

The returned structure contains _tons_ of information about the locale.
Here are the fields of `struct lconv` and their meanings.

First, some conventions. An `_p_` means "positive", and `_n_` means
"negative", and `int_` means "international". Though a lot of these are
type `char` or `char*`, most (or the strings they point to) are actually
treated as integers^[Remember that `char` is just a byte-sized
integer.].

Before we go further, know that `CHAR_MAX` (from `<limits.h>`) is the
maximum value that can be held in a `char`. And that many of the
following `char` values use that to indicate the value isn't available
in the given locale.

|Field|Description|
|-----|-----------|
|`char *mon_decimal_point`|Decimal pointer character for money, e.g. `"."`.|
|`char *mon_thousands_sep`|Thousands separator character for money, e.g. `","`.|
|`char *mon_grouping`|Grouping description for money (see below).|
|`char *positive_sign`|Positive sign for money, e.g. `"+"` or `""`.|
|`char *negative_sign`|Negative sign for money, e.g. `"-"`.|
|`char *currency_symbol`|Currency symbol, e.g. `"$"`.|
|`char frac_digits`|When printing monetary amounts, how many digits to print past the decimal point, e.g. `2`.|
|`char p_cs_precedes`|`1` if the `currency_symbol` comes before the value for a non-negative monetary amount, `0` if after.|
|`char n_cs_precedes`|`1` if the `currency_symbol` comes before the value for a negative monetary amount, `0` if after.|
|`char p_sep_by_space`|Determines the separation of the `currency symbol` from the value for non-negative amounts (see below).|
|`char n_sep_by_space`|Determines the separation of the `currency symbol` from the value for negative amounts (see below).|
|`char p_sign_posn`|Determines the `positive_sign` position for non-negative values.|
|`char p_sign_posn`|Determines the `positive_sign` position for negative values.|
|`char *int_curr_symbol`|International currency symbol, e.g. `"USD "`.|
|`char int_frac_digits`|International value for `frac_digits`.|
|`char int_p_cs_precedes`|International value for `p_cs_precedes`.|
|`char int_n_cs_precedes`|International value for `n_cs_precedes`.|
|`char int_p_sep_by_space`|International value for `p_sep_by_space`.|
|`char int_n_sep_by_space`|International value for `n_sep_by_space`.|
|`char int_p_sign_posn`|International value for `p_sign_posn`.|
|`char int_n_sign_posn`|International value for `n_sign_posn`.|

Even though many of these have `char` type, the value stored within is
meant to be accessed as an integer.

All the `sep_by_space` variants deal with spacing around the currency
sign. Valid values are:

|Value|Description|
|:--:|------------|
|`0`|No space between currency symbol and value.|
|`1`|Separate the currency symbol (and sign, if any) from the value with a space.|
|`2`|Separate the sign symbol from the currency symbol (if adjacent) with a space, otherwise separate the sign symbol from the value with a space.|

The `sign_posn` variants are determined by the following values:

|Value|Description|
|:--:|------------|
|`0`|Put parens around the value and the currency symbol.|
|`1`|Put the sign string in front of the currency symbol and value.|
|`2`|Put the sign string after the currency symbol and value.|
|`3`|Put the sign string directly in front of the currency symbol.|
|`4`|Put the sign string directly behind the currency symbol.|

For more information on the `mon_grouping` field, see ["Monetary Digit
Grouping" in the "Locale and Internationalization"
chapter](#monetary-digit-grouping).

### Return Value {.unnumbered .unlisted}

Returns a pointer to the structure containing the locale information.

The program may not modify this structure.

Subsequent calls to `localeconv()` may overwrite this structure, as
might calls to `setlocale()` with `LC_ALL`, `LC_MONETARY`, or
`LC_NUMERIC`.

### Example {.unnumbered .unlisted}

Here's a program to print the locale information for the native locale.

``` {.c .numberLines}
#include <stdio.h>
#include <locale.h>
#include <limits.h>  // for CHAR_MAX

void print_grouping(char *mg)
{
    int done = 0;

    while (!done) {
        if (*mg == CHAR_MAX)
            printf("CHAR_MAX ");
        else
            printf("%c ", *mg + '0');
        done = *mg == CHAR_MAX || *mg == 0;
        mg++;
    }
}

int main(void)
{
    setlocale(LC_ALL, "");

    struct lconv *lc = localeconv();

    printf("mon_decimal_point : %s\n", lc->mon_decimal_point);
    printf("mon_thousands_sep : %s\n", lc->mon_thousands_sep);
    printf("mon_grouping      : ");
    print_grouping(lc->mon_grouping);
    printf("\n");
    printf("positive_sign     : %s\n", lc->positive_sign);
    printf("negative_sign     : %s\n", lc->negative_sign);
    printf("currency_symbol   : %s\n", lc->currency_symbol);
    printf("frac_digits       : %c\n", lc->frac_digits);
    printf("p_cs_precedes     : %c\n", lc->p_cs_precedes);
    printf("n_cs_precedes     : %c\n", lc->n_cs_precedes);
    printf("p_sep_by_space    : %c\n", lc->p_sep_by_space);
    printf("n_sep_by_space    : %c\n", lc->n_sep_by_space);
    printf("p_sign_posn       : %c\n", lc->p_sign_posn);
    printf("p_sign_posn       : %c\n", lc->p_sign_posn);
    printf("int_curr_symbol   : %s\n", lc->int_curr_symbol);
    printf("int_frac_digits   : %c\n", lc->int_frac_digits);
    printf("int_p_cs_precedes : %c\n", lc->int_p_cs_precedes);
    printf("int_n_cs_precedes : %c\n", lc->int_n_cs_precedes);
    printf("int_p_sep_by_space: %c\n", lc->int_p_sep_by_space);
    printf("int_n_sep_by_space: %c\n", lc->int_n_sep_by_space);
    printf("int_p_sign_posn   : %c\n", lc->int_p_sign_posn);
    printf("int_n_sign_posn   : %c\n", lc->int_n_sign_posn);
}
```

Output on my system:

``` {.default}
mon_decimal_point : .
mon_thousands_sep : ,
mon_grouping      : 3 3 0 
positive_sign     : 
negative_sign     : -
currency_symbol   : $
frac_digits       : 2
p_cs_precedes     : 1
n_cs_precedes     : 1
p_sep_by_space    : 0
n_sep_by_space    : 0
p_sign_posn       : 1
p_sign_posn       : 1
int_curr_symbol   : USD 
int_frac_digits   : 2
int_p_cs_precedes : 1
int_n_cs_precedes : 1
int_p_sep_by_space: 1
int_n_sep_by_space: 1
int_p_sign_posn   : 1
int_n_sign_posn   : 1
```

### See Also {.unnumbered .unlisted}

[`setlocale()`](#man-setlocale)

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