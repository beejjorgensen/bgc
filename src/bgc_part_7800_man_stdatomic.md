<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdatomic.h>` Atomic-Related Functions {#stdatomic}


|Function|Description|
|-|-|

You might need to add `-latomic` to your compilation command line on
Unix-like operating systems.

[[manbreak]]
## `ATOMIC_VAR_INIT()` {#man-ATOMIC_VAR_INIT}

Initialize an atomic variable

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

#define ATOMIC_VAR_INIT(C value)
```

### Description {.unnumbered .unlisted}

You can use this to initialize an atomic variable, especially one of
static storage duration.

The type of the `value` should be the base type of the atomic variable.

The initialization itself is _not_ an atomic operation, ironically.

[fl[CPPReference says this is
deprecated|https://en.cppreference.com/w/cpp/atomic/ATOMIC_VAR_INIT]]
and likely to be removed.

### Return Value {.unnumbered .unlisted}

Expands to an initializer suitable for this atomic variable.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <stdatomic.h>

atomic_int x = ATOMIC_VAR_INIT(3490);

int main(void)
{
    printf("%d\n", x);
}
```

### See Also {.unnumbered .unlisted}

[`atomic_init()`](#man-atomic_init)


[[manbreak]]
## `atomic_init()` {#man-atomic_init}

Initialize automatic atomic variables.

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdatomic.h>

void atomic_init(volatile A *obj, C value);
```

### Description {.unnumbered .unlisted}

You can use this to initialize an atomic variable.

The type of the `value` should be the base type of the atomic variable.

The initialization itself is _not_ an atomic operation, ironically.

### Return Value {.unnumbered .unlisted}

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