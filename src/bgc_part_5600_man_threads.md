
<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<threads.h>` Multithreading Functions {#threads}

Welcome to the multithread section!

[[pagebreak]]
## `call_once()` {#man-call_once}

Call a function one time no matter how many threads try

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

void call_once(once_flag *flag, void (*func)(void));
```

### Description {.unnumbered .unlisted}

If you have a bunch of threads running over the same piece of code that
calls a function, but you only want that function to run one time,
`call_once()` can help you out.

The catch is the function that is called doesn't return anything and
takes no arguments.

If you need more than that, you'll have to set a threadsafe flag such as
`atomic_flag`, or one that you protect with a mutex.

To use this, you need to pass it a pointer to a function to execute,
`func`, and also a pointer to a flag of type `once_flag`.

`once_flag` is an opaque type, so all you need to know is that you
initialize it to the value `ONCE_FLAG_INIT`.

### Return Value {.unnumbered .unlisted}

Returns nothing.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

once_flag of = ONCE_FLAG_INIT;  // Initialize it like this

void run_once_function(void)
{
    printf("I'll only run once!\n");
}

int run(void *arg)
{
    (void)arg;

    printf("Thread running!\n");

    call_once(&of, run_once_function);

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_create(t + i, run, NULL);

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_join(t[i], NULL);
}
```

Output (might vary per run):

```
Thread running!
Thread running!
I'll only run once!
Thread running!
Thread running!
Thread running!
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

<!--
[[pagebreak]]
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