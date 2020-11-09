<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Types III: Conversions

In this chapter, we want to talk all about converting from one type to
another. C has a variety of ways of doing this, and some might be a
little different that you're used to in other languages.

Before we talk about how to make conversions happen, let's talk about
how they work when they _do_ happen.

## Conversion Rules

### Boolean

If you convert a zero to `bool`, the result is `0`. Otherwise it's `1`.

### Integer to Integer Conversions

If an integer type is converted to unsigned and doesn't fit in it, the
unsigned result wraps around odometer-style until it fits in the
unsigned^[In practice, what's probably happening on your implementation
is that the high-order bits are just being dropped from the result, so a
16-bit number `0x1234` being converted to an 8-bit number ends up as
`0x0034`, or just `0x34`.].

If an integer type is converted to a signed number and doesn't fit, the
result is implementation-defined! Something documented will happen, but
you'll have to look it up^[Again, in practice, what will likely happen
on your system is that the bit pattern for the original will be
truncated and then just used to represent the signed number, two's
complement. For example, my system takes an `unsigned char` of `192` and
converts it to `signed char` `-64`. In two's complement, the bit pattern
for both these numbers is binary `11000000`.]

### Integer and Floating Point Conversions

If a floating point type is converted to an integer type, the fractional
part is discarded with prejudice^[Not really---it's just discarded
regularly.].

But---and here's the catch---if the number is too large to fit in the
integer, you get undefined behavior. So don't do that.

Going From integer or floating point to floating point, C makes a best
effort to find the closest floating point number to the integer that it
can.

Again, though, if the original value can't be represented, it's undefined
behavior.

## Implicit Conversions

These are conversions the compiler does automatically for you when you
mix and match types.

### The Integer Promotions

In a number of places, if a `int` can be used to represent a value from
`char` or `short` (signed or unsigned), that value is _promoted_ up to
`int`. If it doesn't fit in an `int`, it's promoted to `unsigned int`.

This is how we can do something like this:

``` {.c}
char x = 10, y = 20;
int i = x + y;
```

In that case, `x` and `y` get promoted to `int` by C before the math
takes place.

The integer promotions take place during The Usual Arithmetic
Conversions, with variadic functions^[Functions with a variable number
of arguments.], unary `+` and `-` operators, or when passing values to
functions without prototypes^[This is rarely done because the compiler
will complain and having a prototype is the _Right Thing_ to do. I think
this still works for historic reasons, before prototypes were a thing.].

### The Usual Arithmetic Conversions

These are automatic conversions that C does around numeric operations
that you ask for. (That's actually what they're called, by the way, by
C99 §6.3.1.8.) Note that for this section, we're just talking about
numeric types---strings will come later.

These conversions answer questions about what happens when you mix
types, like this:

``` {.c}
int x = 3 + 1.2;   // Mixing int and double
float y = 12 * 2;  // Mixing float and int
```

Do they become `int`s? Do they become `float`s? How does it work?

Here are the steps, paraphrased for easy consumption.

1. If one thing in the expression is a floating type, convert the other
   things to that floating type.

2. Otherwise, if both types are integer types, perform the integer
   promotions on each, then make the operand types as big as they need
   to be hold the common largest value. Sometimes this involves changing
   signed to unsigned.

If you want to know the gritty details, check out C99 §6.3.1.8. But you
probably don't.

Just generally remember that int types become float types if there's a
floating point type anywhere in there, and the compiler makes an effort
to make sure mixed integer types don't overflow.

### `void*`

The `void*` type is interesting because it can be converted from or to
any pointer type.

``` {.c}
int x = 10;

void *p = &x;  // &x is type int*, but we store it in a void*

int *q = p;    // p is void*, but we store it in an int*
```

## Explicit Conversions

These are conversions from type to type that you have to ask for; the
compiler won't do it for you.

You can convert from one type to another by assigning one type to
another with an `=`.

You can also convert explicitly with a _cast_.

### Casting

You can explicitly change the type of an expression by putting a new
type in parentheses in front of it. Some C devs frown on the practice
unless absolutely necessary, but it's likely you'll come across some C
code with these in it.

Let's do an example where we want to convert an `int` into a `long` so
that we can store it in a `long`.

Note: this example is contrived and the cast in this case is completely
unnecessary because the `x + 12` expression would automatically be
changed to `long int` to match the wider type of `y`.

``` {.c}
int x = 10;
long int y = (long int)x + 12;
```

In that example, even those `x` was type `int` before, the expression
`(long int)x` has type `long int`. We say, "We cast `x` to `long int`."

More commonly, you might see a cast being used to convert a `void*`
into a specific pointer type so it can be dereferenced.

A callback from the built-in `qsort()` function might display this
behavior since it has `void*`s passed into it:

``` {.c}
int compar(const void *elem1, const void *elem2)
{
    return *((int*)elem2) - *((int*)elem1);
}
```

But you could also clearly write it with an assignment:

``` {.c}
int compar(const void *elem1, const void *elem2)
{
    int *e1 = elem1;
    int *e2 = elem2;

    return *e2 - *e1;
}
```

Again, casting is rarely _needed_ in practice. If you find yourself
casting, there might be another way to do the same thing, or maybe
you're casting unnecessarily.

Or maybe it is necessary. Personally, I try to avoid it, but am not
afraid to use it if I have to.