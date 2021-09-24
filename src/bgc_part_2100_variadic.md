<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Variadic Functions

_Variadic_ is a fancy word for functions that take arbitrary numbers of
arguments.

A regular function takes a specific number of arguments, for example:

``` {.c}
int add(int x, int y)
{
    return x + y;
}
```

You can only call that with exactly two arguments which correspond to
parameters `x` and `y`.

``` {.c}
add(2, 3);
add(5, 12);
```

But if you try it with more, the compiler won't let you:

``` {.c}
add(2, 3, 4);  // ERROR
add(5);        // ERROR
```

Variadic functions get around this limitation to a certain extent.

We've already seen a famous example in `printf()`! You can pass all
kinds of things to it.

``` {.c}
printf("Hello, world!\n");
printf("The number is %d\n", 2);
printf("The number is %d and pi is %f\n", 2, 3.14159);
```

It seems to not care how many arguments you give it!

Well, that's not entirely true. Zero arguments will give you an error:

``` {.c}
printf();  // ERROR
```

This leads us to one of the limitations of variadic functions in C: they
must have at least one argument.

But aside from that, they're pretty flexible, even allows arguments to
have different types just like `printf()` does.

Let's see how they work!

## Ellipses in Function Signatures

So how does it work, syntactically?

What you do is put all the arguments that _must_ be passed first (and
remember there has to be at least one) and after that, you put `...`.
Just like this:

``` {.c}
void func(int a, ...)   // Literally 3 dots here
```

Here's some code to demo that:

``` {.c}
#include <stdio.h>

void func(int a, ...)
{
    printf("a is %d\n", a);  // Prints "a is 2"
}

int main(void)
{
    func(2, 3, 4, 5, 6);
}
```

So, great, we can get that first argument that's in variable `a`, but
what about the rest of the arguments? How do you get to them?

Here's where the fun begins!

## Getting the Extra Arguments

You're going to want to include `<stdarg.h>` to make any of this work.

First things first, we're going to use a special variable of type
`va_list` (variable argument list) to keep track of which variable we're
accessing at a time.

The idea is that we first start processing arguments with a call to
`va_start()`, process each argument in turn with `va_arg()`, and then,
when done, wrap it up with `va_end()`.

When you call `va_start()`, you need to pass in the _last named
parameter_ (the one just before the `...`) so it knows where to start
looking for the additional arguments.

And when you call `va_arg()` to get the next argument, you have to tell
it the type of argument to get next.

Here's a demo that adds together an arbitrary number of integers. The
first argument is the number of integers to add together. We'll make use
of that to figure out how many times we have to call `va_arg()`.

``` {.c .numberLines}
#include <stdio.h>
#include <stdarg.h>

int add(int count, ...)
{
    int total = 0;
    va_list va;

    va_start(va, count);   // Start with arguments after "count"

    for (int i = 0; i < count; i++) {
        int n = va_arg(va, int);   // Get the next int

        total += n;
    }

    va_end(va);  // All done

    return total;
}

int main(void)
{
    printf("%d\n", add(4, 6, 2, -4, 17));  // 6 + 2 - 4 + 17 = 21
    printf("%d\n", add(2, 22, 44));        // 22 + 44 = 66
}
```

When `printf()` is called, it uses the number of `%d`s (or whatever) in
the format string to know how many more arguments there are!

If the syntax of `va_arg()` is looking strange to you (because of that
loose type name floating around in there), you're not alone. These are
implemented with preprocessor macros in order to get all the proper
magic in there.

## `va_list` Functionality

What is that `va_list` variable we're using up there? It's an opaque
variable^[That is, us lowly developers aren't supposed to know what's in
there or what it means. The spec doesn't dictate what it is in detail.]
that holds information about which argument we're going to get next with
`va_arg()`. You see how we just call `va_arg()` over and over? The
`va_list` variable is a placeholder that's keeping track of progress so
far.

But we have to initialize that variable to some sensible value. That's
where `va_start()` comes into play.

When we called `va_start(va, count)`, above, we were saying, "Initialize
the `va` variable to point to the variable argument _immediately after_
`count`."

And that's _why_ we need to have at least one named variable in our
argument list^[Honestly, it would be possible to remove that limitation
from the language, but the idea is that the macros `va_start()`,
`va_arg()`, and `va_end()` should be able to be written in C. And to
make that happen, we need some way to initialize a pointer to the
location of the first parameter. And to do that, we need the _name_ of
the first parameter. It would require a language extension to make this
possible, and so far the committee hasn't found a rationale for doing
so.].

Once you have that pointer to the initial parameter, you can easily get
subsequent argument values by calling `va_arg()` repeatedly. When you
do, you have to pass in your `va_list` variable (so it can keep on
keeping track of where you are), as well as the type of argument you're
about to copy off.

It's up to you as a programmer to figure out which type you're going to
pass to `va_arg()`. In the above example, we just did `int`s. But in the
case of `printf()`, it uses the format specifier to determine which type
to pull off next.

And when you're done, call `va_end()` to wrap it up. You **must** (the
spec says) call this on a particular `va_list` variable before you
decide to call either `va_start()` or `va_copy()` on it again. I know we
haven't talked about `va_copy()` yet.

So the standard progression is:

* `va_start()` to initialize your `va_list` variable
* Repeatedly `va_arg()` to get the values
* `va_end()` to deinitialize your `va_list` variable

I also mentioned `va_copy()` up there; it makes a copy of your `va_list`
variable in the exact same state. That is, if you haven't started with
`va_arg()` with the source variable, the new one won't be started,
either. If you've consumed 5 variables with `va_arg()` so far, the copy
will also reflect that.

`va_copy()` can be useful if you need to scan ahead through the
arguments but need to also remember your current place.

## Library Functions That Use `va_list`s

One of the other uses for these is pretty cool: writing your own custom
`printf()` variant. It would be a pain to have to handle all those
format specifiers right? All zillion of them?

Luckily, there are `printf()` variants that accept a working `va_list`
as an argument. You can use these to wrap up and make your own custom
`printf()`s!

These functions start with the letter `v`, such as `vprintf()`,
`vfprintf()`, `vsprintf()`, and `vsnprintf()`. Basically all your
`printf()` golden oldies except with a `v` in front.

Let's make a function `my_printf()` that works just like `printf()`
except it takes an extra argument up front.

``` {.c .numberLines}
#include <stdio.h>
#include <stdarg.h>

int my_printf(int serial, const char *format, ...)
{
    va_list va;

    // Do my custom work
    printf("The serial number is: %d\n", serial);

    // Then pass the rest off to vprintf()
    va_start(va, format);
    int rv = vprintf(format, va);
    va_end(va);

    return rv;
}

int main(void)
{
    int x = 10;
    float y = 3.2;

    my_printf(3490, "x is %d, y is %f\n", x, y);
}
```

See what we did there?  On lines 12-14 we started a new `va_list`
variable, and then just passed it right into `vprintf()`. And it knows
just want to do with it, because it has all the `printf()` smarts
built-in.

We still have to call `va_end()` when we're done, though, so don't
forget that!