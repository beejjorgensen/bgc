<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Types Part V: Compound Literals and Generic Selections

This is the final chapter for types! We're going to talk about two
things:

* How to have "anonymous" unnamed objects and how that's useful.
* How to generate type-dependent code.

They're not particularly related, but don't really each warrant their
own chapters. So I crammed them in here like a rebel!

## Compound Literals

This is a neat feature of the language that allows you to create an
object of some type on the fly without ever assigning it to a variable.
You can make simple types, arrays, `struct`s, you name it.

One of the main uses for this is passing complex arguments to functions
when you don't want to make a temporary variable to hold the value.

The way you create a compound literal is to put the type name in
parentheses, and then put an initializer list after. For example, an
unnamed array of `int`s, might look like this:

``` {.c}
(int []){1,2,3,4}
```

Now, that line of code doesn't do anything on its own. It creates an
unnamed array of 4 `int`s, and then throws them away without using them.

We could use a pointer to store a reference to the array...

``` {.c}
int *p = (int []){1 ,2 ,3 ,4};

printf("%d\n", p[1]);  // 2
```

But that seems a little like a long-winded way to have an array. I mean,
we could have just done this^[Which isn't quite the same, since it's an
array, not a pointer to an `int`.]:

``` {.c}
int p[] = {1, 2, 3, 4};

printf("%d\n", p[1]);  // 2
```

So let's take a look at a more useful example.

### Passing Unnamed Objects to Functions

Let's say we have a function to sum an array of `int`s:

``` {.c}
int sum(int p[], int count)
{
    int total = 0;

    for (int i = 0; i < count; i++)
        total += p[i];

    return total;
}
```

If we wanted to call it, we'd normally have to do something like this,
declaring an array and storing values in it to pass to the function:

``` {.c}
int a[] = {1, 2, 3, 4};

int s = sum(a, 4);
```

But unnamed objects give us a way to skip the variable by passing it
directly in (parameter names listed above). Check it out---we're going
to replace the variable `a` with an unnamed array that we pass in as the
second argument:

``` {.c}
//                   p[]         count
//           |-----------------|  |
int s = sum((int []){1, 2, 3, 4}, 4);
```

Pretty slick!

### Unnamed `struct`s

We can do something similar with `struct`s.

First, let's do things without unnamed objects. We'll define a `struct`
to hold some `x`/`y` coordinates. Then we'll define one, passing in
values into its initializer. Finally, we'll pass it to a function to
print the values:

``` {.c .numberLines}
#include <stdio.h>

struct coord {
    int x, y;
};

void print_coord(struct coord c)
{
    printf("%d, %d\n", c.x, c.y);
}

int main(void)
{
    struct coord t = {.x=10, .y=20};

    print_coord(t);   // prints "10, 20"
}
```

Straightforward enough?

Let's modify it to use an unnamed object instead of the variable `t`
we're passing to `print_coord()`.

We'll just take `t` out of there and replace it with an unnamed
`struct`:

``` {.c .numberLines startFrom="7"}
    //struct coord t = {.x=10, .y=20};

    print_coord((struct coord){.x=10, .y=20});   // prints "10, 20"
```

Still works!

### Pointers to Unnamed Objects

You might have noticed in the last example that even through we were
using a `struct`, we were passing a copy of the `struct` to
`print_coord()` as opposed to passing a pointer to the `struct`.

Turns out, we can just take the address of an unnamed object with `&`
like always.

This is because, in general, if an operator would have worked on a
variable of that type, you can use that operator on an unnamed object of
that type.

Let's modify the above code so that we pass a pointer to an unnamed
object 

``` {.c .numberLines}
#include <stdio.h>

struct coord {
    int x, y;
};

void print_coord(struct coord *c)
{
    printf("%d, %d\n", c->x, c->y);
}

int main(void)
{
    //     Note the &
    //          |
    print_coord(&(struct coord){.x=10, .y=20});   // prints "10, 20"
}
```

Additionally, this can be a nice way to pass even pointers to simple
objects:

``` {.c}
// Pass a pointer to an int with value 3490
foo(&(int){3490});
```

Easy as that.

### Unnamed Objects and Scope

The lifetime of an unnamed object ends at the end of its scope. The
biggest way this could bite you is if you make a new unnamed object, get
a pointer to it, and then leave the object's scope. In that case, the
pointer will refer to a dead object.

So this is undefined behavior:

``` {.c}
int *p;

{
    p = &(int){10};
}

printf("%d\n", *p);  // INVALID: The (int){10} fell out of scope
```

Likewise, you can't return a pointer to an unnamed object from a
function. The object is deallocated when it falls out of scope:

``` {.c .numberLines}
#include <stdio.h>

int *get3490(void)
{
    // Don't do this
    return &(int){3490};
}

int main(void)
{
    printf("%d\n", *get3490());  // INVALID: (int){3490} fell out of scope
}
```

Just think of their scope like that of an ordinary local variable. You
can't return a pointer to a local variable, either.

### Silly Unnamed Object Example

You can put any type in there and make an unnamed object.

For example, these are effectively equivalent:

``` {.c}
int x = 3490;

printf("%d\n", x);               // 3490 (variable)
printf("%d\n", 3490);            // 3490 (constant)
printf("%d\n", (int){3490});     // 3490 (unnamed object)
```

That last one is unnamed, but it's silly. Might as well do the simple
one on the line before.

But hopefully that provides a little more clarity on the syntax.

## Generic Selections {#type-generics}

This is an expression that allows you to select different pieces of code
depending on the _type_ of the first argument to the expression.

We'll look at an example in just a second, but it's important to know
this is processed at compile time, _not at runtime_. There's no
runtime analysis going on here.

The expression begins with `_Generic`, works kinda like a `switch`, and
it takes at least two arguments.

The first argument is an expression (or variable^[A variable used here
_is_ an expression.]) that has a _type_. All expressions have a type.
The remaining arguments to `_Generic` are the cases of what to
substitute in for the result of the expression if the first argument is
that type.

Wat?

Let's try it out and see.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int i;
    float f;
    char c;

    char *s = _Generic(i,
                    int: "that variable is an int",
                    float: "that variable is a float",
                    default: "that variable is some type"
                );

    printf("%s\n", s);
}
```

Check out the `_Generic` expression starting on line 9.

When the compiler sees it, it look at the type of the first argument.
(In this example, the type of the variable `i`.) It then looks through
the cases for something of that type. And then it substitutes the
argument in place of the entire `_Generic` expression. 

In this case, `i` is an `int`, so it matches that case. Then the string
is substituted in for the expression. So the line turns into this when
the compiler sees it:

``` {.c}
    char *s = "that variable is an int";
```

If the compiler can't find a type match in the `_Generic`, it looks for
the optional `default` case and uses that.

If it can't find a type match and there's no `default`, you'll get a
compile error. The first expression **must** match one of the types or
`default`.

Because it's inconvenient to write `_Generic` over and over, it's often
used to make the body of a macro that can be easily repeatedly reused.

Let's make a macro `TYPESTR(x)` that takes an argument and returns a
string with the type of the argument.

So `TYPESTR(1)` will return the string `"int"`, for example.

Here we go:

``` {.c}
#include <stdio.h>

#define TYPESTR(x) _Generic((x), \
                        int: "int", \
                        long: "long", \
                        float: "float", \
                        double: "double", \
                        default: "something else")

int main(void)
{
    int i;
    long l;
    float f;
    double d;
    char c;

    printf("i is type %s\n", TYPESTR(i));
    printf("l is type %s\n", TYPESTR(l));
    printf("f is type %s\n", TYPESTR(f));
    printf("d is type %s\n", TYPESTR(d));
    printf("c is type %s\n", TYPESTR(c));
}
```

This outputs:

``` {.default}
i is type int
l is type long
f is type float
d is type double
c is type something else
```

Which should be no surprise, because, like we said, that code in
`main()` is replaced with the following when it is compiled:

``` {.c}
    printf("i is type %s\n", "int");
    printf("l is type %s\n", "long");
    printf("f is type %s\n", "float");
    printf("d is type %s\n", "double");
    printf("c is type %s\n", "something else");
```

And that's exactly the output we see.

Let's do one more. I've included some macros here so that when you run:

``` {.c}
int i = 10;
char *s = "Foo!";

PRINT_VAL(i);
PRINT_VAL(s);
```

you get the output:

``` {.default}
i = 10
s = Foo!
```

We'll have to make use of some macro magic to do that.

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

// Macro that gives back a format specifier for a type
#define FMTSPEC(x) _Generic((x), \
                        int: "%d", \
                        long: "%ld", \
                        float: "%f", \
                        double: "%f", \
                        char *: "%s")
                        // TODO: add more types
                        
// Macro that prints a variable in the form "name = value"
#define PRINT_VAL(x) do { \
    char fmt[512]; \
    snprintf(fmt, sizeof fmt, #x " = %s\n", FMTSPEC(x)); \
    printf(fmt, (x)); \
} while(0)

int main(void)
{
    int i = 10;
    float f = 3.14159;
    char *s = "Hello, world!";

    PRINT_VAL(i);
    PRINT_VAL(f);
    PRINT_VAL(s);
}
```

for the output:

``` {.default}
i = 10
f = 3.141590
s = Hello, world!
```

We could have crammed that all in one big macro, but I broke it into two
to prevent eye bleeding.