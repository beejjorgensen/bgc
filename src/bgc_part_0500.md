<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Arrays {#arrays}

Luckily, C has arrays. I mean, I know it's considered a low-level
language^[These days, anyway.] but it does at least have the concept of
arrays built-in. And since a great many languages drew inspiration from C's
syntax, you're probably already familiar with using `[` and `]` for
declaring and using arrays in C.

But only barely! As we'll find out later, arrays are just syntactic
sugar in C---they're actually all pointers and stuff deep down. _Freak
out!_ But for now, let's just use them as arrays. _Phew_.

## Easy Example

Let's just crank out an example:

``` {.c}
#include <stdio.h>

int main(void)
{
    int i;
    float f[4];  // Declare an array of 4 floats

    f[0] = 3.14159;  // Indexing starts at 0, of course.
    f[1] = 1.41421;
    f[2] = 1.61803;
    f[3] = 2.71828;

    // Print them all out:

    for (i = 0; i < 4; i++) {
        printf("%f\n", f[i]);
    }

    return 0;
}
```

When you declare an array, you have to give it a size. And the size has
to be fixed^[Again, not really, but variable-length arrays---of which
I'm not really a fan---are a story for another time.].

In the above example, we made an array of 4 floats. The value in the
square brackets in the declaration lets us know that.

Later on in subsequent lines, we access the values in the array, setting
them or getting them, again with square brackets.

Hopefully this looks familiar from languages you already know!


## Array Initializers

You can initialize an array with constants ahead of time:

``` {.c}
#include <stdio.h>

int main(void)
{
    int i;
    int a[5] = {22, 37, 3490, 18, 95};  // Initialize with these values

    for (i = 0; i < 5; i++) {
        printf("%d\n", a[i]);
    }

    return 0;
}
```

Catch: initializer values must be constant terms. Can't throw variables
in there. Sorry, Illinois!

You should never have more items in your initializer than there is room
for in the array, or the compiler will get cranky:

```
foo.c: In function ‘main’:
foo.c:6:39: warning: excess elements in array initializer
    6 |     int a[5] = {22, 37, 3490, 18, 95, 999};
      |                                       ^~~
foo.c:6:39: note: (near initialization for ‘a’)
```

But (fun fact!) you can have _fewer_ items in your initializer than
there is room for in the array. The remaining elements in the array will
be automatically initialized with zero.

``` {.c}
int a[5] = {22, 37, 3490};

// is the same as:

int a[5] = {22, 37, 3490, 0, 0};
```

It's a common shortcut to see this in an initializer when you want to
set an entire array to zero:

``` {.c}
int a[100] = {0};
```

Which means, "Make the first element 0, and then automatically make the
rest 0, as well."


## Out of Bounds!

C doesn't stop you from accessing arrays out of bounds. It might not
even warn you.

Let's steal the example from above and keep printing off the end of the
array. It only has 5 elements, but let's try to print 10 and see what
happens:

``` {.c}
#include <stdio.h>

int main(void)
{
    int i;
    int a[5] = {22, 37, 3490, 18, 95};

    for (i = 0; i < 10; i++) {  // BAD NEWS: printing too many elements!
        printf("%d\n", a[i]);
    }

    return 0;
}
```

Running it on my computer prints:

```
22
37
3490
18
95
32765
1847052032
1780534144
-56487472
21890
```

Yikes! What's that? Well, turns out printing off the end of an array
results in what C developers call _undefined behavior_. We'll talk more
about this beast later, but for now it means, "You've done something
bad, and anything could happen during your program run."

And by anything, I mean typically things like finding zeroes, finding
garbage numbers, or crashing. But really the C spec says in this
circumstance the compiler is allowed to emit code that does
_anything_^[In the good old MS-DOS days before memory protection was a
thing, I was writing some particularly abusive C code that deliberately
engaged in all kinds of undefined behavior. But I knew what I was doing,
and things were working pretty well. Until I made a misstep that caused
a lockup and, as I found upon reboot, nuked all my BIOS settings. That
was fun. (Shout-out to @man for those fun times.)].

Short version: don't do anything that causes undefined behavior.
Ever^[There are a lot of things that cause undefined behavior, not just
out-of-bounds array accesses. This is what makes the C language so
_exciting_.].

## Multidimensional Arrays

You can add as many dimensions as you want to your arrays.

``` {.c}
int a[10];
int b[2][7];
int c[4][5][6];
```

These are stored in memory in [flw[row-major
order|Row-_and_column-major_order]].

You an also use initializers on multidimensional arrays by nesting them:

``` {.c}
#include <stdio.h>

int main(void)
{
    int row, col;

    int a[2][5] = {      // Initialize a 2D array
        {0, 1, 2, 3, 4},
        {5, 6, 7, 8, 9}
    };

    for (row = 0; row < 2; row++) {
        for (col = 0; col < 5; col++) {
            printf("(%d,%d) = %d\n", row, col, a[row][col]);
        }
    }

    return 0;
}
```

For output of:

```
(0,0) = 0
(0,1) = 1
(0,2) = 2
(0,3) = 3
(0,4) = 4
(1,0) = 5
(1,1) = 6
(1,2) = 7
(1,3) = 8
(1,4) = 9
```

## Arrays and Functions

[_Casually_] So... I kinda might have mentioned up there that arrays
were pointers, deep down? We should take a shallow dive into that now.