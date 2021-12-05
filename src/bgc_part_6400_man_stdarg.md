<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdarg.h>` Variable Arguments {#stdarg}

|Macro|Description|
|--------|----------------------|
|[`va_arg()`](#man-va_arg)|Get the next variable argument|
|[`va_copy()`](#man-va_copy)|Copy a `va_list` and the work done so far|
|[`va_end()`](#man-va_end)|Signify we're done processing variable arguments|
|[`va_start()`](#man-va_start)|Initialize a `va_list` to start variable argument processing|

This header file is what allows you to write functions that take a
variable number of arguments.

In addition to the macros, you get a new type that helps C keep track of
where it is in the variable-number-of-arguments-processing: `va_list`.
This type is opaque, and you'll be passing it around to the various
macros to help get at the arguments.

Note that every variadic function requires at least one non-variable
parameter. You need this to kick off processing with `va_start()`.

[[manbreak]]
## `va_arg()` {#man-va_arg}

Get the next variable argument

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdarg.h>

type va_arg(va_list ap, type);
```

### Description {.unnumbered .unlisted}

If you have a variable argument list you've initialized with
`va_start()`, pass it to this one along with the type of argument you're
trying to get, e.g.

``` {.c}
int   x = va_arg(args, int);
float y = va_arg(args, float);
```

### Return Value {.unnumbered .unlisted}

Evaluates to the value and type of the next variable argument.

### Example {.unnumbered .unlisted}

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

### See Also {.unnumbered .unlisted}

[`va_start()`](#man-va_start),
[`va_end()`](#man-va_end)

[[manbreak]]
## `va_copy()` {#man-va_copy}

Copy a `va_list` and the work done so far

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdarg.h>

void va_copy(va_list dest, va_list src);
```

### Description {.unnumbered .unlisted}

The main intended use of this is to save your state partway through
processing variable arguments so you can scan ahead and then rewind back
to the save point.

You pass in a `src` `va_list` and it copies it to `dest`.

If you've already called this once for a particular `dest`, you can't
call it (or `va_start()`) again with the same `dest` unless you call
`va_end()` on that `dest` first.

``` {.c}
va_copy(dest, src);
va_copy(dest, src2);  // BAD!

va_copy(dest, src);
va_start(dest, var);  // BAD!

va_copy(dest, src);
va_end(dest);
va_copy(dest, src2);  // OK!

va_copy(dest, src);
va_end(dest);
va_start(dest, var);  // OK!
```

### Return Value {.unnumbered .unlisted}

Returns nothing.

### Example {.unnumbered .unlisted}

Here's an example where we're adding together all the variable
arguments, but then we want to go back and add on all the numbers past
the first two, for example if the arguments are:

``` {.default}
10 20 30 40
```

First we add them all for `100`, and then we add on everything from the
third number on, so add on `30+40` for a total of `170`.

We'll do this by saving our place in the variable argument processing
with `va_copy` and then using that later to reprocess the trailing
arguments.

(And yes, I know there's a mathematical way to do this without all the
rewinding, but I'm having an heck of a time coming up with a good
example!)

``` {.c .numberLines}
#include <stdio.h>
#include <stdarg.h>

// Add all the numbers together, but then add on all the numbers
// past the second one again.
int contrived_adder(int count, ...)
{
    if (count < 3) return 0; // OK, I'm being lazy. You got me.

    int total = 0;

    va_list args, mid_args;

    va_start(args, count);

    for (int i = 0; i < count; i++) {

        // If we're at the second number, save our place in
        // mid_args:

        if (i == 2)
            va_copy(mid_args, args);

        total += va_arg(args, int);
    }

    va_end(args); // Done with this

    // But now let's start with mid_args and add all those on:
    for (int i = 0; i < count - 2; i++)
        total += va_arg(mid_args, int);

    va_end(mid_args); // Done with this, too

    return total;
}

int main(void)
{
    // 10+20+30 + 30 == 90
    printf("%d\n", contrived_adder(3, 10, 20, 30));

    // 10+20+30+40+50 + 30+40+50 == 270
    printf("%d\n", contrived_adder(5, 10, 20, 30, 40, 50));
}
```

### See Also {.unnumbered .unlisted}

[`va_start()`](#man-va_start),
[`va_arg()`](#man-va_arg),
[`va_end()`](#man-va_end)

[[manbreak]]
## `va_end()` {#man-va_end}

Signify we're done processing variable arguments

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdarg.h>

void va_end(va_list ap);
```

### Description {.unnumbered .unlisted}

After you've `va_start()`ed or `va_copy`'d a new `va_list`, you **must**
call `va_end()` with it before it goes out of scope. 

You also have to do this if you're going to call `va_start()` or
`va_copy()` _again_ on a variable you've already done that to.

Them's the rules if you want to avoid undefined behavior.

But just think of it as cleanup. You called `va_start()`, so you'll call
`va_end()` when you're done.

### Return Value {.unnumbered .unlisted}

Returns nothing.

### Example {.unnumbered .unlisted}

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

### See Also {.unnumbered .unlisted}

[`va_start()`](#man-va_start),
[`va_copy()`](#man-va_copy)

[[manbreak]]
## `va_start()` {#man-va_start}

Initialize a `va_list` to start variable argument processing

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdarg.h>

void va_start(va_list ap, parmN);
```

### Description {.unnumbered .unlisted}

You've declared a variable of type `va_list` to keep track of the
variable argument processing... now how to initialize it so you can
start calling `va_arg()` to get those arguments?

`va_start()` to the rescue!

What you do is pass in your `va_list`, here shown as parameter `ap`.
Just pass the list, not a pointer to it.

Then for the second argument to `va_start()`, you give the name of the
parameter that you want to start processing arguments _after_. This must
be the parameter right before the `...` in the argument list.

If you've already called `va_start()` on a particular `va_list` and you
want to call `va_start()` on it again, you **must** call `va_end()`
first!

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

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

### See Also {.unnumbered .unlisted}

[`va_arg()`](#man-va_arg),
[`va_end()`](#man-va_end)

<!--
[[manbreak]]
## `example()` `example()` `example()` {#man-example}

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