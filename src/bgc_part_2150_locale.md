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

## Setting the Localization, Quick and Dirty

For these calls, include `<locale.h>`.

There is basically one thing you can portably do here:

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