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
  WOOL,   // Value is 1
  BRICK,  // Value is 2
  WHEAT   // Value is 3
};

printf("%d %d\n", SHEEP, BRICK);  // 0 2
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

It's true: it'll work if you just store them in `int`s. But the compiler
can tell you if you try to store a literal value in an `enum` that's out
of range.

Let's do an example