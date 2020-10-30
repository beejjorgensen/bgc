<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Pointers II {#pointers2}

Time to get more into it with a number of new pointer topics! If you're
not up to speed with pointers, [check out the first section in the guide
on the matter](#pointers).

### Pointer Arithmetic

Turns out you can do math on pointers, notably addition and subtraction.

But what does it mean when you do that?

In short, if you have a pointer to a type, adding one to the pointer
moves to the next item of that type directly after it in memory.

This is a little chicken-and-eggy with [Array/Pointer Equivalence,
below](#arraypointerrequiv), but we're going to give it a shot, anyway.

### Adding to Pointers

First, let's take an array of numbers.

``` {.c}
int a[5] = {11, 22, 33, 44, 55};
```

Then let's get a pointer to the first element in that array:

``` {.c}
int a[5] = {11, 22, 33, 44, 55};

int *p = &a[0];  // Or "int *p = a;" works just as well
```

The let's print the value there by dereferencing the pointer:

``` {.c}
printf("%d\n", *p);  // Prints 11
```

Now let's use pointer arithmetic to print the next element in the array,
the one at index 1:

``` {.c}
printf("%d\n", *(p + 1));  // Prints 22!!
```

What happened there? C knows that `p` is a pointer to an `int`. So it
knows the `sizeof` an `int`^[Recall that the `sizeof` operator tells you
the size in bytes of an object in memory.] and it knows to skip that
many bytes to get to the next `int` after the first one!

In fact, the prior example could be written these two equivalent ways:

``` {.c}
printf("%d\n", *p);        // Prints 11
printf("%d\n", *(p + 0));  // Prints 11
```

because adding `0` to a pointer results in the same pointer.

Let's think of the upshot here. We can iterate over elements of an array
this way instead of using an array:

``` {.c}
int a[5] = {11, 22, 33, 44, 55};

int *p = &a[0];  // Or "int *p = a;" works just as well

for (int i = 0; i < 5; i++) {
    printf("%d\n", *(p + i));  // Same as p[i]!
}
```

And that works the same as if we used array notation! Oooo! Getting
closer to that array/pointer equivalence thing! More on this later in
this chapter.

But what's actually happening, here? How do it work?

Remember from early on that memory is like a big array, where a byte is
stored at each array index.

And the array index into memory has a few names:

* Index into memory
* Location
* Address
* _Pointer!_

So a point is an index into memory, somewhere.

For a random example, say that a number 3490 was stored at address
("index") 23,237,489,202. If we have an `int` pointer to that 3490, that
value of that pointer is 23,237,489,202... because the pointer is the
memory address. Different words for the same thing.

And now let's say we have another number, 4096, stored right after the
3490 at address 23,237,489,210 (8 higher than the 3490 because each
`int` in this example is 8 bytes long).

If we add `1` to that pointer, it actually jumps ahead `sizeof(int)`
bytes to the next `int`. It knows to jump that far ahead because it's an
`int` pointer. If it were a `float` pointer, it'd jump `sizeof(float)`
bytes ahead to get to the next float!

So you can look at the next `int`, by adding `1` to the pointer, the one
after that by adding `2` to the pointer, and so on.

### Changing Pointers

We saw how we could add an integer to a pointer in the previous section.
This time, let's _modify the pointer, itself_.

You can just add (or subtract) integer values directly to (or from) any
pointer!

Let's do that example again, except with a couple changes. First, I'm
going to add a `999` to the end of our numbers to act as a sentinel
value. This will let us know where the end of the data is.

``` {.c}
int a[] = {11, 22, 33, 44, 55, 999};  // Add 999 here as a sentinel

int *p = &a[0];  // p points to the 11
```

And we also have `p` pointing to the element at index `0` of `a`, namely
`11`, just like before.

Now---let's starting _incrementing_ `p` so that it points at subsequent
elements of the array. We'll do this until `p` points to the `999`; that
is, we'll do it until `*p == 999`:

```
while (*p != 999) {       // While the thing p points to isn't 999
    printf("%d\n", *p);   // Print it
    p++;                  // Move p to point to the next int!
}
```

Pretty crazy, right?

When we give it a run, first `p` points to `11`. Then we increment `p`,
and it points to `22`, and then again, it points to `33`. And so on,
until it points to `999` and we quit.

### Subtracting Pointers

TODO new strlen

## Array/Pointer Equivalence {#arraypointerequiv}

TODO

## Iterating Through Bytes of an Object

TODO

## `void` Pointers

TODO
