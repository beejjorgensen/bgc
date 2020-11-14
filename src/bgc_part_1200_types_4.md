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

x = 4;  // COMPILER PUKING SOUNDS, can't assign to a constant
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

Somewhat confusingly, these two things are equivalent:

``` {.c}
const int *p;  // Can't modify p
int const *p;  // Can't modify p, just like the previous line
```

but different than:

``` {.c}
int *const p;  // Can't modify *p, the thing p points to
```

You can also do both!

``` {.c}
const int *const p;  // Can't modify p or *p!
```

#### `const` Correctness

One more thing I have to mention is that the compiler will warn on
something like this:

``` {.c}
const int x = 20;
int *p = &x;
```

saying something to the effect of:

```
initialization discards 'const' qualifier from pointer type target
```

What's happening there?

Well, we need to look at the types on either side of the assignment:

``` {.c}
    const int x = 20;
    int *p = &x;
//    ^       ^
//    |       |
//  int*    const int*
```

The compiler is warning us that the value on the right side of the
assignment is `const`, but the one of the left is not. And the compiler
is letting us know that it is discarding the "const-ness" of the
expression on the right.

That is, we _can_ still try to do the following, but it's just wrong.
The compiler will warn, and it's undefined behavior:

``` {.c}
const int x = 20;
int *p = &x;

*p = 40;  // Undefined behavior--maybe it modifies "x", maybe not!

printf("%d\n", x);  // 40, if you're lucky
```

### `restrict`

TLDR: you never have to use this and you can ignore it every time you
see it.

`restrict` is a hint to the compiler that a particular piece of memory
will only be accessed by one pointer and never another. If a developer
declares a pointer to be `restrict` and then accesses the object it
points to in another way, the behavior is undefined.

Basically you're telling C, "Hey---I guarantee that this one single
pointer is the only way I access this memory, and if I'm lying, you can
pull undefined behavior on me."

And C uses that information to perform certain optimizations.

For example, let's write a function to swap two variables, and we'll use
the `restrict` keyword to assure C that we'll never pass in pointers to
the same thing. And then let's blow it an try passing in pointers to the
same thing.

``` {.c .numberLines}
void swap(int *restrict a, int *restrict b)
{
    int t;

    t = *a;
    *a = *b;
    *b = t;
}

int main(void)
{
    int x = 10, y = 20;

    swap(&x, &y);  // OK! "a" and "b", above, point to different things

    swap(&x, &x);  // Undefined behavior! "a" and "b" point to the same thing
}
```

If we were to take out the `restrict` keywords, above, that would allow
both calls to work safely. But then the compiler might not be able to
optimize.

`restrict` has block scope, that is, the restriction only lasts for the
scope its used. If it's in a parameter list for a function, it's in the
block scope of that function.

If the restricted pointer points to an array, the restriction covers the
entire array.

If it's outside any function in file scope, the restriction covers the
entire program.

You're likely to see this in library functions like `printf()`:

``` {.c}
int printf(const char * restrict format, ...);
```

Again, that's just telling the compiler that inside the `printf()`
function, there will be only one pointer that refers to any part of that
`format` string.

### `volatile`

You're unlikely to see or need this unless you're dealing with hardware
directly.

`volatile` tells the compiler that a value might change behind its back
and should be looked up every time.

An example might be where the compiler is looking in memory at an
address that continuously updates behind the scenes, e.g. some kind of
hardware timer.

If the compiler decides to optimize that and store the value in a
register for a protracted time, the value in memory will update and
won't be reflected in the register.

By declaring something `volatile`, you're telling the compiler, "Hey,
the thing this points at might change at any time for reasons outside
this program code.

``` {.c}
volatile int *p;
```

### `_Atomic`

This is an optional C feature that we'll talk about another time.

## Type Specifiers

Type specifiers are similar to type quantifiers. They give the compiler
more information about the type of a variable.

### `auto`

You barely ever see this keyword, since `auto` is the default for block
scope variables. It's implied.

These are the same:

``` {.c}
{
    int a;         // auto is the default...
    auto int a;    // So this is redundant
}
```

The `auto` keyword indicates that this object has _automatic storage
duration_. That is, it exists in the scope in which it is defined, and
is automatically deallocated when the scope is exited.

One gotcha about automatic variables is that their value is
indeterminate until you explicitly initialize them. We say they're full
of "random" or "garbage" data, though neither of those really makes me
happy. In any case, you won't know what's in it unless you initialize
it.

Always initialize all automatic variables before use!

### `static`

This keyword has two meanings, depending on if the variable is file
scope or block scope.

Let's start with block scope.

#### `static` in Block Scope

In this case, we're basically saying, "I just want a single instance of
this variable to exist, shared between calls."

That is, its value will persist between calls.

`static` in block scope with an initializer will only be initialized one
time on program startup, not each time the function is called.

Let's do an example:

``` {.c. numberLines}
#include <stdio.h>

void counter(void)
{
    static int count = 1;  // This is initialized one time

    printf("This has been called %d time(s)\n", count);

    count++;
}

int main(void)
{
    counter();  // "This has been called 1 time(s)"
    counter();  // "This has been called 2 time(s)"
    counter();  // "This has been called 3 time(s)"
    counter();  // "This has been called 4 time(s)"

    return 0;
}
```

See how the value of `count` persists between calls?

One thing of note is that `static` block scope variables are initialized
to `0` by default.

``` {.c}
static int foo;      // Default starting value is `0`...
static int foo = 0;  // So the `0` assignment is redundant
```

Finally, be advised that if you're writing multithreaded programs, you
have to be sure you don't let multiple threads trample the same variable.

#### `static` in File Scope

When you get out to file scope, outside any blocks, the meaning rather
changes.

Variables at file scope already persist between function calls, so that
behavior is already there.

Instead what `static` means in this context is that this variable isn't
visible outside of this particular source file. Kinda like "global", but
only in this file.

More on that in the section about building with multiple source files.

### `extern`



### `register`