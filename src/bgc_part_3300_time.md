<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Date and Time Functionality

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

* UTC: Coordinated Universal Time is a universally^[On Earth, anyway.
  Who know what crazy systems they use _out there_...] agreed upon,
  absolute time. Everyone on the planet thinks it's the same time right
  now in UTC... even though they have different local times.

* GMT: Greenwich Mean Time, effectively the same as UTC^[OK, don't
  murder me! GMT is technically a timezone while UTC is a global time
  system. Also some countries might adjust GMT for daylight saving time,
  whereas UTC is never adjusted for daylight saving time.]. You probably
  want to say UTC, or "universal time". If you're talking specifically
  about the GMT timezone, say GMT. Confusingly, many of C's UTC
  functions predate UTC and still refer to Greenwich Mean Time. When you
  see that, know the C means UTC.

* Local time: what time it is where the computer running the program is
  located. This is described as an offset from UTC. Although there are
  many timezones in the world, most computers do work in either local
  time or UTC.

If you are describing an event that happens one time, like a rocket
launch, or when pointers finally clicked for you, use UTC.

On the other hand, if it's something that happens the same time _in
every timezone_, like New Year's Eve, or dinner time, use local time.

Since a lot of languages are only good at converting between UTC and
local time, you can cause yourself a lot of pain by choosing to store
your dates in the wrong form. (Ask me how I know.)


## Date Types

There are two^[Admittedly, there are more than two.] main types in C
when it comes to dates: `time_t` and `struct tm`.

The spec doesn't actually say much about them:

* `time_t`: a real type capable of holding a time. So by the spec, this
  could be a floating type. In POSIX (Unix-likes), it's an integer.

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

```
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

Once you have your `time_t` in a `struct tm`, it opens all kinds of
doors. You can print out the time in a variety of ways, figure out which
day of the week a date is, and so on. Or convert it back into a `time_t`.


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

```
Mon Apr 12 12:00:04 1982
Is DST: 0
```

## Formatted Date Output

