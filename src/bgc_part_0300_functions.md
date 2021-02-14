<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Functions {#functions}

Very much like other languages you're used to, C has the concept of
_functions_.

Functions can accept a variety of _arguments_ and return a value. One
important thing, though: the arguments and return value types are
predeclared---because that's how C likes it!

Let's take a look at a function. This is a function that takes an `int`
as an argument, and returns an `int`.

``` {.c .numberLines}
int plus_one(int n)  // The "definition"
{
    return n + 1;
}
 
```

The `int` before the `plus_one` indicates the return type.

The `int n` indicates that this function takes one `int` argument,
stored in _parameter_ `n`.

Continuing the program down into `main()`, we can see the call to the
function, where we assign the return value into local variable `j`:

``` {.c .numberLines startFrom="6"}
int main(void)
{
    int i = 10, j;
    
    j = plus_one(i);  // The "call"

    printf("i + 1 is %d\n", j);
}
```

> Before I forget, notice that I defined the function before I used it.
> If hadn't done that, the compiler wouldn't know about it yet when it
> compiles `main()` and it would have given an unknown function call
> error. There is a more proper way to do the above code with _function
> prototypes_, but we'll talk about that later.

Also notice that `main()` is a function!

It returns an `int`.

But what's this `void` thing? This is a keyword that's used to indicate
that the function accepts no arguments.

You can also return `void` to indicate that you don't return a value:

``` {.c .numberLines}
// This function takes no parameters and returns no value:

void hello(void)
{
    printf("Hello, world!\n");
}

int main(void)
{
    hello();  // Prints "Hello, world!"
}
```

## Passing by Value {#passvalue}

When you pass a value to a function, _a copy of that value_ gets made in
this magical mystery world known as _the stack_^[Now. _technically
speaking_, the C specification doesn't say anything about a stack. It's
true. Your system might not use a stack deep-down for function calls.
But it either does or looks like it does, and every single C programmer
on the planet will know what you're talking about when you talk about
"the stack". It would be just mean for me to keep you in the dark. Plus,
the stack analogy is excellent for describing how recursion works.].
(The stack is just a hunk of memory somewhere that the program allocates
memory on. Some of the stack is used to hold the copies of values that
are passed to functions.)

For now, the important part is that _a copy_ of the variable
or value is being passed to the function. The practical upshot of
this is that since the function is operating on a copy of the value, you
can't affect the value back in the calling function directly. Like if
you wanted to increment a value by one, this would NOT work:

``` {.c .numberLines}
void increment(int a)
{
    a++;
}

int main(void)
{
    int i = 10;

    increment(i);
}
```

You might somewhat sensibly think that the value of `i` after the call
would be 11, since that's what the `++` does, right? This would be
incorrect. What is really happening here?

Well, when you pass `i` to the `increment()` function, a copy gets made
on the stack, right? It's the copy that `increment()` works on, not the
original; the original `i` is unaffected. We even gave the copy a name:
`a`, right? It's right there in the parameter list of the function
definition. So we increment `a`, sure enough, but what good does that do
us out in `main()` ? None! Ha!

That's why in the previous example with the `plus_one()` function, we
`return`ed the locally modified value so that we could see it again in
`main()`.

Seems a little bit restrictive, huh? Like you can only get one piece of
data back from a function, is what you're thinking. There is, however,
another way to get data back; C folks call it _passing by reference_.
But no fancy-schmancy name will distract you from the fact that
_EVERYTHING_ you pass to a function _WITHOUT EXCEPTION_ is copied onto
the stack and the function operates on that local copy, _NO MATTER
WHAT_. Remember that, even when we're talking about this so-called
passing by reference.

But that's a story for another time.


## Function Prototypes {#prototypes}

So if you recall back in the ice age a few sections ago, I mentioned
that you had to define the function before you used it, otherwise the
compiler wouldn't know about it ahead of time, and would bomb out with
an error.

This isn't quite strictly true. You can notify the compiler in
advance that you'll be using a function of a certain type that has a
certain parameter list and that way the function can be defined
anywhere at all, as long as the _function prototype_ has been
declared first.

Fortunately, the function prototype is really quite easy. It's
merely a copy of the first line of the function definition with a
semicolon tacked on the end for good measure. For example, this code
calls a function that is defined later, because a prototype has been
declared first:

``` {.c .numberLines}
int foo(void);  // This is the prototype!

int main(void)
{
    int i;
    
    i = foo();
}

int foo(void)  // this is the definition, just like the prototype!
{
    return 3490;
}
```

You might notice something about the sample code we've been using...that
is, we've been using the good old `printf()` function without defining
it or declaring a prototype! How do we get away with this lawlessness?
We don't, actually. There is a prototype; it's in that header file
`stdio.h` that we included with `#include`, remember? So we're still
legit, officer!

## Empty Parameter Lists

You might see these from time to time in older code, but you shouldn't
ever code one up in new code. Always use `void` to indicate that a
function takes no parameters. There's never^[Never say "never".] a
reason to do this in modern code.

There are two contexts for this:

* Omitting all parameters where the function is defined
* Omitting all parameters in a prototype

Let's look at a potential function definition first:

``` {.c}
void foo()  // Should really have a `void` in there
{
    printf("Hello, world!\n");
}
```

While the spec spells out that the behavior in this instance is _as-if_
you'd indicated `void` (C11 §6.7.6.3¶14), the `void` type is there for a
reason. Use it.

But in the case of a function prototype, there is a _significant_
difference between using `void` and not:

``` {.c}
void foo();
void foo(void);  // Not the same!
```

Leaving `void` out of the prototype indicates to the compiler that there
is no additional information about the parameters to the function. It
effectively turns off all that type checking.

With a prototype **definitely** use `void` when you have an empty
parameter list.
