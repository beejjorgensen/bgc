
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

[[pagebreak]]
## `cnd_broadcast()` {#man-cnd_broadcast}

Wake up all threads waiting on a condition variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int cnd_broadcast(cnd_t *cond);
```

### Description {.unnumbered .unlisted}

This is just like `cnd_signal()` in that it wakes up threads that are
waiting on a condition variable.... except instead of just rousing one
thread, it wakes them all.

Of course, only one will get the mutex, and the rest will have to wait
their turn. But instead of being asleep waiting for a signal, they'll be
asleep waiting to reacquire the mutex. They're rearin' to go, in other
words.

This can make a difference in a specific set of circumstances where
`cnd_signal()` might leave you hanging.

If you're relying on subsequent threads to issue the next
`cnd_signal()`, but you have the `cnd_wait()` in a `while` loop^[Which
you should because of spurious wakeups.] that doesn't allow any threads
to escape, you'll be stuck. No more threads will be woken up from the
wait.

But if you `cnd_broadcast()`, all the threads will be woken, and
presumably at least one of them will be allowed to escape the `while`
loop, freeing it up to broadcast the next wakeup when its work is done.

### Return Value {.unnumbered .unlisted}

Returns `thrd_success` or `thrd_error` depending on how well things
went.

### Example {.unnumbered .unlisted}

In the example below, we launch a bunch of threads, but they're only
allowed to run if their ID matches the current ID. If it doesn't, they
go back to waiting.

If you `cnd_signal()` to wake the next thread, it might not be the one
with the proper ID to run. If it's not, it goes back to sleep and we
hang (because no thread is awake to hit `cnd_signal()` again).

But if you `cnd_broadcast()` to wake them all, then they'll all try (one
after another) to get out of the `while` loop. And one of them will make
it.

Try switching the `cnd_broadcast()` to `cnd_signal()` to see likely
deadlocks. It doesn't happen every time, but usually does.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

cnd_t condvar;
mtx_t mutex;

int run(void *arg)
{
    int id = *(int*)arg;

    static int current_id = 0;

    mtx_lock(&mutex);

    while (id != current_id) {
        printf("THREAD %d: waiting\n", id);
        cnd_wait(&condvar, &mutex);

        if (id != current_id)
            printf("THREAD %d: woke up, but it's not my turn!\n", id);
        else
            printf("THREAD %d: woke up, my turn! Let's go!\n", id);
    }

    current_id++;

    printf("THREAD %d: signaling thread %d to run\n", id, current_id);

    //cnd_signal(&condvar);
    cnd_broadcast(&condvar);
    mtx_unlock(&mutex);

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];
    int id[] = {4, 3, 2, 1, 0};

    mtx_init(&mutex, mtx_plain);
    cnd_init(&condvar);

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_create(t + i, run, id + i);

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_join(t[i], NULL);

    mtx_destroy(&mutex);
    cnd_destroy(&condvar);
}
```

Example run with `cnd_broadcast()`:

```
THREAD 4: waiting
THREAD 1: waiting
THREAD 3: waiting
THREAD 2: waiting
THREAD 0: signaling thread 1 to run
THREAD 2: woke up, but it's not my turn!
THREAD 2: waiting
THREAD 4: woke up, but it's not my turn!
THREAD 4: waiting
THREAD 3: woke up, but it's not my turn!
THREAD 3: waiting
THREAD 1: woke up, my turn! Let's go!
THREAD 1: signaling thread 2 to run
THREAD 4: woke up, but it's not my turn!
THREAD 4: waiting
THREAD 3: woke up, but it's not my turn!
THREAD 3: waiting
THREAD 2: woke up, my turn! Let's go!
THREAD 2: signaling thread 3 to run
THREAD 4: woke up, but it's not my turn!
THREAD 4: waiting
THREAD 3: woke up, my turn! Let's go!
THREAD 3: signaling thread 4 to run
THREAD 4: woke up, my turn! Let's go!
THREAD 4: signaling thread 5 to run
```

Example run with `cnd_signal()`:

```
THREAD 4: waiting
THREAD 1: waiting
THREAD 3: waiting
THREAD 2: waiting
THREAD 0: signaling thread 1 to run
THREAD 4: woke up, but it's not my turn!
THREAD 4: waiting

[deadlock at this point]
```

See how `THREAD 0` signaled that it was `THREAD 1`'s turn? But---bad
news---it was `THREAD 4` that got woken up. So no one continued the
process.  `cnd_broadcast()` would have woken them all, so eventually
`THREAD 1` would have run, gotten out of the `while`, and broadcast for
the next thread to run.

### See Also {.unnumbered .unlisted}

[`cnd_signal()`](#man-signal),
[`mtx_lock()`](#man-mtx_lock),
[`mtx_unlock()`](#man-mtx_unlock)

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