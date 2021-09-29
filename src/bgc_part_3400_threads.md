<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Multithreading

C11 introduced, formally, multithreading to the C language. It's very
eerily similar to [flw[POSIX threads|POSIX_Threads]], if you've ever
used those.

And if you're not, no worries. We'll talk it through.

Do note, however, that I'm not intending this to be a full-blown classic
multithreading how-to^[I'm more a fan of shared-nothing, myself, and my
skills with classic multithreading constructs are rusty, to say the
least.]; you'll have to pick up a different very thick book for that,
specifically. Sorry!

Threading is an optional feature. If a C11+ compiler defines
`__STDC_NO_THREADS__`, threads will **not** be present in the library.
Why they decided to go with a negative sense in that macro is beyond me,
but there we are.

You can test for it like this:

``` {.c}
#ifdef __STDC_NO_THREADS__
#error I need threads to build this program!
#endif
```

Also, you might need to specify certain linker options when building. In
the case of Unix-likes, try appending a `-lpthreads` to the end of the
command line to link the `pthreads` library^[Yes, `pthreads` with a
"`p`". It's short for POSIX threads, a library that C11 borrowed
liberally from for its threads implementation.]:

``` {.zsh}
gcc -std=c11 -o foo foo.c -lpthreads
```

If you're getting linker errors on your system, it could be because the
appropriate library wasn't included.


## Background

Threads are a way to have all those shiny CPU cores you paid for do work
for you in the same program.

Normally, a C program just runs on a single CPU core. But if you know
how to split up the work, you can give pieces of it to a number of
threads and have them do the work simultaneously.

Though the spec doesn't say it, on your system it's very likely that C
(or the OS at its behest) will attempt to balance the threads over all
your CPU cores.

And if you have more threads than cores, that's OK. You just won't
realize all those gains if they're all trying to compete for CPU time.

## Things You Can Do

You can create a thread. It will begin running the function you specify.
The parent thread that spawned it will also continue to run.

And you can wait for the thread to complete. This is called _joining_.

Or if you don't care when the thread completes and don't want to wait,
you can _detach it_.

A thread can explicitly _exit_, or it can implicitly call it quits by
returning from its main function.

A thread can also _sleep_ for a period of time, doing nothing while
other threads run.

The `main()` program is a thread, as well.

Additionally, we have thread local storage, mutexes, and conditional
variables. But more on those later. Let's just look at the basics for
now.

## Data Races and the Standard Library

Some of the functions in the standard library (e.g. `asctime()` and
`strtok()`) return or use `static` data elements that aren't threadsafe.
But in general unless it's said otherwise, the standard library makes
an effort to be so^[Per §7.1.4¶5.].

But keep an eye out. If a standard library function is maintaining state
between calls in a variable you don't own, or if a function is returning
a pointer to a thing that you didn't pass in, it's not threadsafe.

## Creating and Waiting for Threads

Let's hack something up!

We'll make some threads (create) and wait for them to complete (join).

We have a tiny bit to understand first, though.

Every single thread is identified by an opaque variable of type
`thrd_t`. It's a unique identifier per thread in your program. When you
create a thread, it's given a new ID.

Also when you make the thread, you have to give it a pointer to a
function to run, and a pointer to an argument to pass to it (or `NULL`
if you don't have anything to pass).

The thread will begin execution on the function you specify.

When you want to wait for a thread to complete, you have to specify its
thread ID so C knows which one to wait for.

So the basic idea is:

1. Write a function to act as the thread's "`main`". It's not
   `main()`-proper, but analogous to it. The thread will start running
   there.
2. From the main thread, launch a new thread with `thrd_create()`, and
   pass it a pointer to the function to run.
3. In that function, have the thread do whatever it has to do.
4. Meantimes, the main thread can continue doing whatever _it_ has to
   do.
5. When the main thread decides to, it can wait for the child thread to
   complete by calling `thrd_join()`. Generally you **must**
   `thrd_join()` the thread to clean up after it or else you'll leak
   memory^[Unless you `thrd_detach()`. More on this later.]

`thrd_create()` takes a pointer to the function to run, and it's of type
`thrd_start_t`, which is `int (*)(void *)`. That's Greek for "a pointer
to a function that takes an `void*` as an argument, and returns an
`int`."

Let's make a thread!  We'll launch it from the main thread with
`thrd_create()` to run a function, do some other things, then wait for
it to complete with `thrd_join()`. I've named the thread's main function
`run()`, but you can name it anything as long as the types match
`thrd_start_t`.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

// This is the function the thread will run. It can be called anything.
//
// arg is the argument pointer passed to `thrd_create()`.
//
// The parent thread will get the return value back from `thrd_join()`'
// later.

int run(void *arg)
{
    int *a = arg;  // We'll pass in an int* from thrd_create()

    printf("THREAD: Running thread with arg %d\n", *a);

    return 12;  // Value to be picked up by thrd_join() (chose 12 at random)
}

int main(void)
{
    thrd_t t;  // t will hold the thread ID
    int arg = 3490;

    printf("Launching a thread\n");

    // Launch a thread to the run() function, passing a pointer to 3490
    // as an argument. Also stored the thread ID in t:

    thrd_create(&t, run, &arg);

    printf("Doing other things while the thread runs\n");

    printf("Waiting for thread to complete...\n");

    int res;  // Holds return value from the thread exit

    // Wait here for the thread to complete; store the return value
    // in res:

    thrd_join(t, &res);

    printf("Thread exited with return value %d\n", res);
}
```

See how we did the `thrd_create()` there to call the `run()` function?
Then we did other things in `main()` and then stopped and waited for the
thread to complete with `thrd_join()`.

Sample output (yours might vary):

``` {.default}
Launching a thread
Doing other things while the thread runs
Waiting for thread to complete...
THREAD: Running thread with arg 3490
Thread exited with return value 12
```

The `arg` that you pass to the function has to have a lifetime long
enough so that the thread can pick it up before it goes away. Also, it
needs to not be overwritten by the main thread before the new thread can
use it.

Let's look at an example that launches 5 threads. One thing to note here
is how we use an array of `thrd_t`s to keep track of all the thread IDs.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

int run(void *arg)
{
    int i = *(int*)arg;

    printf("THREAD %d: running!\n", i);

    return i;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];

    int i;

    printf("Launching threads...\n");
    for (i = 0; i < THREAD_COUNT; i++)

        // NOTE! In the following line, we pass a pointer to i, 
        // but each thread sees the same pointer. So they'll
        // print out weird things as i changes value here in
        // the main thread! (More in the text, below.)

        thrd_create(t + i, run, &i);

    printf("Doing other things while the thread runs...\n");
    printf("Waiting for thread to complete...\n");

    for (int i = 0; i < THREAD_COUNT; i++) {
        int res;
        thrd_join(t[i], &res);

        printf("Thread %d complete!\n", res);
    }

    printf("All threads complete!\n");
}
```

When I run the threads, I count `i` up from 0 to 4. And pass a pointer
to it to `thrd_create()`. This pointer ends up in the `run()` routine
where we make a copy of it.

Simple enough? Here's the output:

``` {.default}
Launching threads...
THREAD 2: running!
THREAD 3: running!
THREAD 4: running!
THREAD 2: running!
Doing other things while the thread runs...
Waiting for thread to complete...
Thread 2 complete!
Thread 2 complete!
THREAD 5: running!
Thread 3 complete!
Thread 4 complete!
Thread 5 complete!
All threads complete!
```

Whaaa---? Where's `THREAD 0`? And why do we have a `THREAD 5` when
clearly `i` is never more than `4` when we call `thrd_create()`? And two
`THREAD 2`s? Madness!

This is getting into the fun land of _race conditions_. The main thread
is modifying `i` before the thread has a chance to copy it. Indeed, `i`
makes it all the way to  `5` and ends the loop before the last thread
gets a chance to copy it.

We've got to have a per-thread variable that we can refer to so we can
pass it in as the `arg`.

We could have a big array of them. Or we could `malloc()` space (and
free it somewhere---maybe in the thread itself.)

Let's give that a shot:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

int run(void *arg)
{
    int i = *(int*)arg;  // Copy the arg

    free(arg);  // Done with this

    printf("THREAD %d: running!\n", i);

    return i;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];

    int i;

    printf("Launching threads...\n");
    for (i = 0; i < THREAD_COUNT; i++) {

        // Get some space for a per-thread argument:

        int *arg = malloc(sizeof *arg);
        *arg = i;

        thrd_create(t + i, run, arg);
    }

    // ...
```

Notice on lines 27-30 we `malloc()` space for an `int` and copy the
value of `i` into it. Each new thread gets its own freshly-`malloc()`d
variable and we pass a pointer to that to the `run()` function.

Once `run()` makes its own copy of the `arg` on line 7, it `free()`s the
`malloc()`d `int`. And now that it has its own copy, it can do with it
what it pleases.

And a run shows the result:

``` {.default}
Launching threads...
THREAD 0: running!
THREAD 1: running!
THREAD 2: running!
THREAD 3: running!
Doing other things while the thread runs...
Waiting for thread to complete...
Thread 0 complete!
Thread 1 complete!
Thread 2 complete!
Thread 3 complete!
THREAD 4: running!
Thread 4 complete!
All threads complete!
```

There we go! Threads 0-4 all in effect!

Your run might vary---how the threads get scheduled to run is beyond the
C spec. We see in the above example that thread 4 didn't even begin
until threads 0-1 had completed. Indeed, if I run this again, I likely
get different output. We cannot guarantee a thread execution order.

## Detaching Threads

If you want to fire-and-forget a thread (i.e. so you don't have to
`thrd_join()` it later), you can do that with `thrd_detach()`.

This removes the parent thread's ability to get the return value from
the child thread, but if you don't care about that and just want threads
to clean up nicely on their own, this is the way to go.

Basically we're going to do this:

``` {.c}
thrd_create(&t, run, NULL);
thrd_detach(t);
```

where the `thrd_detach()` call is the parent thread saying, "Hey, I'm
not going to wait for this child thread to complete with `thrd_join()`.
So go ahead and clean it up on your own when it completes."

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

int run(void *arg)
{
    (void)arg;

    //printf("Thread running! %lu\n", thrd_current()); // non-portable!
    printf("Thread running!\n");

    return 0;
}

#define THREAD_COUNT 10

int main(void)
{
    thrd_t t;

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_create(&t, run, NULL);
        thrd_detach(t);               // <-- DETACH!
    }

    // Sleep for a second to let all the threads finish
    thrd_sleep(&(struct timespec){.tv_sec=1}, NULL);
}
```

Note that in this code, we put the main thread to sleep for 1 second
with `thrd_sleep()`---more on that later.

Also in the `run()` function, I have a commented-out line in there that
prints out the thread ID as an `unsigned long`. This is non-portable,
because the spec doesn't say what type a `thrd_t` is under the hood---it
could be a `struct` for all we know. But that line works on my system.

Something interesting I saw when I ran the code, above, and printed out
the thread IDs was that some threads had duplicate IDs! This seems like
it should be impossible, but C is allowed to _reuse_ thread IDs after
the corresponding thread has exited. So what I was seeing was that some
threads completed their run before other threads were launched.

## Thread Local Data

Threads are interesting because they don't have their own memory beyond
local variables. If you want a `static` variable or file scope variable,
all threads will see that same variable.

This can lead to race conditions, where you get _Weird Things_™
happening.

Check out this example. We have a `static` variable `foo` in block scope
in `run()`. This variable will be visible to all threads that pass
through the `run()` function. And the various threads can effectively
step on each other's toes.

Each thread copies `foo` into a local variable `x` (which is not shared
between threads---all the threads have their own call stacks). So they
_should_ be the same, right?

And the first time we print them, they are^[Though I don't think they
have to be. It's just that the threads don't seem to get rescheduled
until some system call like might happen with a `printf()`... which is
why I have the `printf()` in there.]. But then right after that, we
check to make sure they're still the same.

And they _usually_ are. But not always!

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

int run(void *arg)
{
    int n = *(int*)arg;  // Thread number for humans to differentiate

    free(arg);

    static int foo = 10;  // Static value shared between threads

    int x = foo;  // Automatic local variable--each thread has its own

    // We just assigned x from foo, so they'd better be equal here.
    // (In all my test runs, they were, but even this isn't guaranteed!)

    printf("Thread %d: x = %d, foo = %d\n", n, x, foo);

    // And they should be equal here, but they're not always!
    // (Sometimes they were, sometimes they weren't!)

    // What happens is another thread gets in and increments foo
    // right now, but this thread's x remains what it was before!

    if (x != foo) {
        printf("Thread %d: Craziness! x != foo! %d != %d\n", n, x, foo);
    }

    foo++;  // Increment shared value

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    thrd_t t[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        int *n = malloc(sizeof *n);  // Holds a thread serial number
        *n = i;
        thrd_create(t + i, run, n);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_join(t[i], NULL);
    }
}
```

Here's an example output (though this varies from run to run):

``` {.default}
Thread 0: x = 10, foo = 10
Thread 1: x = 10, foo = 10
Thread 1: Craziness! x != foo! 10 != 11
Thread 2: x = 12, foo = 12
Thread 4: x = 13, foo = 13
Thread 3: x = 14, foo = 14
```

In thread 1, between the two `printf()`s, the value of `foo` somehow
changed from `10` to `11`, even though clearly there's no increment
between the `printf()`s!

It was another thread that got in there (probably thread 0, from the
look of it) and incremented the value of `foo` behind thread 1's back!

Let's solve this problem two different ways. (If you want all the
threads to share the variable _and_ not step on each other's toes,
you'll have to read on to the [mutex](#mutex) section.)

### `_Thread_local` Storage-Class {#thread-local}

First things first, let's just look at the easy way around this: the
`_Thread_local` storage-class.

Basically we're just going to slap this on the front of our block scope
`static` variable and things will work! It tells C that every thread
should have its own version of this variable, so none of them step on
each other's toes.

The `<threads.h>` header defines `thread_local` as an alias to `_Thread_local`
so your code doesn't have to look so ugly.

Let's take the previous example and make `foo` into a `thread_local`
variable so that we don't share that data.

``` {.c .numberLines startFrom="5"}
int run(void *arg)
{
    int n = *(int*)arg;  // Thread number for humans to differentiate

    free(arg);

    thread_local static int foo = 10;  // <-- No longer shared!!
```

And running we get:

``` {.default}
Thread 0: x = 10, foo = 10
Thread 1: x = 10, foo = 10
Thread 2: x = 10, foo = 10
Thread 4: x = 10, foo = 10
Thread 3: x = 10, foo = 10
```

No more weird problems!

One thing: if a `thread_local` variable is block scope, it **must** be
`static`. Them's the rules. (But this is OK because non-`static`
variables are per-thread already since each thread has it's own
non-`static` variables.)

A bit of a lie there: block scope `thread_local` variables can also be
`extern`.

### Another Option: Thread-Specific Storage

Thread-specific storage (TSS) is another way of getting per-thread data.

One additional feature is that these functions allow you to specify a
destructor that will be called on the data when the TSS variable is
deleted. Commonly this destructor is `free()` to automatically clean up
`malloc()`d per-thread data. Or `NULL` if you don't need to destroy
anything.

The destructor is type `tss_dtor_t` which is a pointer to a function
that returns `void` and takes a `void*` as an argument (the `void*`
points to the data stored in the variable). In other words, it's a `void
(*)(void*)`, if that clears it up. Which I admit it probably doesn't.
Check out the example, below.

Generally, `thread_local` is probably your go-to, but if you like the
destructor idea, then you can make use of that.

The usage is a bit weird in that we need a variable of type `tss_t` to
be alive to represent the value on a per thread basis. Then we
initialize it with `tss_create()`. Eventually we get rid of it with
`tss_delete()`. Note that calling `tss_delete()` doesn't run all the
destructors---it's `thrd_exit()` (or returning from the run function)
that does that. `tss_delete()` just releases any memory allocated by
`tss_create()`.

In the middle, threads can call `tss_set()` and `tss_get()` to set and
get the value.

In the following code, we set up the TSS variable before creating the
threads, then clean up after the threads.

In the `run()` function, the threads `malloc()` some space for a string
and store that pointer in the TSS variable.

When the thread exits, the destructor function (`free()` in this case)
is called for _all_ the threads.

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

    return 0;   // Equivalent to thrd_exit(0)
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

    // All threads are done, so we're done with this
    tss_delete(str);
}
```

Again, this is kind of a painful way of doing things compared to
`thread_local`, so unless you really need that destructor functionality,
I'd use that instead.

## Mutexes {#mutex}

If you want to only allow a single thread into a critical section of
code at a time, you can protect that section with a mutex^[Short for
"mutual exclusion", AKA a "lock" on a section of code that only one
thread is permitted to execute.].

For example, if we had a `static` variable and we wanted to be able to
get and set it in two operations without another thread jumping in the
middle and corrupting it, we could use a mutex for that.

You can acquire a mutex or release it. If you attempt to acquire the
mutex and succeed, you may continue execution. If you attempt and fail
(because someone else holds it), you will _block_^[That is, your process
will go to sleep.] until the mutex is released.

If multiple threads are blocked waiting for a mutex to be released, one
of them will be chosen to run (at random, from our perspective), and the
others will continue to sleep.

The gameplan is that first we'll initialize a mutex variable to make it
ready to use with `mtx_init()`.

Then subsequent threads can call `mtx_lock()` and `mtx_unlock()` to get
and release the mutex.

When we're completely done with the mutex, we can destroy it with
`mtx_destroy()`, the logical opposite of `mtx_init()`.

First, let's look at some code that does _not_ use a mutex, and
endeavors to print out a shared (`static`) serial number and then
increment it. Because we're not using a mutex over the getting of the
value (to print it) and the setting (to increment it), threads might get
in each other's way in that critical section.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

int run(void *arg)
{
    (void)arg;

    static int serial = 0;   // Shared static variable!

    printf("Thread running! %d\n", serial);

    serial++;

    return 0;
}

#define THREAD_COUNT 10

int main(void)
{
    thrd_t t[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_create(t + i, run, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_join(t[i], NULL);
    }
}
```

When I run this, I get something that looks like this:

``` {.default}
Thread running! 0
Thread running! 0
Thread running! 0
Thread running! 3
Thread running! 4
Thread running! 5
Thread running! 6
Thread running! 7
Thread running! 8
Thread running! 9
```

Clearly multiple threads are getting in there and running the `printf()`
before anyone gets a change to update the `serial` variable.

What we want to do is wrap the getting of the variable and setting of it
into a single mutex-protected stretch of code.

We'll add a new variable to represent the mutex of type `mtx_t` in file
scope, initialize it, and then the threads can lock and unlock it in the
`run()` function.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

mtx_t serial_mtx;     // <-- MUTEX VARIABLE

int run(void *arg)
{
    (void)arg;

    static int serial = 0;   // Shared static variable!

    // Acquire the mutex--all threads will block on this call until
    // they get the lock:

    mtx_lock(&serial_mtx);           // <-- ACQUIRE MUTEX

    printf("Thread running! %d\n", serial);

    serial++;

    // Done getting and setting the data, so free the lock. This will
    // unblock threads on the mtx_lock() call:

    mtx_unlock(&serial_mtx);         // <-- RELEASE MUTEX

    return 0;
}

#define THREAD_COUNT 10

int main(void)
{
    thrd_t t[THREAD_COUNT];

    // Initialize the mutex variable, indicating this is a normal
    // no-frills, mutex:

    mtx_init(&serial_mtx, mtx_plain);        // <-- CREATE MUTEX

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_create(t + i, run, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        thrd_join(t[i], NULL);
    }

    // Done with the mutex, destroy it:

    mtx_destroy(&serial_mtx);                // <-- DESTROY MUTEX
}
```

See how on lines 38 and 50 of `main()` we initialize and destroy the
mutex.

But each individual thread acquires the mutex on line 15 and releases it
on line 24.

In between the `mtx_lock()` and `mtx_unlock()` is the _critical
section_, the area of code where we don't want multiple threads mucking
about at the same time.

And now we get proper output!

``` {.default}
Thread running! 0
Thread running! 1
Thread running! 2
Thread running! 3
Thread running! 4
Thread running! 5
Thread running! 6
Thread running! 7
Thread running! 8
Thread running! 9
```

If you need multiple mutexes, no problem: just have multiple mutex
variables.

And always remember the Number One Rule of Multiple Mutexes: _Unlock mutexes in
the opposite order in which you lock them!_

### Different Mutex Types

As hinted earlier, we have a few mutex types that you can create with
`mtx_init()`. (Some of these types are the result of a bitwise-OR
operation, as noted in the table.)

|Type|Description|
|-|-|
|`mtx_plain`|Regular ol' mutex|
|`mtx_timed`|Mutex that supports timeouts|
|`mtx_plain|mtx_recursive`|Recursive mutex|
|`mtx_timed|mtx_recursive`|Recursive mutex that supports timeouts|

"Recursive" means that the holder of a lock can call `mtx_lock()`
multiple times on the same lock. (They have to unlock it an equal number
of times before anyone else can take the mutex.) This might ease coding
from time to time, especially if you call a function that needs to lock
the mutex when you already hold the mutex.

And the timeout gives a thread a chance to _try_ to get the lock for a
while, but then bail out if it can't get it in that timeframe.

For a timeout mutex, be sure to create it with `mtx_timed`:

``` {.c}
mtx_init(&serial_mtx, mtx_timed);
```

And then when you wait for it, you have to specify a time in UTC when it
will unlock^[You might have expected it to be "time from now", but you'd
just like to think that, wouldn't you!].

The function `timespec_get()` from `<time.h>` can be of assistance here.
It'll get you the current time in UTC in a `struct timespec` which is
just what we need. In fact, it seems to exist merely for this purpose.

It has two fields: `tv_sec` has the current time in seconds since epoch,
and `tv_nsec` has the nanoseconds (billionths of a second) as the
"fractional" part.

So you can load that up with the current time, and then add to it to get
a specific timeout.

Then call `mtx_timedlock()` instead of `mtx_lock()`. If it returns the
value `thrd_timedout`, it timed out.

``` {.c}
struct timespec timeout;

timespec_get(&timeout, TIME_UTC);  // Get current time
timeout.tv_sec += 1;               // Timeout 1 second after now

int result = mtx_timedlock(&serial_mtx, &timeout));

if (result == thrd_timedout) {
    printf("Mutex lock timed out!\n");
}
```

Other than that, timed locks are the same as regular locks.

## Condition Variables

Condition Variables are the last piece of the puzzle we need to make
performant multithreaded applications and to compose more complex
multithreaded structures.

A condition variable provides a way for threads to go to sleep until
some event on another thread occurs.

In other words, we might have a number of threads that are rearing to
go, but they have to wait until some event is true before they continue.
Basically they're being told "wait for it!" until they get notified.

And this works hand-in-hand with mutexes since what we're going to wait
on generally depends on the value of some data, and that data generally
needs to be protected by a mutex.

It's important to note that the condition variable itself isn't the
holder of any particular data from our perspective. It's merely the
variable by which C keeps track of the waiting/not-waiting status of a
particular thread or group of threads.

Let's write a contrived program that reads in groups of 5 numbers from
the main thread one at a time. Then, when 5 numbers have been entered,
the child thread wakes up, sums up those 5 numbers, and prints the
result.

The numbers will be stored in a global, shared array, as will the index
into the array of the about-to-be-entered number.

Since these are shared values, we at least have to hide them behind a
mutex for both the main and child threads. (The main will be writing
data to them and the child will be reading data from them.)

But that's not enough. The child thread needs to block ("sleep") until 5
numbers have been read into the array. And then the parent thread needs
to wake up the child thread so it can do its work.

And when it wakes up, it needs to be holding that mutex. And it will!
When a thread waits on a condition variable, it also acquires a mutex
when it wakes up.

How's that work? Let's look at the outline of what the child thread will
do:

1. Lock the mutex with `mtx_lock()`
2. If we haven't entered all the numbers, wait on the condition variable
   with `cnd_wait()`
3. Do the work that needs doing
4. Unlock the mutex with `mtx_unlock()`

Meanwhile the main thread will be doing this:

1. Lock the mutex with `mtx_lock()`
2. Store the recently-read number into the array
3. If the array is full, signal the child to wake up with `cnd_signal()`
4. Unlock the mutex with `mtx_unlock()`

If you didn't skim that too hard (it's OK---I'm not offended), you might
notice something weird: how can the main thread hold the mutex lock and
signal the child, if the child has to hold the mutex lock to wait for
the signal? They can't both hold the lock!

And indeed they don't! There's some behind-the-scenes magic with
condition variables: when you `cnd_wait()`, it releases the mutex that
you specify and the thread goes to sleep. And when someone signals that
thread to wake up, it reacquires the lock as if nothing had happened.

It's a little different on the `cnd_signal()` side of things. This
doesn't do anything with the mutex. The signalling thread still must
manually release the mutex before the waiting threads can wake up.

One more thing on the `cnd_wait()`. You'll probably be calling
`cnd_wait()` if some condition^[And that's why they're called _condition
variables_!] is not yet met (e.g. in this case, if not all the numbers
have yet been entered). Here's the deal: this condition should be in a
`while` loop, not an `if` statement. Why?

It's because of a mysterious phenomenon called a _spurious wakeup_.
Sometimes, in some implementations, a thread can be woken up out of a
`cnd_wait()` sleep for seemingly _no reason_. _[X-Files music]_^[I'm not
saying it's aliens... but it's aliens. OK, really more likely another
thread might have been woken up and gotten to the work first.]. And so
we have to check to see that the condition we need is still actually met
when we wake up. And if it's not, back to sleep with us!

So let's do this thing! Starting with the main thread:

* The main thread will set up the mutex and condition variable, and will
  launch the child thread.

* Then it will, in an infinite loop, get numbers as input from the
  console.

* It will also acquire the mutex to store the inputted number into a
  global array.

* When the array has 5 numbers in it, the main thread will signal the
  child thread that it's time to wake up and do its work.

* Then the main thread will unlock the mutex and go back to reading the
  next number from the console.

Meanwhile, the child thread has been up to its own shenanigans:

* The child thread grabs the mutex

* While the condition is not met (i.e. while the shared array doesn't
  yet have 5 numbers in it), the child thread sleeps by waiting on the
  condition variable. When it waits, it unlocks the mutex.

* Once the main thread signals the child thread to wake up, it wakes up
  to do the work and gets the mutex lock back.

* The child thread sums the numbers and resets the variable that is the
  index into the array.

* It then releases the mutex and runs again in an infinite loop.

And here's the code! Give it some study so you can see where all the
above pieces are being handled:

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

#define VALUE_COUNT_MAX 5

int value[VALUE_COUNT_MAX];  // Shared global
int value_count = 0;   // Shared global, too

mtx_t value_mtx;   // Mutex around value
cnd_t value_cnd;   // Condition variable on value

int run(void *arg)
{
    (void)arg;

    for (;;) {
        mtx_lock(&value_mtx);      // <-- GRAB THE MUTEX

        while (value_count < VALUE_COUNT_MAX) {
            printf("Thread: is waiting\n");
            cnd_wait(&value_cnd, &value_mtx);  // <-- CONDITION WAIT
        }

        printf("Thread: is awake!\n");

        int t = 0;

        // Add everything up
        for (int i = 0; i < VALUE_COUNT_MAX; i++)
            t += value[i];

        printf("Thread: total is %d\n", t);

        // Reset input index for main thread
        value_count = 0;

        mtx_unlock(&value_mtx);   // <-- MUTEX UNLOCK
    }

    return 0;
}

int main(void)
{
    thrd_t t;

    // Spawn a new thread

    thrd_create(&t, run, NULL);
    thrd_detach(t);

    // Set up the mutex and condition variable

    mtx_init(&value_mtx, mtx_plain);
    cnd_init(&value_cnd);

    for (;;) {
        int n;

        scanf("%d", &n);

        mtx_lock(&value_mtx);    // <-- LOCK MUTEX

        value[value_count++] = n;

        if (value_count == VALUE_COUNT_MAX) {
            printf("Main: signaling thread\n");
            cnd_signal(&value_cnd);  // <-- SIGNAL CONDITION
        }

        mtx_unlock(&value_mtx);  // <-- UNLOCK MUTEX
    }

    // Clean up (I know that's an infinite loop above here, but I
    // want to at least pretend to be proper):

    mtx_destroy(&value_mtx);
    cnd_destroy(&value_cnd);
}
```

And here's some sample output (individual numbers on lines are my
input):

``` {.default}
Thread: is waiting
1
1
1
1
1
Main: signaling thread
Thread: is awake!
Thread: total is 5
Thread: is waiting
2
8
5
9
0
Main: signaling thread
Thread: is awake!
Thread: total is 24
Thread: is waiting
```

It's a common use of condition variables in producer-consumer situations
like this. If we didn't have a way to put the child thread to sleep
while it waited for some condition to be met, it would be force to poll
which is a big waste of CPU.

### Timed Condition Wait

There's a variant of `cnd_wait()` that allows you to specify a timeout
so you can stop waiting.

Since the child thread must relock the mutex, this doesn't necessarily
mean that you'll be popping back to life the instant the timeout occurs;
you still must wait for any other threads to release the mutex.

But it does mean that you won't be waiting until the `cnd_signal()`
happens.

To make this work, call `cnd_timedwait()` instead of `cnd_wait()`. If it
returns the value `thrd_timedout`, it timed out.

The timestamp is an absolute time in UTC, not a time-from-now.
Thankfully the `timespec_get()` function in `<time.h>` seems custom-made
for exactly this case.

``` {.c}
struct timespec timeout;

timespec_get(&timeout, TIME_UTC);  // Get current time
timeout.tv_sec += 1;               // Timeout 1 second after now

int result = cnd_timedwait(&condition, &mutex, &timeout));

if (result == thrd_timedout) {
    printf("Condition variable timed out!\n");
}
```

### Broadcast: Wake Up All Waiting Threads

`cnd_signal()` only wakes up one thread to continue working. Depending
on how you have your logic done, it might make sense to wake up more
than one thread to continue once the condition is met.

Of course only one of them can grab the mutex, but if you have a
situation where:

* The newly-awoken thread is responsible for waking up the next one,
  and---

* There's a chance the spurious-wakeup loop condition will prevent it
  from doing so, then---

you'll want to broadcast the wake up so that you're sure to get at least
one of the threads out of that loop to launch the next one.

How, you ask?

Simply use `cnd_broadcast()` instead of `cnd_signal()`. Exact same
usage, except `cnd_broadcast()` wakes up **all** the sleeping threads
that were waiting on that condition variable.

## Running a Function One Time

Let's say you have a function that _could_ be run by many threads, but
you don't know when, and it's not work trying to write all that logic.

There's a way around it: use `call_once()`. Tons of threads could try to
run the function, but only the first one counts^[Survival of the
fittest! Right? I admit it's actually nothing like that.]

To work with this, you need a special flag variable you declare to keep
track of whether or not the thing's been run. And you need a function to
run, which takes no parameters and returns no value.

``` {.c}
once_flag of = ONCE_FLAG_INIT;  // Initialize it like this

void run_once_function(void)
{
    printf("I'll only run once!\n");
}

int run(void *arg)
{
    (void)arg;

    call_once(&of, run_once_function);

    // ...
```

In this example, no matter how many threads get to the `run()`
function, the `run_once_function()` will only be called a single time.
