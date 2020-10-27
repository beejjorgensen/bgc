<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Strings

Finally! Strings! What could be simpler?

Well, turns out strings aren't actually strings in C. That's right!
They're pointers! Of course they are!

Let's check it out---it's not really such a big deal.

## Constant Strings

Before we start, let's talk about constant strings in C. These are
sequences of characters in _double_ quotes (`"`). (Single quotes enclose
characters, and are a different animal entirely.)

Examples:

``` {.c}
"Hello, world!\n"
"This is a test."
"When asked if this string had quotes in it, she replied, \"It does.\""
```

The first one has a newline at the end---quite a common thing to see.

The last one has quotes embedded within it, but you see each is preceded
by (we say "escaped by") a backslash (`\`) indicating that a literal
quote belongs in the string at this point. This is how the C compiler
can tell the difference between printing a double quote and the double
quote at the end of the string.

## String Variables

Now that we know how to make a constant string, let's assign it to a
variable so we can do something with it.

``` {.c}
char *s = "Hello, world!";
```

Check out that type: pointer to a `char`^[It's actually type `const
char*`, but we haven't talked about `const` yet.]. The string variable
`s` is actually a pointer to the first character in that string, namely
the `H`.

And we can print it with the `%s` (for "string") format specifier:

``` {.c}
char *s = "Hello, world!";

printf("%s\n", s);  // "Hello, world!"
```

## String Variables as Arrays

Another option is this, equivalent to the above `char*` usage:

``` {.c}
char s[14] = "Hello, world!";

// or, if we were properly lazy:

char s[] = "Hello, world!";
```

This means you can use array notation to access characters in a string.
Let's do exactly that to print all the characters in a string on the
same line:

``` {.c}
#include <stdio.h>

int main(void)
{
    char s[] = "Hello, world!";

    for (int i = 0; i < 13; i++)
        printf("%c\n", s[i]);

    return 0;
}
```

Note that we're using the format specifier `%c` to print a single
character.

Also, check this out. The program will still work fine if we change the
definition of `s` to be a `char*` type:

``` {.c}
#include <stdio.h>

int main(void)
{
    char *s = "Hello, world!";   // char* here

    for (int i = 0; i < 13; i++)
        printf("%c\n", s[i]);    // But still use arrays here...?

    return 0;
}
```

And we still can use array notation to get the job done when printing it
out! This is surprising, but is still only because we haven't talked
about array/pointer equivalence yet. But this is yet another hint that
arrays and pointers are the same thing, deep down.

## Getting String Length

You can't, since C doesn't track it for you. Well, that's really not
true that you can't get it. There's a function in `<string.h>` called
`strlen()` that can be used to compute the length of any string.

``` {.c}
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *s = "Hello, world!";

    printf("The string is %zu characters long.\n", strlen(s));

    return 0;
}
```

The `strlen()` function returns type `size_t`, which is an integer type
so you can use it for integer math. We print `size_t` with `%zu`.

The above program prints:

```
The string is 13 characters long.
```

## String Termination
