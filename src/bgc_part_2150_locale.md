<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Locale and Internationalization

_Localization_ is the process of making your app ready to work well in
different locales (or countries).

As you might know, not everyone uses the same character for decimal
points or for thousands separators... or for currency.

These locales have names, and you can select one to use. For example, a
US locale might write a number like:

100,000.00

Whereas in Brazil, the same might be written with the commas and decimal
points swapped:

100.000,00

Makes it easier to write your code so it ports to other nationalities
with ease!

Well, sort of. Turns out C only has one built-in locale, and it's
limited. The spec really leaves a lot of ambiguity here; it's hard to be
completely portable.

But we'll do our best!

## Setting the Localization, Quick and Dirty

For these calls, include `<locale.h>`.

There is basically one thing you can portably do here in terms of
declaring a specific locale. This is likely what you want to do if
you're going to do locale anything:

``` {.c}
set_locale(LC_ALL, "");  // Use this environment's locale for everything
```

You'll want to call that so that the program gets initialized with your
current locale.

Getting into more details, there is one more thing you can do and stay
portable:

``` {.c}
set_locale(LC_ALL, "C");  // Use the default C locale
```

but that's called by default every time your program starts, so there's
not much need to do it yourself.

In that second string, you can specify any locale supported by your
system. This is completely system-dependent, so it will vary. On my
system, I can specify this:

``` {.c}
setlocale(LC_ALL, "en_US.UTF-8");  // Non-portable!
```

And that'll work. But it's only portable to systems which have that
exact same name for that exact same locale, and you can't guarantee it.

By passing in an empty string (`""`) for the second argument, you're
telling C, "Hey, figure out what the current locale on this system is so
I don't have to tell you."

## Getting the Monetary Locale Settings

Because moving green pieces of paper around promises to be the key to
happiness^["This planet has---or rather had---a problem, which was this:
most of the people living on it were unhappy for pretty much of the
time. Many solutions were suggested for this problem, but most of these
were largely concerned with the movement of small green pieces of paper,
which was odd because on the whole it wasn't the small green pieces of
paper that were unhappy." ---The Hitchhiker's Guide to the Galaxy,
Douglas Adams], let's talk about monetary locale. When you're writing
portable code, you have to know what to type for cash, right? Whether
that's "$", "€", "¥", "£", or "₹".

How can you write that code without going insane? Luckily, once you
call `setlocale(LC_ALL, "")`, you can just look these up with a call:

``` {.c}
struct lconv *x = localconv();
```

This function returns a pointer to a statically-allocated `struct lconv`
that has all that juicy information you're looking for.

Here are the fields of `struct lconv` and their meanings.

"negative", and `int_` means "international". Though a lot of these are
type `char` or `char*`, most (or the strings they point to) are actually
treated as integers^[Remember that `char` is just a byte-sized
integer.].

|Field|Description|
|-|--------------|
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

### Monetary Digit Grouping

Before we start, know that `CHAR_MAX` (from `<limits.h>`) is the maximum
value that can be held in a `char`.

OK, this is a trippy one. `mon_grouping` is a `char*`, so you might be
thinking it's a string. But in this case, no, it's really an array of
`char`s.

These values describe how to group sets of numbers in currency to the
_left_ of the decimal (the whole number part).

For example, we might have:

```
  2   1   0
 --- --- ---
$100,000,000.00
```

These are groups of three. Group 0 (just left of the decimal) has 3
digits. Group 1 (next group to the left) has 3 digits, and the last one
also has 3.

So we could describe these groups, from the right (the decimal) to the
left with a bunch of integer values representing the group sizes:

```
3 3 3
```

And that would work for values up to $100,000,000.

But what if we had more? We could keep adding `3`s...

```
3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3
```

but that's crazy. Luckily, we can specify `0` to indicate that the
previous group size repeats:

```
3 0
```

Which means to repeat every 3. That would handle $100, $1,000, $10,000,
$10,000,000, $100,000,000,000, and so on.

You can go legitimately crazy with these to indicate some weird
groupings.

For example:

```
4 3 2 1 0
```

would indicate:

```
$1,0,0,0,0,00,000,0000.00
```

One more value that can occur is `CHAR_MAX`. This indicates that no more
grouping should occur, and can appear anywhere in the array, including
the first value.

```
3 2 CHAR_MAX
```

would indicate:

```
100000000,00,000.00
```

for example.

### Separators and Sign Position

### Example Values

When I get the values on my system, this is what I see (grouping string
displayed as individual byte values):

```
mon_decimal_point  = "."
mon_thousands_sep  = ","
mon_grouping       = 3 3 0
positive_sign      = ""
negative_sign      = "-"
currency_symbol    = "$"
frac_digits        = 2
p_cs_precedes      = 1
n_cs_precedes      = 1
p_sep_by_space     = 0
n_sep_by_space     = 0
p_sign_posn        = 1
n_sign_posn        = 1
int_curr_symbol    = "USD "
int_frac_digits    = 2
int_p_cs_precedes  = 1
int_n_cs_precedes  = 1
int_p_sep_by_space = 1
int_n_sep_by_space = 1
int_p_sign_posn    = 1
int_n_sign_posn    = 1
```

LC_ALL
LC_COLLATE
LC_CTYPE
LC_MONETARY
LC_NUMERIC
LC_TIME

char *decimal_point; // "."
char *thousands_sep; // ""
char *grouping; // ""
char *mon_decimal_point; // ""
char *mon_thousands_sep; // ""
char *mon_grouping; // ""
char *positive_sign; // ""
char *negative_sign; // ""
char *currency_symbol; // ""
char frac_digits; // CHAR_MAX
char p_cs_precedes; // CHAR_MAX
char n_cs_precedes; // CHAR_MAX
char p_sep_by_space; // CHAR_MAX
char n_sep_by_space; // CHAR_MAX
char p_sign_posn; // CHAR_MAX
char n_sign_posn; // CHAR_MAX
char *int_curr_symbol; // ""
char int_frac_digits; // CHAR_MAX
char int_p_cs_precedes; // CHAR_MAX
char int_n_cs_precedes; // CHAR_MAX
char int_p_sep_by_space; // CHAR_MAX
char int_n_sep_by_space; // CHAR_MAX
char int_p_sign_posn; // CHAR_MAX
char int_n_sign_posn; // CHAR_MAX