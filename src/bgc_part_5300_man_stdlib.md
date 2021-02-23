<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdlib.h>` Standard Library Functions {#stdlib}

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


[[pagebreak]]
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
    double x = atof("3.141593");
    printf("%f\n", x);  // 3.141593
```

### See Also {.unnumbered .unlisted}

[`atoi()`](#man-atoi),
[`strtod()`](#man-strtod)

[[pagebreak]]
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
int x = atof("3490");
printf("%d\n", x);  // 3490
```

### See Also {.unnumbered .unlisted}

[`atof()`](#man-atof),
[`strtol()`](#man-strtol)

[[pagebreak]]
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
char *inp = "   123.4567beej";
char *badchar;

double val = strtod(inp, &badchar);

printf("Converted string to %f\n", val);
printf("Encountered bad characters: %s\n", badchar);

val = strtod("987.654321beej", NULL);
printf("Ignoring bad chars: %f\n", val);

val = strtod("11.2233", &badchar);

if (*badchar == '\0')
    printf("No bad chars: %f\n", val);
else
    printf("Found bad chars: %f, %s\n", val, badchar);
```

Output:

```
Converted string to 123.456700
Encountered bad characters: beej
Ignoring bad chars: 987.654321
No bad chars: 11.223300
```

### See Also {.unnumbered .unlisted}

[`atof()`](#man-atof),
[`strtol()`](#man-strtol)

[[pagebreak]]
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
```

Output:

```
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
[`setlocale()`](#man-setlocale)

[[pagebreak]]
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

You can explicitly seed the random number generator with `srand()`.

### Return Value {.unnumbered .unlisted}

Returns a random number in the range `0` to `RAND_MAX`, inclusive.

### Example {.unnumbered .unlisted}

Note that all of these examples don't produce perfectly uniform
distributions. But good enough for the untrained eye, and really common
in general use when mediocre random number quality is acceptable.

``` {.c .numberLines}
printf("RAND_MAX = %d\n", RAND_MAX);

printf("0 to 9: %d\n", rand() % 10);

printf("10 to 44: %d\n", rand() % 35 + 10);
printf("0 to 0.99999: %f\n", rand() / ((float)RAND_MAX + 1));
printf("10.5 to 15.7: %f\n", 10.5 + 5.2 * rand() / (float)RAND_MAX);
```

Output on my system:

```
RAND_MAX = 2147483647
0 to 9: 3
10 to 44: 21
0 to 0.99999: 0.783099
10.5 to 15.7: 14.651888
```

### See Also {.unnumbered .unlisted}

[`srand()`](#man-srand)

[[pagebreak]]
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

But if you can't hardcode the seed (because that would give you the same
sequence every time), how are you supposed to do this?

It's really common to use the number of seconds since January 1, 1970 to
seed the generator. This sounds pretty arbitrary except for the fact
that it's exactly the value returned by the library call `time(NULL)`.
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

```
4
20
22
14
9
```

Output from a subsequent run:

```
19
0
31
31
24
```

### See Also {.unnumbered .unlisted}

[`rand()`](#man-rand),
[`time()`](#man-time)


[[pagebreak]]
## `aligned_alloc()` {#man-example}

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

[[pagebreak]]
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
```

### See Also {.unnumbered .unlisted}

[`aligned_alloc()`](#man-aligned_alloc),
[`free()`](#man-free)

[[pagebreak]]
## `free() {#man-free}

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
// Allocate space for 5 ints
int *p = malloc(5 * sizeof(int));

p[0] = 12;
p[1] = 30;

// Free that space
free(p);
```

### See Also {.unnumbered .unlisted}

[`malloc()`](#man-malloc),
[`calloc()`](#man-malloc),
[`aligned_alloc()`](#man-aligned_alloc)

[[pagebreak]]
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
```

### See Also {.unnumbered .unlisted}

[`malloc()`](#man-malloc),
[`calloc()`](#man-malloc)

[[pagebreak]]
## `abort()`, `example()`, `example()` {#man-abort}

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

You can cause the equivalent of an `abort()` by calling
`raise(SIGABRT)`, but I don't know why you'd do that.

The only portable way to stop an `abort()` call midway is to use
`signal()` to catch `SIGABRT` and then `exit()` in the signal handler.

### Return Value {.unnumbered .unlisted}

This function never returns.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
if (bad_thing) {
    printf("This should never have happened!\n");
    fflush(stdout);  // Make sure the message goes out
    abort();
}
```

On my system, this outputs:

```
This should never have happened!
zsh: abort (core dumped)  ./foo
```

### See Also {.unnumbered .unlisted}

[`signal()`](#man-signal)

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