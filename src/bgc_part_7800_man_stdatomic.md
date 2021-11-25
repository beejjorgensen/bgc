<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdatomic.h>` Atomic-Related Functions {#stdatomic}

|Function|Description|
|-|-|
|[`ATOMIC_VAR_INIT()`](#man-ATOMIC_VAR_INIT)|Create an initializer for an atomic variable|
|[`atomic_init()`](#man-atomic_init)|Initialize an atomic variable|
|[`kill_dependency()`](#man-kill_dependency)|End a dependency chain|
|[`atomic_thread_fence()`](#man-atomic_thread_fence)|Set up a fence|
|[`atomic_signal_fence()`](#man-atomic_signal_fence)|Fence for intra-thread signal handlers|

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

The `atomic_flag` type is the only time guaranteed to be lock-free.
Though your PC implementation probably does a lot more.

It is accessed through the
[`atomic_flag_test_and_set()`](#man-atomic_flag_test_and_set) and
[`atomic_flag_clear()`](#man-atomic_flag_clear) functions.

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

[`ATOMIC_VAR_INIT()`](#man-ATOMIC_VAR_INIT)

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
modes with [`atomic_store_explicit()`](#man-atomic_store_explicit) and
[`atomic_load_explicit()`](#man-atomic_load_explicit). Or not.

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

[`atomic_store_explicit()`](#man-atomic_store_explicit),
[`atomic_load_explicit()`](#man-atomic_load_explicit),
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

[`example()`](#man-example),
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