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

In general, `&*E == E`^[Even if `E` is `NULL`, it turns out, weirdly.].
The dereference "undoes" the address-of.

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
}
```

When I build that, I get a warning:

``` {.default}
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

``` {.c}
int x = 3490;
int *const p = &x;
int *const *const q = &p;  // More const!
```

And that works, too. Now we can't modify `q`, or the pointer `q` points
to.

## Multibyte Values {#multibyte-values}

We kinda hinted at this in a variety of places earlier, but clearly not
every value can be stored in a single byte of memory. Things take up
multiple bytes of memory (assuming they're not `char`s). You can tell
how many bytes by using `sizeof`. And you can tell which address in
memory is the _first_ byte of the object by using the standard `&`
operator, which always returns the address of the first byte.

And here's another fun fact! If you iterate over the bytes of any
object, you get its _object representation_. Two things with the same
object representation in memory are equal.

If you want to iterate over the object representation, you should do it
with pointers to `unsigned char`.

Let's make our own version of [`memcpy()`](#man-memcpy) that does
exactly this:

``` {.c}
void *my_memcpy(void *dest, const void *src, size_t n)
{
    // Make local variables for src and dest, but of type unsigned char

    const unsigned char *s = src;
    unsigned char *d = dest;

    while (n-- > 0)   // For the given number of bytes
        *d++ = *s++;  // Copy source byte to dest byte

    // Most copy functions return a pointer to the dest as a convenience
    // to the caller

    return dest;
}
```

(There are some good examples of post-increment and post-decrement in
there for you to study, as well.)

It's important to note that the version, above, is probably less
efficient than the one that comes with your system.

But you can pass pointers to anything into it, and it'll copy those
objects. Could be `int*`, `struct animal*`, or anything.

Let's do another example that prints out the object representation bytes
of a `struct` so we can see if there's any padding in there and what
values it has^[Your C compiler is not required to add padding bytes, and
the values of any padding bytes that are added are indeterminate.].

``` {.c .numberLines}
#include <stdio.h>

struct foo {
    char a;
    int b;
};

int main(void)
{
    struct foo x = {0x12, 0x12345678};
    unsigned char *p = (unsigned char *)&x;

    for (size_t i = 0; i < sizeof x; i++) {
        printf("%02X\n", p[i]);
    }
}
```

What we have there is a `struct foo` that's built in such a way that
should encourage a compiler to inject padding bytes (though it doesn't
have to). And then we get an `unsigned char *` to the first byte of the
`struct foo` variable `x`.

From there, all we need to know is the `sizeof x` and we can loop
through that many bytes, printing out the values (in hex for ease).

Running this gives a bunch of numbers as output. I've annotated it below
to identify where the values were stored:

``` {.default}
12  | x.a == 0x12

AB  |
BF  | padding bytes with "random" value
26  |

78  |
56  | x.b == 0x12345678
34  |
12  |
```

On all systems, `sizeof(char)` is 1, and we see that first byte at the
top of the output holding the value `0x12` that we stored there.

Then we have some padding bytes---for me, these varied from run to run.

Finally, on my system, `sizeof(int)` is 4, and we can see those 4 bytes
at the end. Notice how they're the same bytes as are in the hex value
`0x12345678`, but strangely in reverse order^[This will vary depending
on the architecture, but my system is _little endian_, which means the
least-significant byte of the number is stored first. _Big endian_
systems will have the `12` first and the `78` last. But the spec doesn't
dictate anything about this representation.].

So that's a little peek under the hood at the bytes of a more complex
entity in memory.

## The `NULL` Pointer and Zero

These things can be used interchangeably:

* `NULL`
* `0`
* `'\0'`
* `(void *)0`

Personally, I always use `NULL` when I mean `NULL`, but you might see
some other variants from time to time. Though `'\0'` (a byte with all
bits set to zero) will also compare equal, it's _weird_ to compare it to
a pointer; you should compare `NULL` against the pointer. (Of course,
lots of times in string processing, you're comparing _the thing the
pointer points to_ to `'\0'`, and that's right.)

`0` is called the _null pointer constant_, and, when compared to or
assigned into another pointer, it is converted to a null pointer of the
same type.

## Pointers as Integers

You can cast pointers to integers and vice-versa (since a pointer is
just an index into memory), but you probably only ever need to do this
if you're doing some low-level hardware stuff. The results of such
machinations are implementation-defined, so they aren't portable. And
_weird things_ could happen.

C does make one guarantee, though: you can convert a pointer to a
`uintptr_t` type and you'll be able to convert it back to a pointer
without losing any data.

`uintptr_t` is defined in `<stdint.h>`^[It's an optional feature, so it
might not be there---but it probably is.].

Additionally, if you feel like being signed, you can use `intptr_t` to
the same effect.

## Casting Pointers to other Pointers

There's only one safe pointer conversion:

1. Converting to `intptr_t` or `uintptr_t`.
2. Converting to and from `void*`.

TWO! Two safe pointer conversions.

3. Converting to and from `char*`.

THREE! Three safe conversions!

If you cast to a pointer of another type and then access the object it
points to, the behavior is undefined due to something called _strict
aliasing_.

Plain old _aliasing_ refers to the ability to have more than one way to
access the same object. The access points are aliases for each other.

_Strict aliasing_ says you are only allowed to access an object via
pointers to _compatible types_.

For example, this is definitely allowed:

``` {.c}
int a = 1;
int *p = &a;
```

`p` is a pointer to an `int`, and it points to a compatible
type---namely `int`---so we're golden.

But the following isn't good because `int` and `float` are not
compatible types:

``` {.c}
int a = 1;
float *p = (float *)&a;
```

Here's a demo program that does some aliasing. It takes a variable `v`
of type `int32_t` and aliases it to a pointer to a `struct words`. That
`struct` has two `int16_t`s in it. These types are incompatible, so
we're in violation of strict aliasing rules.

Let's see if we can break something.

``` {.c .numberLines}
#include <stdio.h>
#include <stdint.h>

struct words {
    int16_t v[2];
};

void fun(int32_t *pv, struct words *pw)
{
    for (int i = 0; i < 5; i++) {
        (*pv)++;

        // Print the 32-bit value and the 16-bit values:

        printf("%x, %x-%x\n", *pv, pw->v[1], pw->v[0]);
    }
}

int main(void)
{
    int32_t v = 0x12345678;

    struct words *pw = (struct words *)&v;  // Violates strict aliasing

    fun(&v, pw);
}
```

See how I pass in the two incompatible pointers to `fun()`? One of the
types is `int32_t*` and the other is `struct words*`.

But they both point to the same object: the 32-bit value initialized to
`0x12345678`.

So if we look at the fields in the `struct words`, we should see the two
16-bit halves of that number. Right?

And in the `fun()` loop, we increment the pointer to the `int32_t`.
That's it. But since the `struct` points to that same memory, it, too,
should be updated to the same value.

So let's run it and get this, with the 32-bit value on the left and the
two 16-bit portions on the right. It should match^[I'm printing out the
16-bit values reversed since I'm on a little-endian machine at it makes
it easier to read here.]:

``` {.default}
12345679, 1234-5679
1234567a, 1234-567a
1234567b, 1234-567b
1234567c, 1234-567c
1234567d, 1234-567d
```

and it does... _UNTIL TOMORROW!_

Let's try it compiling GCC with `-O3` and `-fstrict-aliasing`:

``` {.default}
12345679, 1234-5678
1234567a, 1234-5679
1234567b, 1234-567a
1234567c, 1234-567b
1234567d, 1234-567c
```

They're off by one! But they point to the same memory! How could this
be? Answer: it's undefined behavior to alias memory like that. _Anything
is possible_, except not in a good way.

If your code violates strict aliasing rules, whether it works or not
depends on how someone decides to compile it. And that's a bummer since
that's beyond your control. Unless you're some kind of omnipotent deity.

Unlikely, sorry.

Speaking of no fun, these rules also break an age-old tradition in C of
"subclassing" `structs` that have a common initial sequence. Folks used
to do this with abandon:

``` {.c}
struct base_class {
    int a, b;    // Common elements
};

struct subclass_a {
    struct base_class super;  // inheritance

    int x, y, z; // specialization
};

struct subclass_b {
    struct base_class super;  // inheritance

    float f, g, h; // specialization
};
```

And if you had some of these subclasses like this:

``` {.c}
struct subclass_a sa = {.super.a=1, .super.b=2};
struct subclass_b sb = {.super.a=3, .super.b=4};
```

And a function like this, taking a `void*` argument:

``` {.c}
void print_info(void *vp)
{
    struct base_class *p = vp;

    printf("%d %d\n", p->a, p->b);
}
```

We know the `struct`s would have the same initial memory layout as the
base class `structs`, owing to that mix-in being the first member
declared.

People then felt empowered to call `print_info()` with pointers to
either subclass `struct` type.

``` {.c}
print_info(&sa);
print_info(&sb);
```

But this breaks the strict aliasing rules since `p` in `print_info()` is
a pointer to a different type than the actual objects.

So don't do this unless you're OK being non-portable and are sure your
compiler isn't relying on the strict aliasing rules.

GCC can be forced to not use the strict aliasing rules with
`-fno-strict-aliasing`. Compiling the demo program, above, with `-O3`
and this flag causes the output to be as expected.

Lastly, _type punning_ is using pointers of different types to look at
the same data. Before strict aliasing, this kind of things was fairly
common:

``` {.c}
int a = 0x12345678;
short b = *((short *)&a);   // Violates strict aliasing
```

If you want to do type punning (relatively) safely, see the section on
[Unions and Type Punning](#union-type-punning).

## Pointer Differences {#ptr_differences}

As you know from the section on pointer arithmetic, you can subtract one
pointer from another^[Assuming they point to the same array object.] to
get the difference between them in count of array elements.

Now the _type of that difference_ is something that's up to the
implementation, so it could vary from system to system.

To be more portable, you can store the result in a variable of type
`ptrdiff_t` defined in `<stddef.h>`.

``` {.c}
int cats[100];

int *f = cats + 20;
int *g = cats + 60;

ptrdiff_t d = g - f;  // difference is 40
```

And you can print it by prefixing the integer format specifier with `t`:

``` {.c}
printf("%td\n", d);  // Print decimal: 40
printf("%tX\n", d);  // Print hex:     28
```

## Pointers to Functions

Functions are just collections of machine instructions in memory, so
there's no reason we can't get a pointer to the first instruction of the
function.

And then call it.

This can be useful for passing a pointer to a function into another
function as an argument. Then the second one could call whatever was
passed in.

The tricky part with these, though, is that C needs to know the type of
the variable that is the pointer to the function.

And it would really like to know all the details.

Like "this is a pointer to a function that takes two `int` arguments and
returns `void`".

How do you write all that down so you can declare a variable?

Well, it turns out it looks very much like a function prototype, except
with some extra parentheses:

``` {.c}
// Declare p to be a pointer to a function.
// This function returns a float, and takes two ints as arguments.

float (*p)(int, int);
```

Also notice that you don't have to give the parameters names. But you
can if you want; they're just ignored.

``` {.c}
// Declare p to be a pointer to a function.
// This function returns a float, and takes two ints as arguments.

float (*p)(int a, int b);
```

So now that we know how to declare a variable, how do we know what to
assign into it? How do we get the address of a function?

Turns out there's a shortcut just like with getting a pointer to an
array: you can just refer to the bare function name without parens. (You
can put an `&` in front of this if you like, but it's unnecessary and
not idiomatic.)

Once you have a pointer to a function, you can call it just by adding
parens and an argument list.

Let's do a simple example where I effectively make an alias for a
function by setting a pointer to it. Then we'll call it.

This code prints out `3490`:

``` {.c .numberLines}
#include <stdio.h>

void print_int(int n)
{
    printf("%d\n", n);
}

int main(void)
{
    // Assign p to point to print_int:

    void (*p)(int) = print_int;

    p(3490);          // Call print_int via the pointer
}
```

Notice how the type of `p` represents the return value and parameter
types of `print_int`. It has to, or else C will complain about
incompatible pointer types.

One more example here shows how we might pass a pointer to a function as
an argument to another function.

We'll write a function that takes a couple integer arguments, plus a
pointer to a function that operates on those two arguments. Then it
prints the result.

``` {.c .numberLines}
#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int mult(int a, int b)
{
    return a * b;
}

void print_math(int (*op)(int, int), int x, int y)
{
    int result = op(x, y);

    printf("%d\n", result);
}

int main(void)
{
    print_math(add, 5, 7);   // 12
    print_math(mult, 5, 7);  // 35
}
```

Take a moment to digest that. The idea here is that we're going to pass
a pointer to a function to `print_math()`, and it's going to call that
function to do some math.

This way we can change the behavior of `print_math()` by passing another
function into it. You can see we do that on lines 22-23 when we pass in
pointers to functions `add` and `mult`, respectively.

Now, on line 13, I think we can all agree the function signature of
`print_math()` is a sight to behold. And, if you can believe it, this
one is actually pretty straight-forward compared to some things you can
construct^[The Go Programming Language drew its type declaration syntax
inspiration from the opposite of what C does.].

But let's digest it. Turns out there are only three parameters, but
they're a little hard to see:

``` {.c}
//                      op             x      y
//              |-----------------|  |---|  |---|
void print_math(int (*op)(int, int), int x, int y)
```

The first, `op`, is a pointer to a function that takes two `int`s as
arguments and returns an `int`. This matches the signatures for both
`add()` and `mult()`.

The second and third, `x` and `y`, are just standard `int` parameters.

Slowly and deliberately let your eyes play over the signature while you
identify the working parts. One thing that always stands out for me is
the sequence `(*op)(`, the parens and the asterisk. That's the giveaway
it's a pointer to a function.

Finally, jump back to the _Pointers II_ chapter for a
pointer-to-function [example using the built-in
`qsort()`](#qsort-example).
