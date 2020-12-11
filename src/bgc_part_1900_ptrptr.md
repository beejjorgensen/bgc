<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Pointers III: Pointers to Pointers and More

Here's where we cover some intermediate and advanced pointer usage. If
you don't have pointers down well, review the previous chapters on
[pointers](#pointers) and [pointer arithmetic](#pointers2) before
starting on this stuff.

## Pointers to Pointers

If you can have a pointer to a variable, and a variable can be a
pointer, can you have a pointer to a variable that it itself a pointer?

Yes! This is a pointer to a pointer, and it's held in variable of type
pointer-pointer. 

Before we tear into that, I want to try for a _gut feel_ for how
pointers to pointers work.

Remember that a pointer is just a number. It's a number that represents
an index in computer memory, typically one that holds a value we're
interested in for some reason.

That pointer, which is a number, has to be stored somewhere. And that
place is memory, just like everything else^[There's some devil in the
details with values that are stored in registers only, but we can safely
ignore that for our purposes here. Also the C spec makes no stance on
these "register" things beyond the `register` keyword, the description
for which doesn't mention registers.].

But because it's stored in memory, it must have an index it's stored at,
right? The pointer must have an index in memory where it is stored. And
that index is a number. It's the address of the pointer. It's a pointer
to the pointer.

Let's start with a regular pointer to an `int`, back from the earlier
chapters:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int x = 3490;  // Type: int
    int *p = &x;   // Type: pointer to an int

    printf("%d\n", *p);  // 3490

    return 0;
}
```

Straightforward enough, right? We have two types represented: `int` and
`int*`, and we set up `p` to point to `x`. Then we can dereference `p`
on line 8 and print out the value `3490`.

But, like we said, we can have a pointer to any variable... so does that
mean we can have a pointer to `p`?

In other words, what type is this expression?


``` {.c}
int x = 3490;  // Type: int
int *p = &x;   // Type: pointer to an int

&p  // <-- What type is the address of p? AKA a pointer to p?
```

If `x` is an `int`, then `&x` is a pointer to an `int` that we've stored
in `p` which is type `int*`. Follow? (Repeat this paragraph until you
do!)

And therefore `&p` is a pointer to an `int*`, AKA a "pointer to a
pointer to an `int`". AKA "`int`-pointer-pointer".

Got it? (Repeat the previous paragraph until you do!)

We write this type with two asterisks: `int **`. Let's see it in action.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int x = 3490;  // Type: int
    int *p = &x;   // Type: pointer to an int
    int **q = &p;  // Type: pointer to pointer to int

    printf("%d %d\n", *p, **q);  // 3490 3490

    return 0;
}
```

Let's make up some pretend addresses for the above values as examples
and see what these three variables might look like in memory. The
address values, below are just made up by me for example purposes:

|Variable|Stored at Address|Value Stored There|
|-|-|-|
|`x`|`28350`|`3490`---the value from the code|
|`p`|`29122`|`28350`---the address of `x`!|
|`q`|`30840`|`29122`---the address of `p`!|

Indeed, let's try it for real on my computer^[You're very likely to get
different numbers on yours.] and print out the pointer values with `%p`
and I'll do the same table again with actual references (printed in
hex).

|Variable|Stored at Address|Value Stored There|
|-|-|-|
|`x`|`0x7ffd96a07b94`|`3490`---the value from the code|
|`p`|`0x7ffd96a07b98`|`0x7ffd96a07b94`---the address of `x`!|
|`q`|`0x7ffd96a07ba0`|`0x7ffd96a07b98`---the address of `p`!|

You can see those addresses are the same except the last byte, so just
focus on those.

On my system, `int`s are 4 bytes, which is why we're seeing the address
go up by 4 from `x` to `p`^[There is absolutely nothing in the spec that
says this will always work this way, but it happens to work this way on
my system.] and then goes up by 8 from `p` to `q`. On my system, all
pointers are 8 bytes.

Does it matter if it's an `int*` or an `int**`? Is one more bytes than
the other? Nope! Remember that all pointers are addresses, that is
indexes into memory. And on my machine you can represent an index with 8
bytes... doesn't matter what's stored at that index.

Now check out what we did there on line 9 of the previous example: we
_double dereferenced_ `q` to get back to our `3490`.

This is the important bit about pointers and pointers to pointers:

* You can get a pointer to anything with `&` (including to a pointer!)
* You can get the thing a pointer points to with `*` (including a
  pointer!)

So you can think of `&` as being used to make pointers, and `*` being
the inverse---it goes the opposite direction of `&`---to get to the
thing pointed to.

In terms of type, each time you `&`, that adds another pointer level to
the type.

|If you have|Then you run|The result type is|
|:-|:-:|:-|
|`int x`|`&x`|`int *`|
|`int *x`|`&x`|`int **`|
|`int **x`|`&x`|`int ***`|
|`int ***x`|`&x`|`int ****`|

And each time you use dereference (`*`), it does the opposite:

|If you have|Then you run|The result type is|
|:-|:-:|:-|
|`int ****x`|`*x`|`int ***`|
|`int ***x`|`*x`|`int **`|
|`int **x`|`*x`|`int *`|
|`int *x`|`*x`|`int`|

Note that you can use multiple `*`s in a row to quickly dereference,
just like we saw in the example code with `**q`, above. Each one strips
away one level of indirection.

|If you have|Then you run|The result type is|
|:-|:-:|:-|
|`int ****x`|`***x`|`int *`|
|`int ***x`|`**x`|`int *`|
|`int **x`|`**x`|`int`|

But `&` doesn't work the same way---you can only do those one at a time,
and have to store the result in an intermediate variable:

``` {.c}
int x = 3490;     // Type: int
int *p = &x;      // Type: int *, pointer to an int
int **q = &p;     // Type: int **, pointer to pointer to int
int ***r = &q;    // Type: int ***, pointer to pointer to pointer to int
int ****s = &r;   // Type: int ****, you get the idea
int *****t = &s;  // Type: int *****
```

### Pointer Pointers and `const`

If you recall, declaring a pointer like this:

``` {.c}
int *const p;
```

means that you can't modify `p`. Trying to `p++` would give you a
compile-time error.

But how does that work with `int **` or `int ***`? Where does the
`const` go, and what does it mean?

Let's start with the simple bit. The `const` right next to the variable
name refers to that variable. So if you want an `int***` that you can't
change, you can do this:

``` {.c}
int ***const p;

p++;  // Not allowed
```

But here's where things get a little weird.

What if we had this situation:

``` {.c .numberLines}
int main(void)
{
    int x = 3490;
    int *const p = &x;
    int **q = &p;

    return 0;
}
```

When I build that, I get a warning:

```
warning: initialization discards ‘const’ qualifier from pointer target type
    7 |     int **q = &p;
      |               ^
```

What's going on? The 

That is, we're saying that q is type `int **`, and if you dereference
that, the rightmost `*` in the type goes away. So after the dereference,
we have type `int *`.

And we're assigning `&p` into it which is _a pointer to_ an `int
*const`, or, in other words, `int *const *`.

But `q` is `int **`! A type with different `const`ness on the first
`*`! So we get a warning that the `const` in `p`'s `int *const *` is
being ignored and thrown away.

We can fix that by making sure `q`'s type is at least as `const` as `p`.

``` {.c}
int x = 3490;
int *const p = &x;
int *const *q = &p;
```

And now we're happy.

We could make `q` even more `const`. As it is, above, we're saying, "`q`
isn't itself `const`, but the thing it points to is `const`." But we
could make them both `const`:

```
int x = 3490;
int *const p = &x;
int *const *const q = &p;  // More const!
```

And that works, too. Now we can't modify `q`, or the pointer `q` points
to.

## Multibyte Values

We kinda hinted at this in a variety of places earlier, but clearly not
every value can be stored in a single byte of memory. Things take up
multiple bytes of memory (assuming they're not `char`s). You can tell
how many bytes by using `sizeof`. And you can tell which address in
memory is the _first_ byte of the object by using the standard `&`
operator, which always returns the address of the first byte.

And here's another fun fact! If you iterate over the bytes of any
object, you can copy
Let's 

``` {.c}
void *my_memcpy(void *dest, const void *src, size_t n)
{
    const unsigned char *s = src;
    char *d = dest;

    while (n-- > 0)
        *d++ = *s++;

    return dest;
}
```

