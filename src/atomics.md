<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Atomics

> _"They tried and failed, all of them?"_ \
> _"Oh, no." She shook her head. "They tried and died."_
>
> ---Paul Atreides and The Reverend Mother Gaius Helen Mohiam, _Dune_

This is one of the more challenging aspects of multithreading with C.
But we'll try to take it easy.

Basically, I'll talk about the more straightforward uses of atomic
variables, what they are, and how they work, etc.  And I'll mention some
of the more insanely-complex paths that are available to you.

But I won't go down those paths. Not only am I barely qualified to even
write about them, but I figure if you know you need them, you already
know more than I do.

But there are some weird things out here even in the basics. So hang on,
everyone, 'cause Kansas is goin' bye-bye.

## Atomic Variables

Here's _part_ of how atomic variables work:

If you have a shared atomic variable and you write to it from one
thread, that write will be _all-or-nothing_ in a different thread.

That is, the other thread will see the entire write of, say, a 32-bit
value. Not half of it. There's no way for one thread to interrupt
another that is in the _middle_ of an atomic multi-byte write.

It's almost like there's a little lock around the getting and setting of
that one variable. (And there _might_ be---but probably not. See
[Lock-Free Atomic Variables](#lock-free-atomic), below.)

And on that note, you can get away with never using atomics if you use
mutexes to lock your critical sections. It's just that there are a
class of _lock-free data structures_ that always allow other threads to
make progress instead of being blocked by a mutex... but these are tough
to create correctly from scratch, and are one of the things that are
beyond the scope of the guide, sadly.

That's only part of the story. But it's the part we'll start with.

Before we go further, how do you declare a variable to be atomic?

First, include `<stdatomic.h>`.

This gives us types such as `atomic_int`. (See the [`<stdatomic.h>` types
reference page](#stdatomic-types) for the complete list.)

And then we can simply declare variables to be of that type.

But let's do a demo where we have two threads. The first runs for a
while and then sets a variable to a specific value, then exits. The
other runs until it sees that value get set, and then it exits.

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int x;   // THE POWER OF ATOMICS! BWHAHAHA!

int thread1(void *arg)
{
    (void)arg;

    printf("Thread 1: Sleeping for 1.5 seconds\n");
    thrd_sleep(&(struct timespec){.tv_sec=1, .tv_nsec=500000000}, NULL);

    printf("Thread 1: Setting x to 3490\n");
    x = 3490;

    printf("Thread 1: Exiting\n");
    return 0;
}

int thread2(void *arg)
{
    (void)arg;

    printf("Thread 2: Waiting for 3490\n");
    while (x != 3490) {}  // spin here

    printf("Thread 2: Got 3490--exiting!\n");
    return 0;
}

int main(void)
{
    x = 0;

    thrd_t t1, t2;

    thrd_create(&t1, thread1, NULL);
    thrd_create(&t2, thread2, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    printf("Main    : Threads are done, so x better be 3490\n");
    printf("Main    : And indeed, x == %d\n", x);
}
```

The second thread spins in place, looking at the flag and waiting for it
to get set to the value `3490`. And the first one does that.

And I get this output:

``` {.default}
Thread 1: Sleeping for 1.5 seconds
Thread 2: Waiting for 3490
Thread 1: Setting x to 3490
Thread 1: Exiting
Thread 2: Got 3490--exiting!
Main    : Threads are done, so x better be 3490
Main    : And indeed, x == 3490
```

Look, ma! We're accessing a variable from different threads and not
using a mutex! And that'll work every time thanks to the atomic nature
of atomic variables.

You might be wondering what happens if that's a regular non-atomic
`int`, instead. Well, on my system it still works... unless I do an
optimized build in which case it hangs on thread 2 waiting to see the
3490 to get set^[The reason for this is when optimized, my compiler has
put the value of `x` in a register to make the `while` loop fast. But
the register has no way of knowing that the variable was updated in
another thread, so it never sees the `3490`. This isn't really related
to the _all-or-nothing_ part of atomicity, but is more related to the
synchronization aspects in the next section.].

But that's just the beginning of the story. The next part is going to
require more brain power and has to do with something called
_synchronization_.

## Synchronization

The next part of our story is all about when certain memory writes in
one thread become visible to those in another thread.

You might think, it's right away, right? But it's not. A number of
things can go wrong.

The compiler might have rearranged memory accesses so that when you
think you set a value relative to another might not be true. And even if
the compiler didn't, your CPU might have done it on the fly?

Which happens first in the following code, the write to `x` or the write
to `y`?

``` {.c .numberLines}
int x, y;

x = 2;
y = 3;

printf("%d %d\n", x, y);
```

Answer: we don't know. The compiler or CPU could silently reverse lines
3 and 4 and we'd be none-the-wiser. The code would run _as-if_ it were
executed in the order we explicitly stated.

In a multithreaded scenario, we might have something like this pseudocode:

``` {.c .numberLines}
int x = 0, y = 0;

thread1() {
    x = 37;
    y = 1;
}

thread2() {
    while (y != 1) {}  // spin
    printf("x is now %d\n", x);  // 37? ...or 0?
}
```

What is the output from thread 2?

Well, if `x` gets assigned `37` _before_ `y` is assigned `1`, then I'd
expect the output to be the very sensible:

``` {.default}
x is now 37
```

But the compiler or CPU could rearrange lines 4 and 5 causing us
to see the value of `0` for `x` when we print it.

Not only all that, but a write to memory might be delayed by the
hardware as it propagates through the various caches and subsystems that
make up the enormous complexity of modern RAM.

In other words, all bets are off unless we can say, "As of this point, I
expect all previous writes in another thread to be visible in this
thread."

That point is called a _synchronization_. And atomic variables
synchronize by default.

So we can easily fix our example:

``` {.c .numberLines}
int x = 0;
atomic int y = 0;  // Make y atomic

thread1() {
    x = 37;
    y = 1;             // Synchronize on write
}

thread2() {
    while (y != 1) {}  // Synchronize on read
    printf("x is now %d\n", x);  // 37, period.
}
```

Because the threads synchronize across `y`, all writes in thread 1 that
happened before the write to `y` are visible in thread 2 after the read
from `y` (in the `while` loop).

It's important to note a couple things here:

1. Nothing sleeps. The synchronization is not a blocking operation. Both
   threads are running full bore until they exit.

2. The synchronization can be thought of as happening when one thread
   reads an atomic variable another thread wrote. So when thread 2 reads
   `y`, all previous memory writes in thread 1 (namely setting `x`) will
   be visible in thread 2.

3. Notice that `x` isn't atomic. That's OK because we're not
   synchronizing over `x`, and the synchronization over `y` when we
   write it in thread 1 means that all previous writes in thread 1 will
   become visible to other threads... if those other threads read `y` to
   synchronize.

Forcing this synchronization is inefficient and can be a lot slower than
just using a regular variable. This is why we don't use atomics unless
we have to for a particular application.

## Acquire and Release



## Sequential Consistency

## Atomic operators

## Library Functions that automatically synchronize

* thrd_create()
* etc

## Lock-Free Atomic Variables {#lock-free-atomic}

## Signal Handlers

## Fences

## Memory Access Order

* seq cst
* acq rel
* acq consume
  * dependencies, kill_dependency()
* relaxed

