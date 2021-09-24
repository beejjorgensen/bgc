<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Exiting a Program

Turns out there are a lot of ways to do this, and even ways to set up
"hooks" so that a function runs when a program exits.

In this chapter we'll dive in and check them out.

We already covered the meaning of the exit status code in the [Exit
Status](#exit-status) section, so jump back there and review if you have
to.

All the functions in this section are in `<stdlib.h>`.

## Normal Exits

We'll start with the regular ways to exit a program, and then jump to
some of the rarer, more esoteric ones.

When you exit a program normally, all open I/O streams are flushed and
temporary files removed. Basically it's a nice exit where everything
gets cleaned up and handled. It's what you want to do almost all the
time unless you have reasons to do otherwise.

### Returning From `main()`

If you've noticed, `main()` has a return type of `int`... and yet I've
rarely, if ever, been `return`ing anything from `main()` at all.

This is because for `main()` only (and I can't stress enough this
special case _only_ applies to `main()` and no other functions anywhere)
has an _implicit_ `return 0` if you fall off the end.

You can explicitly `return` from `main()` any time you want, and some
programmers feel it's more _Right_ to always have a `return` at the end
of `main()`. But if you leave it off, C will put one there for you.

So... here are the `return` rules for `main()`:

* You can return an exit status from `main()` with a `return` statement.
  `main()` is the only function with this special behavior. Using
  `return` in any other function just returns from that function to the
  caller.
* If you don't explicitly `return` and just fall off the end of
  `main()`, it's just as if you'd returned `0` or `EXIT_SUCCESS`.

### `exit()`

This one has also made an appearance a few times. If you call `exit()`
from anywhere in your program, it will exit at that point.

The argument you pass to `exit()` is the exit status.

### Setting Up Exit Handlers with `atexit()`

You can register functions to be called when a program exits whether by
returning from `main()` or calling the `exit()` function.

A call to `atexit()` with the handler function name will get it done.
You can register multiple exit handlers, and they'll be called in the
reverse order of registration.

Here's an example:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

void on_exit_1(void)
{
    printf("Exit handler 1 called!\n");
}

void on_exit_2(void)
{
    printf("Exit handler 2 called!\n");
}

int main(void)
{
    atexit(on_exit_1);
    atexit(on_exit_2);
    
    printf("About to exit...\n");
}
```

And the output is:

``` {.default}
About to exit...
Exit handler 2 called!
Exit handler 1 called!
```

## Quicker Exits with `quick_exit()`

This is similar to a normal exit, except:

* Open files might not be flushed.
* Temporary files might not be removed.
* `atexit()` handlers won't be called.

But there is a way to register exit handlers: call `at_quick_exit()`
analogously to how you'd call `atexit()`.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

void on_quick_exit_1(void)
{
    printf("Quick exit handler 1 called!\n");
}

void on_quick_exit_2(void)
{
    printf("Quick exit handler 2 called!\n");
}

void on_exit(void)
{
    printf("Normal exit--I won't be called!\n");
}

int main(void)
{
    at_quick_exit(on_quick_exit_1);
    at_quick_exit(on_quick_exit_2);

    atexit(on_exit);  // This won't be called

    printf("About to quick exit...\n");

    quick_exit(0);
}
```

Which gives this output:

``` {.default}
About to quick exit...
Quick exit handler 2 called!
Quick exit handler 1 called!
```

It works just like `exit()`/`atexit()`, except for the fact that file
flushing and cleanup might not be done.

## Nuke it from Orbit: `_Exit()`

Calling `_Exit()` exits immediately, period. No on-exit callback
functions are executed. Files won't be flushed. Temp files won't be
removed.

Use this if you have to exit _right fargin' now_.

## Exiting Sometimes: `assert()`

The `assert()` statement is used to insist that something be true, or
else the program will exit.

Devs often use an assert to catch Should-Never-Happen type errors.

``` {.c}
#define PI 3.14159

assert(PI > 3);   // Sure enough, it is, so carry on
```

versus:

``` {.c}
goats -= 100;

assert(goats >= 0);  // Can't have negative goats
```

In that case, if I try to run it and `goats` falls under `0`, this
happens:

``` {.default}
goat_counter: goat_counter.c:8: main: Assertion `goats >= 0' failed.
Aborted
```

and I'm dropped back to the command line.

This isn't very user-friendly, so it's only used for things the user
will never see. And often people [write their own assert macros that can
more easily be turned off](#my-assert).


## Abnormal Exit: `abort()`

You can use this if something has gone horribly wrong and you want to
indicate as much to the outside environment. This also won't necessarily
clean up any open files, etc.

I've rarely seen this used.

Some foreshadowing about _signals_: this actually works by raising a
`SIGABRT` which will end the process. 

What happens after that is up to the system, but on Unix-likes, it was
common to [flw[dump core|Core_dump]] as the program terminated.