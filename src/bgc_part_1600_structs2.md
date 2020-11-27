<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `struct`s II: More Fun with `struct`s

Turns out there's a lot more you can do with `struct`s than we've talked
about, but it's just a big pile of miscellaneous things. So we'll throw
them in this chapter.

If you're good with `struct` basics, you can round out your knowledge
here.

## Anonymous `struct`s

These are "the `struct` with no name". We also mention these in the
[`typedef`](#typedef-struct) section, but we'll refresh here.

Here's a regular `struct`:

``` {.c}
struct animal {
    char *name;
    int leg_count, speed;
};
```

And here's the anonymous equivalent:

``` {.c}
struct {              // <-- No name!
    char *name;
    int leg_count, speed;
};
```

Okaaaaay. So we have a `struct`, but it has no name, so we have no way
of using it later? Seems pretty pointless.

Admittedly, in that example, it is. But we can still make use of it a
couple ways.

One is rare, but since the anonymous `struct` represents a type, we can
just put some variable names after it and use them.

``` {.c}
struct {                   // <-- No name!
    char *name;
    int leg_count, speed;
} a, b, c;                 // 3 variables of this struct type

a.name = "antelope";
c.leg_count = 4;           // for example
```

But that's still not that useful.

Far more common is use of anonymous `struct`s with a `typedef` so that
we can use it later (e.g. to pass variables to functions).

``` {.c}
typedef struct {                   // <-- No name!
    char *name;
    int leg_count, speed;
} animal;                          // New type: animal

animal a, b, c;

a.name = "antelope";
c.leg_count = 4;           // for example
```

Personally, I don't use many anonymous `struct`s. I think it's more
pleasant to see the entire `struct animal` before the variable name in a
declaration.

But that's just, like, my opinion, man.

## Self-Referential `struct`s

For any graph-like data structure, it's useful to be able to have
pointers to the connected nodes/vertices. But this means that in the
definition of a node, you need to have a pointer to a node. It's chicken
and eggy!

But it turns out you can do this in C with no problem whatsoever.

For example, here's a linked list node:

``` {.c}
struct node {
    int data;
    struct node *next;
};
```

It's important to node that `next` is a pointer. This is what allows the
whole thing to even build. Even though the compiler doesn't know what
the entire `struct node` looks like yet, all pointers are the same size.

Here's a cheesy linked list program to test it out:

``` {.c .numberLines}
#include <stdio.h>

struct node {
    int data;
    struct node next;
};

int main(void)
{
    struct node head = {.data=11}, n1 = {.data=22}, n2 = {.data=33};

    // Set up the linked list
    head.next = &n1;
    n1.next = &n2;
    n2.next = NULL;

    // Traverse it
    for (struct node *cur = &head; cur != NULL; cur = cur->next) {
        printf("%d\n", cur->data);
    }

    return 0;
}
```

Normally that linked list would have been set up with `malloc()`, but
just in case you haven't read that chapter yet, we kinda mocked it up
there another way.

## Flexible Array Members


    * Bitfields
    * Padding bytes
    * Unions
