
<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<threads.h>` Multithreading Functions {#threads}

|Function|Description|
|-|-|
|[`call_once()`](#man-call_once)|Call a function one time no matter how many threads try|
|[`cnd_broadcast()`](#man-cnd_broadcast)|Wake up all threads waiting on a condition variable|
|[`cnd_destroy()`](#man-cnd_destroy)|Free up resources from a condition variable|
|[`cnd_init()`](#man-cnd_init)|Initialize a condition variable to make it ready for use|
|[`cnd_signal()`](#man-cnd_signal)|Wake up a thread waiting on a condition variable|
|[`cnd_timedwait()`](#man-cnd_timedwait)|Wait on a condition variable with a timeout|
|[`cnd_wait()`](#man-cnd_wait)|Wait for a signal on a condition variable|
|[`mtx_destroy()`](#man-mtx_destroy)|Cleanup a mutex when done with it|
|[`mtx_init()`](#man-mtx_init)|Initialize a mutex for use|
|[`mtx_lock()`](#man-mtx_lock)|Acquire a lock on a mutex|
|[`mtx_timedlock()`](#man-mtx_timedlock)|Lock a mutex allowing for timeout|
|[`mtx_trylock()`](#man-mtx_trylock)|Try to lock a mutex, returning if not possible|
|[`mtx_unlock()`](#man-mtx_unlock)|Free a mutex when you're done with the critical section|
|[`thrd_create()`](#man-thrd_create)|Create a new thread of execution|
|[`thrd_current()`](#man-thrd_current)|Get the ID of the calling thread|
|[`thrd_detach()`](#man-thrd_detach)|Automatically clean up threads when they exit|
|[`thrd_equal()`](#man-thrd_equal)|Compare two thread descriptors for equality|
|[`thrd_exit()`](#man-thrd_exit)|Stop and exit this thread|
|[`thrd_join()`](#man-thrd_join)|Wait for a thread to exit|
|[`thrd_yield()`](#man-thrd_yield)|Stop running that other threads might run|
|[`tss_create()`](#man-tss_create)|Create new thread-specific storage|
|[`tss_delete()`](#man-tss_delete)|Clean up a thread-specific storage variable|
|[`tss_get()`](#man-tss_get)|Get thread-specific data|
|[`tss_set()`](#man-tss_set)|Set thread-specific data|

We have a bunch of good things at our disposal with this one:

* Threads
* Mutexes
* Condition Variables
* Thread-Specific Storage
* And, last but not least, the always-fun `call_once()` function!

Enjoy!

[[manbreak]]
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

``` {.default}
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

[[manbreak]]
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

``` {.default}
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

``` {.default}
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

[[manbreak]]
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

``` {.default}
Main creating thread
Thread: waiting...
Main: signaling thread
Thread: running again!
```

### See Also {.unnumbered .unlisted}

[`cnd_init()`](#man-cnd_init)

[[manbreak]]
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

``` {.default}
Main creating thread
Thread: waiting...
Main: signaling thread
Thread: running again!
```

### See Also {.unnumbered .unlisted}

[`cnd_destroy()`](#man-cnd_destroy)

[[manbreak]]
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

``` {.default}
Main creating thread
Thread: waiting...
Main: signaling thread
Thread: running again!
```

### See Also {.unnumbered .unlisted}

[`cnd_init()`](#man-cnd_init),
[`cnd_destroy()`](#man-cnd_destroy)

[[manbreak]]
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
            return 1;

        case thrd_error:
            printf("Thread: Some kind of error\n");
            return 2;
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

``` {.default}
Main creating thread
Thread: waiting...
Thread: timed out!
```

### See Also {.unnumbered .unlisted}

[`cnd_wait()`](#man-cnd_wait),
[`timespec_get()`](#man-timespec_get)

[[manbreak]]
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

``` {.default}
Main creating thread
Thread: waiting...
Main: signaling thread
Thread: running again!
```

### See Also {.unnumbered .unlisted}

[`cnd_timedwait()`](#man-cnd_timedwait)

[[manbreak]]
## `mtx_destroy()` {#man-mtx_destroy}

Cleanup a mutex when done with it

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

``` {.default}
Thread: I got 0!
Thread: I got 1!
Thread: I got 2!
Thread: I got 3!
Thread: I got 4!
```

### See Also {.unnumbered .unlisted}

[`mtx_init()`](#man-mtx_init)

[[manbreak]]
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
the [`mtx_timedlock()`](#man-mtx_timedlock) function with `mtx_timed`
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

``` {.default}
Thread: I got 0!
Thread: I got 1!
Thread: I got 2!
Thread: I got 3!
Thread: I got 4!
```

### See Also {.unnumbered .unlisted}

[`mtx_destroy()`](#man-mtx_destroy)

[[manbreak]]
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

``` {.default}
Thread: I got 0!
Thread: I got 1!
Thread: I got 2!
Thread: I got 3!
Thread: I got 4!
```

### See Also {.unnumbered .unlisted}

[`mtx_unlock()`](#man-mtx_unlock),
[`mtx_trylock()`](#man-mtx_trylock),
[`mtx_timedlock()`](#man-mtx_timedlock)

[[manbreak]]
## `mtx_timedlock()` {#man-mtx_timedlock}

Lock a mutex allowing for timeout

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int mtx_timedlock(mtx_t *restrict mtx, const struct timespec *restrict ts);
```

### Description {.unnumbered .unlisted}

This is just like [`mtx_lock()`](#man-mtx_lock) except you can add a
timeout if you don't want to wait forever.

The timeout is specified as an absolute UTC time since Epoch. You can
get this with the [`timespec_get()`](#man-timespec_get) function and
then add values on to the result to timeout later than now, as shown in
the example.

Beware that you can't have more than 999999999 nanoseconds in the
`tv_nsec` field of the `struct timespec`. Mod those so they stay in
range.

### Return Value {.unnumbered .unlisted}

If everything works and the mutex is obtained, returns `thrd_success`.
If a timeout happens first, returns `thrd_timedout`.

Otherwise, returns `thrd_error`. Because if nothing is right, everything
is wrong.

### Example {.unnumbered .unlisted}

This example has a thread wait on a mutex for a maximum of 1.75 seconds.
And it always times out because no one ever sends a signal.

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>
#include <threads.h>

mtx_t mutex;

int run(void *arg)
{
    (void)arg;

    struct timespec ts;

    // Get the time now
    timespec_get(&ts, TIME_UTC);

    // Add on 1.75 seconds from now
    ts.tv_sec += 1;
    ts.tv_nsec += 750000000L;

    // Handle nsec overflow
    ts.tv_sec += ts.tv_nsec / 1000000000L;
    ts.tv_nsec = ts.tv_nsec % 1000000000L;

    printf("Thread: waiting for lock...\n");
    int r = mtx_timedlock(&mutex, &ts);

    switch (r) {
        case thrd_success:
            printf("Thread: grabbed lock!\n");
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

    mtx_lock(&mutex);

    printf("Main creating thread\n");
    thrd_create(&t, run, NULL);

    // Sleep 3s to allow the other thread to timeout
    thrd_sleep(&(struct timespec){.tv_sec=3}, NULL);

    mtx_unlock(&mutex);

    thrd_join(t, NULL);

    mtx_destroy(&mutex);
}
```

Output:

``` {.default}
Main creating thread
Thread: waiting for lock...
Thread: timed out!
```

### See Also {.unnumbered .unlisted}

[`mtx_lock()`](#man-mtx_lock),
[`mtx_trylock()`](#man-mtx_trylock),
[`timespec_get()`](#man-timespec_get)

[[manbreak]]
## `mtx_trylock()` {#man-mtx_trylock}

Try to lock a mutex, returning if not possible

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int mtx_trylock(mtx_t *mtx);
```

### Description {.unnumbered .unlisted}

This works just like [`mtx_lock`](#man-mtx_lock) except that it returns
instantly if a lock can't be obtained.

The spec notes that there's a chance that `mtx_trylock()` might
spuriously fail with `thrd_busy` even if there are no other threads
holding the lock. I'm not sure why this is, but you should defensively
code against it.

### Return Value {.unnumbered .unlisted}

Returns `thrd_success` if all's well. Or `thrd_busy` if some other
thread holds the lock. Or `thrd_error`, which means something went
right. I mean "wrong".

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <time.h>
#include <threads.h>

mtx_t mutex;

int run(void *arg)
{
    int id = *(int*)arg;

    int r = mtx_trylock(&mutex);   // <-- TRY TO GRAB THE LOCK

    switch (r) {
        case thrd_success:
            printf("Thread %d: grabbed lock!\n", id);
            break;

        case thrd_busy:
            printf("Thread %d: lock already taken :(\n", id);
            return 1;

        case thrd_error:
            printf("Thread %d: Some kind of error\n", id);
            return 2;
    }

    mtx_unlock(&mutex);

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];
    int id[THREAD_COUNT];

    mtx_init(&mutex, mtx_plain);

    for (int i = 0; i < THREAD_COUNT; i++) {
        id[i] = i;
        thrd_create(t + i, run, id + i);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_join(t[i], NULL);

    mtx_destroy(&mutex);
}
```

Output (varies by run):

``` {.default}
Thread 0: grabbed lock!
Thread 1: lock already taken :(
Thread 4: lock already taken :(
Thread 3: grabbed lock!
Thread 2: lock already taken :(
```

### See Also {.unnumbered .unlisted}

[`mtx_lock()`](#man-mtx_lock),
[`mtx_timedlock()`](#man-mtx_timedlock),
[`mtx_unlock()`](#man-mtx_unlock)

[[manbreak]]
## `mtx_unlock()` {#man-mtx_unlock}

Free a mutex when you're done with the critical section

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int mtx_unlock(mtx_t *mtx);
```

### Description {.unnumbered .unlisted}

After you've done all the dangerous stuff you have to do, wherein the
involved threads should not be stepping on each other's toes... you can
free up your stranglehold on the mutex by calling `mtx_unlock()`.

### Return Value {.unnumbered .unlisted}

Returns `thrd_success` on success. Or `thrd_error` on error. It's not
very original in this regard.

### Example {.unnumbered .unlisted}

General-purpose mutex example here, but you can see the `mtx_unlock()`
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

    mtx_lock(&mutex);

    printf("Thread: I got %d!\n", count);
    count++;

    mtx_unlock(&mutex);  // <-- UNLOCK HERE

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

    mtx_destroy(&mutex);
}
```

Output:

``` {.default}
Thread: I got 0!
Thread: I got 1!
Thread: I got 2!
Thread: I got 3!
Thread: I got 4!
```

### See Also {.unnumbered .unlisted}

[`mtx_lock()`](#man-mtx_lock),
[`mtx_timedlock()`](#man-mtx_timedlock),
[`mtx_trylock()`](#man-mtx_trylock)

[[manbreak]]
## `thrd_create()` {#man-thrd_create}

Create a new thread of execution

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
```

### Description {.unnumbered .unlisted}

Now _you_ have the POWER!

Right?

This is how you launch new threads to make your program do multiple
things at once^[Well, as at least as many things as you have free cores.
Your OS will schedule them as it can.]!

In order to make this happen, you need to pass a pointer to a `thrd_t`
that will be used to represent the thread you're spawning.

That thread will start running the function you pass a pointer to in
`func`. This is a value of type `thrd_start_t`, which is a pointer to a
function that returns an `int` and takes a single `void*` as a
parameter, i.e.:

``` {.c}
int thread_run_func(void *arg)
```

And, as you might have guessed, the pointer you pass to `thrd_create()`
for the `arg` parameter is passed on to the `func` function. This is how
you can give additional information to the thread when it starts up.

Of course, for `arg`, you have to be sure to pass a pointer to an object
that is thread-safe or per-thread.

If the thread returns from the function, it exits just as if it had
called `thrd_exit()`.

Finally, the value that the `func` function returns can be picked up by
the parent thread with `thrd_join()`.

### Return Value {.unnumbered .unlisted}

In the case of goodness, returns `thrd_success`. If you're out of
memory, will return `thrd_nomem`. Otherwise, `thrd_error`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

int run(void *arg)
{
    int id = *(int*)arg;

    printf("Thread %d: I'm alive!!\n", id);

    return id;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];
    int id[THREAD_COUNT];  // One of these per thread

    for (int i = 0; i < THREAD_COUNT; i++) {
        id[i] = i; // Let's pass in the thread number as the ID
        thrd_create(t + i, run, id + i);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        int res;

        thrd_join(t[i], &res);

        printf("Main: thread %d exited with code %d\n", i, res);
    }
}
```

Output (might vary from run to run):

``` {.default}
Thread 1: I'm alive!!
Thread 0: I'm alive!!
Thread 3: I'm alive!!
Thread 2: I'm alive!!
Main: thread 0 exited with code 0
Main: thread 1 exited with code 1
Main: thread 2 exited with code 2
Main: thread 3 exited with code 3
Thread 4: I'm alive!!
Main: thread 4 exited with code 4
```

### See Also {.unnumbered .unlisted}

[`thrd_exit()`](#man-thrd_exit),
[`thrd_join()`](#man-thrd_join)

[[manbreak]]
## `thrd_current()` {#man-thrd_current}

Get the ID of the calling thread

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

thrd_t thrd_current(void);
```

### Description {.unnumbered .unlisted}

Each thread has an opaque ID of type `thrd_t`. This is the value we see
get initialized when we call [`thrd_create()`](#man-thrd_create).

But what if you want to get the ID of the currently running thread?

No problem! Just call this function and it will be returned to you.

Why? Who knows!

Well, to be honest, I could see it being used a couple places.

1. You could use it to have a thread detach itself with `thrd_detach()`.
   I'm not sure why you'd want to do this, however.
2. You could use it to compare this thread's ID with another you have
   stored in a variable somewhere by using the `thrd_equal()` function.
   Seems like the most legit use.
3. ...
4. Profit!

If anyone has another use, please let me know.

### Return Value {.unnumbered .unlisted}

Returns the calling thread's ID.

### Example {.unnumbered .unlisted}

Here's a general example that shows getting the current thread ID and
comparing it to a previously-recorded thread ID and taking exciting
action based on the result! Starring Arnold Schwarzenegger! 

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

thrd_t first_thread_id;

int run(void *arg)
{
    (void)arg;

    thrd_t my_id = thrd_current();   // <-- GET MY THREAD ID

    if (thrd_equal(my_id, first_thread_id))
        printf("I'm the first thread!\n");
    else
        printf("I'm not the first!\n");

    return 0;
}

int main(void)
{
    thrd_t t;

    thrd_create(&first_thread_id, run, NULL);
    thrd_create(&t, run, NULL);

    thrd_join(first_thread_id, NULL);
    thrd_join(t, NULL);
}
```

Output:

``` {.default}
Come on, you got what you want, Cohaagen! Give deez people ay-ah!
```

No, wait, that's an Arnold Schwarzenegger quote from _Total Recall_, one
of the best science fiction films of all time. Watch it now and then
come back to finish this reference page.

Man--what an ending! And Johnny Cab? So excellent. Anyway!

Output:

``` {.default}
I'm the first thread!
I'm not the first!
```

### See Also {.unnumbered .unlisted}

[`thrd_equal()`](#man-thrd_equal),
[`thrd_detach()`](#man-thrd_detach)

[[manbreak]]
## `thrd_detach()` {#man-thrd_detach}

Automatically clean up threads when they exit

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int thrd_detach(thrd_t thr);
```

### Description {.unnumbered .unlisted}

Normally you have to `thrd_join()` to get resources associated with a
deceased thread cleaned up. (Most notably, its exit status is still
floating around waiting to get picked up.)

But if you call `thrd_detach()` on the thread first, manual cleanup
isn't necessary. They just exit and are cleaned up by the OS.

(Note that when the main thread dies, all the threads die in any case.)

### Return Value {.unnumbered .unlisted}

`thrd_success` if the thread successfully detaches, `thrd_error`
otherwise.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

thrd_t first_thread_id;

int run(void *arg)
{
    (void)arg;

    printf("Thread running!\n");

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t;

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_create(&t, run, NULL);
        thrd_detach(t);
    }

    // No need to thrd_join()!

    // Sleep a quarter second to let them all finish
    thrd_sleep(&(struct timespec){.tv_nsec=250000000}, NULL);
}
```

### See Also {.unnumbered .unlisted}

[`thrd_join()`](#man-thrd_join),
[`thrd_exit()`](#man-thrd_exit)

[[manbreak]]
## `thrd_equal()` {#man-thrd_equal}

Compare two thread descriptors for equality

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int thrd_equal(thrd_t thr0, thrd_t thr1);
```

### Description {.unnumbered .unlisted}

If you have two thread descriptors in `thrd_t` variables, you can test
them for equality with this function.

For example, maybe one of the threads has special powers the others
don't, and the run function needs to be able to tell them apart, as in
the example.

### Return Value {.unnumbered .unlisted}

Returns non-zero if the threads are equal. Returns `0` if they're not.

### Example {.unnumbered .unlisted}

Here's a general example that shows getting the current thread ID and
comparing it to a previously-recorded thread ID and taking boring action
based on the result.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

thrd_t first_thread_id;

int run(void *arg)
{
    (void)arg;

    thrd_t my_id = thrd_current();

    if (thrd_equal(my_id, first_thread_id))  // <-- COMPARE!
        printf("I'm the first thread!\n");
    else
        printf("I'm not the first!\n");

    return 0;
}

int main(void)
{
    thrd_t t;

    thrd_create(&first_thread_id, run, NULL);
    thrd_create(&t, run, NULL);

    thrd_join(first_thread_id, NULL);
    thrd_join(t, NULL);
}
```

Output:

``` {.default}
I'm the first thread!
I'm not the first!
```

### See Also {.unnumbered .unlisted}

[`thrd_current()`](#man-thrd_current)

[[manbreak]]
## `thrd_exit()` {#man-thrd_exit}

Stop and exit this thread

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

_Noreturn void thrd_exit(int res);
```

### Description {.unnumbered .unlisted}

A thread commonly exits by returning from its run function. But if it
wants to exit early (perhaps from deeper in the call stack), this
function will get that done.

The `res` code can be picked up by a thread calling `thrd_join()`, and
is equivalent to returning a value from the run function.

Like with returning from the run function, this will also properly clean
up all the thread-specific storage associated with this thread---all the
destructors for the threads TSS variables will be called. If there are
any remaining TSS variables with destructors after the first round of
destruction^[For example, if a destructor caused more variables to be
set.], the remaining destructors will be called. This happens repeatedly
until there are no more, or the number of rounds of carnage reaches
`TSS_DTOR_ITERATIONS`.

If the main thread calls this, it's as if you called
`exit(EXIT_SUCCESS)`.

### Return Value {.unnumbered .unlisted}

This function never returns because the thread calling it is killed in
the process. Trippy!

### Example {.unnumbered .unlisted}

Threads in this example exit early with result `22` if they get a `NULL`
value for `arg`.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

thrd_t first_thread_id;

int run(void *arg)
{
    (void)arg;

    if (arg == NULL)
        thrd_exit(22);

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_create(t + i, run, i == 2? NULL: "spatula");


    for (int i = 0; i < THREAD_COUNT; i++) {
        int res;
        thrd_join(t[i], &res);

        printf("Thread %d exited with code %d\n", i, res);
    }
}
```

Output:

``` {.default}
Thread 0 exited with code 0
Thread 1 exited with code 0
Thread 2 exited with code 22
Thread 3 exited with code 0
Thread 4 exited with code 0
```

### See Also {.unnumbered .unlisted}

[`thrd_join()`](#man-thrd_join)

[[manbreak]]
## `thrd_join()` {#man-thrd_join}

Wait for a thread to exit

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int thrd_join(thrd_t thr, int *res);
```

### Description {.unnumbered .unlisted}

When a parent thread fires off some child threads, it can wait for them
to complete with this call

### Return Value {.unnumbered .unlisted}

### Example {.unnumbered .unlisted}

Threads in this example exit early with result `22` if they get a `NULL`
value for `arg`. The parent thread picks up this result code with
`thrd_join()`.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

thrd_t first_thread_id;

int run(void *arg)
{
    (void)arg;

    if (arg == NULL)
        thrd_exit(22);

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_create(t + i, run, i == 2? NULL: "spatula");


    for (int i = 0; i < THREAD_COUNT; i++) {
        int res;
        thrd_join(t[i], &res);

        printf("Thread %d exited with code %d\n", i, res);
    }
}
```

Output:

``` {.default}
Thread 0 exited with code 0
Thread 1 exited with code 0
Thread 2 exited with code 22
Thread 3 exited with code 0
Thread 4 exited with code 0
```

### See Also {.unnumbered .unlisted}

[`thrd_exit()`](#man-thrd_exit)

## `thrd_sleep()` {#man-thrd_sleep}

Sleep for a specific number of seconds and nanoseconds

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int thrd_sleep(const struct timespec *duration, struct timespec *remaining);
```

### Description {.unnumbered .unlisted}

This function puts the current thread to sleep for a while^[Unix-like
systems have a `sleep()` syscall that sleeps for an integer number of
seconds. But `thrd_sleep()` is likely more portable and gives subsecond
resolution, besides!] allowing other threads to run.

The calling thread will wake up after the time has elapsed, or if it
gets interrupted by a signal or something.

If it doesn't get interrupted, it'll sleep at least as long as you
asked. Maybe a tad longer. You know how hard it can be to get out of
bed.

The structure looks like this:

``` {.c}
struct timespec {
    time_t tv_sec;   // Seconds
    long   tv_nsec;  // Nanoseconds (billionths of a second)
};
```

Don't set `tv_nsec` greater than 999,999,999. I can't see what
officially happens if you do, but on my system `thrd_sleep()` returns
`-2` and fails.

### Return Value {.unnumbered .unlisted}

Returns `0` on timeout, or `-1` if interrupted by a signal. Or any
negative value on some other error. Weirdly, the spec allows this "other
error negative value" to also be `-1`, so good luck with that.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

int main(void)
{
    // Sleep for 3.25 seconds
    thrd_sleep(&(struct timespec){.tv_sec=3, .tv_nsec=250000000}, NULL);

    return 0;
}
```

### See Also {.unnumbered .unlisted}

[`thrd_yield()`](#man-thrd_yield)

[[manbreak]]
## `thrd_yield()` {#man-thrd_yield}

Stop running that other threads might run

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

void thrd_yield(void);
```

### Description {.unnumbered .unlisted}

If you have a thread that's hogging the CPU and you want to give your
other threads time to run, you can call `thrd_yield()`. If the system
sees fit, it will put the calling thread to sleep and one of the other
threads will run instead.

It's a good way to be "polite" to the other threads in your program if
you want the encourage them to run instead.

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

This example's kinda poor because the OS is probably going to reschedule
threads on the output anyway, but it gets the point across.

The main thread is giving other threads a chance to run after every
block of dumb work it does.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

int run(void *arg)
{
    int main_thread = arg != NULL;

    if (main_thread) {
        long int total = 0;

        for (int i = 0; i < 10; i++) {
            for (long int j = 0; j < 1000L; j++)
                total++;

            printf("Main thread yielding\n");
            thrd_yield();                       // <-- YIELD HERE
        }
    } else
        printf("Other thread running!\n");

    return 0;
}

#define THREAD_COUNT 10

int main(void)
{
    thrd_t t[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_create(t + i, run, i == 0? "main": NULL);

    for (int i = 0; i < THREAD_COUNT; i++)
        thrd_join(t[i], NULL);

    return 0;
}
```

The output will vary from run to run. Notice that even after
`thrd_yield()` other threads might not yet be ready to run and the main
thread will continue.

``` {.default}
Main thread yielding
Main thread yielding
Main thread yielding
Other thread running!
Other thread running!
Other thread running!
Other thread running!
Main thread yielding
Other thread running!
Other thread running!
Main thread yielding
Main thread yielding
Main thread yielding
Other thread running!
Main thread yielding
Main thread yielding
Main thread yielding
Other thread running!
Other thread running!
```

### See Also {.unnumbered .unlisted}

[`thrd_sleep()`](#man-thrd_sleep)

[[manbreak]]
## `tss_create()` {#man-tss_create}

Create new thread-specific storage

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int tss_create(tss_t *key, tss_dtor_t dtor);
```

### Description {.unnumbered .unlisted}

This helps when you need per-thread storage of different values.

A common place this comes up is if you have a file scope variable that
is shared between a bunch of functions and often returned. That's not
threadsafe. One way to refactor is to replace it with thread-specific
storage so that each thread gets their own code and doesn't step on
other thread's toes.

To make this work, you pass in a pointer to a `tss_t` key---this is the
variable you will use in subsequent `tss_set()` and `tss_get()` calls to
set and get the value associated with the key.

The interesting part of this is the `dtor` destructor pointer of type
`tss_dtor_t`. This is actually a pointer to a function that takes a
`void*` argument and returns `void`, i.e.

``` {.c}
void dtor(void *p) { ... }
```

This function will be called per thread when the thread exits with
`thrd_exit()` (or returns from the run function).

It's unspecified behavior to call this function while other threads'
destructors are running.

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

This is a general-purpose TSS example. Note the TSS variable is created
near the top of `main()`.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

tss_t str;

void some_function(void)
{
    // Retrieve the per-thread value of this string
    char *tss_string = tss_get(str);

    // And print it
    printf("TSS string: %s\n", tss_string);
}

int run(void *arg)
{
    int serial = *(int*)arg;  // Get this thread's serial number
    free(arg);

    // malloc() space to hold the data for this thread
    char *s = malloc(64);
    sprintf(s, "thread %d! :)", serial);  // Happy little string

    // Set this TSS variable to point at the string
    tss_set(str, s);

    // Call a function that will get the variable
    some_function();

    return 0; // Equivalent to thrd_exit(0); fires destructors
}

#define THREAD_COUNT 15

int main(void)
{
    thrd_t t[THREAD_COUNT];

    // Make a new TSS variable, the free() function is the destructor
    tss_create(&str, free);                  // <-- CREATE TSS VAR!

    for (int i = 0; i < THREAD_COUNT; i++) {
        int *n = malloc(sizeof *n);  // Holds a thread serial number
        *n = i;
        thrd_create(t + i, run, n);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_join(t[i], NULL);
    }

    // And all threads are done, so let's free this
    tss_delete(str);
}
```

Output:

``` {.default}
TSS string: thread 0! :)
TSS string: thread 2! :)
TSS string: thread 1! :)
TSS string: thread 5! :)
TSS string: thread 3! :)
TSS string: thread 6! :)
TSS string: thread 4! :)
TSS string: thread 7! :)
TSS string: thread 8! :)
TSS string: thread 9! :)
TSS string: thread 10! :)
TSS string: thread 13! :)
TSS string: thread 12! :)
TSS string: thread 11! :)
TSS string: thread 14! :)
```

### See Also {.unnumbered .unlisted}

[`tss_delete()`](#man-tss_delete),
[`tss_set()`](#man-tss_set),
[`tss_get()`](#man-tss_get),
[`thrd_exit()`](#man-thrd_exit)

[[manbreak]]
## `tss_delete()` {#man-tss_delete}

Clean up a thread-specific storage variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

void tss_delete(tss_t key);
```

### Description {.unnumbered .unlisted}

This is the opposite of `tss_create()`. You create (initialize) the TSS
variable before using it, then, when all the threads are done that need
it, you delete (deinitialize/free) it with this.

This doesn't call any destructors! Those are all called by
`thrd_exit()`!

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

This is a general-purpose TSS example. Note the TSS variable is deleted
near the bottom of `main()`.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

tss_t str;

void some_function(void)
{
    // Retrieve the per-thread value of this string
    char *tss_string = tss_get(str);

    // And print it
    printf("TSS string: %s\n", tss_string);
}

int run(void *arg)
{
    int serial = *(int*)arg;  // Get this thread's serial number
    free(arg);

    // malloc() space to hold the data for this thread
    char *s = malloc(64);
    sprintf(s, "thread %d! :)", serial);  // Happy little string

    // Set this TSS variable to point at the string
    tss_set(str, s);

    // Call a function that will get the variable
    some_function();

    return 0; // Equivalent to thrd_exit(0); fires destructors
}

#define THREAD_COUNT 15

int main(void)
{
    thrd_t t[THREAD_COUNT];

    // Make a new TSS variable, the free() function is the destructor
    tss_create(&str, free);

    for (int i = 0; i < THREAD_COUNT; i++) {
        int *n = malloc(sizeof *n);  // Holds a thread serial number
        *n = i;
        thrd_create(t + i, run, n);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_join(t[i], NULL);
    }

    // And all threads are done, so let's free this
    tss_delete(str);    // <-- DELETE TSS VARIABLE!
}
```

Output:

``` {.default}
TSS string: thread 0! :)
TSS string: thread 2! :)
TSS string: thread 1! :)
TSS string: thread 5! :)
TSS string: thread 3! :)
TSS string: thread 6! :)
TSS string: thread 4! :)
TSS string: thread 7! :)
TSS string: thread 8! :)
TSS string: thread 9! :)
TSS string: thread 10! :)
TSS string: thread 13! :)
TSS string: thread 12! :)
TSS string: thread 11! :)
TSS string: thread 14! :)
```

### See Also {.unnumbered .unlisted}

[`tss_create()`](#man-tss_create),
[`tss_set()`](#man-tss_set),
[`tss_get()`](#man-tss_get),
[`thrd_exit()`](#man-thrd_exit)

[[manbreak]]
## `tss_get()` {#man-tss_get}

Get thread-specific data

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

void *tss_get(tss_t key);
```

### Description {.unnumbered .unlisted}

Once you've set a variable with `tss_set()`, you can retrieve the value
with `tss_get()`---just pass in the key and you'll get a pointer to the
value back.

Don't call this from a destructor.

### Return Value {.unnumbered .unlisted}

Returns the value stored for the given `key`, or `NULL` if there's
trouble.

### Example {.unnumbered .unlisted}

This is a general-purpose TSS example. Note the TSS variable is
retrieved in `some_function()`, below.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

tss_t str;

void some_function(void)
{
    // Retrieve the per-thread value of this string
    char *tss_string = tss_get(str);    // <-- GET THE VALUE

    // And print it
    printf("TSS string: %s\n", tss_string);
}

int run(void *arg)
{
    int serial = *(int*)arg;  // Get this thread's serial number
    free(arg);

    // malloc() space to hold the data for this thread
    char *s = malloc(64);
    sprintf(s, "thread %d! :)", serial);  // Happy little string

    // Set this TSS variable to point at the string
    tss_set(str, s);

    // Call a function that will get the variable
    some_function();

    return 0; // Equivalent to thrd_exit(0); fires destructors
}

#define THREAD_COUNT 15

int main(void)
{
    thrd_t t[THREAD_COUNT];

    // Make a new TSS variable, the free() function is the destructor
    tss_create(&str, free);

    for (int i = 0; i < THREAD_COUNT; i++) {
        int *n = malloc(sizeof *n);  // Holds a thread serial number
        *n = i;
        thrd_create(t + i, run, n);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_join(t[i], NULL);
    }

    // And all threads are done, so let's free this
    tss_delete(str);
}
```

Output:

``` {.default}
TSS string: thread 0! :)
TSS string: thread 2! :)
TSS string: thread 1! :)
TSS string: thread 5! :)
TSS string: thread 3! :)
TSS string: thread 6! :)
TSS string: thread 4! :)
TSS string: thread 7! :)
TSS string: thread 8! :)
TSS string: thread 9! :)
TSS string: thread 10! :)
TSS string: thread 13! :)
TSS string: thread 12! :)
TSS string: thread 11! :)
TSS string: thread 14! :)
```

### See Also {.unnumbered .unlisted}

[`tss_set()`](#man-tss_set)

[[manbreak]]
## `tss_set()` {#man-tss_set}

Set thread-specific data

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <threads.h>

int tss_set(tss_t key, void *val);
```

### Description {.unnumbered .unlisted}

Once you've set up your TSS variable with `tss_create()`, you can set it
on a per thread basis with `tss_set()`.

`key` is the identifier for this data, and `val` is a pointer to it.

The destructor specified in `tss_create()` will be called for the value
set when the thread exits.

Also, if there's a destructor _and_ there is already at value for this
key in place, the destructor will not be called for the already-existing
value. In fact, this function will never cause a destructor to be
called. So you're on your own, there---best clean up the old value
before overwriting it with the new one.

### Return Value {.unnumbered .unlisted}

Returns `thrd_success` when happy, and `thrd_error` when not.

### Example {.unnumbered .unlisted}

This is a general-purpose TSS example. Note the TSS variable is
set in `run()`, below.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

tss_t str;

void some_function(void)
{
    // Retrieve the per-thread value of this string
    char *tss_string = tss_get(str);

    // And print it
    printf("TSS string: %s\n", tss_string);
}

int run(void *arg)
{
    int serial = *(int*)arg;  // Get this thread's serial number
    free(arg);

    // malloc() space to hold the data for this thread
    char *s = malloc(64);
    sprintf(s, "thread %d! :)", serial);  // Happy little string

    // Set this TSS variable to point at the string
    tss_set(str, s);    // <-- SET THE TSS VARIABLE

    // Call a function that will get the variable
    some_function();

    return 0; // Equivalent to thrd_exit(0); fires destructors
}

#define THREAD_COUNT 15

int main(void)
{
    thrd_t t[THREAD_COUNT];

    // Make a new TSS variable, the free() function is the destructor
    tss_create(&str, free);

    for (int i = 0; i < THREAD_COUNT; i++) {
        int *n = malloc(sizeof *n);  // Holds a thread serial number
        *n = i;
        thrd_create(t + i, run, n);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_join(t[i], NULL);
    }

    // And all threads are done, so let's free this
    tss_delete(str);
}
```

Output:

``` {.default}
TSS string: thread 0! :)
TSS string: thread 2! :)
TSS string: thread 1! :)
TSS string: thread 5! :)
TSS string: thread 3! :)
TSS string: thread 6! :)
TSS string: thread 4! :)
TSS string: thread 7! :)
TSS string: thread 8! :)
TSS string: thread 9! :)
TSS string: thread 10! :)
TSS string: thread 13! :)
TSS string: thread 12! :)
TSS string: thread 11! :)
TSS string: thread 14! :)
```

### See Also {.unnumbered .unlisted}

[`tss_get()`](#man-tss_get)

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