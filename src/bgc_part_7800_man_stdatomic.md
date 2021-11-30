<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdatomic.h>` Atomic-Related Functions {#stdatomic}

|Function|Description|
|-|-|
|[`atomic_compare_exchange_strong_explicit()`](#man-atomic_compare_exchange)|Atomic compare and exchange, strong, explicit|
|[`atomic_compare_exchange_strong()`](#man-atomic_compare_exchange)|Atomic compare and exchange, strong|
|[`atomic_compare_exchange_weak_explicit()`](#man-atomic_compare_exchange)|Atomic compare and exchange, weak, explicit|
|[`atomic_compare_exchange_weak()`](#man-atomic_compare_exchange)|Atomic compare and exchange, weak|
|[`atomic_exchange_explicit()`](#man-atomic_exchange)|Replace a value in an atomic object, explicit|
|[`atomic_exchange()`](#man-atomic_exchange)|Replace a value in an atomic object|
|[`atomic_fetch_add_explicit()`](#man-atomic_fetch)|Atomically add to an atomic integer, explicit|
|[`atomic_fetch_add()`](#man-atomic_fetch)|Atomically add to an atomic integer|
|[`atomic_fetch_and_explicit()`](#man-atomic_fetch)|Atomically bitwise-AND an atomic integer, explicit|
|[`atomic_fetch_and()`](#man-atomic_fetch)|Atomically bitwise-AND an atomic integer|
|[`atomic_fetch_or_explicit()`](#man-atomic_fetch)|Atomically bitwise-OR an atomic integer, explicit|
|[`atomic_fetch_or()`](#man-atomic_fetch)|Atomically bitwise-OR an atomic integer|
|[`atomic_fetch_sub_explicit()`](#man-atomic_fetch)|Atomically subtract from an atomic integer, explicit|
|[`atomic_fetch_sub()`](#man-atomic_fetch)|Atomically subtract from an atomic integer|
|[`atomic_fetch_xor_explicit()`](#man-atomic_fetch)|Atomically bitwise-XOR an atomic integer, explicit|
|[`atomic_fetch_xor()`](#man-atomic_fetch)|Atomically bitwise-XOR an atomic integer|
|[`atomic_flag_clear_explicit()`](#man-atomic_flag_clear)|Clear an atomic flag, explicit|
|[`atomic_flag_clear()`](#man-atomic_flag_clear)|Clear an atomic flag|
|[`atomic_flag_test_and_set_explicit()`](#man-atomic_flag_test_and_set)|Test and set an atomic flag, explicit|
|[`atomic_flag_test_and_set()`](#man-atomic_flag_test_and_set)|Test and set an atomic flag|
|[`atomic_init()`](#man-atomic_init)|Initialize an atomic variable|
|[`atomic_is_lock_free()`](#man-atomic_is_lock_free)|Determine if an atomic type is lock free|
|[`atomic_load_explicit()`](#man-atomic_load)|Return a value from an atomic variable, explicit|
|[`atomic_load()`](#man-atomic_load)|Return a value from an atomic variable|
|[`atomic_signal_fence()`](#man-atomic_signal_fence)|Fence for intra-thread signal handlers|
|[`atomic_store_explicit()`](#man-atomic_store)|Store a value in an atomic variable, explicit|
|[`atomic_store()`](#man-atomic_store)|Store a value in an atomic variable|
|[`atomic_thread_fence()`](#man-atomic_thread_fence)|Set up a fence|
|[`ATOMIC_VAR_INIT()`](#man-ATOMIC_VAR_INIT)|Create an initializer for an atomic variable|
|[`kill_dependency()`](#man-kill_dependency)|End a dependency chain|

You might need to add `-latomic` to your compilation command line on
Unix-like operating systems.

## Atomic Types

A bunch of types are predefined by this header:

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

You can make your own additional types with the `_Atomic` type
qualifier:

``` {.c}
_Atomic double x;
```

or the `_Atomic()` type specifier:

``` {.c}
_Atomic(double) x;
```

## Lock-free Macros {#lock-free-macros}

These macros let you know if a type is lock-free or not. Maybe.

They can be used at compile time with `#if`. They apply to both signed
and unsigned types.

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
|`1`|_Sometimes_ lock-free[^916a].|
|`2`|Always lock-free.|

[^916a]: Maybe it depends on the run-time environment and can't be known
at compile-time.

## Atomic Flag

The `atomic_flag` opaque type is the only time guaranteed to be
lock-free. Though your PC implementation probably does a lot more.

It is accessed through the
[`atomic_flag_test_and_set()`](#man-atomic_flag_test_and_set) and
[`atomic_flag_clear()`](#man-atomic_flag_clear) functions.

Before use, it can be initialized to a clear state with:

``` {.c}
atomic_flag f = ATOMIC_FLAG_INIT;
```

## Memory Order

This header introduces a new `enum` type called `memory_order`. This is
used by a bunch of the functions to specify memory orders other than
sequential consistency.

|`memory_order`|Description|
|-|-|
|`memory_order_seq_cst`|Sequential Consistency|
|`memory_order_acq_rel`|Acquire/Release|
|`memory_order_release`|Release|
|`memory_order_acquire`|Acquire|
|`memory_order_consume`|Consume|
|`memory_order_relaxed`|Relaxed|

You can feed these into atomic functions with the `_explicit` suffix.

The non-`_explcit` versions of the functions are the same as if you'd
called the `_explicit` counterpart with `memory_order_seq_cst`.

[[manbreak]]
## `ATOMIC_VAR_INIT()` {#man-ATOMIC_VAR_INIT}

Create an initializer for an atomic variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

#define ATOMIC_VAR_INIT(C value)   // Deprecated
```

### Description {.unnumbered .unlisted}

This macro expands to an initializer, so you can use it when a variable
is defined.

The type of the `value` should be the base type of the atomic variable.

The initialization itself is _not_ an atomic operation, ironically.

[fl[CPPReference says this is
deprecated|https://en.cppreference.com/w/cpp/atomic/ATOMIC_VAR_INIT]]
and likely to be removed. Standards document
[fl[p1138r0|http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1138r0.pdf]]
elaborates that the macro is limited in that it can't properly
initialize atomic `struct`s, and its original _raison d'être_ turned out
to not be useful.

Just initialize the variable straight-up, instead.

### Return Value {.unnumbered .unlisted}

Expands to an initializer suitable for this atomic variable.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = ATOMIC_VAR_INIT(3490);  // Deprecated
    printf("%d\n", x);
}
```

### See Also {.unnumbered .unlisted}

[`atomic_init()`](#man-atomic_init)

[[manbreak]]
## `atomic_init()` {#man-atomic_init}

Initialize an atomic variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

void atomic_init(volatile A *obj, C value);
```

### Description {.unnumbered .unlisted}

You can use this to initialize an atomic variable.

The type of the `value` should be the base type of the atomic variable.

The initialization itself is _not_ an atomic operation, ironically.

As far as I can tell, there's no difference between this and assigning
directly to the atomic variable. The spec says it's there to allow the
compiler to inject any additional initialization that needs doing, but
everything seems fine without it. If anyone has more info, send it my
way.

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x;
    
    atomic_init(&x, 3490);

    printf("%d\n", x);
}
```

### See Also {.unnumbered .unlisted}

[`ATOMIC_VAR_INIT()`](#man-ATOMIC_VAR_INIT),
[`atomic_store()`](#man-atomic_store),
[`atomic_store_explicit()`](#man-atomic_store)

[[manbreak]]
## `kill_dependency()` {#man-kill_dependency}

End a dependency chain

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

type kill_dependency(type y);
```

### Description {.unnumbered .unlisted}

This is potentially useful for optimizing if you're using
`memory_order_consume` anywhere.

And if you know what you're doing. If unsure, learn more before trying
to use this.

### Return Value {.unnumbered .unlisted}

Returns the value passed in.

### Example {.unnumbered .unlisted}

In this example, `i` carries a dependency into `x`. And would do into
`y`, except for the call to `kill_dependency()`.

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int a;
    int i = 10, x, y;

    atomic_store_explicit(&a, 3490, memory_order_release);

    i = atomic_load_explicit(&a, memory_order_consume);
    x = i;
    y = kill_dependency(i);

    printf("%d %d\n", x, y);  // 3490 and either 3490 or 10
}
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
## `atomic_thread_fence()` {#man-atomic_thread_fence}

Set up a fence

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

void atomic_thread_fence(memory_order order);
```

### Description {.unnumbered .unlisted}

This sets up a memory fence with the specified `order`.

|`order`|Description|
|-|-|
|`memory_order_seq_cst`|Sequentially consistency acquire/release fence|
|`memory_order_acq_rel`|Acquire/release dence|
|`memory_order_release`|Release fence|
|`memory_order_acquire`|Acquire fence|
|`memory_order_consume`|Acquire fence (again)|
|`memory_order_relaxed`|No fence at all---no point in calling with this|

You might try to avoid using these and just stick with the different
modes with [`atomic_store_explicit()`](#man-atomic_store) and
[`atomic_load_explicit()`](#man-atomic_load). Or not.

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int shared_1 = 1;
atomic_int shared_2 = 2;

int thread_1(void *arg)
{
    (void)arg;

    atomic_store_explicit(&shared_1, 10, memory_order_relaxed);

    atomic_thread_fence(memory_order_release);

    atomic_store_explicit(&shared_2, 20, memory_order_relaxed);

    return 0;
}

int thread_2(void *arg)
{
    (void)arg;

    // If this fence runs after the release fence, we're
    // guaranteed to see thread_1's changes to the shared
    // varaibles.

    atomic_thread_fence(memory_order_acquire);

    if (shared_2 == 20) {
        printf("Shared_1 better be 10 and it's %d\n", shared_1);
    } else {
        printf("Anything's possible: %d %d\n", shared_1, shared_2);
    }

    return 0;
}

int main(void)
{
    thrd_t t1, t2;

    thrd_create(&t2, thread_2, NULL);
    thrd_create(&t1, thread_1, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
}
```

### See Also {.unnumbered .unlisted}

[`atomic_store_explicit()`](#man-atomic_store),
[`atomic_load_explicit()`](#man-atomic_load),
[`atomic_signal_fence()`](#man-atomic_signal_fence)

[[manbreak]]
## `atomic_signal_fence()` {#man-atomic_signal_fence}

Fence for intra-thread signal handlers

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

void atomic_signal_fence(memory_order order);
```

### Description {.unnumbered .unlisted}

This works like `atomic_thread_fence()` except its purpose is within in
a single thread; notably for use in a signal handler in that thread.

Since signals can happen at any time, we might need a way to be certain
that any writes by the thread that happened before the signal handler be
visible within that signal handler.

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

Partial demo. (Note that it's technically undefined behavior to call
`printf()` in a signal handler.)

``` {.c .numberLines}
#include <stdio.h>
#include <signal.h>
#include <stdatomic.h>

int global;

void handler(int sig)
{
    (void)sig;

    // If this runs before the release, the handler will
    // potentially see global == 0.
    //
    // Otherwise, it will definitely see global == 10.

    atomic_signal_fence(memory_order_acquire);

    printf("%d\n", global);
}

int main(void)
{
    signal(SIGINT, handler);

    global = 10;

    atomic_signal_fence(memory_order_release);

    // If the signal handler runs after the release
    // it will definitely see the value 10 in global.
}
```

### See Also {.unnumbered .unlisted}

[`atomic_thread_fence()`](#man-atomic_thread_fence),
[`signal()`](#man-signal)

[[manbreak]]
## `atomic_is_lock_free()` {#man-atomic_is_lock_free}

Determine if an atomic type is lock free

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

_Bool atomic_is_lock_free(const volatile A *obj);
```

### Description {.unnumbered .unlisted}

Determines if the variable `obj` of type `A` is lock-free. Can be used
with any type.

Unlike the [lock-free macros](#lock-free-macros) which can be used at
compile-time, this is strictly a run-time function. So in places where
the macros say "maybe", this function will definitely tell you one way
or another if the atomic variable is lock-free.

This is useful when you're defining your own atomic variables and want
to know their lock-free status.

### Return Value {.unnumbered .unlisted}

True if the variable is lock-free, false otherwise.

### Example {.unnumbered .unlisted}

Test if a couple `struct`s and an atomic `double` are lock-free. On my
system, the larger `struct` is too big to be lock-free, but the other
two are OK.

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
	struct foo {
		int x, y;
	};

	struct bar {
		int x, y, z;
	};

	_Atomic(double) a;
	struct foo b;
	struct bar c;

	printf("a is lock-free: %d\n", atomic_is_lock_free(&a));
	printf("b is lock-free: %d\n", atomic_is_lock_free(&b));
	printf("c is lock-free: %d\n", atomic_is_lock_free(&c));
}
```

Output on my system (YMMV):

``` {.default}
a is lock-free: 1
b is lock-free: 1
c is lock-free: 0
```

### See Also {.unnumbered .unlisted}

[Lock-free Macros](#lock-free-macros)

[[manbreak]]
## `atomic_store()` {#man-atomic_store}

Store a value in an atomic variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

void atomic_store(volatile A *object, C desired);

void atomic_store_explicit(volatile A *object,
                           C desired, memory_order order);
```

### Description {.unnumbered .unlisted}

Store a value in an atomic variable, possible synchronized.

This is like a plain assignment, but with more flexibility.

These have the same storage effect for an `atomic_int x`:

``` {.c}
x = 10;
atomic_store(&x, 10);
atomic_store_explicit(&x, 10, memory_order_seq_cst);
```

But the last function, `atomic_store_explicit()`, lets you specify the
memory order.

Since this is a "release-y" operation, none of the "acquire-y" memory
orders are legal. `order` can be only be `memory_order_seq_cst`,
`memory_order_release`, or `memory_order_relaxed`.

`order` cannot be `memory_order_acq_rel`, `memory_order_acquire`, or
`memory_order_consume`.

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = 0;
    atomic_int y = 0;

    atomic_store(&x, 10);

    atomic_store_explicit(&y, 20, memory_order_relaxed);

    // Will print either "10 20" or "10 0":
    printf("%d %d\n", x, y);
}
```

### See Also {.unnumbered .unlisted}

[`atomic_init()`](#man-atomic_init),
[`atomic_load()`](#man-atomic_load),
[`atomic_load_explicit()`](#man-atomic_load),
[`atomic_exchange()`](#man-atomic_exchange), \
[`atomic_exchange_explicit()`](#man-atomic_exchange),
[`atomic_compare_exchange_strong()`](#man-atomic_compare_exchange), \
[`atomic_compare_exchange_strong_explicit()`](#man-atomic_compare_exchange),
[`atomic_compare_exchange_weak()`](#man-atomic_compare_exchange), \
[`atomic_compare_exchange_weak_explicit()`](#man-atomic_compare_exchange),
[`atomic_fetch_*()`](#man-atomic_fetch)

[[manbreak]]
## `atomic_load()` {#man-atomic_load}

Return a value from an atomic variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

C atomic_load(const volatile A *object);

C atomic_load_explicit(const volatile A *object, memory_order order);
```

### Description {.unnumbered .unlisted}

For a pointer to an `object` of type `A`, atomically returns its value
`C`. This is a generic function that can be used with any type.

The function `atomic_load_explicit()` lets you specify the memory order.

Since this is an "acquire-y" operation, none of the "release-y" memory
orders are legal. `order` can be only be `memory_order_seq_cst`,
`memory_order_acquire`, `memory_order_consume`, or
`memory_order_relaxed`.

`order` cannot be `memory_order_acq_rel` or `memory_order_release`.

### Return Value {.unnumbered .unlisted}

Returns the value stored in `object`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = 10;

    int v = atomic_load(&x);

    printf("%d\n", v);  // 10
}
```

### See Also {.unnumbered .unlisted}

[`atomic_store()`](#man-atomic_store),
[`atomic_store_explicit()`](#man-atomic_store)

[[manbreak]]
## `atomic_exchange()` {#man-atomic_exchange}

Replace a value in an atomic object

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

C atomic_exchange(volatile A *object, C desired);

C atomic_exchange_explicit(volatile A *object, C desired,
                           memory_order order);
```

### Description {.unnumbered .unlisted}

Sets the value in `object` to `desired`.

`object` is type `A`, some atomic type.

`desired` is type `C`, the respective non-atomic type to `A`.

This is very similar to `atomic_store()`, except the previous value is
atomically returned.

### Return Value {.unnumbered .unlisted}

Returns the previous value of `object`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = 10;

    int previous = atomic_exchange(&x, 20);

    printf("x is  %d\n", x);
    printf("x was %d\n", previous);
}
```

Output:

``` {.default}
x is  20
x was 10
```

### See Also {.unnumbered .unlisted}

[`atomic_init()`](#man-atomic_init),
[`atomic_load()`](#man-atomic_load),
[`atomic_load_explicit()`](#man-atomic_load),
[`atomic_store()`](#man-atomic_store), \
[`atomic_store_explicit()`](#man-atomic_store)
[`atomic_compare_exchange_strong()`](#man-atomic_compare_exchange), \
[`atomic_compare_exchange_strong_explicit()`](#man-atomic_compare_exchange),
[`atomic_compare_exchange_weak()`](#man-atomic_compare_exchange), \
[`atomic_compare_exchange_weak_explicit()`](#man-atomic_compare_exchange)

[[manbreak]]
## `atomic_compare_exchange_*()` {#man-atomic_compare_exchange}

Atomic compare and exchange

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

_Bool atomic_compare_exchange_strong(volatile A *object,
                                     C *expected, C desired);

_Bool atomic_compare_exchange_strong_explicit(volatile A *object,
                                              C *expected, C desired,
                                              memory_order success,
                                              memory_order failure);

_Bool atomic_compare_exchange_weak(volatile A *object,
                                   C *expected, C desired);

_Bool atomic_compare_exchange_weak_explicit(volatile A *object,
                                            C *expected, C desired,
                                            memory_order success,
                                            memory_order failure);
```

### Description {.unnumbered .unlisted}

The venerable basis for some many things lock-free: compare and
exchange.

In the above prototypes, `A` is the type of the atomic object, and `C`
is the equivalent base type.

Ignoring the `_explicit` versions for a moment, what these do is:

* If the value pointed to by `object` is equal to the value pointed to by
  `expected`, then the value pointed to by `object` is set to `desired`.
  And the function returns `true` indicating the exchange did take place.

* Else the value pointed to by `expected` (yes, `expected`) is set to
  `desired` and the function returns `false` indicating the exchange did
  not take place.

Pseudocode for the exchange would look like this^[This effectively does
the same thing, but it's clearly not atomic.]:

``` {.c}
bool compare_exchange(atomic_A *object, C *expected, C desired)
{
    if (*object is the same as *expected) {
        *object = desired
        return true
    }

    *expected = desired
    return false
}
```

The `_weak` variants might spontaneously fail, so even if `*object ==
*desired`, it might not change the value and will return `false`. So
you'll want that in a loop if you use it^[The spec says, "This spurious
failure enables implementation of compare-and-exchange on a broader
class of machines, e.g. load-locked store-conditional machines."  And
adds, "When a compare-and-exchange is in a loop, the weak version will
yield better performance on some platforms. When a weak
compare-and-exchange would require a loop and a strong one would not,
the strong one is preferable."].

The `_explicit` variants have two memory orders: `success` if `*object`
is set to `desired`, and `failure` if it is not.

These are test-and-set functions, so you can use `memory_order_acq_rel`
with the `_explicit` variants.

### Return Value {.unnumbered .unlisted}

Returns `true` if `*object` was `*expected`. Otherwise, `false`.

### Example {.unnumbered .unlisted}

A contrived example where multiple threads add `2` to a shared value in
a lock-free way.

(It would be better to use `+= 2` to get this done in real life unless
you were using some `_explicit` wizardry.)

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#define LOOP_COUNT 10000

atomic_int value;

int run(void *arg)
{
    (void)arg;

    for(int i = 0; i < LOOP_COUNT; i++) {

        int cur = value;
        int next;

        do {
            next = cur + 2;
        } while (!atomic_compare_exchange_strong(&value, &cur, next));
    }

    return 0;
}

int main(void)
{
    thrd_t t1, t2;

    thrd_create(&t1, run, NULL);
    thrd_create(&t2, run, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
    
    printf("%d should equal %d\n", value, LOOP_COUNT * 4);
}
```

Just replacing this with `value = value + 2` causes data trampling.

### See Also {.unnumbered .unlisted}

[`atomic_load()`](#man-atomic_load),
[`atomic_load_explicit()`](#man-atomic_load),
[`atomic_store()`](#man-atomic_store),
[`atomic_store_explicit()`](#man-atomic_store),
[`atomic_exchange()`](#man-atomic_exchange),
[`atomic_exchange_explicit()`](#man-atomic_exchange),
[`atomic_fetch_*()`](#man-atomic_fetch)

[[manbreak]]
## `atomic_fetch_*()` {#man-atomic_fetch}

Atomically modify atomic variables

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

C atomic_fetch_KEY(volatile A *object, M operand);

C atomic_fetch_KEY_explicit(volatile A *object, M operand,
                            memory_order order);
```

### Description {.unnumbered .unlisted}

These are actually a group of 10 functions. You substitute one of the
following for `KEY` to perform that operation:

* `add`
* `sub`
* `or`
* `xor`
* `and`

So these functions can add or subtract values to or from an atomic
variable, or can perform bitwise-OR, XOR, or AND on them.

Use it with integer or pointer types. Though the spec is a little vague
on the matter, other types make C unhappy. It goes out of its way to
avoid undefined behavior with signed integers, as well:

C18 §7.17.7.5 ¶3:

> For signed integer types, arithmetic is defined to use two’s
> complement representation with silent wrap-around on overflow; there
> are no undefined results.

In the synopsis, above, `A` is an atomic type, and `M` is the
corresponding non-atomic type for `A` (or `ptrdiff_t` for atomic
pointers), and `C` is the corresponding non-atomic type for `A`.

For example, here are some operations on an `atomic_int`.

``` {.c}
atomic_fetch_add(&x, 20);
atomic_fetch_sub(&x, 37);
atomic_fetch_xor(&x, 3490);
```

They are the same as `+=`, `-=`, `|=`, `^=` and `&=`, except the return
value is the _previous_ value of the atomic object. (With the assignment
operators, the value of the expression is that _after_ its evaluation.)

``` {.c}
atomic_int x = 10;
int prev = atomic_fetch_add(&x, 20);
printf("%d %d\n", prev, x);  // 10 30
```

versus:

``` {.c}
atomic_int x = 10;
int prev = (x += 20);
printf("%d %d\n", prev, x);  // 30 30
```

And, of course, the `_explicit` version allows you to specify a memory
order and all the assignment operators are `memory_order_seq_cst`.

### Return Value {.unnumbered .unlisted}

Returns the previous value of the atomic object before the modification.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    atomic_int x = 0;
    int prev;

    atomic_fetch_add(&x, 3490);
    atomic_fetch_sub(&x, 12);
    atomic_fetch_xor(&x, 444);
    atomic_fetch_or(&x, 12);
    prev = atomic_fetch_and(&x, 42);

    printf("%d %d\n", prev, x);   // 3118 42
}
```

### See Also {.unnumbered .unlisted}

[`atomic_exchange()`](#man-atomic_exchange),
[`atomic_exchange_explicit()`](#man-atomic_exchange),
[`atomic_compare_exchange_strong()`](#man-atomic_compare_exchange), \
[`atomic_compare_exchange_strong_explicit()`](#man-atomic_compare_exchange),
[`atomic_compare_exchange_weak()`](#man-atomic_compare_exchange), \
[`atomic_compare_exchange_weak_explicit()`](#man-atomic_compare_exchange)


[[manbreak]]
## `atomic_flag_test_and_set()` {#man-atomic_flag_test_and_set}

Test and set an atomic flag

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

_Bool atomic_flag_test_and_set(volatile atomic_flag *object);

_Bool atomic_flag_test_and_set_explicit(volatile atomic_flag *object,
                                        memory_order order);
```

### Description {.unnumbered .unlisted}

One of the venerable old functions of lock-free programming, this
function sets the given atomic flag in `object`, and returns the
previous value of the flag.

As usual, the `_explicit` allows you to specify an alternate memory
order.

### Return Value {.unnumbered .unlisted}

Returns `true` if the flag was set previously, and `false` if it wasn't.

### Example {.unnumbered .unlisted}

Using test-and-set to implement a spin lock^[Don't use this unless you
know what you're doing---use the thread mutex functionality instead.
It'll let your blocked thread sleep and stop chewing up CPU.]:

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

// Shared non-atomic struct
struct {
    int x, y, z;
} s = {1, 2, 3};

atomic_flag f = ATOMIC_FLAG_INIT;

int run(void *arg)
{
    int tid = *(int*)arg;

    printf("Thread %d: waiting for lock...\n", tid);

    while (atomic_flag_test_and_set(&f));

    printf("Thread %d: got lock, s is {%d, %d, %d}\n", tid,
                                                       s.x, s.y, s.z);
    s.x = (tid + 1) * 5 + 0;
    s.y = (tid + 1) * 5 + 1;
    s.z = (tid + 1) * 5 + 2;
    printf("Thread %d: set s to {%d, %d, %d}\n", tid, s.x, s.y, s.z);

    printf("Thread %d: releasing lock...\n", tid);
    atomic_flag_clear(&f);

    return 0;
}

int main(void)
{
    thrd_t t1, t2;
    int tid[] = {0, 1};

    thrd_create(&t1, run, tid+0);
    thrd_create(&t2, run, tid+1);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
}
```

Example output (varies run to run):

``` {.default}
Thread 0: waiting for lock...
Thread 0: got lock, s is {1, 2, 3}
Thread 1: waiting for lock...
Thread 0: set s to {5, 6, 7}
Thread 0: releasing lock...
Thread 1: got lock, s is {5, 6, 7}
Thread 1: set s to {10, 11, 12}
Thread 1: releasing lock...
```

### See Also {.unnumbered .unlisted}

[`atomic_flag_clear()`](#man-atomic_flag_clear)

[[manbreak]]
## `atomic_flag_clear()` {#man-atomic_flag_clear}

Clear an atomic flag

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

void atomic_flag_clear(volatile atomic_flag *object);

void atomic_flag_clear_explicit(volatile atomic_flag *object,
                                memory_order order);
```

### Description {.unnumbered .unlisted}

Clears an atomic flag.

As usual, the `_explicit` allows you to specify an alternate memory
order.

### Return Value {.unnumbered .unlisted}

Returns nothing!

### Example {.unnumbered .unlisted}

Using test-and-set to implement a spin lock^[Don't use this unless you
know what you're doing---use the thread mutex functionality instead.
It'll let your blocked thread sleep and stop chewing up CPU.]:

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

// Shared non-atomic struct
struct {
    int x, y, z;
} s = {1, 2, 3};

atomic_flag f = ATOMIC_FLAG_INIT;

int run(void *arg)
{
    int tid = *(int*)arg;

    printf("Thread %d: waiting for lock...\n", tid);

    while (atomic_flag_test_and_set(&f));

    printf("Thread %d: got lock, s is {%d, %d, %d}\n", tid,
                                                       s.x, s.y, s.z);
    s.x = (tid + 1) * 5 + 0;
    s.y = (tid + 1) * 5 + 1;
    s.z = (tid + 1) * 5 + 2;
    printf("Thread %d: set s to {%d, %d, %d}\n", tid, s.x, s.y, s.z);

    printf("Thread %d: releasing lock...\n", tid);
    atomic_flag_clear(&f);

    return 0;
}

int main(void)
{
    thrd_t t1, t2;
    int tid[] = {0, 1};

    thrd_create(&t1, run, tid+0);
    thrd_create(&t2, run, tid+1);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
}
```

Example output (varies run to run):

``` {.default}
Thread 0: waiting for lock...
Thread 0: got lock, s is {1, 2, 3}
Thread 1: waiting for lock...
Thread 0: set s to {5, 6, 7}
Thread 0: releasing lock...
Thread 1: got lock, s is {5, 6, 7}
Thread 1: set s to {10, 11, 12}
Thread 1: releasing lock...
```

### See Also {.unnumbered .unlisted}

[`atomic_flag_test_and_set()`](#man-atomic_flag_test_and_set)

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