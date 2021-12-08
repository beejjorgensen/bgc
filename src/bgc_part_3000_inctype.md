<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Incomplete Types

It might surprise you to learn that this builds without error:

``` {.c}
extern int a[];

int main(void)
{
    struct foo *x;
    union bar *y;
    enum baz *z;
}
```

We never gave a size for `a`. And we have pointers to `struct`s `foo`,
`bar`, and `baz` that never seem to be declared anywhere.

And the only warnings I get are that `x`, `y`, and `z` are unused.

These are examples of _incomplete types_.

An incomplete type is a type the size (i.e. the size you'd get back from
`sizeof`) for which is not known. Another way to think of it is a type
that you haven't finished declaring.

You can have a pointer to an incomplete type, but you can't dereference
it or use pointer arithmetic on it. And you can't `sizeof` it.

So what can you do with it?

## Use Case: Self-Referential Structures

I only know of one real use case: forward references to `struct`s or
`union`s with self-referential or co-dependent structures. (I'm going to
use `struct` for the rest of these examples, but they all apply equally
to `union`s, as well.)

Let's do the classic example first.

But before I do, know this! As you declare a `struct`, the `struct` is
incomplete until the closing brace is reached!

``` {.c}
struct antelope {              // struct antelope is incomplete here
    int leg_count;             // Still incomplete
    float stomach_fullness;    // Still incomplete
    float top_speed;           // Still incomplete
    char *nickname;            // Still incomplete
};                             // NOW it's complete.
```

So what? Seems sane enough.

But what if we're doing a linked list? Each linked list node needs to
have a reference to another node. But how can we create a reference to
another node if we haven't finished even declaring the node yet?

C's allowance for incomplete types makes it possible. We can't declare a
node, but we _can_ declare a pointer to one, even if it's incomplete!

``` {.c}
struct node {
    int val;
    struct node *next;  // struct node is incomplete, but that's OK!
};
```

Even though the `struct node` is incomplete on line 3, we can still
declare a pointer to one^[This works because in C, pointers are the same
size regardless of the type of data they point to. So the compiler
doesn't need to know the size of the `struct node` at this point; it
just needs to know the size of a pointer.].

We can do the same thing if we have two different `struct`s that refer
to each other:

``` {.c}
struct a {
    struct b *x;  // Refers to a `struct b`
};

struct b {
    struct a *x;  // Refers to a `struct a`
};
```

We'd never be able to make that pair of structures without the relaxed
rules for incomplete types.

## Incomplete Type Error Messages

Are you getting errors like these?

``` {.default}
invalid application of ‘sizeof’ to incomplete type

invalid use of undefined type

dereferencing pointer to incomplete type
```

Most likely culprit: you probably forgot to `#include` the header file
that declares the type.

## Other Incomplete Types

Declaring a `struct` or `union` with no body makes an incomplete type,
e.g. `struct foo;`.

`enums` are incomplete until the closing brace.

`void` is an incomplete type.

Arrays declared `extern` with no size are incomplete, e.g.:

``` {.c}
extern int a[];
```

If it's a non-`extern` array with no size followed by an initializer,
it's incomplete until the closing brace of the initializer.

## Use Case: Arrays in Header Files

It can be useful to declare incomplete array types in header files. In
those cases, the actual storage (where the complete array is declared)
should be in a single `.c` file. If you put it in the `.h` file, it will
be duplicated every time the header file is included.

So what you can do is make a header file with an incomplete type that
refers to the array, like so:

``` {.c .numberLines}
// File: bar.h

#ifndef BAR_H
#define BAR_H

extern int my_array[];  // Incomplete type

#endif
```

And the in the `.c` file, actually define the array:

``` {.c .numberLines}
// File: bar.c

int my_array[1024];     // Complete type!
```

Then you can include the header from as many places as you'd like, and
every one of those places will refer to the same underlying `my_array`.

``` {.c .numberLines}
// File: foo.c

#include <stdio.h>
#include "bar.h"    // includes the incomplete type for my_array

int main(void)
{
    my_array[0] = 10;

    printf("%d\n", my_array[0]);
}
```

When compiling multiple files, remember to specify all the `.c` files
to the compiler, but not the `.h` files, e.g.:

``` {.zsh}
gcc -o foo foo.c bar.c
```

## Completing Incomplete Types

If you have an incomplete type, you can complete it by defining the
complete `struct`, `union`, `enum`, or array in the same scope.

``` {.c}
struct foo;        // incomplete type

struct foo *p;     // pointer, no problem

// struct foo f;   // Error: incomplete type!

struct foo {
    int x, y, z;
};                 // Now the struct foo is complete!

struct foo f;      // Success!
```

Note that though `void` is an incomplete type, there's no way to
complete it. Not that anyone ever thinks of doing that weird thing. But
it does explain why you can do this:

``` {.c}
void *p;             // OK: pointer to incomplete type
```

and not either of these:

``` {.c}
void v;              // Error: declare variable of incomplete type

printf("%d\n", *p);  // Error: dereference incomplete type
```

The more you know...
