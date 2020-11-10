<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `typedef`: Making New Types

Well, not so much making _new_ types as getting new names for existing
types. Sounds kinda pointless on the surface, but we can really use this
to make our code cleaner.

## `typedef` in Theory

Basically, you take an existing type and you make an alias for it with
`typedef`.

Like this:

``` {.c}
typedef int antelope;  // Make "antelope" an alias for "int"

antelope x = 10;       // Type "antelope" is the same as type "int"
```

You can take any existing type and do it. You can even make a number of
types with a comma list:

``` {.c}
typedef int antelope, bagel, mushroom;  // These are all "int"
```

That's really useful, right? That you can type `mushroom` instead of
`int`? You must be _super excited_ about this feature!

OK, Professor Sarcasm---we'll get to some more common applications of
this in a moment.

### Scoping

`typedef` follows regular [scoping rules](#scope).

For this reason, it's quite common to find `typedef` at file scope
("global") so that all functions can use the new types at will.

## `typedef` in Practice

So renaming `int` to something else isn't that exciting. Let's see where
`typedef` commonly makes an appearance.

### `typedef` and `struct`s

Sometimes a `struct` will be `typedef`'d to a new name so you don't have
to type the word `struct` over and over.

``` {.c}
struct animal {
    char *name;
    int leg_count, speed;
};

//  original name      new name
//            |         |
//            v         v
//      |-----------| |----|
typedef struct animal animal;

struct animal y;  // This works
animal z;         // This also works because "animal" is an alias
```

Personally, I don't care for this practice. I like the clarity the code
has when you add the word `struct` to the type; programmers know what
they're getting. But it's really common so I'm including it here.

Now I want to run the exact same example in a way that you might
commonly see. We're going to put the `struct animal` _in_ the `typedef`.
You can mash it all together like this:

``` {.c}
//  original name
//            |
//            v
//      |-----------|
typedef struct animal {
    char *name;
    int leg_count, speed;
} animal;                         // <-- new name

struct animal y;  // This works
animal z;         // This also works because "animal" is an alias
```

That's exactly the same as the previous example, just more concise.

But that's not all! There's another common shortcut that you might see
in code using what are called _anonymous structures_^[We'll talk more
about these later.]. It turns out you don't actually need to name the
structure in a variety of places, and with `typedef` is one of them.

Let's do the same example with an anonymous structure:

``` {.c}
//  anonymous struct!
//         |
//         v
//      |----|
typedef struct {
    char *name;
    int leg_count, speed;
} animal;                         // <-- new name

//struct animal y;  // ERROR: this no longer works
animal z;           // This works because "animal" is an alias
```

As another example, we might find something like this:

``` {.c}
typedef struct {
    int x, y;
} point;

point p = {.x=20, .y=40};
```


### `typedef` and Pointers

