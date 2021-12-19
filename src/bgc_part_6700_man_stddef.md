<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stddef.h>` A Few Standard Definitions {#stddef}

Despite its name, I've haven't seen this frequently included.

It includes several types and macros.

|Name|Description
|-|-|
|[`ptrdiff_t`](#man-ptrdiff_t)|Signed integer difference between two pointers|
|[`size_t`](#man-size_t)|Unsigned integer type returned by `sizeof`|
|[`max_align_t`](#man-max_align_t)|Declare a type with the biggest possible alignment|
|[`wchar_t`](#man-wchar_t)|Wide character type|
|`NULL`|`NULL` pointer, as defined a number of places|
|[`offsetof`](#man-offsetof)|Get the byte offsets of `struct` or `union` fields|

## `ptrdiff_t` {#man-ptrdiff_t}

This holds the different between two pointers. You could store this in
another type, but the result of a pointer subtraction is an
implementation-defined type; you can be maximally portable by using
`ptrdiff_t`.

``` {.c .numberLines}
#include <stdio.h>
#include <stddef.h>

int main(void)
{
	int cats[100];

	int *f = cats + 20;
	int *g = cats + 60;

	ptrdiff_t d = g - f;  // difference is 40

```

And you can print it by prefixing the integer format specifier with `t`:

``` {.c .numberLines startFrom="13"}
	printf("%td\n", d);  // Print decimal: 40
	printf("%tX\n", d);  // Print hex:     28
}
```

## `size_t` {#man-size_t}

This is the type returned by `sizeof` and used in a few other places.
It's an unsigned integer.

You can print it using the `z` prefix in `printf()`:

``` {.c .numberLines}
#include <stdio.h>
#include <uchar.h>
#include <string.h>
#include <stddef.h>

int main(void)
{
    size_t x;

    x = sizeof(int);

    printf("%zu\n", x);

```

Some functions return negative numbers cast to `size_t` as error values
(such as [`mbrtoc16()`](#man-mbrtoc16)). If you want to print these as
negative values, you can do it with `%zd`:

``` {.c .numberLines startFrom="14"}
    char16_t a;
    mbstate_t mbs;
    memset(&mbs, 0, sizeof mbs);

    x = mbrtoc16(&a, "b", 8, &mbs);

    printf("%zd\n", x);
}
```

## `max_align_t` {#man-max_align_t}

As far as I can tell, this exists to allow the runtime computation of
the maximum fundamental [flw[alignment|Data_structure_alignment]] on the
current platform. Someone please mail me if there's another use.

Maybe you need this if you're writing your own memory allocator or
somesuch.

``` {.c .numberLines}
#include <stddef.h>
#include <stdio.h>      // For printf()
#include <stdalign.h>   // For alignof

int main(void)
{
    int max = alignof(max_align_t);

    printf("Maximum fundamental alignment: %d\n", max);
}
```

On my system, this prints:

``` {.default}
Maximum fundamental alignment: 16
```

See also [`alignas`](#man-alignas), [`alignof`](#man-alignof).

## `wchar_t` {#man-wchar_t}

This is analogous to `char`, except it's for [wide
characters](#wide-characters).

It's an integer type that has enough range to hold unique values for all
characters in all supported locales.

The value `0` is the wide `NUL` character.

Finally, the values of character constants from the [basic character
set](#src-exec-charset) will be the same as their corresponding
`wchar_t` values... unless `__STDC_MB_MIGHT_NEQ_WC__` is defined.

## `offsetof` {#man-offsetof}

If you have a `struct` or `union`, you can use this to get the byte
offset of fields within that type.

Usage is:

``` {.c}
offsetof(type, fieldname);
```

The resulting value has type `size_t`.

Here's an example that prints the field offsets of a `struct`:

``` {.c .numberLines}
#include <stdio.h>
#include <stddef.h>

struct foo {
    int a;
    char b;
    char c;
    float d;
};

int main(void)
{
    printf("a: %zu\n", offsetof(struct foo, a));
    printf("b: %zu\n", offsetof(struct foo, b));
    printf("c: %zu\n", offsetof(struct foo, c));
    printf("d: %zu\n", offsetof(struct foo, d));
}
```

On my system, this outputs:

``` {.default}
a: 0
b: 4
c: 5
d: 8
```

And you can't use `offsetof` on a bitfield, so don't get your hopes up.
