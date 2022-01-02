<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Arrays {#arrays}

> _“Should array indices start at 0 or 1?  My compromise of 0.5 was
> rejected without, I thought, proper consideration.”_
>
> ---Stan Kelly-Bootle, computer scientist

Luckily, C has arrays. I mean, I know it's considered a low-level
language^[These days, anyway.] but it does at least have the concept of
arrays built-in. And since a great many languages drew inspiration from C's
syntax, you're probably already familiar with using `[` and `]` for
declaring and using arrays.

But C only _barely_ has arrays! As we'll find out later, arrays are just
syntactic sugar in C---they're actually all pointers and stuff deep
down. _Freak out!_ But for now, let's just use them as arrays. _Phew_.

## Easy Example

Let's just crank out an example:

``` {.c .numberLines}
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

## Getting the Length of an Array

You can't...ish. C doesn't record this information^[Since arrays are
just pointers to the first element of the array under the hood, there's
no additional information recording the length.]. You have to manage
it separately in another variable.

When I say "can't", I actually mean there are some circumstances when
you _can_. There is a trick to get the number of elements in an array in
the scope in which an array is declared. But, generally speaking, this
won't work the way you want if you pass the array to a
function^[Because when you pass an array to a function, you're actually
just passing a pointer to the first element of that array, not the
"entire" array.].

Let's take a look at this trick. The basic idea is that you take the
`sizeof` the array, and then divide that by the size of each element to
get the length. For example, if an `int` is 4 bytes, and the array is 32
bytes long, there must be room for $\frac{32}{4}$ or $8$
`int`s in there.

``` {.c}
int x[12];  // 12 ints

printf("%zu\n", sizeof x);     // 48 total bytes
printf("%zu\n", sizeof(int));  // 4 bytes per int

printf("%zu\n", sizeof x / sizeof(int));  // 48/4 = 12 ints!
```

If it's an array of `char`s, then `sizeof` the array _is_ the number of
elements, since `sizeof(char)` is defined to be 1. For anything else,
you have to divide by the size of each element.

But this trick only works in the scope in which the array was defined.
If you pass the array to a function, it doesn't work. Even if you make
it "big" in the function signature:

``` {.c}
void foo(int x[12])
{
    printf("%zu\n", sizeof x);     // 8?! What happened to 48?
    printf("%zu\n", sizeof(int));  // 4 bytes per int

    printf("%zu\n", sizeof x / sizeof(int));  // 8/4 = 2 ints?? WRONG.
}
```

This is because when you "pass" arrays to functions, you're only passing
a pointer to the first element, and that's what `sizeof` measures. More
on this in the [Passing Single Dimensional Arrays to
Functions](#passing1darrays) section, below.

One more thing you can do with `sizeof` and arrays is get the size of an
array of a fixed number of elements without declaring the array. This is
like how you can get the size of an `int` with `sizeof(int)`.

For example, to see how many bytes would be needed for an array of 48
`double`s, you can do this:

``` {.c}
sizeof(double [48]);
```

## Array Initializers

You can initialize an array with constants ahead of time:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int i;
    int a[5] = {22, 37, 3490, 18, 95};  // Initialize with these values

    for (i = 0; i < 5; i++) {
        printf("%d\n", a[i]);
    }
}
```

Catch: initializer values must be constant terms. Can't throw variables
in there. Sorry, Illinois!

You should never have more items in your initializer than there is room
for in the array, or the compiler will get cranky:

``` {.zsh}
foo.c: In function ‘main’:
foo.c:6:39: warning: excess elements in array initializer
    6 |     int a[5] = {22, 37, 3490, 18, 95, 999};
      |                                       ^~~
foo.c:6:39: note: (near initialization for ‘a’)
```

But (fun fact!) you can have _fewer_ items in your initializer than
there is room for in the array. The remaining elements in the array will
be automatically initialized with zero. This is true in general for all
types of array initializers: if you have an initializer, anything not
explicitly set to a value will be set to zero.

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

Which means, "Make the first element zero, and then automatically make
the rest zero, as well."

You can set specific array elements in the initializer, as well, by
specifying an index for the value! When you do this, C will happily keep
initializing subsequent values for you until the initializer runs out,
filling everything else with `0`.

To do this, put the index in square brackets with an `=` after, and then
set the value.

Here's an example where we build an array:

``` {.c}
int a[10] = {0, 11, 22, [5]=55, 66, 77};
```

Because we listed index 5 as the start for `55`, the resulting data in
the array is:

``` {.default}
0 11 22 0 0 55 66 77 0 0
```

You can put simple constant expressions in there, as well.

``` {.c}
#define COUNT 5

int a[COUNT] = {[COUNT-3]=3, 2, 1};
```

which gives us:

``` {.default}
0 0 3 2 1
```

Lastly, you can also have C compute the size of the array from the
initializer, just by leaving the size off:

``` {.c}
int a[3] = {22, 37, 3490};

// is the same as:

int a[] = {22, 37, 3490};  // Left the size off!
```

## Out of Bounds!

C doesn't stop you from accessing arrays out of bounds. It might not
even warn you.

Let's steal the example from above and keep printing off the end of the
array. It only has 5 elements, but let's try to print 10 and see what
happens:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int i;
    int a[5] = {22, 37, 3490, 18, 95};

    for (i = 0; i < 10; i++) {  // BAD NEWS: printing too many elements!
        printf("%d\n", a[i]);
    }
}
```

Running it on my computer prints:

``` {.default}
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
order|Row-_and_column-major_order]]. This means with a 2D array, the
first index listed indicates the row, and the second the column.

You an also use initializers on multidimensional arrays by nesting them:

``` {.c .numberLines}
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
}
```

For output of:

``` {.default}
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

And you can initialize with explicit indexes:

``` {.c}
// Make a 3x3 identity matrix

int a[3][3] = {[0][0]=1, [1][1]=1, [2][2]=1};
```

which builds a 2D array like this:

``` {.default}
1 0 0
0 1 0
0 0 1
```

## Arrays and Pointers

[_Casually_] So... I kinda might have mentioned up there that arrays
were pointers, deep down? We should take a shallow dive into that now so
that things aren't completely confusing. Later on, we'll look at what
the real relationship between arrays and pointers is, but for now I just
want to look at passing arrays to functions.

### Getting a Pointer to an Array

I want to tell you a secret. Generally speaking, when a C programmer
talks about a pointer to an array, they're talking about a pointer _to
the first element_ of the array^[This is technically incorrect, as a
pointer to an array and a pointer to the first element of an array have
different types. But we can burn that bridge when we get to it.].

So let's get a pointer to the first element of an array.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int a[5] = {11, 22, 33, 44, 55};
    int *p;

    p = &a[0];  // p points to the array
                // Well, to the first element, actually

    printf("%d\n", *p);  // Prints "11"
}
```

This is so common to do in C that the language allows us a shorthand:

``` {.c .numberLines}
p = &a[0];  // p points to the array

// is the same as:

p = a;      // p points to the array, but much nicer-looking!
```

Just referring to the array name in isolation is the same as getting a
pointer to the first element of the array! We're going to use this
extensively in the upcoming examples.

But hold on a second--isn't `p` an `int*`? And `*p` gives us `11`, same
as `a[0]`? Yessss. You're starting to get a glimpse of how arrays and
pointers are related in C.


### Passing Single Dimensional Arrays to Functions {#passing1darrays}

Let's do an example with a single dimensional array. I'm going to write
a couple functions that we can pass the array to that do different
things.

Prepare for some mind-blowing function signatures!

``` {.c .numberLines}
#include <stdio.h>

// Passing as a pointer to the first element
void times2(int *a, int len)
{
    for (int i = 0; i < len; i++)
        printf("%d\n", a[i] * 2);
}

// Same thing, but using array notation
void times3(int a[], int len)
{
    for (int i = 0; i < len; i++)
        printf("%d\n", a[i] * 3);
}

// Same thing, but using array notation with size
void times4(int a[5], int len)
{
    for (int i = 0; i < len; i++)
        printf("%d\n", a[i] * 4);
}

int main(void)
{
    int x[5] = {11, 22, 33, 44, 55};

    times2(x, 5);
    times3(x, 5);
    times4(x, 5);
}
```

All those methods of listing the array as a parameter in the function
are identical.

``` {.c}
void times2(int *a, int len)
void times3(int a[], int len)
void times4(int a[5], int len)
```

In usage by C regulars, the first is the most common, by far.

And, in fact, in the latter situation, the compiler doesn't even care
what number you pass in (other than it has to be greater than zero^[C11
§6.7.6.2¶1 requires it be greater than zero. But you might see code out
there with arrays declared of zero length at the end of `struct`s and
GCC is particularly lenient about it unless you compile with
`-pedantic`. This zero-length array was a hackish mechanism for making
variable-length structures. Unfortunately, it's technically undefined
behavior to access such an array even though it basically worked
everywhere. C99 codified a well-defined replacement for it called
_flexible array members_, which we'll chat about later.]). It doesn't
enforce anything at all. 

Now that I've said that, the size of the array in the function
declaration actually _does_ matter when you're passing multidimensional
arrays into functions, but let's come back to that.

### Changing Arrays in Functions

We've said that arrays are just pointers in disguise. This means that if
you pass an array to a function, you're likely passing a pointer to the
first element in the array.

But if the function has a pointer to the data, it is able to manipulate
that data! So changes that a function makes to an array will be visible
back out in the caller.

Here's an example where we pass a pointer to an array to a function,
the function manipulates the values in that array, and those changes are
visible out in the caller.

``` {.c .numberLines}
#include <stdio.h>

void double_array(int *a, int len)
{
    // Multiple each element by 2
    //
    // This doubles the values in x in main() since x and a both point
    // to the same array in memory!

    for (int i = 0; i < len; i++)
        a[i] *= 2;
}

int main(void)
{
    int x[5] = {1, 2, 3, 4, 5};

    double_array(x, 5);

    for (int i = 0; i < 5; i++)
        printf("%d\n", x[i]);  // 2, 4, 6, 8, 10!
}
```

Even though we passed the array in as parameter `a` which is type
`int*`, look at how we access it using array notation with `a[i]`!
Whaaaat. This is totally allowed.

Later when we talk about the equivalence between arrays and pointers,
we'll see how this makes a lot more sense. For now, it's enough to know
that functions can make changes to arrays that are visible out in the
caller.


### Passing Multidimensional Arrays to Functions

The story changes a little when we're talking about multidimensional
arrays. C needs to know all the dimensions (except the first one) so it
has enough information to know where in memory to look to find a value.

Here's an example where we're explicit with all the dimensions:

``` {.c .numberLines}
#include <stdio.h>

void print_2D_array(int a[2][3])
{
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 3; col++)
            printf("%d ", a[row][col]);
        printf("\n");
    }
}

int main(void)
{
    int x[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    print_2D_array(x);
}
```

But in this case, these two^[This is also equivalent: `void
print_2D_array(int (*a)[3])`, but that's more than I want to get into
right now.] are equivalent:

``` {.c}
void print_2D_array(int a[2][3])
void print_2D_array(int a[][3])
```

The compiler really only needs the second dimension so it can figure out
how far in memory to skip for each increment of the first dimension. In
general, it needs to know all the dimensions except the first one.

Also, remember that the compiler does minimal compile-time bounds
checking (if you're lucky), and C does zero runtime checking of bounds.
No seat belts! Don't crash by accessing array elements out of bounds!
