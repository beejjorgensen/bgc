<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Types IV: Qualifiers and Specifiers

Now that we have some more types under our belts, turns out we can give
these types some additional attributes that control their behavior.
These are the _type qualifiers_ and _storage class specifiers_.

## Type Qualifiers

These are going to allow you to declare constant values, and also to
give the compiler optimization hints that it can use.

### `const`

This is the most common type qualifier you'll see. It means the variable
is constant, and any attempt to modify it will result in a very angry
compiler.

``` {.c}

const int x = 2;

x = 4;  // COMPILER PUKING SOUNDS
```

You can't change a `const` value.

Often you see `const` in parameter lists for functions:

``` {.c}
void foo(const int x)
{
    printf("%d\n", x + 30);  // OK, doesn't modify "x"
}
```

#### `const` and Pointers

This one gets a little funky, because there are two usages that have two
meanings when it comes to pointers.

``` {c.}
const int *p;   // We can't modify "p" with pointer arithmetic

p++;  // Compiler error!
```

But we can modify what they point to:

``` {.c}
int x = 10;
const int *p = &x;

*p = 20;   // Set "x" to 20, no problem
```

Great, so we can't change the pointer, but we can change what it points
to. What if we want the other way around? We want to be able to change
the pointer, but _not_ what it points to?

``` {.c}
int x[] = {10, 20};
int *const p = x;   // Move the const close to the variable name

p++;  // No problem

*p = 30; // Compiler error! Can't change what it points to
```

### `restrict`

### `volatile`

### `_Atomic`
