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



### Return Value {.unnumbered .unlisted}

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
```

### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
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