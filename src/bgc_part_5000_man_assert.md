<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<assert.h>` Runtime and Compile-time Diagnostics {#assert}

|Macro|Description|
|--------|----------------------|
|`assert()`|Runtime assertion|
|`static_assert()`|Compile-time assertion|

This functionality has to do with things that Should Never Happen™. If
you have something that should never be true and you want your program
to bomb out because it happened, this is the header file for you.

There are two types of assertions: compile-time assertions (called
"static assertions") and runtime assertions. If the assertion _fails_
(i.e. the thing that you need to be true is not true) then the program
will bomb out either at compile-time or runtime.

## Macros

If you define the macro `NDEBUG` **before** you include `<assert.h>`,
then the `assert()` macro will have no effect. You can define `NDEBUG`
to be anything, but `1` seems like a good value.

Since `assert()` causes your program to bomb out at runtime, you might
not desire this behavior when you go into production. Defining `NDEBUG`
causes `assert()` to be ignored.

`NDEBUG` has no effect on `static_assert()`.

[[manbreak]]
## `assert()` {#man-assert}

Bomb out at runtime if a condition fails

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <assert.h>

void assert(scalar expression);
```

### Description {.unnumbered .unlisted}

You pass in an expression to this macro. If it evaluates to false, the
program will crash with an assertion failure (by calling the `abort()`
function).

Basically, you're saying, "Hey, I'm assuming this condition is true, and
if it's not, I don't want to continue running."

This is used while debugging to make sure no unexpected conditions
arise. And if you find during development that the condition does arise,
maybe you should modify the code to handle it before going to
production.

If you've defined the macro `NDEBUG` to any value before `<assert.h>`
was included, the `assert()` macro is ignored. This is a good idea
before production.

Unlike `static_assert()`, this macro doesn't allow you to print an
arbitrary message. If you want to do this, check out [the example in the
Preprocessor chapter](#my-assert).

### Return Value {.unnumbered .unlisted}

This macro doesn't return (since it calls `abort()` which never
returns).

If `NDEBUG` is set, the macro evaluates to `((void)0)`, which does
nothing.

### Example {.unnumbered .unlisted}

Here's a function that divides the size of our goat herd. But we're
assuming we'll never get a `0` passed to us.

So we assert that `amount != 0`... and if it is, the program aborts/

``` {.c .numberLines}
// #define NDEBUG 1   // uncomment this to disable the assert

#include <assert.h>

void divide_goat_herd_by(int amount)
{
    assert(amount != 0);

    goat_count /= amount;
}
```

When I run this and pass `0` to the function, I get the following on my
system (the exact output may vary):

``` {.default}
Assertion failed: (amount != 0), function divide_goat_herd_by, file foo.c, line 7.
```

### See Also {.unnumbered .unlisted}

[`static_assert()`](#man-static_assert),
[`abort()`](#man-abort)

[[manbreak]]
## `static_assert()` {#man-static_assert}

Bomb out at compile-time if a condition fails

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <assert.h>

static_assert(constant-expression, string-literal);
```

### Description {.unnumbered .unlisted}

This macro prevents your program from even compiling if a condition
isn't true.

And it prints the string literal you give it.

Basically if `constant-expression` is false, then compilation will cease
and the `string-literal` will be printed.

The constant expression must be truly constant--just values, no
variables. And the same is true for the string literal: no variables,
just a literal string in double quotes. (It has to be this way since
the program's not running at this point.)

### Return Value {.unnumbered .unlisted}

Not applicable, as this is a compile-time feature.

### Example {.unnumbered .unlisted}

Here's a partial example with an algorithm that presumably has poor
performance or memory issues if the size of the local array is too
large. We prevent that eventuality at compile-time by catching it with
the `static_assert()`.

``` {.c .numberLines}
#include <assert.h>

#define ARRAY_SIZE 5150

void some_algorithm(void)
{
    static_assert(ARRAY_SIZE <= 32, "ARRAY_SIZE too large");

    int array[ARRAY_SIZE];

    // ...
```

On my system, when I try to compile it, this prints (your output may vary):

``` {.default}
foo.c:7:5: error: static_assert failed due to requirement '5150 <= 32'
    "ARRAY_SIZE too large"
    static_assert(ARRAY_SIZE <= 32, "ARRAY_SIZE too large");
    ^             ~~~~~~~~~~~~~~~~
```

### See Also {.unnumbered .unlisted}

[`assert()`](#man-assert)

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