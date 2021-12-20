<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdlib.h>` Standard Library Functions {#stdlib}

Some of the following functions have variants that handle different
types: `atoi()`, `strtod()`, `strtol()`, `abs()`, and `div()`. Only a
single one is listed here for brevity.

|Function|Description|
|--------|----------------------|
|[`_Exit()`](#man-exit)|Exit the currently-running program and don't look back|
|[`abort()`](#man-abort)|Abruptly end program execution|
|[`abs()`](#man-abs)|Compute the absolute value of an integer|
|[`aligned_alloc()`](#man-aligned_alloc)|Allocate specifically-aligned memory|
|[`at_quick_exit()`](#man-atexit)|Set up handlers to run when the program quickly exits|
|[`atexit()`](#man-atexit)|Set up handlers to run when the program exits|
|[`atof()`](#man-atof)|Convert a string to a floating point value|
|[`atoi()`](#man-atoi)|Convert an integer in a string into a integer type|
|[`bsearch()`](#man-bsearch)|Binary Search (maybe) an array of objects|
|[`calloc()`](#man-malloc)|Allocate and clear memory for arbitrary use|
|[`div()`](#man-div)|Compute the quotient and remainder of two numbers|
|[`exit()`](#man-exit)|Exit the currently-running program|
|[`free()`](#man-free)|Free a memory region|
|[`getenv()`](#man-getenv)|Get the value of an environment variable|
|[`malloc()`](man-malloc)|Allocate memory for arbitrary use|
|[`mblen()`](#man-mblen)|Return the number of bytes in a multibyte character|
|[`mbstowcs()`](#man-mbstowcs)|Convert a multibyte string to a wide character string|
|[`mbtowc()`](#man-mbtowc)|Convert a multibyte character to a wide character|
|[`qsort()`](#man-qsort)|Quicksort (maybe) some data|
|[`quick_exit()`](#man-exit)|Exit the currently-running program quickly|
|[`rand()`](#man-rand)|Return a pseudorandom number|
|[`realloc()`](#man-realloc)|Resize a previously allocated stretch of memory|
|[`srand()`](#man-srand)|Seed the built-in pseudorandom number generator|
|[`strtod()`](#man-strtod)|Convert a string to a floating point number|
|[`strtol()`](#man-strtol)|Convert a string to an integer|
|[`system()`](#man-system)|Run an external program|
|[`wcstombs()`](#man-wcstombs)|Convert a wide character string to a multibyte string|
|[`wctomb()`](#man-wctomb)|Convert a wide character to a multibyte character|

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


[[manbreak]]
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
and (optionally) a decimal point into a floating point value. Leading
whitespace is ignored, and translation stops at the first invalid
character.

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
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    double x = atof("3.141593");

    printf("%f\n", x);  // 3.141593
}
```

### See Also {.unnumbered .unlisted}

[`atoi()`](#man-atoi),
[`strtod()`](#man-strtod)

[[manbreak]]
## `atoi()`, `atol()`, `atoll()` {#man-atoi}

Convert an integer in a string into a integer type

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

int atoi(const char *nptr);

long int atol(const char *nptr);

long long int atoll(const char *nptr);
```

### Description {.unnumbered .unlisted}

Back in the day, `atoi()` stood for
[fl["ASCII-To_Integer"|http://man.cat-v.org/unix-1st/3/atoi]] but now
the spec makes no mention of that.

These functions take a string with a number in them and convert it to an
integer of the specified return type. Leading whitespace is ignored.
Translation stops at the first invalid character.

If the result doesn't fit in the return type, behavior is undefined.

It generally works as if you'd called [`strtol()`](#man-strtol) family
of functions:

``` {.c}
atoi(nptr)                 // is basically the same as...
(int)strtol(nptr, NULL, 10)

atol(nptr)                 // is basically the same as...
strtol(nptr, NULL, 10)

atoll(nptr)                // is basically the same as...
strtoll(nptr, NULL, 10)
```

Again, the [`strtol()`](#man-strtol) functions are generally better, so
I recommend them instead of these.

### Return Value {.unnumbered .unlisted}

Returns an integer result corresponding to the return type.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int x = atoi("3490");

    printf("%d\n", x);  // 3490
}
```

### See Also {.unnumbered .unlisted}

[`atof()`](#man-atof),
[`strtol()`](#man-strtol)

[[manbreak]]
## `strtod()`, `strtof()`, `strtold()` {#man-strtod}

Convert a string to a floating point number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

double strtod(const char * restrict nptr, char ** restrict endptr);

float strtof(const char * restrict nptr, char ** restrict endptr);

long double strtold(const char * restrict nptr, char ** restrict endptr);
```

### Description {.unnumbered .unlisted}

These are some neat functions that convert strings to floating point
numbers (or even NaN or Infinity) and provide some error checking,
besides.

Firstly, leading whitespace is skipped.

Then the functions attempt to convert characters into the floating point
result. Finally, when an invalid character (or NUL character) is
reached, they set `endptr` to point to the invalid character.

Set `endptr` to `NULL` if you don't care about where the first invalid
character is.

If you didn't set `endptr` to `NULL`, it will point to a NUL character
if the translation didn't find any bad characters. That is:

``` {.c}
if (*endptr == '\0') {
    printf("What a perfectly-formed number!\n");
} else {
    printf("I found badness in your number: \"%s\"\n", endptr);
}
```

But guess what! You can also translate strings into special values, like
NaN and Infinity!

If `nptr` points to a string containing `INF` or `INFINITY` (upper or
lowercase), the value for Infinity will be returned.

If `nptr` points to a string containing `NAN`, then (a quiet,
non-signalling) NaN will be returned. You can tag the `NAN` with a
sequence of characters from the set `0`-`9`, `a`-`z`, `A`-`Z`, and `_`
by enclosing them in parens:

``` {.c}
NAN(foobar_3490)
```

What your compiler does with this is implementation-defined, but it can
be used to specify different kinds of NaN.

You can also specify a number in hexadecimal with a power-of-two
exponent ($2^x$) if you lead with `0x` (or `0X`). For the exponent, use
a `p` followed by a base 10 exponent. (You can't use `e` because that's
a valid hex digit!)

Example:

``` {.c}
0xabc.123p15
```

Which computes to $0xabc.123\times2^{15}$.

You can put in `FLT_DECIMAL_DIG`, `DBL_DECIMAL_DIG`, or
`LDBL_DECIMAL_DIG` digits and get a correctly-rounded result for the
type.

### Return Value {.unnumbered .unlisted}

Returns the converted number. If there was no number, returns `0`.
`endptr` is set to point to the first invalid character, or the NUL
terminator if all characters were consumed.

If there's an overflow, `HUGE_VAL`, `HUGE_VALF`, or `HUGE_VALL` is
returned, signed like the input, and `errno` is set to `ERANGE`.

If there's an underflow, it returns the smallest number closest to zero
with the input sign. `errno` may be set to `ERANGE`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *inp = "   123.4567beej";
    char *badchar;

    double val = strtod(inp, &badchar);

    printf("Converted string to %f\n", val);
    printf("Encountered bad characters: %s\n", badchar);

    val = strtod("987.654321beej", NULL);
    printf("Ignoring bad chars for result: %f\n", val);

    val = strtod("11.2233", &badchar);

    if (*badchar == '\0')
        printf("No bad chars: %f\n", val);
    else
        printf("Found bad chars: %f, %s\n", val, badchar);
}
```

Output:

``` {.default}
Converted string to 123.456700
Encountered bad characters: beej
Ignoring bad chars: 987.654321
No bad chars: 11.223300
```

### See Also {.unnumbered .unlisted}

[`atof()`](#man-atof),
[`strtol()`](#man-strtol)

[[manbreak]]
## `strtol()`, `strtoll()`, `strtoul()`, `strtoull()` {#man-strtol}

Convert a string to an integer

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

long int strtol(const char * restrict nptr,
                char ** restrict endptr, int base);

long long int strtoll(const char * restrict nptr,
                      char ** restrict endptr, int base);

unsigned long int strtoul(const char * restrict nptr,
                          char ** restrict endptr, int base);

unsigned long long int strtoull(const char * restrict nptr,
                                char ** restrict endptr, int base);

```

### Description {.unnumbered .unlisted}

These convert a string to an integer like `atoi()`, but they have a few
more bells and whistles.

Most notable, they can tell you where conversion started going wrong,
i.e. where invalid characters, if any, appear. Leading spaces are
ignored. A `+` or `-` sign may precede the number.

The basic idea is that if things go well, these functions will return
the integer values contained in the strings. And if you pass in the
`char**` typed `endptr`, it'll set it to point at the NUL at the end of
the string.

If things don't go well, they'll set `endptr` to point at the first
character where things have gone awry. That is, if you're converting a
value `103z2!` in base 10, they'll send `endptr` to point at the `z`
because that's the first non-numeric character.

You can pass in `NULL` for `endptr` if you don't care to do any of that
kind of error checking.

Wait---did I just say we could set the number base for the conversion?
Yes! Yes, I did. Now [flw[number bases|Radix]] are out of scope for this
document, but certainly some of the more well-known are binary (base 2),
octal (base 8), decimal (base 10), and hexadecimal (base 16).

You can specify the number base for the conversion as the third
parameter. Bases from 2 to 36 are supported, with case-insensitive
digits running from `0` to `Z`.

If you specify a base of `0`, the function will make an effort to
determine it. It'll default to base 10 except for a couple cases:

* If the number has a leading `0`, it will be octal (base 8)
* If the number has a leading `0x` or `0X`, it will be hex (base 16)

The locale might affect the behavior of these functions.

### Return Value {.unnumbered .unlisted}

Returns the converted value.

`endptr`, if not `NULL` is set to the first invalid character, or to the
beginning of the string if no conversion was performed, or to the string
terminal NUL if all characters were valid.

If there's overflow, one of these values will be returned: `LONG_MIN`,
`LONG_MAX`, `LLONG_MIN`, `LLONG_MAX`, `ULONG_MAX`, `ULLONG_MAX`. And
`errno` is set to `ERANGE`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // All output in decimal (base 10)

    printf("%ld\n", strtol("123", NULL, 0));      // 123
    printf("%ld\n", strtol("123", NULL, 10));     // 123
    printf("%ld\n", strtol("101010", NULL, 2));   // binary, 42
    printf("%ld\n", strtol("123", NULL, 8));      // octal, 83
    printf("%ld\n", strtol("123", NULL, 16));     // hex, 291

    printf("%ld\n", strtol("0123", NULL, 0));     // octal, 83
    printf("%ld\n", strtol("0x123", NULL, 0));    // hex, 291

    char *badchar;
    long int x = strtol("   1234beej", &badchar, 0);

    printf("Value is %ld\n", x);               // Value is 1234
    printf("Bad chars at \"%s\"\n", badchar);  // Bad chars at "beej"
}
```

Output:

``` {.default}
123
123
42
83
291
83
291
Value is 1234
Bad chars at "beej"
```

### See Also {.unnumbered .unlisted}

[`atoi()`](#man-atoi),
[`strtod()`](#man-strtod),
[`setlocale()`](#man-setlocale),
[`strtoimax()`](#man-strtoimax),
[`strtoumax()`](#man-strtoimax)

[[manbreak]]
## `rand()` {#man-rand}

Return a pseudorandom number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

int rand(void);
```

### Description {.unnumbered .unlisted}

This gives us back a pseudorandom number in the range `0` to `RAND_MAX`,
inclusive. (`RAND_MAX` will be at least $32767$.)

If you want to force this to a certain range, the classic way to do this
is to force it with the modulo operator `%`, although [fl[this
introduces
biases|https://stackoverflow.com/questions/10984974/why-do-people-say-there-is-modulo-bias-when-using-a-random-number-generator]]
if `RAND_MAX+1` is not a multiple of the number you're modding by. Dealing
with this is out of scope for this guide.

If you want to to make a floating point number between `0` and `1`
inclusive, you can divide the result by `RAND_MAX`. Or `RAND_MAX+1` if
you don't want to include `1`. But of course, there are out-of-scope
[fl[problems with this, as
well|https://mumble.net/~campbell/2014/04/28/uniform-random-float]].

In short, `rand()` is a great way to get potentially poor random numbers
with ease. Probably good enough for the game you're writing.

The spec elaborates:

> There are no guarantees as to the quality of the random sequence
> produced and some implementations are known to produce sequences with
> distressingly non-random low-order bits. Applications with particular
> requirements should use a generator that is known to be sufficient for
> their needs.

Your system probably has a good random number generator on it if you
need a stronger source. Linux users have `getrandom()`, for example, and
Windows has `CryptGenRandom()`.

For other more demanding random number work, you might find a library
like the [fl[GNU Scientific
Library|https://www.gnu.org/software/gsl/doc/html/rng.html]] of use.

With most implementations, the numbers produced by `rand()` will be the
same from run to run. To get around this, you need to start it off in a
different place by passing a _seed_ into the random number generator.
You can do this with [`srand()`](#man-srand).

### Return Value {.unnumbered .unlisted}

Returns a random number in the range `0` to `RAND_MAX`, inclusive.

### Example {.unnumbered .unlisted}

Note that all of these examples don't produce perfectly uniform
distributions. But good enough for the untrained eye, and really common
in general use when mediocre random number quality is acceptable.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("RAND_MAX = %d\n", RAND_MAX);

    printf("0 to 9: %d\n", rand() % 10);

    printf("10 to 44: %d\n", rand() % 35 + 10);
    printf("0 to 0.99999: %f\n", rand() / ((float)RAND_MAX + 1));
    printf("10.5 to 15.7: %f\n", 10.5 + 5.2 * rand() / (float)RAND_MAX);
}
```

Output on my system:

``` {.default}
RAND_MAX = 2147483647
0 to 9: 3
10 to 44: 21
0 to 0.99999: 0.783099
10.5 to 15.7: 14.651888
```

Example of seeding the RNG with the time:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    // time(NULL) very likely returns the number of seconds since
    // January 1, 1970:

    srand(time(NULL));

    for (int i = 0; i < 5; i++)
        printf("%d\n", rand());
}
```

### See Also {.unnumbered .unlisted}

[`srand()`](#man-srand)

[[manbreak]]
## `srand()` {#man-srand}

Seed the built-in pseudorandom number generator

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

void srand(unsigned int seed);
```

### Description {.unnumbered .unlisted}

The dirty little secret of pseudorandom number generation is that
they're completely deterministic. There's nothing random about them.
They just look random.

If you use `rand()` and run your program several times, you might notice
something _fishy_: they produce the same random numbers over and over
again.

To mix it up, we need to give the pseudorandom number generator a new
"starting point", if you will. We call that the _seed_. It's just a
number, but it is used as the basic for subsequent number generation.
Give a different seed, and you'll get a different sequence of random
numbers. Give the same seed, and you'll get the same sequence of random
numbers corresponding to it^[Minecraft enthusiasts might recall that
when generating a new world, they were given the option to enter a
random number seed. That single value is used to generate that entire
random world. And if your friend starts a world with the same seed you
did, they'll get the same world you did.].

So if you call `srand(3490)` before you start generating numbers with
`rand()`, you'll get the same sequence every time. `srand(37)` would
also give you the same sequence every time, but it would be a different
sequence than the one you got with `srand(3490)`.

But if you can't hardcode the seed (because that would give you the same
sequence every time), how are you supposed to do this?

It's really common to use the number of seconds since January 1, 1970
(this date is known as the [flw[_Unix epoch_|Unix_time]]) to seed the
generator. This sounds pretty arbitrary except for the fact that it's
exactly the value most implementations return from the library call
`time(NULL)`^[The C spec doesn't say exactly what `time(NULL)` will
return, but the POSIX spec does! And virtually everyone returns exactly
that: the number of seconds since epoch.].

We'll do that in the example.

If you don't call `srand()`, it's as if you called `srand(1)`.

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // for the time() call

int main(void)
{
    srand(time(NULL));

    for (int i = 0; i < 5; i++)
        printf("%d\n", rand() % 32);
}
```

Output:

``` {.default}
4
20
22
14
9
```

Output from a subsequent run:

``` {.default}
19
0
31
31
24
```

### See Also {.unnumbered .unlisted}

[`rand()`](#man-rand),
[`time()`](#man-time)


[[manbreak]]
## `aligned_alloc()` {#man-aligned_alloc}

Allocate specifically-aligned memory

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

void *aligned_alloc(size_t alignment, size_t size);
```

### Description {.unnumbered .unlisted}

Maybe you wanted [`malloc()`](#man-malloc) or [`calloc()`](#man-malloc)
instead of this. But if you're sure you don't, read on!

Normally you don't have to think about this, since `malloc()` and
`realloc()` both provide memory regions that are suitably
[flw[aligned|Data_structure_alignment]] for use with any data type.

But if you need a more specific alignment, you can specify it with this
function.

When you're done using the memory region, be sure to free it with a call
to [`free()`](#man-free).

Don't pass in `0` for the size. It probably won't do anything you want.

In case you're wondering, all dynamically-allocated memory is
automatically freed by the system when the program ends. That said, it's
considered to be _Good Form_ to explicitly `free()` everything you
allocate. This way other programmers don't think you were being sloppy.

### Return Value {.unnumbered .unlisted}

Returns a pointer to the newly-allocated memory, aligned as specified.
Returns `NULL` if something goes wrong.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void)
{
    int *p = aligned_alloc(256, 10 * sizeof(int));

    // Just for fun, let's convert to intptr_t and mod with 256
    // to make sure we're actually aligned on a 256-byte boundary.
    //
    // This is probably some kind of implementation-defined
    // behavior, but I'll bet it works.

    intptr_t ip = (intptr_t)p;

    printf("%ld\n", ip % 256);   // 0!

    // Free it up
    free(p);
}
```

### See Also {.unnumbered .unlisted}

[`malloc()`](#man-malloc),
[`calloc()`](#man-malloc),
[`free()`](#man-free)

[[manbreak]]
## `calloc()`, `malloc()` {#man-malloc}

Allocate memory for arbitrary use

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

void *calloc(size_t nmemb, size_t size);

void *malloc(size_t size);
```

### Description {.unnumbered .unlisted}

Both of these functions allocate memory for general-purpose use. It will
be aligned such that it's useable for storing any data type.

`malloc()` allocates exactly the specified number of bytes of memory in
a contiguous block. The memory might be full of garbage data. (You can
clear it with [`memset()`](#man-memset), if you wish.)

`calloc()` is different in that it allocates space for `nmemb` objects
of `size` bytes each. (You can do the same with `malloc()`, but you have
to do the multiplication yourself.)

`calloc()` has an additional feature: it clears all the memory to `0`.

So if you're planning to zero the memory anyway, `calloc()` is probably
the way to go. If you're not, you can avoid that overhead by calling
`malloc()`.

When you're done using the memory region, free it with a call to
`free()`.

Don't pass in `0` for the size. It probably won't do anything you want.

In case you're wondering, all dynamically-allocated memory is
automatically freed by the system when the program ends. That said, it's
considered to be _Good Form_ to explicitly `free()` everything you
allocate. This way other programmers don't think you were being sloppy.

### Return Value {.unnumbered .unlisted}

Both functions return a pointer to the shiny, newly-allocated memory. Or
`NULL` if something's gone awry.

### Example {.unnumbered .unlisted}

Comparison of `malloc()` and `calloc()` for allocating 5 `int`s:

``` {.c .numberLines}
#include <stdlib.h>

int main(void)
{
    // Allocate space for 5 ints
    int *p = malloc(5 * sizeof(int));

    p[0] = 12;
    p[1] = 30;

    // Allocate space for 5 ints
    // (Also clear that memory to 0)
    int *q = calloc(5, sizeof(int));

    q[0] = 12;
    q[1] = 30;

    // All done
    free(p);
    free(q);
}
```

### See Also {.unnumbered .unlisted}

[`aligned_alloc()`](#man-aligned_alloc),
[`free()`](#man-free)

[[manbreak]]
## `free()` {#man-free}

Free a memory region

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

void free(void *ptr);
```

### Description {.unnumbered .unlisted}

You know that pointer you got back from `malloc()`, `calloc()`, or
`aligned_alloc()`? You pass that pointer to `free()` to free the memory
associated with it.

If you don't do this, the memory will stay allocated FOREVER AND EVER! (Well,
until your program exits, anyway.)

Fun fact: `free(NULL)` does nothing. You can safely call that. Sometimes
it's convenient.

Don't `free()` a pointer that's already been `free()`d. Don't `free()` a
pointer that you didn't get back from one of the allocation functions.
It would be _Bad_^["Try to imagine all life as you know it stopping
instantaneously and every molecule in your body exploding at the speed
of light." ---Egon Spengler].

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdlib.h>

int main(void)
{
    // Allocate space for 5 ints
    int *p = malloc(5 * sizeof(int));

    p[0] = 12;
    p[1] = 30;

    // Free that space
    free(p);
}
```

### See Also {.unnumbered .unlisted}

[`malloc()`](#man-malloc),
[`calloc()`](#man-malloc),
[`aligned_alloc()`](#man-aligned_alloc)

[[manbreak]]
## `realloc()` {#man-realloc}

Resize a previously allocated stretch of memory

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

void *realloc(void *ptr, size_t size);
```

### Description {.unnumbered .unlisted}

This takes a pointer to some memory previously allocated with `malloc()`
or `calloc()` and resizes it to the new size.

If the new size is smaller than the old size, any data larger than the
new size is discarded.

If the new size is larger than the old size, the new larger part is
uninitialized. (You can clear it with [`memset()`](#man-memset).)

Important note: the memory might move! If you resize, the system might
need to relocate the memory to a larger continguous chunk. If this
happens, `realloc()` will copy the old data to the new location for you.

Because of this, it's important to save the returned value to your
pointer to update it to the new location if things move. (Also, be sure
to error-check so that you don't overwrite your old pointer with `NULL`,
leaking the memory.)

You can also `relloc()` memory allocated with `aligned_alloc()`, but it
will potentially lose its alignment if the block is moved.

### Return Value {.unnumbered .unlisted}

Returns a pointer to the resized memory region. This might be equivalent
to the `ptr` passed in, or it might be some other location.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // Allocate space for 5 ints
    int *p = malloc(5 * sizeof(int));

    p[0] = 12;
    p[1] = 30;

    // Reallocate for 10 bytes
    int *new_p = realloc(p, 10 * sizeof(int));

    if (new_p == NULL) {
        printf("Error reallocing\n");
    } else {
        p = new_p;  // It's good; let's keep it
        p[7] = 99;
    }

    // All done
    free(p);
}
```

### See Also {.unnumbered .unlisted}

[`malloc()`](#man-malloc),
[`calloc()`](#man-malloc)

[[manbreak]]
## `abort()` {#man-abort}

Abruptly end program execution

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

_Noreturn void abort(void);
```

### Description {.unnumbered .unlisted}

This ends program execution _abnormally_ and immediately. Use this in
rare, unexpected circumstances.

Open streams might not be flushed. Temporary files created might not be
removed. Exit handlers are not called.

A non-zero exit status is returned to the environment.

On some systems, `abort()` might [flw[dump core|Core_dump]], but this is
outside the scope of the spec.

You can cause the equivalent of an `abort()` by calling
`raise(SIGABRT)`, but I don't know why you'd do that.

The only portable way to stop an `abort()` call midway is to use
`signal()` to catch `SIGABRT` and then `exit()` in the signal handler.

### Return Value {.unnumbered .unlisted}

This function never returns.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int bad_thing = 1;

    if (bad_thing) {
        printf("This should never have happened!\n");
        fflush(stdout);  // Make sure the message goes out
        abort();
    }
}
```

On my system, this outputs:

``` {.default}
This should never have happened!
zsh: abort (core dumped)  ./foo
```

### See Also {.unnumbered .unlisted}

[`signal()`](#man-signal)

[[manbreak]]
## `atexit()`, `at_quick_exit()` {#man-atexit}

Set up handlers to run when the program exits

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

int atexit(void (*func)(void));

int at_quick_exit(void (*func)(void));
```

### Description {.unnumbered .unlisted}

When the program does a normal exit with `exit()` or returns from
`main()`, it looks for previously-registered handlers to call on the way
out. These handlers are registered with the `atexit()` call.

Think of it like, "Hey, when you're about to exit, do these extra
things."

For the `quick_exit()` call, you can use the `at_quick_exit()` function
to register handlers for that^[`quick_exit()` differs from `exit()` in
that open files might not be flushed and temporary files might not be
removed.]. There's no crossover in handlers from `exit()` to
`quick_exit()`, i.e. for a call to one, none of the other's handlers
will fire.

You can register multiple handlers to fire---at least 32 handlers are
supported by both `exit()` and `quick_exit()`.

The argument `func` to the functions looks a little weird---it's a
pointer to a function to call. Basically just put the function name to
call in there (without parentheses after). See the example, below.

If you call `atexit()` from inside your `atexit()` handler (or equivalent
in your `at_quick_exit()` handler), it's unspecified if it will get
called. So get them all registered before you exit.

When exiting, the functions will be called in the reverse order they
were registered.

### Return Value {.unnumbered .unlisted}

These functions return `0` on success, or nonzero on failure.

### Example {.unnumbered .unlisted}

`atexit()`:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

void exit_handler_1(void)
{
    printf("Exit handler 1 called!\n");
}

void exit_handler_2(void)
{
    printf("Exit handler 2 called!\n");
}

int main(void)
{
    atexit(exit_handler_1);
    atexit(exit_handler_2);

    exit(0);
}
```

For the output:

``` {.default}
Exit handler 2 called!
Exit handler 1 called!
```

And a similar example with `quick_exit()`:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

void exit_handler_1(void)
{
    printf("Exit handler 1 called!\n");
}

void exit_handler_2(void)
{
    printf("Exit handler 2 called!\n");
}

int main(void)
{
    at_quick_exit(exit_handler_1);
    at_quick_exit(exit_handler_2);

    quick_exit(0);
}
```

### See Also {.unnumbered .unlisted}

[`exit()`](#man-exit),
[`quick_exit()`](#man-exit)

[[manbreak]]
## `exit()`, `quick_exit()`, `_Exit()` {#man-exit}

Exit the currently-running program

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

_Noreturn void exit(int status);

_Noreturn void quick_exit(int status);

_Noreturn void _Exit(int status);
```

### Description {.unnumbered .unlisted}

All these functions cause the program to exit, with various levels of
cleanup performed.

`exit()` does the most cleanup and is the most normal exit.

`quick_exit()` is the second most.

`_Exit()` unceremoniously drops everything and ragequits on the spot.

Calling either of `exit()` or `quick_exit()` causes their respective
`atexit()` or `at_quick_exit()` handlers to be called in the reverse
order in which they were registered.

`exit()` will flush all streams and delete all temporary files.

`quick_exit()` or `_Exit()` might not perform that nicety.

`_Exit()` doesn't call any of the at-exit handlers, either.

For all functions, the exit `status` is returned to the environment.

Defined exit statuses are:

|Status|Description|
|-|-|
|`EXIT_SUCCESS`|Typically returned when good things happen|
|`0`|Same as `EXIT_SUCCESS`|
|`EXIT_FAILURE`|Oh noes! Definitely failure!|
|Any positive value|Generally indicates another failure of some kind|

OS X note: `quick_exit()` is not supported.

### Return Value {.unnumbered .unlisted}

None of these functions ever return.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdlib.h>

int main(void)
{
    int contrived_exit_type = 1;

    switch(contrived_exit_type) {
        case 1:
            exit(EXIT_SUCCESS);

        case 2:
            // Not supported in OS X
            quick_exit(EXIT_SUCCESS);

        case 3:
            _Exit(2);
    }
}
```

### See Also {.unnumbered .unlisted}

[`atexit()`](#man-atexit),
[`at_quick_exit()`](#man-atexit)

[[manbreak]]
## `getenv()` {#man-getenv}

Get the value of an environment variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

char *getenv(const char *name);
```

### Description {.unnumbered .unlisted}

The environment often provides variables that are set before the program
run that you can access at runtime.

Of course the exact details are system dependent, but these variables
are key/value pairs, and you can get the value by passing the key to
`getenv()` as the `name` parameter.

You're not allowed to overwrite the string that's returned.

This is pretty limited in the standard, but your OS often provides
better functionality. See the [Environment Variables](#env-var) section
for more details.

### Return Value {.unnumbered .unlisted}

Returns a pointer to the environment variable value, or `NULL` if the
variable doesn't exist.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("PATH is %s\n", getenv("PATH"));
}
```

Output (truncated in my case):

``` {.default}
PATH is /usr/bin:/usr/local/bin:/usr/sbin:/home/beej/.cargo/bin [...]
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `system()` {#man-system}

Run an external program

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

int system(const char *string);
```

### Description {.unnumbered .unlisted}

This will run an external program and then return to the caller.

The manner in which it runs the program is system-defined, but typically
you can pass something to it just like you'd run on the command line,
searching the `PATH`, etc.

Not all systems have this capability, but you can test for it by passing
`NULL` to `system()` and seeing if it returns 0 (no command processor is
available) or non-zero (a command processor is available! Yay!)

If you're getting user input and passing it to the `system()` call, be
extremely careful to escape all special shell characters (everything
that's not alphanumeric) with a backslash to keep a villain from running
something you don't want them to.

### Return Value {.unnumbered .unlisted}

If `NULL` is passed, returns nonzero if a command processor is available
(i.e. `system()` will work at all).

Otherwise returns an implementation-defined value.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("Here's a directory listing:\n\n");

    system("ls -l");   // Run this command and return

    printf("\nAll done!\n");
}
```

Output:

``` {.default}
Here's a directory listing:

total 92
drwxr-xr-x 3 beej beej  4096 Oct 14 21:38 bin
drwxr-xr-x 2 beej beej  4096 Dec 20 20:07 examples
-rwxr-xr-x 1 beej beej 16656 Feb 23 21:49 foo
-rw-rw-rw- 1 beej beej   155 Feb 23 21:49 foo.c
-rw-r--r-- 1 beej beej  1350 Jan 27 22:11 Makefile
-rw-r--r-- 1 beej beej  4644 Jan 18 09:12 README.md
drwxr-xr-x 3 beej beej  4096 Feb 23 20:21 src
drwxr-xr-x 6 beej beej  4096 Feb 21 20:24 stage
drwxr-xr-x 2 beej beej  4096 Sep 27 20:54 translations
drwxr-xr-x 2 beej beej  4096 Sep 27 20:54 website

All done!
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `bsearch()` {#man-bsearch}

Binary Search (maybe) an array of objects

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

void *bsearch(const void *key, const void *base,
              size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));
```

### Description {.unnumbered .unlisted}

This crazy-looking function searches an array for a value.

It probably is a binary search or some fast, efficient search. But the
spec doesn't really say.

However, the array must be sorted! So binary search seems likely.

* `key` is a pointer to the value to find.
* `base` is a pointer to the start of the array---the array must be
  sorted!
* `nmemb` is the number of elements in the array.
* `size` is the `sizeof` each element in the array.
* `compar` is a pointer to a function that will compare the key against
  other values.

The comparison function takes the key as the first argument and the
value to compare against as the second. It should return a negative
number if the key is less than the value, `0` if the key equals the
value, and a positive number if the key is greater than the value.

This is commonly computed by taking the difference between the key and
the value to be compared. If subtraction is supported.

The return value from the [`strcmp()`](#man-strcmp) function can be used
for comparing strings.

Again, the array must be sorted according to the order of the comparison
function before running `bsearch()`. Luckily for you, you can just call
[`qsort()`](#man-qsort) with the same comparison function to get this
done.

It's a general-purpose function---it'll search any type of array for
anything. The catch is you have to write the comparison function.

And that's not as scary as it looks. Jump down to the example

### Return Value {.unnumbered .unlisted}

The function returns a pointer to the found value, or `NULL` if it can't
be found.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int compar(const void *key, const void *value)
{
    const int *k = key, *v = value;  // Need ints, not voids

    return *k - *v;
}

int main(void)
{
    int a[9] = {2, 6, 9, 12, 13, 18, 20, 32, 47};

    int *r, key;

    key = 12;  // 12 is in there
    r = bsearch(&key, a, 9, sizeof(int), compar);
    printf("Found %d\n", *r);

    key = 30;  // Won't find a 30
    r = bsearch(&key, a, 9, sizeof(int), compar);
    if (r == NULL)
        printf("Didn't find 30\n");

    // Searching with an unnamed key, pointer to 32
    r = bsearch(&(int){32}, a, 9, sizeof(int), compar);
    printf("Found %d\n", *r);  // Found it
}
```

Output:

``` {.default}
Found 12
Didn't find 30
Found 32
```

### See Also {.unnumbered .unlisted}

[`strcmp()`](#man-strcmp),
[`qsort()`](#man-qsort)


[[manbreak]]
## `qsort()` {#man-qsort}

Quicksort (maybe) some data

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));
```

### Description {.unnumbered .unlisted}

This function will quicksort (or some other sort, probably speedy) an
array of data in-place^["In-place" meaning that the original array will
hold the results; no new array is allocated.].

Like `bsearch()`, it's data-agnostic. Any data for which you can define
a relative ordering can be sorted, whether `int`s, `struct`s, or
anything else.

Also like `bsearch()`, you have to give a comparison function to do the
actual compare.

* `base` is a pointer to the start of the array to be sorted.
* `nmemb` is the number of elements in the array.
* `size` is the `sizeof` each element.
* `compar` is a pointer to the comparison function.

The comparison function takes pointers to two elements of the array as
arguments and compares them. It should return a negative number if the
first argument is less than the second, `0` if they are equal, and a
positive number if the first argument is greater than the second.

This is commonly computed by taking the difference between the first
argument and the second. If subtraction is supported.

The return value from the [`strcmp()`](#man-strcmp) function can provide
sort order for strings.

If you have to sort a `struct`, just subtract the specific field you
want to sort by.

This comparison function can be used by [`bsearch()`](#man-bsearch) to
do searches after the list is sorted.

To reverse the sort, subtract the second argument from the first, i.e.
negate the return value from `compar()`.


### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int compar(const void *elem0, const void *elem1)
{
    const int *x = elem0, *y = elem1;  // Need ints, not voids

    return *x - *y;
}

int main(void)
{
    int a[9] = {14, 2, 3, 17, 10, 8, 6, 1, 13};

    // Sort the list

    qsort(a, 9, sizeof(int), compar);

    // Print sorted list

    for (int i = 0; i < 9; i++)
        printf("%d ", a[i]);

    putchar('\n');

    // Use the same compar() function to binary search
    // for 17 (passed in as an unnamed object)

    int *r = bsearch(&(int){17}, a, 9, sizeof(int), compar);
    printf("Found %d!\n", *r);
}
```

Output:

``` {.default}
1 2 3 6 8 10 13 14 17
Found 17!
```

### See Also {.unnumbered .unlisted}

[`strcmp()`](#man-strcmp),
[`bsearch()`](#man-bsearch)

[[manbreak]]
## `abs()`, `labs()`, `llabs()` {#man-abs}

Compute the absolute value of an integer

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

int abs(int j);

long int labs(long int j);

long long int llabs(long long int j);
```

### Description {.unnumbered .unlisted}

Compute the absolute value of `j`. If you don't remember, that's how far
from zero `j` is.

In other words, if `j` is negative, return it as a positive. If it's
positive, return it as a positive. Always be positive. Enjoy life.

If the result cannot be represented, the behavior is undefined. Be
especially aware of the upper half of unsigned numbers.

### Return Value {.unnumbered .unlisted}

Returns the absolute value of `j`, $|j|$.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("|-2| = %d\n", abs(-2));
    printf("|4|  = %d\n", abs(4));
}
```

Output:

``` {.default}
|-2| = 2
|4|  = 4
```

### See Also {.unnumbered .unlisted}

[`fabs()`](#man-fabs)


[[manbreak]]
## `div()`, `ldiv()`, `lldiv()` {#man-div}

Compute the quotient and remainder of two numbers
 
### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

div_t div(int numer, int denom);

ldiv_t ldiv(long int numer, long int denom);

lldiv_t lldiv(long long int numer, long long int denom);
```

### Description {.unnumbered .unlisted}

These functions get you the quotient and remainder of a pair of numbers
in one go.

They return a structure that has two fields, `quot`, and `rem`, the
types of which match types of `numer` and `denom`. Note how each
function returns a different variant of `div_t`.

These `div_t` variants are equivalent to the following:

``` {.c}
typedef struct {
    int quot, rem;
} div_t;

typedef struct {
    long int quot, rem;
} ldiv_t;

typedef struct {
    long long int quot, rem;
} lldiv_t;
```

Why use these instead of the division operator?

The C99 Rationale says:

> Because C89 had implementation-defined semantics for division of
> signed integers when negative operands were involved, `div` and
> `ldiv`, and `lldiv` in C99, were invented to provide well-specified
> semantics for signed integer division and remainder operations. The
> semantics were adopted to be the same as in Fortran. Since these
> functions return both the quotient and the remainder, they also serve
> as a convenient way of efficiently modeling underlying hardware that
> computes both results as part of the same operation. Table 7.2
> summarizes the semantics of these functions.

Indeed, K&R2 (C89) says:

> The direction of truncation for `/` and the sign of the result for `%`
> are machine-dependent for negative operands [...]

The Rationale then goes on to spell out what the signs of the quotient
and remainder will be given the signs of a numerator and denominator
when using the `div()` functions:

|`numer`|`denom`|`quot`|`rem`|
|:-:|:-:|:-:|:-:|
|$+$|$+$|$+$|$+$|
|$-$|$+$|$-$|$-$|
|$+$|$-$|$-$|$+$|
|$-$|$-$|$+$|$-$|


### Return Value {.unnumbered .unlisted}

A `div_t`, `ldiv_t`, or `lldiv_t` structure with the `quot` and `rem`
fields loaded with the quotient and remainder of the operation of
`numer/denom`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    div_t d = div(64, -7);

    printf("64 / -7 = %d\n", d.quot);
    printf("64 %% -7 = %d\n", d.rem);
}
```

Output:

``` {.default}
64 / -7 = -9
64 % -7 = 1
```

### See Also {.unnumbered .unlisted}

[`fmod()`](#man-fmod),
[`remainder()`](#man-remainder)


[[manbreak]]
## `mblen()` {#man-mblen}

Return the number of bytes in a multibyte character

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

int mblen(const char *s, size_t n);
```

### Description {.unnumbered .unlisted}

If you have a multibyte character in a string, this will tell you how
many bytes long it is.

`n` is the maximum number of bytes `mblen()` will scan before giving up.

If `s` is a `NULL` pointer, tests if this encoding has state dependency,
as noted in the return value, below. It also resets the state, if there
is one.

The behavior of this function is influenced by the locale.

### Return Value {.unnumbered .unlisted}

Returns the number of bytes used to encode this character, or `-1` if
there is no valid multibyte character in the next `n` bytes.

Or, if `s` is NULL, returns true if this encoding has state dependency.

### Example {.unnumbered .unlisted}

For the example, I used my extended character set to put Unicode
characters in the source. If this doesn't work for you, use the `\uXXXX`
escape.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(void)
{
    setlocale(LC_ALL, "");

    printf("State dependency: %d\n", mblen(NULL, 0));
    printf("Bytes for €: %d\n", mblen("€", 5));
    printf("Bytes for \u00e9: %d\n", mblen("\u00e9", 5));  // \u00e9 == é
    printf("Bytes for &: %d\n", mblen("&", 5));
}
```

Output (in my case, the encoding is UTF-8, but your mileage may vary):

``` {.default}
State dependency: 0
Bytes for €: 3
Bytes for é: 2
Bytes for &: 1
```

### See Also {.unnumbered .unlisted}

[`mbtowc()`](#man-mbtowc),
[`mbstowcs())`](#man-mbstowcs),
[`setlocale()`](#man-setlocale)


[[manbreak]]
## `mbtowc()` {#man-mbtowc}

Convert a multibyte character to a wide character

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

int mbtowc(wchar_t * restrict pwc, const char * restrict s, size_t n);
```

### Description {.unnumbered .unlisted}

If you have a multibyte character, this function will convert it to a
wide character and stored at the address pointed to by `pwc`. Up to `n`
bytes of the multibyte character will be analyzed.

If `pwc` is `NULL`, the resulting character will not be stored. (Useful
for just getting the return value.)

If `s` is a `NULL` pointer, tests if this encoding has state dependency,
as noted in the return value, below. It also resets the state, if there
is one.

The behavior of this function is influenced by the locale.

### Return Value {.unnumbered .unlisted}

Returns the number of bytes used in the encoded wide character, or `-1`
if there is no valid multibyte character in the next `n` bytes.

Returns `0` if `s` points to the NUL character.

Or, if `s` is NULL, returns true if this encoding has state dependency.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

int main(void)
{
    setlocale(LC_ALL, "");

    printf("State dependency: %d\n", mbtowc(NULL, NULL, 0));

    wchar_t wc;
    int bytes;

    bytes = mbtowc(&wc, "€", 5);

    printf("L'%lc' takes %d bytes as multibyte char '€'\n", wc, bytes);
}
```

Output on my system:

``` {.default}
State dependency: 0
L'€' takes 3 bytes as multibyte char '€'
```

### See Also {.unnumbered .unlisted}

[`mblen()`](#man-mblen),
[`mbstowcs()`](#man-mbstowcs),
[`wcstombs()`](#man-wcstombs),
[`setlocale()`](#man-setlocale)

[[manbreak]]
## `wctomb()` {#man-wctomb}

Convert a wide character to a multibyte character

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

int wctomb(char *s, wchar_t wc);
```

### Description {.unnumbered .unlisted}

If you have your hands on a wide character, you can use this to make it
multibyte.

The wide character `wc` is stored as a multibyte character in the string
pointed to by `s`. The buffer `s` points to should be at least
`MB_CUR_MAX` characters long. Note that `MB_CUR_MAX` changes with
locale.

If `wc` is a NUL wide character, a NUL is stored in `s` after the bytes
needed to reset the shift state (if any).

If `s` is a `NULL` pointer, tests if this encoding has state dependency,
as noted in the return value, below. It also resets the state, if there
is one.

The behavior of this function is influenced by the locale.

### Return Value {.unnumbered .unlisted}

Returns the number of bytes used in the encoded multibyte character, or
`-1` if `wc` does not correspond to any valid multibyte character.

Or, if `s` is NULL, returns true if this encoding has state dependency.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

int main(void)
{
    setlocale(LC_ALL, "");

    printf("State dependency: %d\n", mbtowc(NULL, NULL, 0));

    int bytes;
    char mb[MB_CUR_MAX + 1];

    bytes = wctomb(mb, L'€');
    mb[bytes] = '\0';

    printf("L'€' takes %d bytes as multibyte char '%s'\n", bytes, mb);
}
```

Output on my system:

``` {.default}
State dependency: 0
L'€' takes 3 bytes as multibyte char '€'
```

### See Also {.unnumbered .unlisted}

[`mbtowc()`](#man-mbtowc),
[`mbstowcs()`](#man-mbstowcs),
[`wcstombs()`](#man-wcstombs),
[`setlocale()`](#man-setlocale)


[[manbreak]]
## `mbstowcs()` {#man-mbstowcs}

Convert a multibyte string to a wide character string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

size_t mbstowcs(wchar_t * restrict pwcs, const char * restrict s, size_t n);
```

### Description {.unnumbered .unlisted}

If you have a multibyte string (AKA a regular string), you can convert
it wto a wide character string with this function.

At most `n` wide characters are written to the destination `pwcs` from
the source `s`.

A NUL character is stored as a wide NUL character.

Non-portable POSIX extension: if you're using a POSIX-complaint library,
this function allows `pwcs` to be `NULL` if you're only interested in
the return value. Most notably, this will give you the number of
characters in a multibyte string (as opposed to
[`strlen()`](#man-strlen) which counts the bytes.)

### Return Value {.unnumbered .unlisted}

Returns the number of wide characters written to the destination `pwcs`.

If an invalid multibyte character was found, returns `(size_t)(-1)`.

If the return value is `n`, it means the result was _not_
NUL-terminated.

### Example {.unnumbered .unlisted}

This source uses an extended character set. If your compiler doesn't
support it, you'll have to replace them with `\u` escapes.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

int main(void)
{
    setlocale(LC_ALL, "");

    wchar_t wcs[128];
    char *s = "€200 for this spoon?";  // 20 characters

    size_t char_count, byte_count;

    char_count = mbstowcs(wcs, s, 128);
    byte_count = strlen(s);

    printf("Wide string: L\"%ls\"\n", wcs);
    printf("Char count : %zu\n", char_count);    // 20
    printf("Byte count : %zu\n\n", byte_count);  // 22 on my system

    // POSIX Extension that allows you to pass NULL for
    // the destination so you can just use the return
    // value (which is the character count of the string, 
    // if no errors have occurred)

    s = "§¶°±π€•";  // 7 characters

    char_count = mbstowcs(NULL, s, 0);  // POSIX-only, nonportable
    byte_count = strlen(s);

    printf("Multibyte str: \"%s\"\n", s);
    printf("Char count   : %zu\n", char_count);  // 7
    printf("Byte count   : %zu\n", byte_count);  // 16 on my system
}
```

Output on my system (byte count will depend on your encoding):

``` {.default}
Wide string: L"€200 for this spoon?"
Char count : 20
Byte count : 22

Multibyte str: "§¶°±π€•"
Char count   : 7
Byte count   : 16
```

### See Also {.unnumbered .unlisted}

[`mblen()`](#man-mblen),
[`mbtowc()`](#man-mbtowc),
[`wcstombs()`](#man-wcstombs),
[`setlocale()`](#man-setlocale)


[[manbreak]]
## `wcstombs()` {#man-wcstombs}

Convert a wide character string to a multibyte string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdlib.h>

size_t wcstombs(char * restrict s, const wchar_t * restrict pwcs, size_t n);
```

### Description {.unnumbered .unlisted}

If you have a wide character string and you want it as multibyte string,
this is the function for you!

It'll take the wide characters pointed to by `pwcs` and convert them to
multibyte characters stored in `s`. No more than `n` bytes will be
written to `s`.

Non-portable POSIX extension: if you're using a POSIX-complaint library,
this function allows `s` to be `NULL` if you're only interested in
the return value. Most notably, this will give you the number of
bytes needed to encode the wide characters in a multibyte string.

### Return Value {.unnumbered .unlisted}

Returns the number of bytes written to `s`, or `(size_t)(-1)` if one of
the characters can't be encoded into a multibyte string.

If the return value is `n`, it means the result was _not_
NUL-terminated.

### Example {.unnumbered .unlisted}

This source uses an extended character set. If your compiler doesn't
support it, you'll have to replace them with `\u` escapes.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

int main(void)
{
    setlocale(LC_ALL, "");

    char mbs[128];
    wchar_t *wcs = L"€200 for this spoon?";  // 20 characters

    size_t byte_count;

    byte_count = wcstombs(mbs, wcs, 128);

    printf("Wide string: L\"%ls\"\n", wcs);
    printf("Multibyte  : \"%s\"\n", mbs);
    printf("Byte count : %zu\n\n", byte_count);  // 22 on my system

    // POSIX Extension that allows you to pass NULL for
    // the destination so you can just use the return
    // value (which is the character count of the string, 
    // if no errors have occurred)

    wcs = L"§¶°±π€•";  // 7 characters

    byte_count = wcstombs(NULL, wcs, 0);  // POSIX-only, nonportable

    printf("Wide string: L\"%ls\"\n", wcs);
    printf("Byte count : %zu\n", byte_count);  // 16 on my system
}
```

Output on my system (byte count will depend on your encoding):

``` {.default}
Wide string: L"€200 for this spoon?"
Multibyte  : "€200 for this spoon?"
Byte count : 22

Wide string: L"§¶°±π€•"
Byte count : 16
```

### See Also {.unnumbered .unlisted}

[`mblen()`](#man-mblen),
[`wctomb()`](#man-wctomb),
[`mbstowcs()`](#man-mbstowcs),
[`setlocale()`](#man-setlocale)

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
