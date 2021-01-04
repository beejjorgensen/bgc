<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Variable-Length Arrays (VLAs)

C provides a way for you to declare an array whose size is determined at
runtime. This gives you the benefits of dynamic runtime sizing like you
get with `malloc()`, but without needing to worry about `free()`ing the
memory after.

Now, a lot of people don't like VLAs. They've been banned from the Linux
kernel, for example. We'll dig into more of that rationale later.

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
  the stack, where as `static` variables are on the heap. And the whole
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

int add(int *v, int count)
{
    int total = 0;

    for (int i = 0; i < count; i++)
        total += v[i];

    return total;
}

int main(void)
{
    int x[5];
    int a = 5;
    int y[a];

    for (int i = 0; i < a; i++)
        x[i] = y[i] = i + 1;

    printf("%d\n", add(x, 5));
    printf("%d\n", add(y, a));
}
```

But there's a bit more to it than that. You can also let C know that the
array is a specific VLA size by passing that in first and then giving
that dimension in the parameter list:

``` {.c}
void do_something(int count, int v[count])
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

## Compatibility with Regular Arrays

## `typedef` and VLAs

## Jumping Pitfalls

-----

fixed at typedef moment

Any array size expressions associated with variable length array
declarators are evaluated each time the declaration of the typedef name
is reached in the order of execution.

longjmp pitfalls
goto pitfalls
