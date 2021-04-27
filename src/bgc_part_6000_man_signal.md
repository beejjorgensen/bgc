<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<signal.h>` signal handling {#signal}

Handle signals.

[[manbreak]]
## `signal()` {#man-signal}

Set a signal handler for a given signal

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <signal.h>

void (*signal(int sig, void (*func)(int)))(int);
```

### Description {.unnumbered .unlisted}

How's **that** for a function definition?

Let's ignore it for a moment and just talk about what this function
does.

(Before we start, I'd like to point out that your OS probably has far
superior signal handling capabilities than the C standard library. For
instance, Unix-likes should check out `sigaction`.)

When a signal is raised, _something_ is going to happen. This function
lets you decide to do one of these things when the signal is raised:

* Ignore the signal
* Perform the default action
* Have a specific function called

The `signal()` function takes two arguments. The first, `sig`, is the
name of the signal to handle.

|Signal|Description|
|-|-|
|`SIGABRT`|Raised when `abort()` is called|
|`SIGFPE`|Floating-point arithmetic exception|
|`SIGILL`|CPU tried to execute an illegal instruction|
|`SIGINT`|Interrupt signal, as if `CTRL-C` were pressed|
|`SIGSEGV`|Segmention Violation: attempted to access restricted memory|
|`SIGTERM`|Termination request[^b43b]|

[^b43b]: As if might be sent from Unix's `kill` command.]

So that's the first bit when you call `signal()`---tell it the signal in
question:

``` {.c}
signal(SIGINT, ...
```

But what's that `func` parameter?

For spoilers, it's a pointer to a function that takes an `int` argument
and returns `void`. We can use this to call an arbitrary function when
the signal occurs.

Before we do that, though, let's look at the easy ones: telling the
system to ignore the signal or perform the default action (which it does
by default if you never call `signal()`).

You can set `func` to one of two special values to make this happen:

|`func`|Description|
|-|-|
|`SIG_DFL`|Perform the default action on this signal|
|`SIG_IGN`|Ignore this signal|

For example:

``` {.c}
signal(SIGTERM, SIG_DFL);  // Default action on SIGTERM
signal(SIGINT, SIG_IGN);   // Ignore SIGINT
```

But what if you want to have your own handler do something instead of
the default or ignoring it? You can pass in your own function to be
called. That's what the crazy function signature is partially about.
It's saying that the argument can be a pointer to a function that takes
an `int` argument and returns `void`.

So if you wanted to call your handler, you could have code like this:

``` {.c}
int handler(int sig)
{
    // Handle the signal
}

int main(void)
{
    signal(SIGINT, handler);
```

What can you do in the signal handler? Not much.

If the signal is due to `abort()` or `raise()`, the handler can't call
`raise()`.

If the signal is **not** due to `abort()` or `raise()`, you're only
allowed to call these functions from the standard library (though the
spec doesn't prohibit calling other non-library functions):

* [`abort()`](#man-abort)
* [`_Exit()`](#man-exit)
* [`quick_exit()`](#man-exit)
* Functions in [`<stdatomic.h>`](#man-stdatomic) when the atomic
  arguments are lock-free
* `signal()` with a first argument equivalent to the argument that was
  passed into the handler

In addition, if the signal was **not** due to `abort()` or `raise()`,
the handler can't access any object with static or thread-storage
duration unless it's lock-free.

An exception is that you can assign to (but not read from!) a variable
of type `volatile sig_atomic_t`.

It's up to the implementation, but the signal handler might be reset to
`SIG_DFL` just before the handler is called.

It's undefined behavior to call `signal()` in a multithreaded program.

It's undefined behavior to return from the handler for `SIGFPE`,
`SIGILL`, `SIGSEGV`, or any implementation-defined value.

The implementation might or might not prevent other signals from arising
while in the signal handler.

### Return Value {.unnumbered .unlisted}

### Example {.unnumbered .unlisted}

Here's a program that causes `SIGINT` to be ignored. Commonly you
trigger this signal by hitting `CTRL-C`.

``` {.c .numberLines}
#include <stdio.h>
#include <signal.h>

int main(void)
{
    signal(SIGINT, SIG_IGN);

    printf("You can't hit CTRL-C to exit this program. Try it!\n\n");
    printf("Press return to exit, instead.");
    fflush(stdout);
    getchar();
}
``` 

Output:

```
You can't hit CTRL-C to exit this program. Try it!

Press return to exit, instead.^C^C^C^C^C^C^C^C^C^C^C
```

This program sets the signal handler, then raises the signal. The signal
handler fires.

``` {.c .numberLines}
#include <stdio.h>
#include <signal.h>

void handler(int sig)
{
    // Undefined behavior to call printf() if this handler was not
    // as the result of a raise(), i.e. if you hit CTRL-C.

    printf("Got signal %d!\n", sig);

    // Common to reset the handler just in case the implementation set
    // it to SIG_DFL when the signal occurred.

    signal(sig, handler);
}

int main(void)
{
    signal(SIGINT, handler);

    raise(SIGINT);
    raise(SIGINT);
    raise(SIGINT);
}
```

Output:

```
Got signal 2!
Got signal 2!
Got signal 2!
```

This example catches `SIGINT` but then sets a flag to `1`. Then the main
loop sees the flag and exits.

``` {.c .numberLines}
#include <stdio.h>
#include <signal.h>

volatile sig_atomic_t x;

void handler(int sig)
{
    x = 1;
}

int main(void)
{
    signal(SIGINT, handler);

    printf("Hit CTRL-C to exit\n");
    while (x != 1);
}
```

### See Also {.unnumbered .unlisted}

[`raise()`](#man-raise),
[`abort()`](#man-abort)

<!--
[[manbreak]]
## `example()` {#man-example}

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