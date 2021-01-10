<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Arrays Part II

We're going to go over a few extra misc things this chapter concerning
arrays.

* Type qualifiers with array parameters
* The `static` keyword with array parameters
* Partial multi-dimensional array initializers

They're not super-commonly seen, but we'll peek at them since they're
part of the newer spec.

## Type Qualifiers for Arrays in Parameter Lists

If you recall from earlier, these two things are equivalent in function
parameter lists:

``` {.c}
int func(int *p) {...}
int func(int p[]) {...}
```

And you might also recall that you can add type qualifiers to a pointer
variable like so:

``` {.c}
int *const p;
int *volatile p;
int *const volatile p;
// etc.
```

But how can we do that when we're using array notation in your parameter
list?

Turns out it goes in the brackets. And you can put the optional count
after. The two following lines are equivalent:

``` {.c}
int func(int *const volatile p) {...}
int func(int p[const volatile]) {...}
int func(int p[const volatile 10]) {...}
```

If you have a multidimensional array, you need to put the type
qualifiers in the first set of brackets.

## `static` for Arrays in Parameter Lists

Similarly, you can use the keyword static in the array in a parameter
list.

This is something I've never seen in the wild. It is **always** followed
by a dimension:

``` {.c}
int func(int p[static 4]) {...}
```

What this means, in the above example, is the compiler is going to
assume that any array you pass to the function will be _at least_ 4
elements.

Anything else is undefined behavior.

``` {.c}
int func(int p[static 4]) {...}

int main(void)
{
    int a[] = {11, 22, 33, 44};
    int b[] = {11, 22, 33, 44, 55};
    int c[] = {11, 22};

    func(a);  // OK! a is 4 elements, the minimum
    func(b);  // OK! b is at least 4 elements
    func(c);  // Undefined behavior! c is under 4 elements!
}
```

This basically sets the minimum size array you can have.

Important note: there is nothing in the compiler that prohibits you from
passing in a smaller array. The compiler probably won't warn you, and it
won't detect it at runtime.

By putting `static` in there, you're saying, "I double secret PROMISE
that I will never pass in a smaller array than this." And the compiler
says, "Yeah, fine," and trusts you to not do it.

And then the compiler can make certain cone optimization, safe in the
knowledge that you, the programmer, will always do the right thing.



## equivalent initializers
