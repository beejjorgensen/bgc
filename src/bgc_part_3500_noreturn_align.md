<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Function Specifiers, Alignment Specifiers/Operators

These don't see a heck of a lot of use in my experience, but we'll
cover them here for the sake of completeness.

## Function Specifiers

[i[Function specifiers]<]

When you declare a function, you can give the compiler a couple tips
about how the functions could or will be used. This enables or encourages
the compiler to make certain optimizations.

### `inline` for Speed---Maybe

[i[`inline` function specifier]<]

You can declare a function to be inline like this:

``` {.c}
static inline int add(int x, int y) {
    return x + y;
}
```

This is meant to encourage the compiler to make this function call as
fast as possible. And, historically, one way to do this was _inlining_,
which means that the body of the function would be embedded in its
entirety where the call was made. This would avoid all the overhead of
setting up the function call and tearing it down at the expense of
larger code size as the function was copied all over the place instead
of being reused.

The quick-and-dirty things to remember are:

1. You probably don't need to use `inline` for speed. Modern compilers
   know what's best.

2. If you do use it for speed, use it with file scope, i.e. `static
   inline`. This avoids the messy rules of external linkage and inline
   functions.

Stop reading this section now.

Glutton for punishment, eh?

Let's try leaving the `static` off.

``` {.c .numberLines}
#include <stdio.h>

inline int add(int x, int y)
{
    return x + y;
}

int main(void)
{
    printf("%d\n", add(1, 2));
}
```

`gcc` gives a linker error on `add()`^[Unless you compile with
optimizations on (probably)! But I think when it does this, it's not
behaving to spec.]. The spec requires that if you have a non-`extern`
inline function you must also provide a version with external linkage.

So you'd have to have an `extern` version somewhere else for this to
work. If the compiler has both an `inline` function in the current file
and an external version of the same function elsewhere, it gets to
choose which one to call. So I highly recommend they be the same.

Another thing you can do is to declare the function as `extern inline`.
This will attempt to inline in the same file (for speed), but will also
create a version with external linkage.

[i[`inline` function specifier]>]

### `noreturn` and `_Noreturn` {#noreturn}

[i[`noreturn` function specifier]<]
[i[`_Noreturn` function specifier]<]

This indicates to the compiler that a particular function will not ever
return to its caller, i.e. the program will exit by some mechanism
before the function returns.

It allows the compiler to perhaps perform some optimizations around the
function call.

It also allows you to indicate to other devs that some program logic
depends on a function _not_ returning.

You'll likely never need to use this, but you'll see it on some library
calls like
[fl[`exit()`|https://beej.us/guide/bgclr/html/split/stdlib.html#man-exit]]
and
[fl[`abort()`|https://beej.us/guide/bgclr/html/split/stdlib.html#man-abort]].

The built-in keyword is `_Noreturn`, but if it doesn't break your
existing code, everyone would recommend including `<stdnoreturn.h>` and
using the easier-to-read `noreturn` instead.

It's undefined behavior if a function specified as `noreturn` actually
does return. It's computationally dishonest, see.

Here's an example of using `noreturn` correctly:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

noreturn void foo(void) // This function should never return!
{
    printf("Happy days\n");

    exit(1);            // And it doesn't return--it exits here!
}

int main(void)
{
    foo();
}
```

If the compiler detects that a `noreturn` function could return, it
might warn you, helpfully.

Replacing the `foo()` function with this:

``` {.c}
noreturn void foo(void)
{
    printf("Breakin' the law\n");
}
```

gets me a warning:

``` {.default}
foo.c:7:1: warning: function declared 'noreturn' should not return
```

[i[`noreturn` function specifier]>]
[i[`_Noreturn` function specifier]>]
[i[Function specifiers]>]

## Alignment Specifiers and Operators

[i[Alignment]<]

[flw[_Alignment_|Data_structure_alignment]] is all about multiples of
addresses on which objects can be stored. Can you store this at any
address? Or must it be a starting address that's divisible by 2? Or 8?
Or 16?

If you're coding up something low-level like a memory allocator that
interfaces with your OS, you might need to consider this. Most devs go
their careers without using this functionality in C.

### `alignas` and `_Alignas`

[i[`alignas` alignment specifier]<]
[i[`_Alignas` alignment specifier]<]

This isn't a function. Rather, it's an _alignment specifier_ that you
can use with a variable declaration.

The built-in specifier is `_Alignas`, but the header `<stdalign.h>`
defines it as `alignas` for something better looking.

If you need your `char` to be aligned like an `int`, you can force it
like this when you declare it:

``` {.c}
char alignas(int) c;
```

You can also pass a constant value or expression in for the alignment.
This has to be something supported by the system, but the spec stops
short of dictating what values you can put in there. Small powers of 2
(1, 2, 4, 8, and 16) are generally safe bets.

``` {.c}
char alignas(8) c;   // align on 8-byte boundaries
```

If you want to align at the maximum used alignment by your system,
include `<stddef.h>` and use the type `max_align_t`, like so:

``` {.c}
char alignas(max_align_t) c;
```

You could potentially _over-align_ by specifying an alignment more than
that of `max_align_t`, but whether or not such things are allowed is
system dependent.

[i[`alignas` alignment specifier]>]
[i[`_Alignas` alignment specifier]>]

### `alignof` and `_Alignof`

[i[`alignof` operator]<]
[i[`_Alignof` operator]<]

This operator will return the address multiple a particular type uses
for alignment on this system. For example, maybe `char`s are aligned
every 1 address, and `int`s are aligned every 4 addresses.

The built-in operator is `_Alignof`, but the header `<stdalign.h>`
defines it as `alignof` if you want to look cooler.

Here's a program that will print out the alignments of a variety of
different types. Again, these will vary from system to system. Note that
the type `max_align_t` will give you the maximum alignment used by the
system.

``` {.c .numberLines}
#include <stdalign.h>
#include <stdio.h>     // for printf()
#include <stddef.h>    // for max_align_t

struct t {
    int a;
    char b;
    float c;
};

int main(void)
{
    printf("char       : %zu\n", alignof(char));
    printf("short      : %zu\n", alignof(short));
    printf("int        : %zu\n", alignof(int));
    printf("long       : %zu\n", alignof(long));
    printf("long long  : %zu\n", alignof(long long));
    printf("double     : %zu\n", alignof(double));
    printf("long double: %zu\n", alignof(long double));
    printf("struct t   : %zu\n", alignof(struct t));
    printf("max_align_t: %zu\n", alignof(max_align_t));
}
```

Output on my system:

``` {.default}
char       : 1
short      : 2
int        : 4
long       : 8
long long  : 8
double     : 8
long double: 16
struct t   : 16
max_align_t: 16
```

[i[`alignof` operator]>]
[i[`_Alignof` operator]>]

## `memalignment()` Function

[i[`memalignment()` function]<]

New in C23!

(Caveat: none of my compilers support this function yet, so the code is
largely untested.)

`alignof` is great if you know the type of your data. But what if you're
_woefully ignorant_ of the type, and only have a pointer to the data?

How could that even happen?

Well, with our good friend the `void*`, of course. We can't pass that to
`alignof`, but what if we need to know the alignment of the thing it
points to?

We might want to know this if we're about to use the memory for
something that has significant alignment needs. For example, atomic and
floating types often behave badly if misaligned.

So with this function we can check the alignment of some data as long as
we have a pointer to that data, even if it's a `void*`.

Let's do a quick test to see if a void pointer is well-aligned for use
as an atomic type, and, if so, let's get a variable to use it as that
type:

``` {.c}
void foo(void *p)
{
    if (memalignment(p) >= alignof(atomic int)) {
        atomic int *i = p;
        do_things(i);
    } else
        puts("This pointer is no good as an atomic int\n");

...
```

I suspect you will rarely (to the point of never, likely) need to use
this function unless you're doing some low-level stuff.

[i[`memalignment()` function]>]

And there you have it. Alignment!

[i[Alignment]>]
