<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdbool.h>` Boolean Types {#stdbool}

This is a small header file that defines a number of convenient Boolean
macros. If you really need that kind of thing.

|Macro  |Description                         |
|-------|------------------------------------|
|`bool` |Type for Boolean, expands to `_Bool`|
|`true` |True value, expands to `1`          |
|`false`|False value, expands to `0`         |

There's on more macro that I'm not putting in the table because it's
such a long name it'll blow up the table alignment:

``` {.c}
__bool_true_false_are_defined
```

which expands to `1`.

## Example

Here's a lame example that shows off these macros.

``` {.c .numberLines}
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    bool x;

    x = (3 > 2);

    if (x == true)
        printf("The universe still makes sense.\n");

    x = false;

    printf("x is now %d\n", x);  // 0
}
```

Output:

``` {.default}
The universe still makes sense.
x is now 0
```

## `_Bool`?

What's the deal with `_Bool`? Why didn't they just make it `bool`?

Well, there was a lot of C code out there where people had defined their
own `bool` type and adding an official `bool` would have broken those
`typedef`s. 

But C has already reserved all identifiers that start with an underscore
followed by a capital letter, so it was clear to make up a new `_Bool`
type and go with that.

And, if you know your code can handle it, you can include this header to
get all this juicy syntax.

One more note on conversions: unlike converting to `int`, the _only_
thing that converts to `false` in a `_Bool` is a scalar zero value.
Anything at all that's not zero, like `-3490`, `0.12`, or `NaN`,
converts to `true`.