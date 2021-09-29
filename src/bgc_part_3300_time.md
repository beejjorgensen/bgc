<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Date and Time Functionality

> "Time is an illusion. Lunchtime doubly so." \
> ---Ford Prefect, The Hitchhikers Guide to the Galaxy

This isn't too complex, but it can be a little intimidating at first,
both with the different types available and the way we can convert
between them.

Mix in GMT (UTC) and local time and we have all the _Usual Fun_™ one
gets with times and dates.

And of course never forget the golden rule of dates and times: _Never
attempt to write your own date and time functionality. Only use what the
library gives you._

Time is too complex for mere mortal programmers to handle correctly.
Seriously, we all owe a point to everyone who worked on any date and
time library, so put that in your budget.

## Quick Terminology and Information

Just a couple quick terms in case you don't have them down.

* **UTC**: Coordinated Universal Time is a universally^[On Earth,
  anyway. Who know what crazy systems they use _out there_...] agreed
  upon, absolute time. Everyone on the planet thinks it's the same time
  right now in UTC... even though they have different local times.

* **GMT**: Greenwich Mean Time, effectively the same as UTC^[OK, don't
  murder me! GMT is technically a timezone while UTC is a global time
  system. Also some countries might adjust GMT for daylight saving time,
  whereas UTC is never adjusted for daylight saving time.]. You probably
  want to say UTC, or "universal time". If you're talking specifically
  about the GMT timezone, say GMT. Confusingly, many of C's UTC
  functions predate UTC and still refer to Greenwich Mean Time. When you
  see that, know the C means UTC.

* **Local time**: what time it is where the computer running the program
  is located. This is described as an offset from UTC. Although there
  are many timezones in the world, most computers do work in either
  local time or UTC.

If you are describing an event that happens one time, like a log entry,
or a rocket launch, or when pointers finally clicked for you, use UTC.

On the other hand, if it's something that happens the same time _in
every timezone_, like New Year's Eve or dinner time, use local time.

Since a lot of languages are only good at converting between UTC and
local time, you can cause yourself a lot of pain by choosing to store
your dates in the wrong form. (Ask me how I know.)


## Date Types

There are two^[Admittedly, there are more than two.] main types in C
when it comes to dates: `time_t` and `struct tm`.

The spec doesn't actually say much about them:

* `time_t`: a real type capable of holding a time. So by the spec, this
  could be a floating type or integer type. In POSIX (Unix-likes), it's
  an integer. This holds _calendar time_. Which you can think of as UTC
  time.

* `struct tm`: holds the components of a calendar time. This is a
  _broken-down time_, i.e. the components of the time, like hour,
  minute, second, day, month, year, etc.

On a lot of systems, `time_t` represents the number of seconds since
[flw[_Epoch_|Unix_time]]. Epoch is in some ways the start of time from
the computer's perspective, which is commonly January 1, 1970 UTC.  `time_t`
can go negative to represent times before Epoch. Windows behaves the
same way as Unix from what I can tell.

And what's in a `struct tm`? The following fields:

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

Note that everything is zero-based except the day of the month.

It's important to know that you can put any values in these types you
want. There are functions to help get the time _now_, but the types hold
_a_ time, not _the_ time.

So the question becomes: "How do you initialize data of these types, and
how do you convert between them?"

## Initialization and Conversion Between Types

First, you can get the current time and store it in a `time_t` with the
`time()` function.

``` {.c}
time_t now;  // Variable to hold the time now

now = time(NULL);  // You can get it like this...

time(&now);        // ...or this. Same as the previous line.
```

Great! You have a variable that gets you the time now.

Amusingly, there's only one portable way to print out what's in a
`time_t`, and that's the rarely-used `ctime()` function that prints the
value in local time:


``` {.c}
now = time(NULL);
printf("%s", ctime(&now));
```

This returns a string with a very specific form that includes a newline
at the end:

``` {.default}
Sun Feb 28 18:47:25 2021
```

So that's kind of inflexible. If you want more control, you should
convert that `time_t` into a `struct tm`.

### Converting `time_t` to `struct tm`

There are two amazing ways to do this conversion:

* `localtime()`: this function converts a `time_t` to a `struct tm` in
  local time.

* `gmtime()`: this function converts a `time_t` to a `struct tm` in
  UTC. (See ye olde GMT creeping into that function name?)

Let's see what time it is now by printing out a `struct tm` with the
`asctime()` function:

``` {.c}
printf("Local: %s", asctime(localtime(&now)));
printf("  UTC: %s", asctime(gmtime(&now)));
```

Output (I'm in Pacific Standard Time, out of daylight ):

``` {.default}
Local: Sun Feb 28 20:15:27 2021
  UTC: Mon Mar  1 04:15:27 2021
```

Once you have your `time_t` in a `struct tm`, it opens all kinds of
doors. You can print out the time in a variety of ways, figure out which
day of the week a date is, and so on. Or convert it back into a `time_t`.

More on that soon!

### Converting `struct tm` to `time_t`

If you want to go the other way, you can use `mktime()` to get that
information.

`mktime()` sets the values of `tm_wday` and `tm_yday` for you, so don't
bother filling them out because they'll just be overwritten.

Also, you can set `tm_isdst` to `-1` to have it make the determination
for you. Or you can manually set it to true or false.

``` {.c}
struct tm some_time = {
    .tm_year=82,   // years since 1900
    .tm_mon=3,     // months since January -- [0, 11]
    .tm_mday=12,   // day of the month -- [1, 31]
    .tm_hour=12,   // hours since midnight -- [0, 23]
    .tm_min=00,    // minutes after the hour -- [0, 59]
    .tm_sec=04,    // seconds after the minute -- [0, 60]
    .tm_isdst=-1,  // Daylight Saving Time flag
};

time_t some_time_epoch;

some_time_epoch = mktime(&some_time);

printf("%s", ctime(&some_time_epoch));
printf("Is DST: %d\n", some_time.tm_isdst);
```

Output:

``` {.default}
Mon Apr 12 12:00:04 1982
Is DST: 0
```

When you manually load a `struct tm` like that, it should be in local
time.  `mktime()` will convert that local time into a `time_t` calendar
time.

Weirdly, however, the standard doesn't give us a way to load up a
`struct tm` with a UTC time and convert that to a `time_t`. If you want
to do that with Unix-likes, try the non-standard `timegm()`. On Windows,
`_mkgmtime()`.

## Formatted Date Output

We've already seen a couple ways to print formatted date output to the
screen. With `time_t` we can use `ctime()`, and with `struct tm` we can
use `asctime()`.

``` {.c}
time_t now = time(NULL);
struct tm *local = localtime(&now);
struct tm *utc = gmtime(&now);

printf("Local time: %s", ctime(&now));     // Local time with time_t
printf("Local time: %s", asctime(local));  // Local time with struct tm
printf("UTC       : %s", asctime(utc));    // UTC with a struct tm
```

But what if I told you, dear reader, that there's a way to have much
more control over how the date was printed?

Sure, we could fish individual fields out of the `struct tm`, but 
there's a great function called `strftime()` that will do a lot of the
hard work for you. It's like `printf()`, except for dates!

Let's see some examples. In each of these, we pass in a destination
buffer, a maximum number of characters to write, and then a format
string (in the style of---but not the same as---`printf()`) which tells
`strftime()` which components of a `struct tm` to print and how.

You can add other constant characters to include in the output in the
format string, as well, just like with `printf()`.

We get a `struct tm` in this case from `localtime()`, but any source
works fine.

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

There are a _ton_ of date printing format specifiers for `strftime()`,
so be sure to check them out in the [`strftime()` reference
page](#man-strftime).


## More Resolution with `timespec_get()`

You can get the number of seconds and nanoseconds since Epoch with
`timespec_get()`.

Maybe.

Implementations might not have nanosecond resolution (that's one
billionth of a second) so who knows how many significant places you'll
get, but give it a shot and see.

`timespec_get()` takes two arguments. One is a pointer to a `struct
timespec` to hold the time information. And the other is the `base`,
which the spec lets you set to `TIME_UTC` indicating that you're
interested in seconds since Epoch. (Other implementations might give you
more options for the `base`.)

And the structure itself has two fields:

``` {.c}
struct timespec {
    time_t tv_sec;   // Seconds
    long   tv_nsec;  // Nanoseconds (billionths of a second)
};
```

Here's an example where we get the time and print it out both as integer
values and also a floating value:

``` {.c}
struct timespec ts;

timespec_get(&ts, TIME_UTC);

printf("%ld s, %ld ns\n", ts.tv_sec, ts.tv_nsec);

double float_time = ts.tv_sec + ts.tv_nsec/1000000000.0;
printf("%f seconds since epoch\n", float_time);
```

Example output:

``` {.default}
1614581530 s, 806325800 ns
1614581530.806326 seconds since epoch
```

`struct timespec` also makes an appearance in a number of the threading
functions that need to be able to specify time with that resolution.

## Differences Between Times

One quick note about getting the difference between two `time_t`s: since
the spec doesn't dictate how that type represents a time, you might not
be able to simply subtract two `time_t`s and get anything sensible^[You
will on POSIX, where `time_t` is definitely an integer. Unfortunately
the entire world isn't POSIX, so there we are.].

Luckily you can use `difftime()` to compute the difference in seconds
between two dates.

In the following example, we have two events that occur some time apart,
and we use `difftime()` to compute the difference.

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>

int main(void)
{
    struct tm time_a = {
        .tm_year=82,   // years since 1900
        .tm_mon=3,     // months since January -- [0, 11]
        .tm_mday=12,   // day of the month -- [1, 31]
        .tm_hour=4,    // hours since midnight -- [0, 23]
        .tm_min=00,    // minutes after the hour -- [0, 59]
        .tm_sec=04,    // seconds after the minute -- [0, 60]
        .tm_isdst=-1,  // Daylight Saving Time flag
    };

    struct tm time_b = {
        .tm_year=120,  // years since 1900
        .tm_mon=10,    // months since January -- [0, 11]
        .tm_mday=15,   // day of the month -- [1, 31]
        .tm_hour=16,   // hours since midnight -- [0, 23]
        .tm_min=27,    // minutes after the hour -- [0, 59]
        .tm_sec=00,    // seconds after the minute -- [0, 60]
        .tm_isdst=-1,  // Daylight Saving Time flag
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

And there you have it! Remember to use `difftime()` to take the time
difference. Even though you can just subtract on a POSIX system, might
as well stay portable.