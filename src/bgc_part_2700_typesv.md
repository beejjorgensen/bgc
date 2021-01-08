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
directly in (parameter names listed above):

``` {.c}
//                   p[]         count
//           |-----------------|  |
int s = sum((int []){1, 2, 3, 4}, 4));
```

Pretty slick!

### Unnamed `struct`s

### Pointers to Unnamed Objects

### Unnamed Objects and Scope

## Generics