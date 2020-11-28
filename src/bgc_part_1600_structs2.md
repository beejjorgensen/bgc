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
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

int main(void)
{
    struct node *head;

    // Hackishly set up a linked list (11)->(22)->(33)
    head = malloc(sizeof(struct node));
    head->data = 11;
    head->next = malloc(sizeof(struct node));
    head->next->data = 22;
    head->next->next = malloc(sizeof(struct node));
    head->next->next->data = 33;
    head->next->next->next = NULL;

    // Traverse it
    for (struct node *cur = head; cur != NULL; cur = cur->next) {
        printf("%d\n", cur->data);
    }

    return 0;
}
```

Normally that linked list would have been set up with `malloc()`, but
just in case you haven't read that chapter yet, we kinda mocked it up
there another way.

## Flexible Array Members

Back in the good old days, when people carved C code out of wood, some
folks thought would be neat if they could allocate `struct`s that had
variable length arrays at the end of them.

I want to be clear that the first part of the section is the old way of
doing things, and we're going to do things the new way after that.

For example, maybe you could define a `struct` for holding strings and
the length of that string. It would have a length and an array to hold
the data. Maybe something like this:

``` {.c}
struct len_string {
    int length;
    char data[8];
};
```

But that has `8` hardcoded as the maximum length of a string, and that's
not much. What if we did something _clever_ and just `malloc()`d some
extra space at the end after the struct, and then let the data overflow
into that space?

Let's do that, and then allocate another 40 bytes on top of it:

``` {.c}
struct len_string *s = malloc(sizeof *s + 40);
```

Because `data` is the last field of the `struct`, if we overflow that
field, it runs out into space that we already allocated! For this
reason, this trick only works if the short array is the _last_ field in
the `struct`.

``` {.c}
// Copy more than 8 bytes!

strcpy(s->data, "Hello, world!");  // Won't crash. Probably.
```

In fact, there was a common compiler workaround for doing this, where
you'd allocate a zero length array at the end:

``` {.c}
struct len_string {
    int length;
    char data[0];
};
```

And then every extra byte you allocated was ready for use in that
string.

Because `data` is the last field of the `struct`, if we overflow that
field, it runs out into space that we already allocated!

``` {.c}
// Copy more than 8 bytes!

strcpy(s->data, "Hello, world!");  // Won't crash. Probably.
```

But, of course, actually accessing the data beyond the end of that array
is undefined behavior! In these modern times, we no longer deign to
resort to such savagery.

Luckily for us, we can still get the same effect with C99, but now it's
legal.

Let's just change our above definition to have no size for the
array^[Technically we say that it has an _incomplete type_.]:

``` {.c}
struct len_string {
    int length;
    char data[];
};
```

Again, this only works if the flexible array member is the _last_ field
in the `struct`.

And then we can allocate all the space we want for those strings by
`malloc()`ing larger than the `struct len_string`, as we do in this
example that makes a new `struct len_string` from a C string:


``` {.c}
struct len_string *len_string_from_c_string(char *s)
{
    int len = strlen(s);

    // Allocate "len" more bytes than we'd normally need
    struct len_string *ls = malloc(sizeof *ls + len);

    ls->length = len;

    // Copy the string into those extra bytes
    memcpy(ls->data, s, len);

    return ls;
}
```

## Padding Bytes

Beware that C is allowed to add padding bytes within or after a `struct`
as it sees fit. You can't trust that they will be directly adjacent in
memory^[Though some compilers have options to force this to
occur---search for `__attribute__((packed))` to see how to do this with
GCC.].

Let's take a look at this program. We output two numbers. One is the sum
of the `sizeof`s the individual field types. The other is the `sizeof`
the entire `struct`.

One would expect them to be the same. The size of the total is the size
of the sum of its parts, right?

``` {.c .numberLines}
#include <stdio.h>

struct foo {
    int a;
    char b;
    int c;
    char d;
};

int main(void)
{
    printf("%zu\n", sizeof(int) + sizeof(char) + sizeof(int) + sizeof(char));
    printf("%zu\n", sizeof(struct foo));

    return 0;
}
```

But on my system, this outputs:

```
10
16
```

They're not the same! The compiler has added 6 bytes of padding to help
it be more performant. Maybe you got different output with your
compiler, but unless you're forcing it, you can't be sure there's no
padding.

## `offsetof`

In the previous section, we saw that the compiler could inject padding
bytes at will inside a structure.

What if we needed to know where those were? We can measure it with
`offsetof`, defined in `<stddef.h>`.

Let's modify the code from above to print the offsets of the individual
fields in the `struct`:

``` {.c .numberLines}
#include <stdio.h>
#include <stddef.h>

struct foo {
    int a;
    char b;
    int c;
    char d;
};

int main(void)
{
    printf("%zu\n", offsetof(struct foo, a));
    printf("%zu\n", offsetof(struct foo, b));
    printf("%zu\n", offsetof(struct foo, c));
    printf("%zu\n", offsetof(struct foo, d));

    return 0;
}
```

For me, this outputs:

```
0
4
8
12
```

indicating that we're using 4 bytes for each of the fields. It's a
little weird, because `char` is only 1 byte, right? C is putting 3
padding bytes after each `char` so that all the fields are 4 bytes long.
Presumably this will run faster on my CPU.

## Bit-Fields

These are a rarely 

<!--
packed in
:0
unnamed
int signed or unsigned
-->

## Unions
