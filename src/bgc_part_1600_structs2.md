<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `struct`s II: More Fun with `struct`s

Turns out there's a lot more you can do with `struct`s than we've talked
about, but it's just a big pile of miscellaneous things. So we'll throw
them in this chapter.

If you're good with `struct` basics, you can round out your knowledge
here.

## Initializers of Nested `struct`s and Arrays

Remember how you could [initialize structure members along these
lines](#struct-initializers)?

``` {.c}
struct foo x = {.a=12, .b=3.14};
```

Turns out we have more power in these initializers than we'd originally
shared. Exciting!

For one thing, if you have a nested substructure like the following, you
can initialize members of that substructure by following the variable
names down the line:

``` {.c}
struct foo x = {.a.b.c=12};
```

Let's look at an example:

``` {.c .numberLines}
#include <stdio.h>

struct cabin_information {
    int window_count;
    int o2level;
};

struct spaceship {
    char *manufacturer;
    struct cabin_information ci;
};

int main(void)
{
    struct spaceship s = {
        .manufacturer="General Products",
        .ci.window_count = 8,   // <-- NESTED INITIALIZER!
        .ci.o2level = 21
    };

    printf("%s: %d seats, %d%% oxygen\n",
        s.manufacturer, s.ci.window_count, s.ci.o2level);
}
```

Check out lines 16-17! That's where we're initializing members of the
`struct cabin_information` in the definition of `s`, our `struct
spaceship`.

And here is another option for that same initializer---this time we'll
do something more standard-looking, but either approach works:

``` {.c .numberLines startFrom="15"}
    struct spaceship s = {
        .manufacturer="General Products",
        .ci={
            .window_count = 8,
            .o2level = 21
        }
    };
```

Now, as if the above information isn't spectacular enough, we can also
mix in array initializers in there, too.

Let's change this up to get an array of passenger information in there,
and we can check out how the initializers work in there, too.

``` {.c .numberLines}
#include <stdio.h>

struct passenger {
    char *name;
    int covid_vaccinated; // Boolean
};

#define MAX_PASSENGERS 8

struct spaceship {
    char *manufacturer;
    struct passenger passenger[MAX_PASSENGERS];
};

int main(void)
{
    struct spaceship s = {
        .manufacturer="General Products",
        .passenger = {
            // Initialize a field at a time
            [0].name = "Gridley, Lewis",
            [0].covid_vaccinated = 0,

            // Or all at once
            [7] = {.name="Brown, Teela", .covid_vaccinated=1},
        }
    };

    printf("Passengers for %s ship:\n", s.manufacturer);

    for (int i = 0; i < MAX_PASSENGERS; i++)
        if (s.passenger[i].name != NULL)
            printf("    %s (%svaccinated)\n",
                s.passenger[i].name,
                s.passenger[i].covid_vaccinated? "": "not ");
}
```

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

It's important to note that `next` is a pointer. This is what allows the
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
}
```

Running that prints:

```
11
22
33
```

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

Luckily for us, we can still get the same effect with C99 and later, but
now it's legal.

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
little weird, because `char` is only 1 byte, right? The compiler is
putting 3 padding bytes after each `char` so that all the fields are 4
bytes long. Presumably this will run faster on my CPU.

## Fake OOP

There's a slightly abusive thing that's sort of OOP-like that you can do
with `struct`s.

Since the pointer to the `struct` is the same as a pointer to the first
element of the `struct`, you can freely cast a pointer to the `struct`
to a pointer to the first element.

What this means is that we can set up a situation like this:

``` {.c}
struct parent {
    int a, b;
};

struct child {
    struct parent super;  // MUST be first
    int c, d;
};
```

Then we are able to pass a pointer to a `struct child` to a function
that expects either that _or_ a pointer to a `struct parent`!

Because `struct parent super` is the first item in the `struct child`, a
pointer to any `struct child` is the same as a pointer to that `super`
field^[`super` isn't a keyword, incidentally. I'm just stealing some OOP
terminology.].

Let's set up an example here. We'll make `struct`s as above, but then
we'll pass a pointer to a `struct child` into a function that needs a
pointer to a `struct parent`... and it'll still work.

``` {.c .numberLines}
#include <stdio.h>

struct parent {
    int a, b;
};

struct child {
    struct parent super;  // MUST be first
    int c, d;
};

// Making the argument `void*` so we can pass any type into it
// (namely a struct parent or struct child)
void print_parent(void *p)
{
    // Expects a struct parent--but a struct child will also work
    // because the pointer points to the struct parent in the first
    // field:
    struct parent *self = p;

    printf("Parent: %d, %d\n", self->a, self->b);
}

void print_child(struct child *self)
{
    printf("Child: %d, %d\n", self->c, self->d);
}

int main(void)
{
    struct child c = {.super.a=1, .super.b=2, .c=3, .d=4};

    print_child(&c);
    print_parent(&c);  // Also works even though it's a struct child!
}
```

See what we did on the last line of `main()`? We called `print_parent()`
but passed a `struct child*` as the argument! Even though
`print_parent()` needs the argument to point to a `struct parent`, we're
_getting away with it_ because the first field in the `struct child` is
a `struct parent`.

Again, this works because a pointer to a `struct` has the same value as
a pointer to the first field in that `struct`.


## Bit-Fields

In my experience, these are rarely used, but you might see them out
there from time to time, especially in lower-level applications that
pack bits together into larger spaces.

Let's take a look at some code to demonstrate a use case:

``` {.c .numberLines}
#include <stdio.h>

struct foo {
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
};

int main(void)
{
    printf("%zu\n", sizeof(struct foo));
}
```

For me, this prints `16`. Which makes sense, since `unsigned`s are 4
bytes on my system.

But what if we knew that all the values that were going to be stored in
`a` and `b` could be stored in 5 bits, and the values in `c`, and `d`
could be stored in 3 bits?  That's only a total 16 bits. Why have 128
bits reserved for them if we're only going to use 16?

Well, we can tell C to pretty-please try to pack these values in. We can
specify the maximum number of bits that values can take (from 1 up the
size of the containing type).

We do this by putting a colon after the field name, followed by the
field width in bits.

``` {.c .numberLines startFrom="3"}
struct foo {
    unsigned int a:5;
    unsigned int b:5;
    unsigned int c:3;
    unsigned int d:3;
};
```

Now when I ask C how big my `struct foo` is, it tells me 4! It was 16
bytes, but now it's only 4. It has "packed" those 4 values down into 4
bytes, which is a four-fold memory savings.

The tradeoff is, of course, that the 5-bit fields can only hold values
from 0-31 and the 3-bit fields can only hold values from 0-7. But life's
all about compromise, after all.

### Non-Adjacent Bit-Fields

A gotcha: C will only combine **adjacent** bit-fields. If they're
interrupted by non-bit-fields, you get no savings:

``` {.c}
struct foo {            // sizeof(struct foo) == 16 (for me)
    unsigned int a:1;   // since a is not adjacent to c.
    unsigned int b;
    unsigned int c:1;
    unsigned int d;
};
```

In that example, since `a` is not adjacent to `c`, they are both
"packed" in their own `int`s.

So we have one `int` each for `a`, `b`, `c`, and `d`. Since my `int`s
are 4 bytes, that's a grand total of 16 bytes.

A quick rearrangement yields some space savings from 16 bytes down to 12
bytes (on my system):

``` {.c}
struct foo {            // sizeof(struct foo) == 12 (for me)
    unsigned int a:1;
    unsigned int c:1;
    unsigned int b;
    unsigned int d;
};
```

And now, since `a` is next to `c`, the compiler puts them together into
a single `int`.

So we have one `int` for a combined `a` and `c`, and one `int` each for
`b` and `d`. For a grand total of 3 `int`s, or 12 bytes.

Put all your bitfields together to get the compiler to combine them.

### Signed or Unsigned `int`s

If you just declare a bit-field to be `int`, the different compilers
will treat it as `signed` or `unsigned`. Just like the situation with
`char`.

Be specific about the signedness when using bit-fields.

### Unnamed Bit-Fields

In some specific circumstances, you might need to reserve some bits for
hardware reasons, but not need to use them in code.

For example, let's say you have a byte where the top 2 bits have a
meaning, the bottom 1 bit has a meaning, but the middle 5 bits do not
get used by you^[Assuming 8-bit `char`s, i.e. `CHAR_BIT == 8`.].

We _could_ do something like this:

``` {.c}
struct foo {
    unsigned char a:2;
    unsigned char dummy:5;
    unsigned char b:1;
};
```

And that works---in our code we use `a` and `b`, but never `dummy`. It's
just there to eat up 5 bits to make sure `a` and `b` are in the
"required" (by this contrived example) positions within the byte.

C allows us a way to clean this up: _unnamed bit-fields_. You can just
leave the name (`dummy`) out in this case, and C is perfectly happy for
the same effect:

``` {.c}
struct foo {
    unsigned char a:2;
    unsigned char :5;   // <-- unnamed bit-field!
    unsigned char b:1;
};
```

### Zero-Width Unnamed Bit-Fields

Some more esoterica out here... Let's say you were packing bits into an
`unsigned int`, and you needed some adjacent bit-fields to pack into the
_next_ `unsigned int`.

That is, if you do this:

``` {.c}
struct foo {
    unsigned int a:1;
    unsigned int b:2;
    unsigned int c:3;
    unsigned int d:4;
};
```

the compiler packs all those into a single `unsigned int`. But what if
you needed `a` and `b` in one `int`, and `c` and `d` in a different one?

There's a solution for that: put an unnamed bit-field of width `0` where
you want the compiler to start anew with packing bits in a different
`int`:

``` {.c}
struct foo {
    unsigned int a:1;
    unsigned int b:2;
    unsigned int :0;   // <--Zero-width unnamed bit-field
    unsigned int c:3;
    unsigned int d:4;
};
```

It's analogous to an explicit page break in a word processor. You're
telling the compiler, "Stop packing bits in this `unsigned`, and start
packing them in the next one."

By adding the zero-width unnamed bit field in that spot, the compiler
puts `a` and `b` in one `unsigned int`, and `c` and `d` in another
`unsigned int`. Two total, for a size of 8 bytes on my system (`unsigned
int`s are 4 bytes each).

## Unions

These are basically just like `struct`s, except the fields overlap in
memory. The `union` will be only large enough for the largest field, and
you can only use one field at a time.

It's a way to reuse the same memory space for different types of data.

You declare them just like `struct`s, except it's `union`. Take a look
at this:

``` {.c}
union foo {
    int a, b, c, d, e, f;
    float g, h;
    char i, j, k, l;
};
```

Now, that's a lot of fields. If this were a `struct`, my system would
tell me it took 36 bytes to hold it all.

But it's a `union`, so all those fields overlap in the same stretch of
memory. The biggest one is `int` (or `float`), taking up 4 bytes on my
system. And, indeed, if I ask for the `sizeof` the `union foo`, it tells
me 4!

The tradeoff is that you can only portably use one of those fields at a
time. If you try to read from a field that was not the last one written
to, the behavior is unspecified.

Let's take that crazy `union` and first store an `int` in it, then a
`float`. Then we'll print out the `int` again to see what's in
there---even though, since it wasn't the last value we stored, the
result is unspecified.

``` {.c .numberLines}
#include <stdio.h>

union foo {
    int a, b, c, d, e, f;
    float g, h;
    char i, j, k, l;
};

int main(void)
{
    union foo x;

    x.a = 12;
    printf("%d\n", x.a);  // OK--x.a was the last thing we stored into

    x.g = 3.141592;
    printf("%f\n", x.g);  // OK--x.g was the last thing we stored into

    printf("%d\n", x.a);  // Unspecified behavior!
}
```

On my machine, this prints:

```
12
3.141592
1078530008
```

Probably deep down the decimal value `1078530008` is probably the same
pattern of bits as `3.141592`, but the spec makes no guarantees about
this.

### Pointers to `union`s

If you have a pointer to a `union`, you can cast that pointer to any of
the types of the fields in that `union` and get the values out that way.

In this example, we see that the `union` has `int`s and `float`s in it.
And we get pointers to the `union`, but we cast them to `int*` and
`float*` types (the cast silences compiler warnings). And then if we
dereference those, we see that they have the values we stored directly
in the `union`.

``` {.c .numberLines}
#include <stdio.h>

union foo {
    int a, b, c, d, e, f;
    float g, h;
    char i, j, k, l;
};

int main(void)
{
    union foo x;

    int *foo_int_p = (int *)&x;
    float *foo_float_p = (float *)&x;

    x.a = 12;
    printf("%d\n", x.a);           // 12
    printf("%d\n", *foo_int_p);    // 12, again

    x.g = 3.141592;
    printf("%f\n", x.g);           // 3.141592
    printf("%f\n", *foo_float_p);  // 3.141592, again
}
```

The reverse is also true. If we have a pointer to a type inside the
`union`, we can cast that to a pointer to the `union` and access its
members.

``` {.c}
union foo x;
int *foo_int_p = (int *)&x;             // Pointer to int field
union foo *p = (union foo *)foo_int_p;  // Back to pointer to union

p->a = 12;  // This line the same as...
x.a = 12;   // this one.
```

All this just lets you know that, under the hood, all these values in a
`union` start at the same place in memory, and that's the same as where
the entire `union` is.

