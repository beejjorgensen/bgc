<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Structs {#structs}

In C, have something called a `struct`, which is a user-definable type
that holds multiple pieces of data, potentially of different types.

It's a convenient way to bundle multiple variables into a single one.
This can be beneficial for passing variables to functions (so you just
have to pass one instead of many), and useful for organizing data and
making code more readable.

If you've come from another language, you might be familiar with the
idea of _classes_ and _objects_. These don't exist in C,
natively^[Although in C individual items in memory like `int`s are
referred to as "objects", they're not objects in an object-oriented
programming sense.]. You can think of a `struct` as a class with only
data members, and no methods.

## Declaring a Struct

You can declare a `struct` in your code like so:

``` {.c}
struct car {
    char *name;
    float price;
    int speed;
};
```

When you do this, you're making a new _type_. The full type name is
`struct car`. (Not just `car`---that won't work.)

There aren't any variables of that type yet, but we can declare some:

``` {.c}
struct car saturn;
```

And now we have an uninitialized variable `saturn`^[The Saturn was a
popular brand of economy car in the United States until it was put out
of business by the 2008 crash.] of type `struct car`.

We should initialize it! But how do we set the values of those
individual fields?

Like in many other languages that stole it from C, we're going to use
the dot operator (`.`) to access the individual fields.

``` {.c}
saturn.name = "Saturn SL/2";
saturn.price = 16000.99;
saturn.speed = 175;

printf("Name:           %s\n", saturn.name);
printf("Price:          %f\n", saturn.price);
printf("Top Speed (km): %d\n", saturn.speed);
```

## Struct Initializers

That example in the previous section was a little unwieldy. There must
be a better way to initialize that `struct` variable!

You can do it with an initializer by putting values in for the fields
_in the order they appear in the `struct`_ when you define the
variable. (This won't work after the variable has been defined---it has
to happen in the definition).

``` {.c}
struct car {
    char *name;
    float price;
    int speed;
};

// Now with an initializer! Same field order as in the struct declaration:
struct car saturn = {"Saturn SL/2", 16000.99, 175};

printf("Name:      %s\n", saturn.name);
printf("Price:     %f\n", saturn.price);
printf("Top Speed: %d km\n", saturn.speed);
```

The fact that the fields in the initializer need to be in the same order
is a little freaky. If someone changes the order in `struct car`, it
could break all the other code!

We can be more specific with our initializers:

``` {.c}
struct car saturn = {.speed=172, .name="Saturn SL/2"};
```

Now it's independent of the order in the `struct` declaration. Which is
safer code, for sure.

Similar to array initializers, any missing field designators are
initialized to zero (in this case, that would be `.price`, which I've
omitted).

