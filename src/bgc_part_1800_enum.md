<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Enumerated Types: `enum`

C offers us another way to have constant integer values by name: `enum`.

For example:

``` {.c}
enum {
  ONE=1,
  TWO=2
};

printf("%d %d", ONE, TWO);  // 1 2
```

In some ways, it can be better---or different---than using a `#define`.
Key differences:

* `enum`s can only be integer types.
* `#define` can define anything at all.
* `enum`s are often shown by their symbolic identifier name in a debugger.
* `#define`d numbers just show as raw numbers which are harder to know
  the meaning of while debugging.

Since they're integer types, they can be used any place integers can be
used, including in array dimensions and `case` statements.

Let's tear into this more.

## Behavior of `enum`

### Numbering

`enum`s are automatically numbered unless you override them.

They start at `0`, and autoincrement up from there, by default:

``` {.c}
enum {
    SHEEP,  // Value is 0
    WHEAT,  // Value is 1
    WOOD,   // Value is 2
    BRICK,  // Value is 3
    ORE     // Value is 4
};

printf("%d %d\n", SHEEP, BRICK);  // 0 3
```

You can force particular integer values, as we saw earlier:

``` {.c}
enum {
  X=2,
  Y=18,
  Z=-2
};
```

Duplicates are not a problem:

``` {.c}
enum {
  X=2,
  Y=2,
  Z=2
};
```

if values are omitted, numbering continues counting in the positive
direction from whichever value was last specified. For example:

``` {.c}
enum {
  A,    // 0, default starting value
  B,    // 1
  C=4,  // 4, manually set
  D,    // 5
  E,    // 6
  F=3   // 3, manually set
  G,    // 4
  H     // 5
}
```

### Trailing Commas

This is perfectly fine, if that's your style:

``` {.c}
enum {
  X=2,
  Y=18,
  Z=-2,   // <-- Trailing comma
};
```

It's gotten more popular in languages of the recent decades so you might
be pleased to see it.

### Scope

`enum`s scope as you'd expect. If at file scope, the whole file can see
it. If in a block, it's local to that block.

It's really common for `enum`s to be defined in header files so they can
be `#include`d at file scope.

### Style

As you've noticed, it's common to declare the `enum` symbols in
uppercase (with underscores).

This isn't a requirement, but is a very, very common idiom.

## Your `enum` is a Type

This is an important thing to know about `enum`: they're a type,
analogous to how a `struct` is a type.

You can give them a tag name so you can refer to the type later and
declare variables of that type.

Now, since `enum`s are integer types, why not just use `int`?

In C, the best reason for this is code clarity--it's a nice, typed way
to describe your thinking in code. C (unlike C++) doesn't actually
enforce any values being in range for a particular `enum`.

Let's do an example where we declare a variable `r` of type `enum
resource` that can hold those values:

``` {.c}
// Named enum, type is "enum resource"

enum resource {
    SHEEP,
    WHEAT,
    WOOD,
    BRICK,
    ORE
};

// Declare a variable "r" of type "enum resource"

enum resource r = BRICK;

if (r == BRICK) {
    printf("I'll trade you a brick for two sheep.\n");
}
```

You can also `typedef` these, of course, though I personally don't like
to.


``` {.c}
typedef enum {
    SHEEP,
    WHEAT,
    WOOD,
    BRICK,
    ORE
} RESOURCE;

RESOURCE r = BRICK;
```

Another shortcut that's legal but rare is to declare variables when you
declare the `enum`:

``` {.c}
// Declare an enum and some initialized variables of that type:

enum {
    SHEEP,
    WHEAT,
    WOOD,
    BRICK,
    ORE
} r = BRICK, s = WOOD;
```

You can also give the `enum` a name so you can use it later, which is
probably what you want to do in most cases:


``` {.c}
// Declare an enum and some initialized variables of that type:

enum resource {   // <-- type is now "enum resource"
    SHEEP,
    WHEAT,
    WOOD,
    BRICK,
    ORE
} r = BRICK, s = WOOD;
```

In short, `enum`s are a great way to write nice, scoped, typed, clean
code.