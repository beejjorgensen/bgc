<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Atomics {#chapter-atomics}

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
version with `__STDC_VERSION__`[^The `__STDC_VERSION__` macro didn't
exist in early C89, so if you're worried about that, check it with
`#ifdef`.].

``` {.c}
#if __STDC_VERSION__ < 201112L || __STDC_NO_ATOMICS__ == 1
#define HAS_ATOMICS 0
#else
#define HAS_ATOMICS 1
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

This part is a bit weirder.

The compiler might have rearranged memory accesses so that when you
think you set a value relative to another might not be true. And even if
the compiler didn't, your CPU might have done it on the fly?

Which happens first in the following code, the write to `x` or the write
to `y`?

``` {.c .numberLines}
int x, y;  // global

// ...

x = 2;
y = 3;

printf("%d %d\n", x, y);
```

Answer: we don't know. The compiler or CPU could silently reverse lines
3 and 4 and we'd be none-the-wiser. The code would run single-threaded
_as-if_ it were executed in the order we explicitly stated.

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

Two threads _synchronize_ when they agree on the state of shared memory.
As we've seen, they're not always in agreement. So how do they agree?

Using atomic variables can force the agreement. If a thread writes to an
atomic variable, it's saying "anyone who reads this atomic variable in
the future will also see all the changes I made to memory up to and
including the atomic variable".

Or, in more human terms, let's sit around the conference table and make
sure we're on the same page as to which pieces of shared memory hold
what values. You agree that the memory changes that you'd made
up-to-and-including the atomic store will be visible to me when I do a
load of the same atomic variable.

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

What are these? Let's line them up with terms you already know when it
comes to atomic variables:

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
  be reordered to happen before this acquire.

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
  be reordered to happen after this release.

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

All the stuff we've been talking about so far has happened within the
realm of sequential consistency.

We've talked about how the compiler or CPU can rearrange memory reads
and writes in a single thread as long as it follows the _as-if_ rule.

And we've seen how we can put the brakes on this behavior by
synchronizing over atomic variables.

Let's formalize just a little more.

If operations are _sequentially consistent_, it means at the end of the
day, when all is said and done, all the threads can kick up their heels,
open their beverage of choice, and _all agree on the order in which
memory changes occurred_. 

One won't say, "But didn't _B_ happen before _A_?" if the rest of them
say, "_A_ definitely happened before _B_".

In particular, within a thread, no acquires can be moved after any
releases. And no releases can be moved before any acquires.

This rule gives an additional level of sanity to the progression of
atomic loads/acquires and stores/releases.

Every other memory order in C involves a relaxation of the reordering
rules, either for acquires/releases or other memory accesses, atomic or
otherwise. You'd do that if you _really_ knew what you were doing and
needed the speed boost. _Here be armies of dragons..._

More on that later.

## Atomic Assignments and Operators

Certain operators on atomic variables are atomic. And others don't.

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

## Library Functions that Automatically Synchronize

So far we've talked about how you can synchronize with atomic variables,
but it turns out there are a few library functions that do some limited
behind-the-scenes synchronization, themselves.

``` {.c}
call_once()      thrd_create()       thrd_join()
mtx_lock()       mtx_timedlock()     mtx_trylock()
malloc()         calloc()            realloc()
aligned_alloc()
```

[**`call_once()`**](#man-call_once): synchronizes with all subsequent calls
to `call_once()` for a particular flag. This way subsequent calls can
rest assured that in another thread set the flag, they will see it.

[**`thrd_create()`**](#man-thrd_create): synchronizes with the beginning
of the new thread. The new thread can be sure it will see all shared
memory writes from the parent thread from before the `thrd_create()`
call.

[**`thrd_join()`**](#man-thrd_join): when a thread dies, it synchronizes
with this function. The thread that has called `thrd_join()` can be
assured that it can see all the late thread's shared writes.

[**`mtx_lock()`**](#man-mutex_lock): earlier calls to `mtx_unlock()` on
the same mutex synchronize on this call. This is the case that most
mirrors the acquire/release process we've already talked about.
`mtx_unlock()` performs a release on the mutex variable, assuring any
subsequent thread that makes an acquire with `mtx_lock()` can see all
the shared memory changes in the critical section.

[**`mtx_timedlock()`**](#man-mutex_timedlock) and
[**`mtx_trylock()`**](#man-mutex_trylock): similar to the situation with
`mtx_lock()`, if this call succeeds, earlier calls to `mtx_unlock()`
synchronize with this one.

**Dynamic Memory Functions**: if you allocate memory, it synchronizes
with the previous deallocation of that same memory. And allocations and
deallocations of that particular memory region happen in a single total
order that all threads can agree upon. I _think_ the idea here is that
the deallocation can wipe the region if it chooses, and we want to be
sure that a subsequent allocation doesn't see the non-wiped data.
Someone let me know if there's more to it.

## Atomic Type Specifier, Qualifier

Let's talk it down a notch and see what types we have available, and how
we can even make new atomic types.

First things first, let's look at the built-in atomic types and what
they are `typedef`'d to. (Spoiler: `_Atomic` is a type qualifier!)

|Atomic type|Longhand equivalent|
|-|-|
|`atomic_bool`|`_Atomic _Bool`|
|`atomic_char`|`_Atomic char`|
|`atomic_schar`|`_Atomic signed char`|
|`atomic_uchar`|`_Atomic unsigned char`|
|`atomic_short`|`_Atomic short`|
|`atomic_ushort`|`_Atomic unsigned short`|
|`atomic_int`|`_Atomic int`|
|`atomic_uint`|`_Atomic unsigned int`|
|`atomic_long`|`_Atomic long`|
|`atomic_ulong`|`_Atomic unsigned long`|
|`atomic_llong`|`_Atomic long long`|
|`atomic_ullong`|`_Atomic unsigned long long`|
|`atomic_char16_t`|`_Atomic char16_t`|
|`atomic_char32_t`|`_Atomic char32_t`|
|`atomic_wchar_t`|`_Atomic wchar_t`|
|`atomic_int_least8_t`|`_Atomic int_least8_t`|
|`atomic_uint_least8_t`|`_Atomic uint_least8_t`|
|`atomic_int_least16_t`|`_Atomic int_least16_t`|
|`atomic_uint_least16_t`|`_Atomic uint_least16_t`|
|`atomic_int_least32_t`|`_Atomic int_least32_t`|
|`atomic_uint_least32_t`|`_Atomic uint_least32_t`|
|`atomic_int_least64_t`|`_Atomic int_least64_t`|
|`atomic_uint_least64_t`|`_Atomic uint_least64_t`|
|`atomic_int_fast8_t`|`_Atomic int_fast8_t`|
|`atomic_uint_fast8_t`|`_Atomic uint_fast8_t`|
|`atomic_int_fast16_t`|`_Atomic int_fast16_t`|
|`atomic_uint_fast16_t`|`_Atomic uint_fast16_t`|
|`atomic_int_fast32_t`|`_Atomic int_fast32_t`|
|`atomic_uint_fast32_t`|`_Atomic uint_fast32_t`|
|`atomic_int_fast64_t`|`_Atomic int_fast64_t`|
|`atomic_uint_fast64_t`|`_Atomic uint_fast64_t`|
|`atomic_intptr_t`|`_Atomic intptr_t`|
|`atomic_uintptr_t`|`_Atomic uintptr_t`|
|`atomic_size_t`|`_Atomic size_t`|
|`atomic_ptrdiff_t`|`_Atomic ptrdiff_t`|
|`atomic_intmax_t`|`_Atomic intmax_t`|
|`atomic_uintmax_t`|`_Atomic uintmax_t`|

Use those at will!

But what if you want more?

You can do it either with a type qualifier or type specifier.

First, specifier! It's the keyword `_Atomic` with a type in parens
after---suitable for use with `typedef`:

``` {.c}
typedef _Atomic(double) atomic_double;

atomic_double f;
```

Restrictions on the specifier: the type you're making atomic can't be of
type array or function, nor can it be atomic or otherwise qualified.

Next, qualifier! It's the keyword `_Atomic` _without_ a type in parens.

So these do similar things[^The spec notes that they might differ in
size, representation, and alignment.]:

``` {.c}
_Atomic(int) i;   // type specifier
_Atomic int  j;   // type qualifier
```

The thing is, you can include other type qualifiers with the latter:

``` {.c}
_Atomic volatile int k;   // qualified atomic variable
```

Restrictions on the qualifier: the type you're making atomic can't be of
type array or function.

## Lock-Free Atomic Variables {#lock-free-atomic}

Hardware architectures are limited in the amount of data they can
atomically read and write. It depends on how it's wired together. And it
varies.

If you use an atomic type, you can be assured that accesses to that type
will be atomic... but there's a catch: if the hardware can't do it, it's
done with a lock, instead.

So the atomic access becomes lock-access-unlock, which is rather slower
and has some implications for signal handlers.

[Atomic flags](#atomic-flags), below, are the only atomic types that are
guaranteed to be lock-free in all conforming implementations. In typical
desktop/laptop computer world, other larger types are likely lock-free.

Luckily, we have a couple ways to determine if a particular type is
a lock-free atomic or not.

First of all, some macros---you can use these at compile time with
`#if`. They apply to both signed and unsigned types.

|Atomic Type|Lock Free Macro|
|-|-|
|`atomic_bool`|`ATOMIC_BOOL_LOCK_FREE`|
|`atomic_char`|`ATOMIC_CHAR_LOCK_FREE`|
|`atomic_char16_t`|`ATOMIC_CHAR16_T_LOCK_FREE`|
|`atomic_char32_t`|`ATOMIC_CHAR32_T_LOCK_FREE`|
|`atomic_wchar_t`|`ATOMIC_WCHAR_T_LOCK_FREE`|
|`atomic_short`|`ATOMIC_SHORT_LOCK_FREE`|
|`atomic_int`|`ATOMIC_INT_LOCK_FREE`|
|`atomic_long`|`ATOMIC_LONG_LOCK_FREE`|
|`atomic_llong`|`ATOMIC_LLONG_LOCK_FREE`|
|`atomic_intptr_t`|`ATOMIC_POINTER_LOCK_FREE`|

These macros can interestingly have _three_ different values:

|Value|Meaning|
|-|-|
|`0`|Never lock-free.|
|`1`|_Sometimes_ lock-free.|
|`2`|Always lock-free.|

Wait---how can something be _sometimes_ lock-free? This just means the
answer isn't known at compile-time, but could later be known at runtime.
Maybe the answer varies depending on whether or not you're running this
code on Genuine Intel or AMD, or something like that[^I just pulled that
example out of nowhere. Maybe it doesn't matter on Intel/AMD, but it
could matter somewhere, dangit!].

But you can always test at runtime with the `atomic_is_lock_free()`
function. This function returns true or false if the particular type is
atomic right now.

So why do we care?

Lock-free is faster, so maybe there's a speed concern that you'd code
around another way. Or maybe you need to use an atomic variable in a
signal handler.

### Signal Handlers and Lock Free Atomics

If you read or write a shared variable (static storage duration or
`_Thread_Local` in a signal handler, it's undefined behavior (gasp!)...
Unless you do one of the following:

1. Write to a variable of type `volatile sig_atomic_t`.

2. Read or write a lock-free atomic variable.

As far as I can tell, lock-free atomic variables are one of the few ways
you get portably get information out of a signal handler.

The spec is a bit vague about the memory order when it comes to
acquiring or releasing atomic variables in the signal handler. C++ says,
and it makes sense, that such accesses are unsequenced with respect to
the rest of the program[^C++ elaborates that if the signal is the result
of a call to `raise()`, it is sequenced _after_ the `raise()`.]. The
signal can be raised, after all, at any time. So likely C's behavior is
similar.

## Atomic Flags {#atomic-flags}

There's only one type the standard guarantees will be a lock-free
atomic: `atomic_flag`. This is an opaque type for
[flw[test-and-set|Test-and-set]] operations.

It can be either _set_ or _clear_. You can initialize it to clear with:

``` {.c}
atomic_flag f = ATOMIC_FLAG_INIT;
```

You can set the flag atomically with `atomic_flag_test_and_set()`, which
will set the flag and return its previous status as a `_Bool` (true for
set).

You can clear the flag atomically with `atomic_flag_clear()`.

Here's an example where we init the flag to clear, set it twice, then
clear it again.

``` {.c}
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>

atomic_flag f = ATOMIC_FLAG_INIT;

int main(void)
{
    bool r = atomic_flag_test_and_set(&f);
    printf("Value was: %d\n", r);           // 0

    r = atomic_flag_test_and_set(&f);
    printf("Value was: %d\n", r);           // 1

    atomic_flag_clear(&f);
    r = atomic_flag_test_and_set(&f);
    printf("Value was: %d\n", r);           // 0
}
```

## Atomic `struct`s and `union`s

Using the `_Atomic` qualifier or specifier, you can make atomic
`struct`s or `union`s! Pretty astounding.

If there's not a lot of data in there (i.e. a handful of bytes), the
resulting atomic type might be lock-free. Test it with
`atomic_is_lock_free()`.

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    struct point {
        float x, y;
    };

    _Atomic(struct point) p;

    printf("Is lock free: %d\n", atomic_is_lock_free(&p));
}
```

Here's the catch: you can't access fields of an atomic `struct` or
`union`... so what's the point? Well, you can atomically _copy_ the entire `struct`
into a non-atomic variable and then used it. You can atomically copy the
other way, too.

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    struct point {
        float x, y;
    };

    _Atomic(struct point) p;
    struct point t;

    p = (struct point){1, 2};  // Atomic copy

    //printf("%f\n", p.x);  // Error

    t = p;   // Atomic copy

    printf("%f\n", t.x);  // OK!
}
```

You can also declare a `struct` where individual fields are atomic.  It
is implementation defined if atomic types are allowed on bitfields.

## Atomic Pointers

Just a note here about placement of `_Atomic` when it comes to pointers.

First, pointers to atomics (i.e. the pointer value is not atomic, but
the thing it points to is):

``` {.c}
_Atomic int x;
_Atomic int *p;  // p is a pointer to an atomic int

p = &x;  // OK!
```

Second, atomic pointers to non-atomic values (i.e. the pointer value
itself is atomic, but the thing it points to is not):

``` {.c}
int x;
int * _Atomic p;  // p is an atomic pointer to an int

p = &x;  // OK!
```

Lastly, atomic pointers to atomic values (i.e. the pointer and the thing
it points to are both atomic):

``` {.c}
_Atomic int x;
_Atomic int * _Atomic p;  // p is an atomic pointer to an atomic int

p = &x;  // OK!
```

## Fences

## `volatile` and Atomics

## Atomic and Bitfields

## Memory Access Order

* seq cst
* acq rel
* acq consume
  * dependencies, kill_dependency()
* relaxed
