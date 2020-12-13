<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Manual Memory Allocation

This is one of the big areas where C likely diverges from languages you
already know: _manual memory management_.

Other languages uses reference counting, garbage collection, or other
means to determine when to allocate new memory for some data---and when
to deallocate it when no variables refer to it.

And that's nice. It's nice to be able to not worry about it, to just
drop all the references to an item and trust that at some point the
memory associated with it will be freed.

But C's not like that, entirely.

Of course, in C, some variables are automatically allocated and
deallocated when they come into scope and leave scope. We call these
automatic variables. They're your average run-of-the-mill block scope
"local" variables. No problem.

But what if you want something to persist longer than a particular
block? This is where manual memory management comes into play.

You can tell C explicitly to allocate for you a certain number of bytes
that you can use as you please. And these bytes will remain allocated
until you explicitly free that memory^[Or until the program exits, in
which case all the memory allocated by it is freed. Asterisk: some
systems allow you to allocate memory that persists after a program
exits, but it's system dependent, out of scope for this guide, and
you'll certainly never do it on accident.].

It's important to free the memory you're done with! If you don't, we
call that a _memory leak_ and your process will continue to reserve that
memory until it exits.

_If you manually allocated it, you have to manually free it when you're
done with it._

So how do we do this? We're going to learn a couple new functions, and
make use of the `sizeof` operator to help us learn how many bytes to
allocate.

In common C parlance, devs say that automatic local variables are
allocated "on the stack", and manually-allocated memory is "on the
heap". The spec doesn't talk about either of those things, but all C
devs will know what you're talking about if you bring them up.

All functions we're going to learn in this chapter can be found in
`<stdlib.h>`.

## Allocating and Deallocating, `malloc()` and `free()`

The `malloc()` function accepts a number of bytes to allocate, and
returns a void pointer to that block of newly-allocated memory.

Since it's a `void*`, you can assign it into whatever pointer type you
want... normally this will correspond in some way to the number of bytes
you're allocating.

So... how many bytes should I allocate? We can use `sizeof` to help with
that. If we want to allocate enough room for a single `int`, we can use
`sizeof(int)` and pass that to `malloc()`.

After we're done with some allocated memory, we can call `free()` to
indicate we're done with that memory and it can be used for something
else. As an argument, you pass the same pointer you got from `malloc()`
(or a copy of it). It's undefined behavior to use a memory region after
you `free()` it.

Let's try. We'll allocate enough memory for an `int`, and then store
something there, and the print it.

``` {.c}
// Allocate space for a single int (sizeof(int) bytes-worth):

int *p = malloc(sizeof(int));

*p = 12;  // Store something there

printf("%d\n", *p);  // Print it: 12

free(p);  // All done with that memory

//*p = 3490;  // ERROR: undefined behavior! Use after free()!
```

Now, in that contrived example, there's really no benefit to it. We
could have just used an automatic `int` and it would have worked. But
we'll see how the ability to allocate memory this way has its
advantages, especially with more complex data structures.

One more thing you'll commonly see takes advantage of the fact that
`sizeof` can give you the size of the result type of any constant
expression. So you could put a variable name in there, too, and use
that. Here's an example of that, just like the previous one:

``` {.c}
int *p = malloc(sizeof *p);  // *p is an int, so same as sizeof(int)
```

## Error Checking

All the allocation functions return a pointer to the newly-allocated
stretch of memory, or `NULL` if the memory cannot be allocated for some
reason.

Some OSes like Linux can be configured in such a way that `malloc()`
never returns `NULL`, even if you're out of memory. But despite this,
you should always code it up with protections in mind.

``` {.c}
int *x;

x = malloc(sizeof(int) * 10);

if (x == NULL) {
    printf("Error allocating 10 ints\n");
    // do something here to handle it
}
```

Here's a common pattern that you'll see, where we do the assignment 
and the condition on the same line:

``` {.c}
int *x;

if ((x = malloc(sizeof(int) * 10)) == NULL)
    printf("Error allocating 10 ints\n");
    // do something here to handle it
}
```

## Allocating Space for an Array

We've seen how to allocate space for a single thing; now what about for
a bunch of them in an array?

In C, an array is a bunch of the same thing back-to-back in a contiguous
stretch of memory.

We can allocate a contiguous stretch of memory---we've seen how to do
that. If we wanted 3490 bytes of memory, we could just ask for it:

``` {.c}
char *p = malloc(3490);  // Voila
```

And---indeed!---that's an array of 3490 `char`s (AKA a string!) since
each `char` is 1 byte. In other words, `sizeof(char)` is `1`.

Note: there's no initialization done on the newly-allocated
memory---it's full of garbage. Clear it with `memset()` if you want to,
or see `calloc()`, below.

But we can just multiply the size of the thing we want by the number of
elements we want, and then access them using either pointer or array
notation. Example!

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // Allocate space for 10 ints
    int *p = malloc(sizeof(int) * 10);

    // Assign them values 0-45:
    for (int i = 0; i < 10; i++)
        p[i] = i * 5;

    // Print all values 0, 5, 10, 15, ..., 40, 45
    for (int i = 0; i < 10; i++)
        printf("%d\n", p[i]);

    // Free the space
    free(p);
}
```

The key's in that `malloc()` line. If we know each `int` takes
`sizeof(int)` bytes to hold it, and we know we want 10 of them, we can
just allocate exactly that many bytes with:

``` {.c}
sizeof(int) * 10
```

And this trick works for every type. Just pass it to `sizeof` and
multiply by the size of the array.

## An Alternative: `calloc()`

This is another allocation function that works similarly to `malloc()`,
with two key differences:

* Instead of a single argument, you pass the size of one element, and
  the number of elements you wish to allocate. It's like it's made for
  allocating arrays.
* It clears the memory to zero.

You still use `free()` to deallocate memory obtained through `calloc()`.

Here's a comparison of `calloc()` and `malloc()`.

``` {.c}
// Allocate space for 10 ints with calloc(), initialized to 0:
int *p = calloc(sizeof(int), 10);

// Allocate space for 10 ints with malloc(), initialized to 0:
int *q = malloc(sizeof(int) * 10);
memset(q, 0, sizeof(int) * 10);   // set to 0
```

Again, the result is the same for both except `malloc()` doesn't zero
the memory by default.

## Changing Allocated Size with `realloc()`

If you've already allocated 10 `int`s, but later you decide you need 20,
what can you do?

One option is to allocate some new space, and then `memcpy()` the memory
over... but it turns out that sometimes you don't need to move anything.
And there's one function that's just smart enough to do the right thing
in all the right circumstances: `realloc()`.

It takes a pointer to some previously-allocted memory (by `malloc()` or
`calloc()`) and a new size for the memory region to be.

It then grows or shrinks that memory, and returns a pointer to it.
Sometimes it might return the same pointer (if the data didn't have to
be copied elsewhere), or it might return a different one (if the data
did have to be copied).

Let's allocate an array of 20 `float`s, and then change our mind and
make it an array of 40.

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // Allocate space for 20 floats
    float *p = malloc(sizeof *p * 20);

    // Assign them fractional values 0.0-1.0:
    for (int i = 0; i < 20; i++)
        p[i] = i / 20.0;

    // But wait! Let's actually make this an array of 40 elements
    p = realloc(p, sizeof *p * 40);

    // And assign the new elements values in the range 1.0-2.0
    for (int i = 20; i < 40; i++)
        p[i] = 1.0 + (i - 20) / 20.0;

    // Print all values 0.0-2.0 in the 40 elements:
    for (int i = 0; i < 40; i++)
        printf("%f\n", p[i]);

    // Free the space
    free(p);
}
```

Notice in there how we took the return value from `realloc()` and
reassigned it into the same pointer variable `p` that we passed in.
That's pretty common to do.

### `realloc()` with `NULL`

Trivia time! These two lines are equivalent:

``` {.c}
char *p = malloc(3490);
char *p = realloc(NULL, 3490);
```

That could be convenient if you have some kind of allocation loop and
you don't want to special-case the first `malloc()`.

```
int *p = NULL;
int length = 0;

while (!done) {
    // Allocate 10 more ints:
    length += 10;
    p = realloc(p, sizeof *p * length);

    // Do amazing things
    // ...
}
```

In that example, we didn't need an initial `malloc()` since `p` was
`NULL` to start.

## Aligned Allocations

You probably aren't going to need to use this.

And I don't want to get too far off in the weeds talking about it right
now, but there's this thing called _memory alignment_, which has to do
with the memory address (pointer value) being a multiple of a certain
number.

For example, a system might require that 16-bit values begin on memory
addresses that are multiples of 2. Or that 64-bit values begin on memory
addresses that are multiples of 2, 4, or 8, for example. It depends on
the CPU.

Some systems require this kind of alignment for fast memory access, or
some even for memory access at all.

Now, if you use `malloc()`, `calloc()`, or `realloc()`, C will give you
a chunk of memory that's well-aligned for any value at all, even
`struct`s. Works in all cases.

But there might be times that you know that some data can be aligned at
a smaller boundary, or must be aligned at a larger one for some reason.
I imagine this is more common with embedded systems programming.

In those cases, you can specify an alignment with `aligned_alloc()`.

The alignment is an integer power of two greater than zero, so `2`, `4`,
`8`, `16`, etc. and you give that to `aligned_alloc()` before the number
of bytes you're interested in.

The other restriction is that the number of bytes you allocate needs to
be a multiple of the alignment. But this might be changing. See [fl[C
Defect Report
460|http://www.open-std.org/jtc1/sc22/wg14/www/docs/summary.htm#dr_460]]

Let's do an example, allocating on a 64-byte boundary:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    // Allocate 256 bytes aligned on a 64-byte boundary
    char *p = aligned_alloc(64, 256);  // 256 == 64 * 4

    // Copy a string in there and print it
    strcpy(p, "Hello, world!");
    printf("%s\n", p);

    // Free the space
    free(p);
}
```

I want to throw a note here about `realloc()` and `aligned_alloc()`.
`realloc()` doesn't have any alignment guarantees, so if you need to get
some aligned reallocated space, you'll have to do it the hard way with
`memcpy()`.

Here's a non-standard `aligned_realloc()` function, if you need it:

``` {.c}
void *aligned_realloc(void *ptr, size_t alignment, size_t size)
{
    char *new_ptr = aligned_alloc(alignment, size);

    if (new_ptr == NULL)
        return NULL;

    if (ptr != NULL)
        memcpy(new_ptr, ptr, size);

    return new_ptr;
}
```

Note that it _always_ copies data, taking time, while real `realloc()`
will avoid that if it can. So this is hardly efficient. Avoid needing to
reallocate custom-aligned data.
