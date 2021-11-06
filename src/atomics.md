<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Atomics

> _"They tried and failed, all of them?"_ \
> _"Oh, no." She shook her head. "They tried and died."_
>
> ---Paul Atreides and The Reverend Mother Gaius Helen Mohiam, _Dune_

**[Caveat: this section needs peer review. It's at the edge of my
knowledge.]**

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

## Testing for Atomic Support

Atomics are an optional feature. There's a macro `__STDC_NO_ATOMICS__`
that's `1` if you _don't_ have atomics.

That macro might not exist pre-C11, so we should test the language
version with `__STDC_VERSION__`. Of course _that_ macro didn't exist in
C89 (though it was applied retroactively in 1995), so to be extra safe
we should test that, too.

``` {.c}
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L && __STDC_NO_ATOMICS__ == 0
#define HAS_ATOMICS 1
#else
#define HAS_ATOMICS 0
#endif
```

And if you want to just bomb out, you can:

``` {.c}
#if !defined __STDC_VERSION__ || __STDC_VERSION__ < 201112L || __STDC_NO_ATOMICS__ == 1
#error No atomic support!
#endif
```

If those tests pass, then you can safely include `<stdatomic.h>, the
header on which the rest of this chapter is based. But if there is no
atomic support, that header might not even exist.

On some systems, you might need to add `-latomic` to the end of your
compilation command line to use any functions in the header file.

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

So that's the basics. When thread A reads a shared atomic variable that
thread B has written to, thread A can then see all the memory writes
thread B did before it wrote to the atomic variable.

Let's look deeper.

## Acquire and Release

More terminology! It'll pay off to learn this now.

When a thread reads an atomic variable, it is said to be an _acquire_
operation.

When a thread writes an atomic variable, it is said to be a _release_
operation.

What are these? Let's line them up with terms you already know:

**Read = Load = Acquire**. Like when you compare an atomic variable or
read it to copy it ot another value.

**Write = Store = Release**. Like when you assign a value into an atomic
variable.

C spells out what can happen when around these two operations on atomic
variables.

The short of this was what we already looked at in the previous section:
when a thread reads (acquires) an atomic variable that another thread
has written to (release), all writes to memory in the releasing thread
are visible to the acquiring thread. It's a way to make sure that all
the work you've done assigning to and changing regular variables becomes
visible to other threads.

More details:

With read/load/acquire of a particular atomic variable:

* All writes (atomic or non-atomic) in other threads that happened
  before the other thread wrote/stored/released this atomic variable are
  now visible in this thread.

* The new value of the atomic variable set by the other thread is also
  visible in this thread.

* No reads or writes of any variables/memory in the current thread can
  be reordered to happen before this acquire. They must resolve after.

* The acquire acts as a one-way barrier when it comes to code
  reordering; reads and writes in the current thread can be moved down
  from _after_ the release to _before_ it. But, more importantly for
  synchronization, nothing can move down from _after_ the acquire to
  _before_ it.

With write/store/release of a particular atomic variable:

* All writes (atomic or non-atomic) in the current thread that happened
  before this release become visible to other threads that have
  read/loaded/acquired the same atomic variable.

* The value written to this atomic variable by this thread is also
  visible to other threads.

* No reads or writes of any variables/memory in the current thread can
  be reordered to happen after this release. They must resolve
  beforehand.

* The release acts as a one-way barrier when it comes to code
  reordering: reads and writes in the current thread can be moved up
  from _after_ the release to _before_ it. But, more importantly for
  synchronization, nothing can move down from _before_ the release to
  _after_ it.

Again, the upshot is synchronization of memory from one thread to
another. The second thread can be sure that variables and memory are
written in the order the programmer intended.

```
int x, y, z;
atomic_int a = 0;

thread1() {
    x = 10;
    y = 20;
    a = 999;  // Release
    z = 30;
}

thread2()
{
    while (a != 999) { } // Acquire

    assert(x == 10);  // never asserts, x is always 10
    assert(y == 20);  // never asserts, y is always 20

    assert(z == 30);  // might assert!!
}
```

In the above example, `thread2` can be sure of the values in `x` and `y`
after it acquires `a` because they were set before `thread1` released
the atomic `a`.

But `thread2` can't be sure of `z`'s value because it happened after the
`release`.

An important note: releasing one atomic variable has no effect on
acquires of different atomic variables. Each variable is isolated from
the others.

## Sequential Consistency

You hanging in there? We're through the meat of the simpler usage of
atomics.

_Sequential consistency_ is what's called a _memory ordering_. There are
many memory orderings, but sequential consistency is the sanest[^Sanest
from a programmer perspective.] C has to offer. It is also the default.
You have to go out of your way to use other memory orderings.

We've talked about how the compiler or CPU can rearrange memory reads
and writes as long as it follows the _as-if_ rule.

In order for operations to be _sequentially consistent_, rules are
imposed about the reordering of atomic acquires and releases.

In particular, within a thread, no acquires can be moved after any
releases. And no releases can be moved before any acquires.

This rule gives a level of sanity to the progression of atomic
loads/acquires and stores/releases.

More on other crazier memory orders later.

## Atomic Assignments and Operators

Certain operators on atomic variables give atomic results. And others
don't.

Let's start with a counter-example:

``` {.c}
atomic_int x = 0;

thread1() {
    x = x + 3;  // NOT atomic!
}
```

Since there's a read of `x` on the right hand side of the assignment and
a write effectively on the left, these are two operations. Another
thread could sneak in the middle.

But you _can_ use the shorthand `+=` to get an atomic operation:

``` {.c}
atomic_int x = 0;

thread1() {
    x += 3;   // ATOMIC!
}
```

In particular, the following operators are atomic read-modify-write
operations with sequential consistency, so use them with gleeful
abandon. (In the example, `a` is atomic, and `b` is atomic, non-atomic,
or scalar.)

``` {.c}
a++       a--       --a       ++a
a += b    a -= b    a *= b    a /= b    a %= b
a %= b    a |= b    a ^= b    a >>= b   a <<= b
```

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

## Flags

## Relaxed

## Atomic Type Specifier, Qualifier

_Atomic() -- not array, function, atomic, qualified
_Atomic -- not array, function

typedef list

## Atomic and Bitfields


## Atomic `struct`s and `union`s

Must assign copy to access fields, UB to access field of atomic struct

Implementation Defined if allowed

## Atomic Pointers


## `volatile` and Atomics