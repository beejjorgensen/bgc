<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<time.h>` Date and Time Functions {#time}

|Function|Description|
|--------|----------------------|
|[`clock()`](#man-clock)|How much processor time has been used by this process|
|[`difftime()`](#man-difftime)|Compute the difference between two times|
|[`mktime()`](man-mktime)|Convert a `struct tm` into a `time_t`|
|[`time()`](#man-time)|Get the current calendar time|
|[`timespec_get()`](#man-timespec_get)|Get a higher resolution time, probably now|
|[`asctime()`](#man-asctime)|Return a human-readable version of a `struct tm`|
|[`ctime()`](#man-ctime)|Return a human-readable version of a `time_t`|
|[`gmtime()`](#man-gmtime)|Convert a calendar time into a UTC broken-down time|
|[`localtime()`](#man-localtime)|Convert a calendar time into a broken-down local time|
|[`strftime()`](#man-strftime)|Formatted date and time output|


When it comes to time and C, there are two main types to look for:

* **`time_t`** holds a _calendar time_. This is an potentially opaque
  numeric type that represents an absolute time that can be converted to
  UTC^[When you say GMT, unless you're talking specifically about the
  timezone and not the time, you probably mean "UTC".] or local time.

* **`struct tm`** holds a _broken-down time_. This has things like the
  day of the week, the day of the month, the hour, the minute, the
  second, etc.

On POSIX systems and Windows, `time_t` is an integer and represents the
number of seconds that have elapsed since January 1, 1970 at 00:00 UTC.

A `struct tm` contains the following fields:

``` {.c}
struct tm {
    int tm_sec;    // seconds after the minute -- [0, 60]
    int tm_min;    // minutes after the hour -- [0, 59]
    int tm_hour;   // hours since midnight -- [0, 23]
    int tm_mday;   // day of the month -- [1, 31]
    int tm_mon;    // months since January -- [0, 11]
    int tm_year;   // years since 1900
    int tm_wday;   // days since Sunday -- [0, 6]
    int tm_yday;   // days since January 1 -- [0, 365]
    int tm_isdst;  // Daylight Saving Time flag
};
```

You can convert between the two with `mktime()`, `gmtime()`, and
`localtime()`.

You can print time information to strings with `ctime()`, `asctime()`,
and `strftime()`.

## Thread Safety Warning

`asctime()`, `ctime()`: These two functions return a pointer to a
`static` memory region. They both might return the same pointer. If
you need thread safety, you'll need a mutex across them. If you need
both results at once, `strcpy()` one of them out.

All these problems with `asctime()` and `ctime()` can be avoided by
using the more flexible and thread-safe `strftime()` function instead.

`localtime()`, `gmtime()`: These other two functions also return a
pointer to a `static` memory region. They both might return the same
pointer. If you need thread safety, you'll need a mutex across them. If
you need both results at once, copy the `struct` to another.

[[manbreak]]
## `clock()` {#man-clock}

How much processor time has been used by this process

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

clock_t clock(void);
```

### Description {.unnumbered .unlisted}

Your processor is juggling a lot of things right now. Just because a
process has been alive for 20 minutes doesn't mean that it used 20
minutes of "CPU time".

Most of the time your average process spends asleep, and that doesn't
count toward the CPU time spent.

This function returns an opaque type representing the number of "clock
ticks"^[The spec doesn't actually say "clock ticks", but I... am.] the
process has spent in operation.

You can get the number of seconds out of that by dividing by the macro
`CLOCKS_PER_SEC`. This is an integer, so you will have to cast part of
the expression to a floating type to get a fractional time.

Note that this is not the "wall clock time" of the program. If you want
to get that loosely use `time()` and `difftime()` (which might only
offer 1-second resolution) or `timespec_get()` (which might only also
offer low resolution, but at least it _might_ go to nanosecond level).

### Return Value {.unnumbered .unlisted}

Returns the amount of CPU time spent by this process. This comes back in
a form that can be divided by `CLOCKS_PER_SEC` to determine the time in
seconds.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

// Deliberately naive Fibonacci
long long int fib(long long int n) {
    if (n <= 1) return n;

    return fib(n-1) + fib(n-2);
}

int main(void)
{
    printf("The 42nd Fibonacci Number is %lld\n", fib(42));

    printf("CPU time: %f\n", clock() / (double)CLOCKS_PER_SEC);
}
```

Output on my system:

``` {.default}
The 42nd Fibonacci Number is 267914296
CPU time: 1.863078
```

### See Also {.unnumbered .unlisted}

[`time()`](#man-time),
[`difftime()`](#man-difftime),
[`timespec_get()`](#man-timespec_get)


[[manbreak]]
## `difftime()` {#man-difftime}

Compute the difference between two times

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

double difftime(time_t time1, time_t time0);
```

### Description {.unnumbered .unlisted}

Since the `time_t` type is technically opaque, you can't just
straight-up subtract to get the difference between two of them^[Unless
you're on a POSIX system where `time_t` is definitely an integer, in
which case you can subtract. But you should still use `difftime()` for
maximum portability.]. Use this function to do it.

There is no guarantee as to the resolution of this difference, but it's
probably to the second.

### Return Value {.unnumbered .unlisted}

Returns the difference between two `time_t`s in seconds.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

int main(void)
{
    // April 12, 1982 and change
    struct tm time_a = { .tm_year=82, .tm_mon=3, .tm_mday=12,
        .tm_hour=4, .tm_min=00, .tm_sec=04, .tm_isdst=-1,
    };

    // November 15, 2020 and change
    struct tm time_b = { .tm_year=120, .tm_mon=10, .tm_mday=15,
        .tm_hour=16, .tm_min=27, .tm_sec=00, .tm_isdst=-1,
    };

    time_t cal_a = mktime(&time_a);
    time_t cal_b = mktime(&time_b);

    double diff = difftime(cal_b, cal_a);

    double years = diff / 60 / 60 / 24 / 365.2425;  // close enough

    printf("%f seconds (%f years) between events\n", diff, years);
}
```

Output:

``` {.default}
1217996816.000000 seconds (38.596783 years) between events
```

### See Also {.unnumbered .unlisted}

[`time()`](#man-time),
[`mktime()`](#man-mktime)


[[manbreak]]
## `mktime()` {#man-mktime}

Convert a `struct tm` into a `time_t`

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

time_t mktime(struct tm *timeptr);
```

### Description {.unnumbered .unlisted}

If you have a local date and time and want it converted to a `time_t`
(so that you can `difftime()` it or whatever), you can convert it with
this function.

Basically you fill out the fields in your `struct tm` in local time and
`mktime()` will convert those to the UTC `time_t` equivalent.

A couple notes:

* Don't bother filling out `tm_wday` or `tm_yday`. `mktime()` will fill
  these out for you.

* You can set `tm_isdst` to `0` to indicate your time isn't Daylight
  Saving Time (DST), `1` to indicate it is, and `-1` to have `mktime()`
  fill it in according to your locale's preference.

If you need input in UTC, see the non-standard functions
[fl[`timegm()`|https://man.archlinux.org/man/timegm.3.en]] for
Unix-likes and
[fl[`_mkgmtime()`|https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mkgmtime-mkgmtime32-mkgmtime64?view=msvc-160]]
for Windows.

### Return Value {.unnumbered .unlisted}

Returns the local time in the `struct tm` as a `time_t` calendar time.

Returns `(time_t)(-1)` on error.

### Example {.unnumbered .unlisted}

In the following example, we have `mktime()` tell us if that time was
DST or not.

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

int main(void)
{
    struct tm broken_down_time = {
        .tm_year=82,   // years since 1900
        .tm_mon=3,     // months since January -- [0, 11]
        .tm_mday=12,   // day of the month -- [1, 31]
        .tm_hour=4,    // hours since midnight -- [0, 23]
        .tm_min=00,    // minutes after the hour -- [0, 59]
        .tm_sec=04,    // seconds after the minute -- [0, 60]
        .tm_isdst=-1,  // Daylight Saving Time flag
    };

    time_t calendar_time = mktime(&broken_down_time);

    char *days[] = {"Sunday", "Monday", "Tuesday",
        "Wednesday", "Furzeday", "Friday", "Saturday"};

    // This will print what was in broken_down_time
    printf("Local time : %s", asctime(localtime(&calendar_time)));
    printf("Is DST     : %d\n", broken_down_time.tm_isdst);
    printf("Day of week: %s\n\n", days[broken_down_time.tm_wday]);

    // This will print UTC for the local time, above
    printf("UTC        : %s", asctime(gmtime(&calendar_time)));
}
```

Output (for me in Pacific Time---UTC is 8 hours ahead):

``` {.default}
Local time : Mon Apr 12 04:00:04 1982
Is DST     : 0
Day of week: Monday

UTC        : Mon Apr 12 12:00:04 1982
```

### See Also {.unnumbered .unlisted}

[`localtime()`](#man-localtime),
[`gmtime()`](#man-gmtime)

[[manbreak]]
## `time()` {#man-time}

Get the current calendar time

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

time_t time(time_t *timer);
```

### Description {.unnumbered .unlisted}

Returns the current calendar time right now. I mean, now. No, now!

If `timer` is not `NULL`, it gets loaded with the current time, as well.

This can be converted into a `struct tm` with `localtime()` or
`gmtime()`, or printed directly with `ctime()`.

### Return Value {.unnumbered .unlisted}

Returns the current calendar time. Also loads `timer` with the current
time if it's not `NULL`.

Or returns `(time_t)(-1)` if the time isn't available because you've
fallen out of the space-time continuum and/or the system doesn't support
times.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t now = time(NULL);

    printf("The local time is %s", ctime(&now));
}
```

Example output:

``` {.default}
The local time is Mon Mar  1 18:45:14 2021
```

### See Also {.unnumbered .unlisted}

[`localtime()`](#man-localtime),
[`gmtime()`](#man-gmtime),
[`ctime()`](#man-ctime)

[[manbreak]]
## `timespec_get()` {#man-timespec_get}

Get a higher resolution time, probably now

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

int timespec_get(struct timespec *ts, int base);
```

### Description {.unnumbered .unlisted}

This function loads the current time UTC (unless directed otherwise)
into the given `struct timespec`, `ts`.

That structure has two fields:

``` {.c}
struct timespec {
    time_t tv_sec;   // Whole seconds
    long   tv_nsec;  // Nanoseconds, 0-999999999
}
```

Nanoseconds are billionths of a second. You can divide by 1000000000.0
to convert to seconds.

The `base` parameter has only one defined value, by the spec:
`TIME_UTC`. So portably make it that. This will load `ts` with the
current time in seconds since a system-defined [flw[Epoch|Unix_time]],
often January 1, 1970 at 00:00 UTC.

Your implementation might define other values for `base`.

### Return Value {.unnumbered .unlisted}

When `base` is `TIME_UTC`, loads `ts` with the current UTC time.

On success, returns `base`, valid values for which will always be
non-zero. On error, returns `0`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
struct timespec ts;

timespec_get(&ts, TIME_UTC);

printf("%ld s, %ld ns\n", ts.tv_sec, ts.tv_nsec);

double float_time = ts.tv_sec + ts.tv_nsec/1000000000.0;
printf("%f seconds since epoch\n", float_time);
```

Example output:

``` {.default}
1614654187 s, 825540756 ns
1614654187.825541 seconds since epoch
```

Here's a helper function to add values to a `struct timespec` that
handles negative values and nanosecond overflow.

``` {.c}
#include <stdlib.h>

// Add delta seconds and delta nanoseconds to ts.
// Negative values are allowed. Each component is added individually.
//
// Subtract 1.5 seconds from the current value:
//
// timespec_add(&ts, -1, -500000000L);

struct timespec *timespec_add(struct timespec *ts, long dsec, long dnsec)
{
    long sec = (long)ts->tv_sec + dsec;
    long nsec = ts->tv_nsec + dnsec;

    ldiv_t qr = ldiv(nsec, 1000000000L);

    if (qr.rem < 0) {
        nsec = 1000000000L + qr.rem;
        sec += qr.quot - 1;
    } else {
        nsec = qr.rem;
        sec += qr.quot;
    }

    ts->tv_sec = sec;
    ts->tv_nsec = nsec;

    return ts;
}
```

And here are some functions to convert from `long double` to `struct
timespec` and back, just in case you like thinking in decimals. This is
more limited in significant figures than using the integer values.

``` {.c}
#include <math.h>

// Convert a struct timespec into a long double
long double timespec_to_ld(struct timespec *ts)
{
    return ts->tv_sec + ts->tv_nsec / 1000000000.0;
}

// Convert a long double to a struct timespec
struct timespec ld_to_timespec(long double t)
{
    long double f;
    struct timespec ts;
    ts.tv_nsec = modfl(t, &f) * 1000000000L;
    ts.tv_sec = f;

    return ts;
}
```

### See Also {.unnumbered .unlisted}

[`time()`](#man-time),
[`mtx_timedlock()`](#man-mtx_timedlock),
[`cnd_timedwait()`](#man-cnd_timedwait)

[[manbreak]]
## `asctime()` {#man-asctime}

Return a human-readable version of a `struct tm`

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

char *asctime(const struct tm *timeptr)
```

### Description {.unnumbered .unlisted}

This takes a time in a `struct tm` and returns a string with that date
in the form:

``` {.default}
Sun Sep 16 01:03:52 1973
```

with a newline included at the end, rather unhelpfully.
([`strftime()`](#man-strftime) will give you more flexibility.)

It's just like `ctime()`, except it takes a `struct tm` instead of a
`time_t`.

**WARNING**: This function returns a pointer to a `static char*` region
that isn't thread-safe and might be shared with the `ctime()` function.
If you need thread safety, use `strftime()` or use a mutex that covers
`ctime()` and `asctime()`.

Behavior is undefined for:

* Years less than 1000
* Years greater than 9999
* Any members of `timeptr` are out of range

### Return Value {.unnumbered .unlisted}

Returns a pointer to the human-readable date string.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t now = time(NULL);

    printf("Local: %s", asctime(localtime(&now)));
    printf("UTC  : %s", asctime(gmtime(&now)));
}
```

Sample output:

``` {.default}
Local: Mon Mar  1 21:17:34 2021
UTC  : Tue Mar  2 05:17:34 2021
```

### See Also {.unnumbered .unlisted}

[`ctime()`](#man-ctime),
[`localtime()`](#man-localtime),
[`gmtime()`](#man-gmtime)

[[manbreak]]
## `ctime()` {#man-ctime}

Return a human-readable version of a `time_t`

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

char *ctime(const time_t *timer);
```

### Description {.unnumbered .unlisted}

This takes a time in a `time_t` and returns a string with the local time
and date in the form:

``` {.default}
Sun Sep 16 01:03:52 1973
```

with a newline included at the end, rather unhelpfully.
([`strftime()`](#man-strftime) will give you more flexibility.)

It's just like `asctime()`, except it takes a `time_t` instead of a
`struct tm`.

**WARNING**: This function returns a pointer to a `static char*` region
that isn't thread-safe and might be shared with the `asctime()`
function. If you need thread safety, use `strftime()` or use a mutex
that covers `ctime()` and `asctime()`.

Behavior is undefined for:

* Years less than 1000
* Years greater than 9999
* Any members of `timeptr` are out of range

### Return Value {.unnumbered .unlisted}

A pointer to the human-readable local time and data string.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
time_t now = time(NULL);

printf("Local: %s", ctime(&now));
```

Sample output:

``` {.default}
Local: Mon Mar  1 21:32:23 2021
```

### See Also {.unnumbered .unlisted}

[`asctime()`](#man-asctime)

[[manbreak]]
## `gmtime()` {#man-gmtime}

Convert a calendar time into a UTC broken-down time

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

struct tm *gmtime(const time_t *timer);
```

### Description {.unnumbered .unlisted}

If you have a `time_t`, you can run it through this function to get a
`struct tm` back full of the corresponding broken-down UTC time
information.

This is just like `localtime()`, except it does UTC instead of local
time.

Once you have that `struct tm`, you can feed it to `strftime()` to print
it out.

**WARNING**: This function returns a pointer to a `static struct tm*`
region that isn't thread-safe and might be shared with the `localtime()`
function. If you need thread safety use a mutex that covers `gmtime()`
and `localtime()`.

### Return Value {.unnumbered .unlisted}

Returns a pointer to the broken-down UTC time, or `NULL` if it can't be
obtained.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t now = time(NULL);

    printf("UTC  : %s", asctime(gmtime(&now)));
    printf("Local: %s", asctime(localtime(&now)));
}
```

Sample output:

``` {.default}
UTC  : Tue Mar  2 05:40:05 2021
Local: Mon Mar  1 21:40:05 2021
```

### See Also {.unnumbered .unlisted}

[`localtime()`](#man-localtime),
[`asctime()`](#man-asctime),
[`strftime()`](#man-strftime)

[[manbreak]]
## `localtime()` {#man-localtime}

Convert a calendar time into a broken-down local time

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

struct tm *localtime(const time_t *timer);
```

### Description {.unnumbered .unlisted}

If you have a `time_t`, you can run it through this function to get a
`struct tm` back full of the corresponding broken-down local time
information.

This is just like `gmtime()`, except it does local time instead of UTC.

Once you have that `struct tm`, you can feed it to `strftime()` to print
it out.

**WARNING**: This function returns a pointer to a `static struct tm*`
region that isn't thread-safe and might be shared with the `gmtime()`
function. If you need thread safety use a mutex that covers `gmtime()`
and `localtime()`.

### Return Value {.unnumbered .unlisted}

Returns a pointer to the broken-down local time, or `NULL` if it can't
be obtained.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t now = time(NULL);

    printf("Local: %s", asctime(localtime(&now)));
    printf("UTC  : %s", asctime(gmtime(&now)));
}
```

Sample output:

``` {.default}
Local: Mon Mar  1 21:40:05 2021
UTC  : Tue Mar  2 05:40:05 2021
```

### See Also {.unnumbered .unlisted}

[`gmtime()`](#man-gmtime),
[`asctime()`](#man-asctime),
[`strftime()`](#man-strftime)

[[manbreak]]
## `strftime()` {#man-strftime}

Formatted date and time output

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>

size_t strftime(char * restrict s, size_t maxsize,
                const char * restrict format,
                const struct tm * restrict timeptr);
```

### Description {.unnumbered .unlisted}

This is the [`sprintf()`](#man-printf) of date and time functions. It'll
take a `struct tm` and produce a string in just about whatever form you
desire, for example:

``` {.default}
2021-03-01
Monday, March 1 at 9:54 PM
It's Monday!
```

It's a super flexible version of `asctime()`. And thread-safe, besides,
since it doesn't rely on a static buffer to hold the results.

Basically what you do is give it a destination, `s`, and its max size in
bytes in `maxsize`. Also, provide a `format` string that's analogous to
`printf()`'s format string, but with different format specifiers. And
lastly, a `struct tm` with the broken-down time information to use for
printing.

The `format` string works like this, for example:

``` {.c}
"It's %A, %B %d!"
```

Which produces:

``` {.default}
It's Monday, March 1!
```

The `%A` is the full day-of-week name, the `%B` is the full month name,
and the `%d` is the day of the month. `strftime()` substitutes the right
thing to produce the result. Brilliant!

So what are all the format specifiers? Glad you asked!

I'm going to be lazy and just drop this table in right from the spec.

|Specifier|Description|
|----|-----------------------------------|
|`%a`|Locale’s abbreviated weekday name. [`tm_wday`]|
|`%A`|Locale’s full weekday name. [`tm_wday`]|
|`%b`|Locale’s abbreviated month name. [`tm_mon`]|
|`%B`|Locale’s full month name. [`tm_mon`]|
|`%c`|Locale’s appropriate date and time representation.|
|`%C`|Year divided by 100 and truncated to an integer, as a decimal number (00–99). [`tm_year`]|
|`%d`|Day of the month as a decimal number (01–31). [`tm_mday`]|
|`%D`|Equivalent to `"%m/%d/%y"`. [`tm_mon`, `tm_mday`, `tm_year`]|
|`%e`|Day of the month as a decimal number (1–31); a single digit is preceded by a space. [`tm_mday`]|
|`%F`|Equivalent to "%Y-%m-%d" (the ISO 8601 date format). [`tm_year`, `tm_mon`, `tm_mday`]|
|`%g`|Last 2 digits of the week-based year (see below) as a decimal number (00–99). [`tm_year`, `tm_wday`, `tm_yday`]|
|`%G`|Week-based year (see below) as a decimal number (e.g., 1997). [`tm_year`, `tm_wday`, `tm_yday`]|
|`%h`|Equivalent to "%b". [`tm_mon`]|
|`%H`|Hour (24-hour clock) as a decimal number (00–23). [`tm_hour`]|
|`%I`|Hour (12-hour clock) as a decimal number (01–12). [`tm_hour`]|
|`%j`|Day of the year as a decimal number (001–366). [`tm_yday`]|
|`%m`|Month as a decimal number (01–12).|
|`%M`|Minute as a decimal number (00–59). [`tm_min`]|
|`%n`|A new-line character.|
|`%p`|Locale’s equivalent of the AM/PM designations associated with a 12-hour clock. [`tm_hour`]|
|`%r`|Locale’s 12-hour clock time. [`tm_hour`, `tm_min`, `tm_sec`]|
|`%R`|Equivalent to `"%H:%M"`. [`tm_hour`, `tm_min`]|
|`%S`|Second as a decimal number (00–60). [`tm_sec`]|
|`%t`|A horizontal-tab character.|
|`%T`|Equivalent to `"%H:%M:%S"` (the ISO 8601 time format). [`tm_hour`, `tm_min`, `tm_sec`]|
|`%u`|ISO 8601 weekday as a decimal number (1–7), where Monday is 1. [`tm_wday`]|
|`%U`|Week number of the year (the first Sunday as the first day of week 1) as a decimal number (00–53). [`tm_year`, `tm_wday`, `tm_yday`]|
|`%V`|ISO 8601 week number (see below) as a decimal number (01–53). [`tm_year`, `tm_wday`, `tm_yday`]|
|`%w`|Weekday as a decimal number (0–6), where Sunday is 0.|
|`%W`|Week number of the year (the first Monday as the first day of week 1) as a decimal number (00–53). [`tm_year`, `tm_wday`, `tm_yday`]|
|`%x`|Locale’s appropriate date representation.|
|`%X`|Locale’s appropriate time representation.|
|`%y`|Last 2 digits of the year as a decimal number (00–99). [`tm_year`]|
|`%Y`|Year as a decimal number (e.g., 1997). [`tm_year`]|
|`%z`|Offset from UTC in the ISO 8601 format `"-0430"` (meaning 4 hours 30 minutes behind UTC, west of Greenwich), or by no characters if no time zone is determinable. [`tm_isdst`]
|`%Z`|Locale’s time zone name or abbreviation, or by no characters if no time zone is determinable. [`tm_isdst`]|
|`%%`|A plain ol' %|

Phew. That's love.

`%G`, `%g`, and `%v` are a little funky in that they use something
called the ISO 8601 week-based year. I'd never heard of it. But, again
stealing from the spec, these are the rules:

> `%g`, `%G`, and `%V` give values according to the ISO 8601 week-based
> year. In this system, weeks begin on a Monday and week 1 of the year
> is the week that includes January 4th, which is also the week that
> includes the first Thursday of the year, and is also the first week
> that contains at least four days in the year. If the first Monday of
> January is the 2nd, 3rd, or 4th, the preceding days are part of the
> last week of the preceding year; thus, for Saturday 2nd January 1999,
> `%G` is replaced by `1998` and `%V` is replaced by `53`. If December 29th,
> 30th, or 31st is a Monday, it and any following days are part of week
> 1 of the following year. Thus, for Tuesday 30th December 1997, `%G` is
> replaced by `1998` and `%V` is replaced by `01`.

Learn something new every day! If you want to know more, [flw[Wikipedia
has a page on it|ISO_week_date]].

If you're in the "C" locale, the specifiers produce the following
(again, stolen from the spec):

|Specifier|Description|
|----|-----------------------------------|
|`%a`|The first three characters of `%A`.|
|`%A`|One of `Sunday`, `Monday`, ... , `Saturday`.|
|`%b`|The first three characters of `%B`.|
|`%B`|One of `January`, `February`, ... , `December`.|
|`%c`|Equivalent to `%a %b %e %T %Y`.|
|`%p`|One of `AM` or `PM`.|
|`%r`|Equivalent to `%I:%M:%S %p`.|
|`%x`|Equivalent to `%m/%d/%y`.|
|`%X`|Equivalent to `%T`.|
|`%Z`|Implementation-defined.|

There are additional variants of the format specifiers that indicate you
want to use a locale's alternative format. These don't exist for all
locales. It's one of the format specifies above, with either an `E` or
`O` prefix:

``` {.default}
%Ec %EC %Ex %EX %Ey %EY %Od %Oe %OH %OI
%Om %OM %OS %Ou %OU %OV %Ow %OW %Oy
```

The `E` and `O` prefixes are ignored in the "C" locale.

### Return Value {.unnumbered .unlisted}

Returns the total number of bytes put into the result string, not
including the NUL terminator.

If the result doesn't fit in the string, zero is returned and the value
in `s` is indeterminate.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

int main(void)
{
    char s[128];
    time_t now = time(NULL);

    // %c: print date as per current locale
    strftime(s, sizeof s, "%c", localtime(&now));
    puts(s);   // Sun Feb 28 22:29:00 2021

    // %A: full weekday name
    // %B: full month name
    // %d: day of the month
    strftime(s, sizeof s, "%A, %B %d", localtime(&now));
    puts(s);   // Sunday, February 28

    // %I: hour (12 hour clock)
    // %M: minute
    // %S: second
    // %p: AM or PM
    strftime(s, sizeof s, "It's %I:%M:%S %p", localtime(&now));
    puts(s);   // It's 10:29:00 PM

    // %F: ISO 8601 yyyy-mm-dd
    // %T: ISO 8601 hh:mm:ss
    // %z: ISO 8601 timezone offset
    strftime(s, sizeof s, "ISO 8601: %FT%T%z", localtime(&now));
    puts(s);   // ISO 8601: 2021-02-28T22:29:00-0800
}
```

### See Also {.unnumbered .unlisted}

[`ctime()`](#man-ctime),
[`asctime()`](#man-asctime)

<!--
[[manbreak]]
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
