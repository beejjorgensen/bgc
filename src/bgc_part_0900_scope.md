<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Scope {#scope}

Scope is all about what variables are visible in what contexts.

## Block Scope

This is the scope of almost all the variables devs define. It includes
what other languages might call "function scope", i.e. variables that
are declared inside functions.

The basic rule is that if you've declared a variable in a block
delimited by squirrelly braces, the scope of that variable is that
block.

If there's a block inside a block, then variables declared in the
_inner_ block are local to that block, and cannot be seen in the outer
scope.

Once a variable's scope ends, that variable can no longer be referenced,
and you can consider its value to be gone into the great [flw[bit
bucket|Bit_bucket]] in the sky.

An example with nested scope:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int a = 12;         // Local to outer block, but visible in inner block

    if  (a == 12) {
        int b = 99;     // Local to inner block, not visible in outer block

        printf("%d %d\n", a, b);  // OK: "12 99"
    }

    printf("%d\n", a);  // OK, we're still in a's scope

    printf("%d\n", b);  // ILLEGAL, out of b's scope
}
```

### Where To Define Variables

Another fun fact is that you can define variables anywhere in the
block, within reason---they have the scope of that block, but cannot be
used before they are defined.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int i = 0;

    printf("%d\n", i);     // OK: "0"

    //printf("%d\n", j);   // ILLEGAL--can't use j before it's defined

    int j = 5;

    printf("%d %d\n", i, j);   // OK: "0 5"
}
```

Historically, C required all the variables be defined before any code in
the block, but this is no longer the case in the C99 standard.

### Variable Hiding

If you have a variable named the same thing at an inner scope as one at
an outer scope, the one at the inner scope takes precedence at long as
you're running in the inner scope. That is, it _hides_ the one at outer
scope for the duration of its lifetime.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int i = 10;

    {
        int i = 20;

        printf("%d\n", i);  // Inner scope i, 20 (outer i is hidden)
    }

    printf("%d\n", i);  // Outer scope i, 10
}
```

You might have noticed in that example that I just threw a block in
there at line 7, not so much as a `for` or `if` statement to kick it
off! This is perfectly legal. Sometimes a dev will want to group a bunch
of local variables together for a quick computation and will do this,
but it's rare to see.

## File Scope

If you define a variable outside of a block, that variable has _file
scope_. It's visible in all functions in the file that come after it,
and shared between them. (An exception is if a block defines a variable
of the same name, it would hide the one at file scope.)

This is closest to what you would consider to be "global" scope in
another language.

For example:

``` {.c .numberLines}
#include <stdio.h>

int shared = 10;    // File scope! Visible to the whole file after this!

void func1(void)
{
    shared += 100;  // Now shared holds 110
}

void func2(void)
{
    printf("%d\n", shared);  // Prints "110"
}

int main(void)
{
    func1();
    func2();
}
```

Note that if `shared` were declared at the bottom of the file, it
wouldn't compile. It has to be declared _before_ any functions use it.

There are ways to further modify items at file scope, namely with
(static)[#static] and (extern)[#extern], but we'll talk more about those
later.

## `for`-loop Scope

I really don't know what to call this, as C11 §6.8.5.3¶1 doesn't give it
a proper name. We've done it already a few times in this guide, as well.
It's when you declare a variable inside the first clause of a
`for`-loop:

``` {.c}
for (int i = 0; i < 10; i++)
    printf("%d\n", i);

printf("%d\n", i);  // ILLEGAL--i is only in scope for the for-loop
```

In that example, `i`'s lifetime begins the moment it is defined, and
continues for the duration of the loop.

If the loop body is enclosed in a block, the variables defined in the
`for`-loop are visible from that inner scope. 

Unless, of course, that inner scope hides them. This crazy example
prints `999` five times:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    for (int i = 0; i < 5; i++) {
        int i = 999;  // Hides the i in the for-loop scope
        printf("%d\n", i);
    }
}
```

## A Note on Function Scope

The C spec does refer to _function scope_, but it's used exclusively
with _labels_, something we haven't discussed yet. More on that another
day.

