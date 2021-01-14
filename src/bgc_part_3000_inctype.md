<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Incomplete Types

It might surprise you to learn that this builds without error:

``` {.c .numberLines}
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

An incomplete type is a type the size for which is not known.
