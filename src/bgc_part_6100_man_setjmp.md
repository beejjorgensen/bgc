<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<setjmp.h>` Non-local Goto {#setjmp}

These functions enable you to rewind the call stack to an earlier point,
with a bunch of gotchas. See the [chapter on
`setjmp()`/`longjmp()`](#setjmp-longjmp) for more info.

|Function|Description|
|--------|----------------------|
|`longjmp()`|Return to the previously-placed bookmark|
|`setjmp()`|Bookmark this place to return to later|

There's also a new opaque type, `jmp_buf`, that holds all the
information needed to pull off this magic trick.

If you want your automatic local variables to be correct after a call to
`longjmp()`. declare them as `volatile` where you called `setjmp()`.

[[manbreak]]
## `setjmp()` {#man-setjmp}

Save this location as one to return to later

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <setjmp.h>

int setjmp(jmp_buf env);
```

### Description {.unnumbered .unlisted}

This is how you save your position so you can `longjmp()` back it,
later. Think of it as setting up a warp destination for later use.

Basically, you call this, giving it an `env` it can fill in with all the
information it needs to come back here later. This `env` is one you'll
pass to `longjmp()` later when you want to teleport back here.

And the really funky part is this can return two different ways:

1. It can return `0` from the call where you set up the jump
   destination.

2. If can return non-zero when you actually warp back here as the result
   of a call to `longjmp()`.

What you can do is check the return value to see which case has occurred.

You're only allowed to call `setjmp()` in a limited number of
circumstances.

1. As a standalone expression:

   ``` {.c}
   setjmp(env);
   ```

   You can also cast it to `(void)` if you really wanted to do such a
   thing.

2. As the complete controlling expression in an `if` or `switch`. 

   ``` {.c}
   if (setjmp(env)) { ... }
   
   switch (setjmp(env)) { ... }
   ```

   But not this as it's not the complete controlling expression in this
   case:

   ``` {.c}
   if (x == 2 && setjmp()) { ... }   // Undefined behavior
   ```

3. The same as (2), above, except with a comparison to an integer
   constant:

   ``` {.c}
   if (setjmp(env) == 0) { ... }

   if (setjmp(env) > 2) { ... }
   ```

4. As the operand to the not (`!`) operator:

   ``` {.c}
   if (!setjmp(env)) { ... }
   ```

Anything else is (you guessed it) undefined behavior!

This can be a macro or a function, but you'll treat it the same way in
any case.

### Return Value {.unnumbered .unlisted}

This one is funky. It returns one of two things:

Returns `0` if this was the call to `setjmp()` to set it up.

Returns non-zero if being here was the result of a call to `longjmp()`.
(Namely, it returns the value passed into the `longjmp()` function.)

### Example {.unnumbered .unlisted}

Here's a function that calls `setjmp()` to set things up (where it
returns `0`), then calls a couple levels deep into functions, and
finally short-circuits the return path by `longjmp()`ing back to the
place where `setjmp()` was called, earlier. This time, it passes `3490`
as a value, which `setjmp()` returns.

``` {.c .numberLines}
#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void depth2(void)
{
    printf("Entering depth 2\n");
    longjmp(env, 3490);           // Jump back to setjmp()!!
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
    switch (setjmp(env)) {
        case 0:
            printf("Calling into functions, setjmp() returned 0\n");
            depth1();
            printf("Returned from functions\n");  // This won't happen
            break;

        case 3490:
            printf("Bailed back to main, setjmp() returned 3490\n");
            break;
    }
}
```

When run, this outputs:

``` {.default}
Calling into functions, setjmp() returned 0
Entering depth 1
Entering depth 2
Bailed back to main, setjmp() returned 3490
```

Notice that the second `printf()` in `case 0` didn't run; it got jumped
over by `longjmp()`!

### See Also {.unnumbered .unlisted}

[`longjmp()`](#man-longjmp)

[[manbreak]]
## `longjmp()` {#man-longjmp}

Return to the previous `setjmp()` location

### Synopsis {.unnumbered .unlisted}

``` {.c}
 #include <setjmp.h>

_Noreturn void longjmp(jmp_buf env, int val);
```

### Description {.unnumbered .unlisted}

This returns to a previous call to `setjmp()` back in the call history.
`setjmp()` will return the `val` passed into `longjmp()`.

The `env` passed to `setjmp()` should be the same one you pass into
`longjmp()`.

There are a bunch of potential issues with doing this, so you'll want to
be careful that you avoid undefined behavior by not doing the following:

1. Don't call `longjmp()` if the corresponding `setjmp()` was in a
   different thread.

2. Don't call `longjmp()` if you didn't call `setjmp()` first.

3. Don't call `longjmp()` if the function that called `setjmp()` has
   completed.

4. Don't call `longjmp()` if the call to `setjmp()` had a variable
   length array (VLA) in scope and the scope has ended.

5. Don't call `longjmp()` if there are any VLAs in any active scopes
   between the `setjmp()` and the `longjmp()`. A good rule of thumb here
   is to not mix VLAs and `longjmp()`.

Though `longjmp()` attempts to restore the machine to the state at the
`setjmp()`, including local variables, there are some things that aren't
brought back to life:

* Non-volatile local variables that might have changed
* Floating point status flags
* Open files
* Any other component of the abstract machine

### Return Value {.unnumbered .unlisted}

This one is also funky in that it is one of the few functions in C that
never returns!

### Example {.unnumbered .unlisted}

Here's a function that calls `setjmp()` to set things up (where it
returns `0`), then calls a couple levels deep into functions, and
finally short-circuits the return path by `longjmp()`ing back to the
place where `setjmp()` was called, earlier. This time, it passes `3490`
as a value, which `setjmp()` returns.

``` {.c .numberLines}
#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void depth2(void)
{
    printf("Entering depth 2\n");
    longjmp(env, 3490);           // Jump back to setjmp()!!
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
    switch (setjmp(env)) {
        case 0:
            printf("Calling into functions, setjmp() returned 0\n");
            depth1();
            printf("Returned from functions\n");  // This won't happen
            break;

        case 3490:
            printf("Bailed back to main, setjmp() returned 3490\n");
            break;
    }
}
```

When run, this outputs:

``` {.default}
Calling into functions, setjmp() returned 0
Entering depth 1
Entering depth 2
Bailed back to main, setjmp() returned 3490
```

Notice that the second `printf()` in `case 0` didn't run; it got jumped
over by `longjmp()`!

### See Also {.unnumbered .unlisted}

[`setjmp()`](#man-setjmp)

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