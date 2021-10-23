<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Function Specifiers, Alignment Specifiers/Operators

These don't see a heck of a lot of use in my experience, but we'll
cover them here for the sake of completeness.

## Function Specifiers

When you declare a function, you can give the compiler a couple tips
about how the functions could or will be used. This enables or encourages
the compiler to make certain optimizations.

### `inline` for Speed---Maybe

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

That would seem to be the end of the story, but it's not. `inline` comes
with a whole pile of rules that make for _interesting times_. I'm not
sure I even understand them all, and behavior seems to vary from
compiler to compiler.

The short answer is define the `inline` function as `static` in the
file that you need it. And then use it in that one file. And you never
have to worry about the rest of it.

But if you're wondering, here are more fun times.

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

`gcc` gives a linker error on `add()`... unless you compile with
optimizations on (probably)!

See, a compiler can choose to inline or not, but if it chooses not to,
you're left with no function at all. `gcc` doesn't inline unless you're
doing an optimized build.

One way around this is to define a non-`inline` external linkage version
of the function elsewhere, and that one will be used when the `inline`
one isn't. But you as the programmer can't determine which, portably. If
both are available, it's unspecified which one the compiler chooses.
With `gcc` the inline function will be used if you're compiling with
optimizations, and the non-inline one will be used otherwise. Even if
the bodies of these functions are completely different. Zany!

Another way is to declare the function as `extern inline`. This will attempt to
inline in this file, but will also create a version with external
linkage. And so `gcc` will use one or the other depending on
optimizations, but at least they're the same function.

Unless, of course, you have another source file with an `inline`
function of the same name; it will use its `inline` function or the one
with external linkage depending on optimizations.

But let's say you're doing a build where the compiler _is_ inlining the
function. In that case, you can just use a plain `inline` in the
definition. However, there are now additional restrictions.

You can't refer to any `static` globals:

``` {.c}
static int b = 13;

inline int add(int x, int y)
{
    return x + y + b;  // BAD -- can't refer to b
}
```

And you can't define any non-`const` `static` local variables:

``` {.c}
inline int add(int x, int y)
{
    static int b = 13;  // BAD -- can't define static

    return x + y + b;
}
```

But making it `const` is OK:

``` {.c}
inline int add(int x, int y)
{
    static const int b = 13;  // OK -- static const

    return x + y + b;
}
```

Now, you know the functions are `extern` by default, so we should be
able to call `add()` from another file. You'd like to think that,
wouldn't you!

But you can't! If it's just a plain `inline`, it's similar to `static`:
it's only visible in that file.

Okay, so what if you throw an `extern` on there? Now we're coming full
circle to when we discussed having `inline` mixed with functions with
external linkage.

If both are visible, the compiler can choose which to use. 

Let's do a demo of this behavior. We'll have two files, `foo.c` and
`bar.c`. They'll both call `func()` which is `inline` in `foo.c` and
external linkage in `bar.c`.

Here's `foo.c` with the `inline`.

``` {.c .numberLines}
// foo.c

#include <stdio.h>

inline char *func(void)
{
    return "foo's function";
}

int main(void)
{
    printf("foo.c: %s\n", func());

    void bar(void);
    bar();
}
```

Recall that unless we're doing an optimized build with `gcc`. `func()`
will vanish and we'll get a linker error. Unless, or course, we have a
version with external linkage defined elsewhere.

And we do. In `bar.c`.

``` {.c .numberLines}
// bar.c

#include <stdio.h>

char *func(void)
{
    return "bar's function";
}

void bar(void)
{
    printf("bar.c: %s\n", func());
}

```

So the question is, what is the output?

Seems like when we call `func()` from `foo.c`, it should print "`foo's function`". And
from `bar.c`, that `func()` should print "`bar's function`".

And if I compile with `gcc` with optimizations^[You can do this with
`-O` on the command line.] it will use inline functions, and we'll get
the expected:

``` {.default}
foo.c: foo's function
bar.c: bar's function
```

Great!

But if we compile in `gcc` without optimizations, it ignores the inline
function and uses the external linkage `func()` from `bar.c`! And we get
this:

``` {.default}
foo.c: bar's function
bar.c: bar's function
```

In short, the rules are surprisingly complex. I give myself a good 30%
chance of having described them correctly.

### `noreturn` and `_Noreturn` {#noreturn}

This indicates to the compiler that a particular function will not ever
return to its caller, i.e. the program will exit by some mechanism
before the function returns.

It allows the compiler to perhaps perform some optimizations around the
function call.

It also allows you to indicate to other devs that some program logic
depends on a function _not_ returning.

You'll likely never need to use this, but you'll see it on some library
calls like [`exit()`](#man-exit) and [`abort()`](#man-abort).

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

## Alignment Specifiers and Operators

[flw[_Alignment_|Data_structure_alignment]] is all about multiples of
addresses on which objects can be stored. Can you store this at any
address? Or must it be a starting address that's divisible by 2? Or 8?
Or 16?

If you're coding up something low-level like a memory allocator that
interfaces with your OS, you might need to consider this. Most devs go
their careers without using this functionality in C.

### `alignas` and `_Alignas`

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

### `alignof` and `_Alignof`

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
    printf("struct t   : %zu\n", alignof(long double));
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

And there you have it. Alignment!
