<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Pointers II: Arithmetic {#pointers2}

Time to get more into it with a number of new pointer topics! If you're
not up to speed with pointers, [check out the first section in the guide
on the matter](#pointers).

## Pointer Arithmetic

Turns out you can do math on pointers, notably addition and subtraction.

But what does it mean when you do that?

In short, if you have a pointer to a type, adding one to the pointer
moves to the next item of that type directly after it in memory.

It's **important** to remember that as we move pointers around and look
at different places in memory, we need to make sure that we're always
pointing to a valid place in memory before we dereference. If we're off in
the weeds and we try to see what's there, the behavior is undefined and
a crash is a common result.

This is a little chicken-and-eggy with [Array/Pointer Equivalence,
below](#arraypointerequiv), but we're going to give it a shot, anyway.

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

Then let's print the value there by dereferencing the pointer:

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

But what's actually happening, here? How does it work?

Remember from early on that memory is like a big array, where a byte is
stored at each array index?

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

``` {.c}
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

You can subtract a value from a pointer to get to earlier address, as
well, just like we were adding to them before.

But we can also subtract two pointers to find the difference between
them, e.g. we can calculate how many `int`s there are between two
`int*`s. The catch is that this only works within a single array^[Or
string, which is really an array of `char`s. Somewhat peculiarly, you
can also have a pointer that references _one past_ the end of the array
without a problem and still do math on it. You just can't dereference it
when it's out there.]---if the pointers point to anything else, you get
undefined behavior.

Remember how strings are `char*`s in C? Let's see if we can use this to
write another variant of `strlen()` to compute the length of a string
that utilizes pointer subtraction.

The idea is that if we have a pointer to the beginning of the string, we
can find a pointer to the end of the string by scanning ahead for the
`NUL` character.

And if we have a pointer to the beginning of the string, and we computed
the pointer to the end of the string, we can just subtract the two
pointers to come up with the length!

``` {.c .numberLines}
#include <stdio.h>

int my_strlen(char *s)
{
    // Start scanning from the beginning of the string
    char *p = s;

    // Scan until we find the NUL character
    while (*p != '\0')
        p++;

    // Return the difference in pointers
    return p - s;
}

int main(void)
{
    printf("%d\n", my_strlen("Hello, world!"));  // Prints "13"
}
```

Remember that you can only use pointer subtraction between two pointers
that point to the same array!


## Array/Pointer Equivalence {#arraypointerequiv}

We're finally ready to talk about this! We've seen plenty of examples of
places where we've intermixed array notation, but let's give out the
_fundamental formula of array/pointer equivalence_:

``` {.c}
a[b] == *(a + b)
```

Study that! Those are equivalent and can be used interchangeably!

I've oversimplified a bit, because in my above example `a` and `b` can
both be expressions, and we might want a few more parentheses to force
order of operations in case the expressions are complex.

The spec is specific, as always, declaring (in C11 §6.5.2.1¶2):

> `E1[E2]` is identical to `(*((E1)+(E2)))`

but that's a little harder to grok. Just make sure you include
parentheses if the expressions are complicated so all your math
happens in the right order.

This means we can _decide_ if we're going to use array or pointer
notation for any array or pointer (assuming it points to an element of
an array).

Let's use an array and pointer with both array and pointer notation:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int a[] = {11, 22, 33, 44, 55};

    int *p = a;  // p points to the first element of a, 11

    // Print all elements of the array a variety of ways:

    for (int i = 0; i < 5; i++)
        printf("%d\n", a[i]);      // Array notation with a

    for (int i = 0; i < 5; i++)
        printf("%d\n", p[i]);      // Array notation with p

    for (int i = 0; i < 5; i++)
        printf("%d\n", *(a + i));  // Pointer notation with a

    for (int i = 0; i < 5; i++)
        printf("%d\n", *(p + i));  // Pointer notation with p

    for (int i = 0; i < 5; i++)
        printf("%d\n", *(p++));    // Moving pointer p
        //printf("%d\n", *(a++));    // Moving array variable a--ERROR!
}
```

So you can see that in general, if you have an array variable, you can
use pointer or array notion to access elements. Same with a pointer
variable.

The one big difference is that you can _modify_ a pointer to point to a
different address, but you can't do that with an array variable. <!--
6.3.2.1p2 -->

### Array/Pointer Equivalence in Function Calls

This is where you'll encounter this concept the most, for sure.

If you have a function that takes a pointer argument, e.g.:

``` {.c}
int my_strlen(char *s)
```

this means you can pass either an array or a pointer to this function
and have it work!

``` {.c}
char s[] = "Antelopes";
char *t = "Wombats";

printf("%d\n", my_strlen(s));  // Works!
printf("%d\n", my_strlen(t));  // Works, too!
```

And it's also why these two function signatures are equivalent:

``` {.c}
int my_strlen(char *s)    // Works!
int my_strlen(char s[])   // Works, too!
```

## `void` Pointers

You've already seen the `void` keyword used with functions, but this is
an entirely separate, unrelated animal.

Sometimes it's useful to have a pointer to a thing _that you don't know
the type of_.

I know. Bear with me just a second.

There are basically two use cases for this.

1. A function is going to operate on something byte-by-byte. For
   example, `memcpy()` copies bytes of memory from one pointer to
   another, but those pointers can point to any type. `memcpy()` takes
   advantage of the fact that if you iterate through `char*`s, you're
   iterating through the bytes of an object no matter what type the
   object is. More on this in the [Multibyte Values](#multibyte-values)
   subsection.

2. Another function is calling a function you passed to it (a callback),
   and it's passing you data. You know the type of the data, but the
   function calling you doesn't. So it passes you `void*`s---'cause it
   doesn't know the type---and you convert those to the type you need.
   The built-in [`qsort()`](#man-qsort) and [`bsearch()`](#man-bsearch)
   use this technique.

Let's look at an example, the built-in `memcpy()` function:

``` {.c}
void *memcpy(void *s1, void *s2, size_t n);
```

This function copies `n` bytes of memory starting from address `s1` into
the memory starting at address `s2`.

But look! `s1` and `s2` are `void*`s! Why? What does it mean? Let's run
more examples to see.

For instance, we could copy a string with `memcpy()` (though `strcpy()`
is more appropriate for strings):

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    char s[] = "Goats!";
    char t[100];

    memcpy(t, s, 7);  // Copy 7 bytes--including the NUL terminator!

    printf("%s\n", t);  // "Goats!"
}
```

Or we can copy some `int`s:

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    int a[] = {11, 22, 33};
    int b[3];

    memcpy(b, a, 3 * sizeof(int));  // Copy 3 ints of data

    printf("%d\n", b[1]);  // 22
}
```

That one's a little wild---you see what we did there with `memcpy()`? We
copied the data from `a` to `b`, but we had to specify how many _bytes_
to copy, and an `int` is more than one byte.

OK, then---how many bytes does an `int` take? Answer: depends on the
system. But we can tell how many bytes any type takes with the `sizeof`
operator.

So there's the answer: an `int` takes `sizeof(int)` bytes of memory to
store.

And if we have 3 of them in our array, like we did in that example, the
entire space used for the 3 `int`s must be `3 * sizeof(int)`.

(In the string example, earlier, it would have been more technically
accurate to copy `7 * sizeof(char)` bytes. But `char`s are always one
byte large, by definition, so that just devolves into `7 * 1`.)

We could even copy a `float` or a `struct` with `memcpy()`! (Though this
is abusive---we should just use `=` for that):

``` {.c}
struct antelope my_antelope;
struct antelopy my_clone_antelope;

// ...

memcpy(&my_clone_antelope, &my_antelope, sizeof my_antelope);
```

Look at how versatile `memcpy()` is! If you have a pointer to a source
and a pointer to a destination, and you have the number of bytes you
want to copy, you can copy _any type of data_.

Imagine if we didn't have `void*`. We'd have to write specialized
`memcpy()` functions for each type:

``` {.c}
memcpy_int(int *a, int *b, int count);
memcpy_float(float *a, float *b, int count);
memcpy_double(double *a, double *b, int count);
memcpy_char(char *a, char *b, int count);
memcpy_unsigned_char(unsigned char *a, unsigned char *b, int count);

// etc... blech!
```

Much better to just use `void*` and have one function that can do it
all.

That's the power of `void*`. You can write functions that don't care
about the type and is still able to do things with it.

But with great power comes great responsibility. Maybe not _that_ great
in this case, but there are some limits.

1. You cannot do pointer arithmetic on a `void*`.
2. You cannot dereference a `void*`.
3. You cannot use the arrow operator on a `void*`, since it's also a
   dereference.
4. You cannot use array notation on a `void*`, since it's also a
   dereference, as well^[Because remember that array notation is just a
   dereference and some pointer math, and you can't dereference a
   `void*`!].

And if you think about it, these rules make sense. All those operations
rely on knowing the `sizeof` the type of data pointed to, and with
`void*`, we don't know the size of the data being pointed to---it could
be anything!

But wait---if you can't dereference a `void*` what good can it ever do
you?

Like with `memcpy()`, it helps you write generic functions that can
handle multiple types of data. But the secret is that, deep down, _you
convert the `void*` to another type before you use it_!

And conversion is easy: you can just assign into a variable of the
desired type^[You can also _cast_ the `void*` to another type, but we
haven't gotten to casts yet.].

``` {.c}
char a = 'X';  // A single char

void *p = &a;  // p points to the 'X'
char *q = p;   // q also points to the 'X'

printf("%c\n", *p);  // ERROR--cannot dereference void*!
printf("%c\n", *q);  // Prints "X"
```

Let's write our own `memcpy()` to try this out. We can copy bytes
(`char`s), and we know the number of bytes because it's passed in.

``` {.c}
void *my_memcpy(void *dest, void *src, int byte_count)
{
    // Convert void*s to char*s
    char *s = src, *d = dest;

    // Now that we have char*s, we can dereference and copy them
    while (byte_count--) {
        *d++ = *s++;
    }

    // Most of these functions return the destination, just in case
    // that's useful to the caller.
    return dest;
}
```

Right there at the beginning, we copy the `void*`s into `char*`s so that
we can use them as `char*`s. It's as easy as that.

Then some fun in a while loop, where we decrement `byte_count` until it
becomes false (`0`). Remember that with post-decrement, the value of the
expression is computed (for `while` to use) and _then_ the variable is
decremented.

And some fun in the copy, where we assign `*d = *s` to copy the byte,
but we do it with post-increment so that both `d` and `s` move to the
next byte after the assignment is made.

Lastly, most memory and string functions return a copy of a pointer to
the destination string just in case the caller wants to use it.

Now that we've done that, I just want to quickly point out that we can
use this technique to iterate over the bytes of _any_ object in C,
`float`s, `struct`s, or anything!


[Let's]{#qsort-example} run one more real-world example with the
built-in `qsort()` routine that can sort _anything_ thanks to the magic
of `void*`s.

(In the following example, you can ignore the word `const`, which we
haven't covered yet.)

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

// The type of structure we're going to sort
struct animal {
    char *name;
    int leg_count;
};

// This is a comparison function called by qsort() to help it determine
// what exactly to sort by. We'll use it to sort an array of struct
// animals by leg_count.
int compar(const void *elem1, const void *elem2)
{
    // We know we're sorting struct animals, so let's make both
    // arguments pointers to struct animals
    const struct animal *animal1 = elem1;
    const struct animal *animal2 = elem2;

    // Return <0 =0 or >0 depending on whatever we want to sort by.

    // Let's sort ascending by leg_count, so we'll return the difference
    // in the leg_counts
    return animal1->leg_count - animal2->leg_count;
}

int main(void)
{
    // Let's build an array of 4 struct animals with different
    // characteristics. This array is out of order by leg_count, but
    // we'll sort it in a second.
    struct animal a[4] = {
        {.name="Dog", .leg_count=4},
        {.name="Monkey", .leg_count=2},
        {.name="Antelope", .leg_count=4},
        {.name="Snake", .leg_count=0}
    };

    // Call qsort() to sort the array. qsort() needs to be told exactly
    // what to sort this data by, and we'll do that inside the compar()
    // function.
    //
    // This call is saying: qsort array a, which has 4 elements, and
    // each element is sizeof(struct animal) bytes big, and this is the
    // function that will compare any two elements.
    qsort(a, 4, sizeof(struct animal), compar);

    // Print them all out
    for (int i = 0; i < 4; i++) {
        printf("%d: %s\n", a[i].leg_count, a[i].name);
    }
}
```

As long as you give `qsort()` a function that can compare two items that
you have in your array to be sorted, it can sort anything. And it does
this without needing to have the types of the items hardcoded in there
anywhere. `qsort()` just rearranges blocks of bytes based on the results
of the `compar()` function you passed in.
