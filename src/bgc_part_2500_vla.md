<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Variable-Length Arrays (VLAs)

C provides a way for you to declare an array whose size is determined at
runtime. This gives you the benefits of dynamic runtime sizing like you
get with `malloc()`, but without needing to worry about `free()`ing the
memory after.

Now, a lot of people don't like VLAs. They've been banned from the Linux
kernel, for example. We'll dig into more of that rationale
[later](#vla-general-issues).

This is an optional feature of the language. The macro `__STDC_NO_VLA__`
is set to `1` if VLAs are _not_ present. (They were mandatory in C99,
and then became optional in C11.)

``` {.c}
#if __STDC_NO_VLA__ == 1
   #error Sorry, need VLAs for this program!
#endif
```

Let's dive in first with an example, and then we'll look for the devil
in the details.

## The Basics

A normal array is declared with a constant size, like this:

``` {.c}
int v[10];
```

But with VLAs, we can use a size determined at runtime to set the array,
like this:

``` {.c}
int n = 10;
int v[n];
```

Now, that looks like the same thing, and in many ways is, but this gives
you the flexibility to compute the size you need, and then get an array
of exactly that size.

Let's ask the user to input the size of the array, and then store the
index-times-10 in each of those array elements:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int n;

    printf("Enter a number: "); fflush(stdout);
    scanf(" %d", &n);

    int v[n];

    for (int i = 0; i < n; i++)
        v[i] = i * 10;

    for (int i = 0; i < n; i++)
        printf("v[%d] = %d\n", i, v[i]);
}
```

(On line 7, I have an `fflush()` that should force the line to output
even though I don't have a newline at the end.)

Line 10 is where we declare the VLA---once execution gets past that
line, the size of the array is set to whatever `n` was at that moment.
The array length can't be changed later.

You can put an expression in the brackets, as well:

``` {.c}
int v[x * 100];
```

Some restrictions:

* You can't declare a VLA at file scope, and you can't make a `static`
  one in block scope^[This is due to how VLAs are typically allocated on
  the stack, whereas `static` variables are on the heap. And the whole
  idea with VLAs is they'll be automatically dellocated when the stack
  frame is popped at the end of the function.].
* You can't use an initializer list to initialize the array.

Also, entering a negative value for the size of the array invokes
undefined behavior---in this universe, anyway.

## `sizeof` and VLAs

We're used to `sizeof` giving us the size in bytes of any particular
object, including arrays. And VLAs are no exception.

The main difference is that `sizeof` on a VLA is executed at _runtime_,
whereas on a non-variably-sized variable it is computed at _compile
time_.

But the usage is the same.

You can even compute the number of elements in a VLA with the usual
array trick:

``` {.c}
size_t num_elems = sizeof v / sizeof v[0];
```

There's a subtle and correct implication from the above line: pointer
arithmetic works just like you'd expect for a regular array. So go ahead
and use it to your heart's content:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int n = 5;
    int v[n];

    int *p = v;

    *(p+2) = 12;
    printf("%d\n", v[2]);  // 12

    p[3] = 34;
    printf("%d\n", v[3]);  // 34
}
```

Like with regular arrays, you can use parentheses with `sizeof()` to get
the size of a would-be VLA without actually declaring one:

``` {.c}
int x = 12;

printf("%zu\n", sizeof(int [x]));  // Prints 48 on my system
```

## Multidimensional VLAs

You can go ahead and make all kinds of VLAs with one or more dimensions
set to a variable

``` {.c}
int w = 10;
int h = 20;

int x[h][w];
int y[5][w];
int z[10][w][20];
```

Again, you can navigate these just like you would a regular array.

## Passing One-Dimensional VLAs to Functions

Passing single-dimensional VLAs into a function can be no different than
passing a regular array in. You just go for it.

``` {.c .numberLines}
#include <stdio.h>

int sum(int count, int *v)
{
    int total = 0;

    for (int i = 0; i < count; i++)
        total += v[i];

    return total;
}

int main(void)
{
    int x[5];   // Standard array

    int a = 5;
    int y[a];   // VLA

    for (int i = 0; i < a; i++)
        x[i] = y[i] = i + 1;

    printf("%d\n", sum(5, x));
    printf("%d\n", sum(a, y));
}
```

But there's a bit more to it than that. You can also let C know that the
array is a specific VLA size by passing that in first and then giving
that dimension in the parameter list:

``` {.c}
int sum(int count, int v[count])
{
    // ...
}
```

Incidentally, there are a couple ways of listing a prototype for the
above function; one of them involves an `*` if you don't want to
specifically name the value in the VLA. It just indicates that the type
is a VLA as opposed to a regular pointer.

VLA prototypes:

``` {.c}
void do_something(int count, int v[count]);  // With names
void do_something(int, int v[*]);            // Without names
```

Again, that `*` thing only works with the prototype---in the function
itself, you'll have to put the explicit size.

Now---_let's get multidimensional_! This is where the fun begins.

## Passing Multi-Dimensional VLAs to Functions

Same thing as we did with the second form of one-dimensional VLAs,
above, but this time we're passing in two dimensions and using those.

In the following example, we build a multiplication table matrix of a
variable width and height, and then pass it to a function to print it
out.

``` {.c .numberLines}
#include <stdio.h>

void print_matrix(int h, int w, int m[h][w])
{
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++)
            printf("%2d ", m[row][col]);
        printf("\n");
    }
}

int main(void)
{
    int rows = 4;
    int cols = 7;

    int matrix[rows][cols];

    for (int row = 0; row < rows; row++)
        for (int col = 0; col < cols; col++)
            matrix[row][col] = row * col;

    print_matrix(rows, cols, matrix);
}
```

### Partial Multidimensional VLAs

You can have some of the dimensions fixed and some variable. Let's say
we have a record length fixed at 5 elements, but we don't know how many
records there are.

``` {.c .numberLines}
#include <stdio.h>

void print_records(int count, int record[count][5])
{
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 5; j++)
            printf("%2d ", record[i][j]);
        printf("\n");
    }
}

int main(void)
{
    int rec_count = 3;
    int records[rec_count][5];

    // Fill with some dummy data
    for (int i = 0; i < rec_count; i++)
        for (int j = 0; j < 5; j++)
            records[i][j] = (i+1)*(j+2);

    print_records(rec_count, records);
}
```

## Compatibility with Regular Arrays

Because VLAs are just like regular arrays in memory, it's perfectly
permissible to pass them interchangeably... as long as the dimensions
match.

For example, if we have a function that specifically wants a $3\times5$
array, we can still pass a VLA into it.

``` {.c}
int foo(int m[5][3]) {...}

\\ ...

int w = 3, h = 5;
int matrix[h][w];

foo(matrix);   // OK!
```

Likewise, if you have a VLA function, you can pass a regular array into
it:

``` {.c}
int foo(int h, int w, int m[h][w]) {...}

\\ ...

int matrix[3][5];

foo(3, 5, matrix);   // OK!
```

Beware, though: if your dimensions mismatch, you're going to have some
undefined behavior going on, likely.

## `typedef` and VLAs

You can `typedef` a VLA, but the behavior might not be as you expect.

Basically, `typedef` makes a new type with the values as they existed
the moment the `typedef` was executed.

So it's not a `typedef` of a VLA so much as a new fixed size array type
of the dimensions at the time.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int w = 10;

    typedef int goat[w];

    // goat is an array of 10 ints
    goat x;

    // Init with squares of numbers
    for (int i = 0; i < w; i++)
        x[i] = i*i;

    // Print them
    for (int i = 0; i < w; i++)
        printf("%d\n", x[i]);

    // Now let's change w...

    w = 20;

    // But goat is STILL an array of 10 ints, because that was the
    // value of w when the typedef executed.
}
```

So it acts like an array of fixed size.

But you still can't use an initializer list on it.

## Jumping Pitfalls

You have to watch out when using `goto` near VLAs because a lot of
things aren't legal.

And when you're using `longjmp()` there's a case where you could leak
memory with VLAs.

But both of these things we'll cover in their respective chapters.

## General Issues {#vla-general-issues}

VLAs have been banned from the Linux kernel for a few reasons:

* Lots of places they were used should have just been fixed-size.
* The code behind VLAs is slower (to a degree that most people wouldn't
  notice, but makes a difference in an operating system).
* VLAs are not supported to the same degree by all C compilers.
* Stack size is limited, and VLAs go on the stack. If some code
  accidentally (or maliciously) passes a large value into a kernel
  function that allocates a VLA, _Bad Things_™ could happen.

Other folks online point out that there's no way to detect a VLA's
failure to allocate, and programs that suffered such problems would
likely just crash. While fixed-size arrays also have the same issue,
it's far more likely that someone accidentally make a _VLA Of Unusual
Size_ than somehow accidentally declare a fixed-size, say, 30 megabyte
array.
