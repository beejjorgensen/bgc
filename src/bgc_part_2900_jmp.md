<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Long Jumps with `setjmp`, `longjmp`

We've already seen `goto`, which jumps in function scope. But
`longjmp()` allows you to jump back to an earlier point in execution,
back to a function that called this one.

There are a lot of limitations and caveats, but this can be a useful
function for bailing out from deep in the call stack back up to an
earlier state.

In my experience, this is very rarely-used functionality.

## Using `setjmp` and `longjmp`

The dance we're going to do here is to basically put a bookmark in
execution with `setjmp()`. Later on, we'll call `longjmp()` and it'll
jump back to the earlier point in execution where we set the bookmark
with `setjmp()`.

And it can do this even if you've called subfunctions.

Here's a quick demo where we call into functions a couple levels deep
and then bail out of it.

We're going to use a file scope variable `env` to keep the _state_ of
things when we call `setjmp()` so we can restore them when we call
`longjmp()` later.

``` {.c .numberLines}
#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void depth2(void)
{
    printf("Entering depth 2\n");
    longjmp(env, 3490);           // Bail out
    printf("Leaving depth 2\n");  // This won't happen
}

void depth1(void)
{
    printf("Entering depth 1\n");
    depth2();
    printf("Leaving depth 1\n");  // This won't happen
}

int main(void)
{
    int x = setjmp(env);

    if (x == 0) {
        printf("Calling into functions, x is %d\n", x);
        depth1();
        printf("Returned from functions\n");  // This won't happen
    } else {
        printf("Bailed back to main, x is %d\n", x);
    }
}
```

When run, this outputs:

```
Calling into functions, x is 0
Entering depth 1
Entering depth 2
Bailed back to main, x is 3490
```

If you try to take that output and match it up with the code, it's clear
there's some really _funky_ stuff going on.

One of the most notable things is that `setjmp()` returns _twice_. What
the actual frank? What is this sorcery?!

So here's the deal: if `setjmp()` returns `0`, it means that you've
successfully set the "bookmark" at that point.

If it returns non-zero, it means you've just returned to the "bookmark"
set earlier.

This way you can tell the difference between setting the bookmark and
returning to it later.

So when the code, above, calls `setjmp()` the first time, 

## Pitfalls

## VLA Pitfalls
