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
returning from it's main function.

A thread can also _sleep_ for a period of time, doing nothing while
other threads run.

The `main()` program is a thread, as well.

Additionally, we have thread local storage, mutexes, and conditional
variables. But more on those later. Let's just look at the basics for
now.

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

When you want to wait for a thread to complete, you have to specify it's
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

Let's make a thread!  We'll launch it from the main thread with
`thrd_create()` to run a function, do some other things, then wait for
it to complete with `thrd_join()`.

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

    return 12;  // Value to be picked up by thrd_join()
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

```
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

    free(arg);

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
        thrd_create(t + i, run, &i);   // <--- NOTE THIS

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

```
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
variable and we pass a pointer to that into the `run()` function.

Once `run()` makes its own copy of the `arg` on line 7, it `free()`s the
`malloc()`d `int`. And now that it has its own copy, it can do with it
what it pleases.

And a run shows the result:

```
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
to clean up nicely on their own
