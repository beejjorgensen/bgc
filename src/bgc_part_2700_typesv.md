<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Types Part V: Unnamed Types and Generics

This is the final chapter for types! We're going to talk about two
things:

* How to have "anonymous" unnamed objects and how that's useful.
* How to generate type-dependent code.

They're not particularly related, but don't really each warrant their
own chapters. So I crammed them in here like a rebel!

## Unnamed Types

This is a neat feature of the language that allows you to create an
object of some type on the fly without ever assigning it to a variable.
You can make simple types, arrays, `struct`s, you name it.

One of the main uses for this is passing complex arguments to functions
when you don't want to make a temporary variable to hold the value.

The way you create an unnamed type is to put the type name in
parentheses, and then put an initializer list after. For example, an
unnamed array of `int`s, might look like this:

``` {.c}
(int []){1,2,3,4}
```

Now, that line of code doesn't do anything on its own. It creates an
array of 4 `int`s, and then throws them away without using them.

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
int s = sum((int []){1, 2, 3, 4}, 4));
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

``` {.c .numberLines .startFrom="14"}
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

Easy as that.

### Unnamed Objects and Scope

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

## Generics