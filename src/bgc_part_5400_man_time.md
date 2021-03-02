<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<time.h>` Date and Time Functions {#time}

When it comes to time and C, there are two main types to look for:

* **`time_t`** holds a _calendar time_. This is an opaque type that
  represents an absolute time that can be converted to UTC^[When you say
  GMT, unless you're talking specifically about the timezone and not the
  time, you probably mean "UTC".] or local time.

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

[[pagebreak]]
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

```
The 42nd Fibonacci Number is 267914296
CPU time: 1.863078
```

### See Also {.unnumbered .unlisted}

[`time()`](#man-time),
[`difftime()`](#man-difftime),
[`timespec_get()`](#man-timespec_get)


[[pagebreak]]
## `difftime()` {#man-example}

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

```
1217996816.000000 seconds (38.596783 years) between events
```

### See Also {.unnumbered .unlisted}

[`time()`](#man-time),
[`mktime()`](#man-mktime)


[[pagebreak]]
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

### Example {.unnumbered .unlisted}

In the following example, we have `mktime()` tell us if that time was
DST or not.

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

int main(void)
{
    // April 12, 1982 and change
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

    // This will print what was in broken_down_time
    printf("Local time: %s", asctime(localtime(&calendar_time)));
    printf("Is DST    : %d\n\n", broken_down_time.tm_isdst);

    // This will print UTC for the local time, above
    printf("UTC       : %s", asctime(gmtime(&calendar_time)));
}
```

Output (for me in Pacific Time---UTC is 8 hours ahead):

```
Local time: Mon Apr 12 04:00:04 1982
Is DST    : 0

UTC       : Mon Apr 12 12:00:04 1982
```

### See Also {.unnumbered .unlisted}

[`localtime()`](#man-localtime),
[`gmtime()`](#man-gmtime)

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