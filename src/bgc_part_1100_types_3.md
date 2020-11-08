<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Types III: Conversions

In this chapter, we want to talk all about converting from one type to
another. C has a variety of ways of doing this, and some might be a
little different that you're used to in other languages.

## The Usual Arithmetic Conversions

These are automatic conversions that C does around numeric operations
that you ask for. (That's actually what they're called, by the way, by
C99 §6.3.1.8.) Note that for this section, we're just talking about
numeric types---strings will come later.

These conversions answer questions about what happens when you mix
types, like this:

``` {.c}
int x = 3 + 1.2;   // Mixing int and double
float y = 12 * 2;  // Mixing float and int
```

Do they become `int`s? Do they become `float`s?




