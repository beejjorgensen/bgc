
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

[[pagebreak]]
## `cnd_destroy()` {#man-cnd_destroy}

Free up resources from a condition variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

void cnd_destroy(cnd_t *cond);
```

### Description {.unnumbered .unlisted}

This is the opposite of `cnd_init()` and should be called when all
threads are done using a condition variable.

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

General-purpose condition variable example here, but you can see the
`cnd_destroy()` down at the end.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

cnd_t condvar;
mtx_t mutex;

int run(void *arg)
{
    (void)arg;

    mtx_lock(&mutex);

    printf("Thread: waiting...\n");
    cnd_wait(&condvar, &mutex);
    printf("Thread: running again!\n");

    mtx_unlock(&mutex);

    return 0;
}

int main(void)
{
    thrd_t t;

    mtx_init(&mutex, mtx_plain);
    cnd_init(&condvar);

    printf("Main creating thread\n");
    thrd_create(&t, run, NULL);

    // Sleep 0.1s to allow the other thread to wait
    thrd_sleep(&(struct timespec){.tv_nsec=100000000L}, NULL);

    mtx_lock(&mutex);
    printf("Main: signaling thread\n");
    cnd_signal(&condvar);
    mtx_unlock(&mutex);

    thrd_join(t, NULL);

    mtx_destroy(&mutex);
    cnd_destroy(&condvar);  // <-- DESTROY CONDITION VARIABLE
}
```

Output:

```
Main creating thread
Thread: waiting...
Main: signaling thread
Thread: running again!
```

### See Also {.unnumbered .unlisted}

[`cnd_init()`](#man-cnd_init)

[[pagebreak]]
## `cnd_init()` {#man-cnd_init}

Initialize a condition variable to make it ready for use

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int cnd_init(cnd_t *cond);
```

### Description {.unnumbered .unlisted}

This is the opposite of `cnd_destroy()`. This prepares a condition
variable for use, doing behind-the-scenes work on it.

Don't use a condition variable without calling this first!

### Return Value {.unnumbered .unlisted}

If all goes well, returns `thrd_success`. It all doesn't go well, it
could return `thrd_nomem` if the system is out of memory, or
`thread_error` in the case of any other error.

### Example {.unnumbered .unlisted}

General-purpose condition variable example here, but you can see the
`cnd_init()` down at the start of `main()`.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

cnd_t condvar;
mtx_t mutex;

int run(void *arg)
{
    (void)arg;

    mtx_lock(&mutex);

    printf("Thread: waiting...\n");
    cnd_wait(&condvar, &mutex);
    printf("Thread: running again!\n");

    mtx_unlock(&mutex);

    return 0;
}

int main(void)
{
    thrd_t t;

    mtx_init(&mutex, mtx_plain);
    cnd_init(&condvar);      // <-- INITIALIZE CONDITION VARIABLE

    printf("Main creating thread\n");
    thrd_create(&t, run, NULL);

    // Sleep 0.1s to allow the other thread to wait
    thrd_sleep(&(struct timespec){.tv_nsec=100000000L}, NULL);

    mtx_lock(&mutex);
    printf("Main: signaling thread\n");
    cnd_signal(&condvar);
    mtx_unlock(&mutex);

    thrd_join(t, NULL);

    mtx_destroy(&mutex);
    cnd_destroy(&condvar);
}
```

Output:

```
Main creating thread
Thread: waiting...
Main: signaling thread
Thread: running again!
```

### See Also {.unnumbered .unlisted}

[`cnd_destroy()`](#man-cnd_destroy)

[[pagebreak]]
## `cnd_signal()` {#man-cnd_signal}

Wake up a thread waiting on a condition variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int cnd_signal(cnd_t *cond);
```

### Description {.unnumbered .unlisted}

If you have a thread (or a bunch of threads) waiting on a condition
variable, this function will wake one of them up to run.

Compare to `cnd_broadcast()` that wakes up all the threads. See the
[`cnd_broadcast()`](#man-cnd_broadcast) page for more information on
when you're want to use that versus this.

### Return Value {.unnumbered .unlisted}

Returns `thrd_success` or `thrd_error` depending on how happy your
program is.

### Example {.unnumbered .unlisted}

General-purpose condition variable example here, but you can see the
`cnd_signal()` in the middle of `main()`.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

cnd_t condvar;
mtx_t mutex;

int run(void *arg)
{
    (void)arg;

    mtx_lock(&mutex);

    printf("Thread: waiting...\n");
    cnd_wait(&condvar, &mutex);
    printf("Thread: running again!\n");

    mtx_unlock(&mutex);

    return 0;
}

int main(void)
{
    thrd_t t;

    mtx_init(&mutex, mtx_plain);
    cnd_init(&condvar);

    printf("Main creating thread\n");
    thrd_create(&t, run, NULL);

    // Sleep 0.1s to allow the other thread to wait
    thrd_sleep(&(struct timespec){.tv_nsec=100000000L}, NULL);

    mtx_lock(&mutex);
    printf("Main: signaling thread\n");
    cnd_signal(&condvar);    // <-- SIGNAL CHILD THREAD HERE!
    mtx_unlock(&mutex);

    thrd_join(t, NULL);

    mtx_destroy(&mutex);
    cnd_destroy(&condvar);
}
```

Output:

```
Main creating thread
Thread: waiting...
Main: signaling thread
Thread: running again!
```

### See Also {.unnumbered .unlisted}

[`cnd_init()`](#man-cnd_init),
[`cnd_destroy()`](#man-cnd_destroy)

[[pagebreak]]
## `cnd_timedwait()` {#man-cnd_timedwait}

Wait on a condition variable with a timeout

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mtx,
                  const struct timespec *restrict ts);
```

### Description {.unnumbered .unlisted}

This is like [`cnd_wait()`](#man-cnd_wait) except we get to specify a
timeout, as well.

Note that the thread still must reacquire the mutex to get more work
done even after the timeout. The the main difference is that regular
`cnd_wait()` will only try to get the mutex after a `cnd_signal()` or
`cnd_broadcast()`, whereas `cnd_timedwait()` will do that, too,  **and**
try to get the mutex after the timeout.

The timeout is specified as an absolute UTC time since Epoch. You can
get this with the [`timespec_get()`](#man-timespec_get) function and
then add values on to the result to timeout later than now, as shown in
the example.

Beware that you can't have more than 999999999 nanoseconds in the
`tv_nsec` field of the `struct timespec`. Mod those so they stay in
range.

### Return Value {.unnumbered .unlisted}

If the thread wakes up for a non-timeout reason (e.g. signal or
broadcast), returns `thrd_success`. If woken up due to timeout, returns
`thrd_timedout`. Otherwise returns `thrd_error`.

### Example {.unnumbered .unlisted}

This example has a thread wait on a condition variable for a maximum of
1.75 seconds. And it always times out because no one ever sends a
signal. Tragic.

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>
#include <threads.h>

cnd_t condvar;
mtx_t mutex;

int run(void *arg)
{
    (void)arg;

    mtx_lock(&mutex);

    struct timespec ts;

    // Get the time now
    timespec_get(&ts, TIME_UTC);

    // Add on 1.75 seconds from now
    ts.tv_sec += 1;
    ts.tv_nsec += 750000000L;

    // Handle nsec overflow
    ts.tv_sec += ts.tv_nsec / 1000000000L;
    ts.tv_nsec = ts.tv_nsec % 1000000000L;

    printf("Thread: waiting...\n");
    int r = cnd_timedwait(&condvar, &mutex, &ts);

    switch (r) {
        case thrd_success:
            printf("Thread: signaled!\n");
            break;

        case thrd_timedout:
            printf("Thread: timed out!\n");
            break;

        case thrd_error:
            printf("Thread: Some kind of error\n");
            break;
    }

    mtx_unlock(&mutex);

    return 0;
}

int main(void)
{
    thrd_t t;

    mtx_init(&mutex, mtx_plain);
    cnd_init(&condvar);

    printf("Main creating thread\n");
    thrd_create(&t, run, NULL);

    // Sleep 3s to allow the other thread to timeout
    thrd_sleep(&(struct timespec){.tv_sec=3}, NULL);

    thrd_join(t, NULL);

    mtx_destroy(&mutex);
    cnd_destroy(&condvar);
}
```

Output:

```
Main creating thread
Thread: waiting...
Thread: timed out!
```

### See Also {.unnumbered .unlisted}

[`cnd_wait()`](#man-cnd_wait),
[`timespec_get()`](#man-timespec_get)

[[pagebreak]]
## `cnd_wait()` {#man-cnd_wait}

Wait for a signal on a condition variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int cnd_wait(cnd_t *cond, mtx_t *mtx);
```

### Description {.unnumbered .unlisted}

This puts the calling thread to sleep until it is awakened by a call to
`cnd_signal()` or `cnd_broadcast()`.

### Return Value {.unnumbered .unlisted}

If everything's fantastic, returns `thrd_success`. Otherwise it returns
`thrd_error` to report that something has gone fantastically, horribly
awry.

### Example {.unnumbered .unlisted}

General-purpose condition variable example here, but you can see the
`cnd_wait()` in the `run()` function.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

cnd_t condvar;
mtx_t mutex;

int run(void *arg)
{
    (void)arg;

    mtx_lock(&mutex);

    printf("Thread: waiting...\n");
    cnd_wait(&condvar, &mutex);       // <-- WAIT HERE!
    printf("Thread: running again!\n");

    mtx_unlock(&mutex);

    return 0;
}

int main(void)
{
    thrd_t t;

    mtx_init(&mutex, mtx_plain);
    cnd_init(&condvar);

    printf("Main creating thread\n");
    thrd_create(&t, run, NULL);

    // Sleep 0.1s to allow the other thread to wait
    thrd_sleep(&(struct timespec){.tv_nsec=100000000L}, NULL);

    mtx_lock(&mutex);
    printf("Main: signaling thread\n");
    cnd_signal(&condvar);    // <-- SIGNAL CHILD THREAD HERE!
    mtx_unlock(&mutex);

    thrd_join(t, NULL);

    mtx_destroy(&mutex);
    cnd_destroy(&condvar);
}
```

Output:

```
Main creating thread
Thread: waiting...
Main: signaling thread
Thread: running again!
```

### See Also {.unnumbered .unlisted}

[`cnd_timedwait()`](#man-cnd_timedwait)

[[pagebreak]]
## `mtx_destroy()` {#man-mtx_destroy}

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

void mtx_destroy(mtx_t *mtx);
```

### Description {.unnumbered .unlisted}

The opposite of [`mtx_init()`](#man-mtx_init), this function frees up
any resources associated with the given mutex.

You should call this when all threads are done using the mutex.

### Return Value {.unnumbered .unlisted}

Returns nothing, the selfish ingrate!

### Example {.unnumbered .unlisted}

General-purpose mutex example here, but you can see the `mtx_destroy()`
down at the end.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

cnd_t condvar;
mtx_t mutex;

int run(void *arg)
{
    (void)arg;

    static int count = 0;

    mtx_lock(&mutex);

    printf("Thread: I got %d!\n", count);
    count++;

    mtx_unlock(&mutex);

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];

    mtx_init(&mutex, mtx_plain);

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_create(t + i, run, NULL);

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_join(t[i], NULL);

    mtx_destroy(&mutex);   // <-- DESTROY THE MUTEX HERE
}
```

Output:

```
Thread: I got 0!
Thread: I got 1!
Thread: I got 2!
Thread: I got 3!
Thread: I got 4!
```

### See Also {.unnumbered .unlisted}

[`mtx_init()`](#man-mtx_init)

[[pagebreak]]
## `mtx_init()` {#man-mtx_init}

Initialize a mutex for use

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int mtx_init(mtx_t *mtx, int type);
```

### Description {.unnumbered .unlisted}

Before you can use a mutex variable, you have to initialize it with this
call to get it all prepped and ready to go.

But wait! It's not quite that simple. You have to tell it what `type` of
mutex you want to create.

|Type|Description|
|-|-|
|`mtx_plain`|Regular ol' mutex|
|`mtx_timed`|Mutex that supports timeouts|
|`mtx_plain|mtx_recursive`|Recursive mutex|
|`mtx_timed|mtx_recursive`|Recursive mutex that supports timeouts|

As you can see, you can make a plain or timed mutex _recursive_ by
bitwise-ORing the value with `mtx_recursive`.

"Recursive" means that the holder of a lock can call `mtx_lock()`
multiple times on the same lock. (They have to unlock it an equal number
of times before anyone else can take the mutex.) This might ease coding
from time to time, especially if you call a function that needs to lock
the mutex when you already hold the mutex.

And the timeout gives a thread a chance to _try_ to get the lock for a
while, but then bail out if it can't get it in that timeframe. You use
the [`mtx_timedlock()`](#man-muted_timedlock) function with `mtx_timed`
mutexes.

### Return Value {.unnumbered .unlisted}

Returns `thrd_success` in a perfect world, and potentially `thrd_error`
in an imperfect one.

### Example {.unnumbered .unlisted}

General-purpose mutex example here, but you can see the `mtx_init()`
down at the top of `main()`:

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

cnd_t condvar;
mtx_t mutex;

int run(void *arg)
{
    (void)arg;

    static int count = 0;

    mtx_lock(&mutex);

    printf("Thread: I got %d!\n", count);
    count++;

    mtx_unlock(&mutex);

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];

    mtx_init(&mutex, mtx_plain);  // <-- CREATE THE MUTEX HERE

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_create(t + i, run, NULL);

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_join(t[i], NULL);

    mtx_destroy(&mutex);   // <-- DESTROY THE MUTEX HERE
}
```

Output:

```
Thread: I got 0!
Thread: I got 1!
Thread: I got 2!
Thread: I got 3!
Thread: I got 4!
```

### See Also {.unnumbered .unlisted}

[`mtx_destroy()`](#man-mtx_destroy)

[[pagebreak]]
## `mtx_lock()` {#man-mtx_lock}

Acquire a lock on a mutex

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int mtx_lock(mtx_t *mtx);
```

### Description {.unnumbered .unlisted}

If you're a thread and want to enter a critical section, do I have the
function for you!

A thread that calls this function will wait until it can acquire the
mutex, then it will grab it, wake up, and run!

If the mutex is recursive and is already locked by this thread, it will
be locked again and the lock count will increase. If the mutex is not
recursive and the thread already holds it, this call will error out.

### Return Value {.unnumbered .unlisted}

Returns `thrd_success` on goodness and `thrd_error` on badness.

### Example {.unnumbered .unlisted}

General-purpose mutex example here, but you can see the `mtx_lock()`
in the `run()` function:

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

cnd_t condvar;
mtx_t mutex;

int run(void *arg)
{
    (void)arg;

    static int count = 0;

    mtx_lock(&mutex);  // <-- LOCK HERE

    printf("Thread: I got %d!\n", count);
    count++;

    mtx_unlock(&mutex);

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];

    mtx_init(&mutex, mtx_plain);  // <-- CREATE THE MUTEX HERE

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_create(t + i, run, NULL);

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_join(t[i], NULL);

    mtx_destroy(&mutex);   // <-- DESTROY THE MUTEX HERE
}
```

Output:

```
Thread: I got 0!
Thread: I got 1!
Thread: I got 2!
Thread: I got 3!
Thread: I got 4!
```

### See Also {.unnumbered .unlisted}

[`mtx_unlock()`](#man-mtx_unlock),
[`mtx_timedlock()`](#man-mtx_timedlock)

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