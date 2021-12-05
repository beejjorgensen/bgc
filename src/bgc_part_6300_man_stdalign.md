<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdalign.h>` Macros for Alignment {#stdalign}

If you're coding up something low-level like a memory allocator that
interfaces with your OS, you might need this header file. But most C
devs go their careers without using it.

[flw[_Alignment_|Data_structure_alignment]] is all about multiples of
addresses on which objects can be stored. Can you store this at any
address? Or must it be a starting address that's divisible by 2? Or 8?
Or 16?

|Name|Description
|-|-|
|[`alignas()`](#man-alignas)|Specify alignment, expands to `_Alignas`|
|[`alignof()`](#man-alignof)|Get alignment, expands to `_Alignof`|

These two additional macros are defined to be `1`:

``` {.c}
__alignas_is_defined
__alignof_is_defined
```

Quick note: alignments greater than that of `max_align_t` are known as
_overalignments_ and are implementation-defined.


## `alignas()` `_Alignas()` {#man-alignas}

Force a variable to have a certain alignment

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdalign.h>

alignas(type-name)
alignas(constant-expression)
```

``` {.c}
_Alignas(type-name)
_Alignas(constant-expression)
```

### Description {.unnumbered .unlisted}

Use this _alignment specifier_ to force the alignment of particular
variables. For instance, we can declare `c` to be `char`, but aligned as
if it were an `int`:

``` {.c}
char alignas(int) c;
```

You can put a constant integer expression in there, as well. The
compiler will probably impose limits on what these values can be. Small
powers of 2 (1, 2, 4, 8, and 16) are generally safe bets.

``` {.c}
char alignas(8) c;   // align on 8-byte boundaries
```

For convenience, you can also specify `0` if you want the default
alignment (as if you hadn't said `alignas()` at all):

``` {.c}
char alignas(0) c;   // use default alignment for this type
```

<!--
### Return Value {.unnumbered .unlisted}
-->

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdalign.h>
#include <stdio.h>     // for printf()
#include <stddef.h>    // for max_align_t

int main(void)
{
    int i, j;
    char alignas(max_align_t) a, b;
    char alignas(int) c, d;
    char e, f;

    printf("i: %p\n", (void *)&i);
    printf("j: %p\n\n", (void *)&j);
    printf("a: %p\n", (void *)&a);
    printf("b: %p\n\n", (void *)&b);
    printf("c: %p\n", (void *)&c);
    printf("d: %p\n\n", (void *)&d);
    printf("e: %p\n", (void *)&e);
    printf("f: %p\n", (void *)&f);
}
```

Output on my system follows. Notice the difference between the pairs of
values.

* `i` and `j`, both `int`s, are aligned on 4-byte boundaries.
* `a` and `b` have been forced to the boundary of the type
  `max_align_t`, which is every 16 bytes on my system.
* `c` and `d` have been forced to the same alignment as `int`, which is
  4 bytes, just like with `i` and `j`.
* `e` and `f` do not have an alignment specified, so they were stored
  with their default alignment of 1 byte.

``` {.default}
i: 0x7ffee7dfb4cc    <-- difference of 4 bytes
j: 0x7ffee7dfb4c8

a: 0x7ffee7dfb4c0    <-- difference of 16 bytes
b: 0x7ffee7dfb4b0

c: 0x7ffee7dfb4ac    <-- difference of 4 bytes
d: 0x7ffee7dfb4a8

e: 0x7ffee7dfb4a7    <-- difference of 1 byte
f: 0x7ffee7dfb4a6
```

### See Also {.unnumbered .unlisted}

[`alignof`](#man-alignof),
[`max_align_t`](#man-max_align_t)

[[manbreak]]
## `alignof()` `_Alignof()` {#man-alignof}

Get the alignment of a type

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdalign.h>

alignof(type-name)
```

``` {.c}
_Alignof(type-name)
```

### Description {.unnumbered .unlisted}

This evaluates to a value of type `size_t` that gives the alignment of a
particular type on your system.

### Return Value {.unnumbered .unlisted}

Returns the alignment value, i.e. the address of the beginning of the
given type of object must begin on an address boundary divisible by this
number.

### Example {.unnumbered .unlisted}

Print out the alignments of a variety of different types.

``` {.c .numberLines}
#include <stdalign.h>
#include <stdio.h>     // for printf()
#include <stddef.h>    // for max_align_t

struct t {
    int a;
    char b;
    float c;
};

int main(void)
{
    printf("char       : %zu\n", alignof(char));
    printf("short      : %zu\n", alignof(short));
    printf("int        : %zu\n", alignof(int));
    printf("long       : %zu\n", alignof(long));
    printf("long long  : %zu\n", alignof(long long));
    printf("double     : %zu\n", alignof(double));
    printf("long double: %zu\n", alignof(long double));
    printf("struct t   : %zu\n", alignof(long double));
    printf("max_align_t: %zu\n", alignof(max_align_t));
}
```

Output on my system:

``` {.default}
char       : 1
short      : 2
int        : 4
long       : 8
long long  : 8
double     : 8
long double: 16
struct t   : 16
max_align_t: 16
```

### See Also {.unnumbered .unlisted}

[`alignas`](#man-alignas),
[`max_align_t`](#man-max_align_t)

<!--
[[manbreak]]
## `example()` `example()` `example()` {#man-example}

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