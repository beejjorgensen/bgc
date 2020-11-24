<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# The C Preprocessor

Before your program gets compiled, it actually runs through a phase
called _preprocessing_. It's almost like there's a language _on top_ of
the C language that runs first. And it outputs the C code, which then
gets compiled.

We've already seen this to an extent with `#include`! That's the C
Preprocessor! Where it sees that directive, it includes the named file
right there, just as if you'd typed it in there. And _then_ the
compiler builds the whole thing.

But it turns out it's a lot more powerful than just being able to
include things. You can define _macros_ that are substituted... and even
macros that take arguments!

## `#include`

Let's start with the one we've already seen a bunch. This is, of course,
a way to include other sources in your source. Very commonly used with
header files.

While the spec allows for all kinds of behavior with `#include`, we're
going to take a more pragmatic approach and talk about the way it works
on every system I've ever seen.

We can split header files into two categories: system and local. Things
that are built-in, like `stdio.h`, `stdlib.h`, `math.h`, and so on, you
can include with angle brackets:

``` {.c}
#include <stdio.h>
#include <stdlib.h>
```

The angle brackets tell C, "Hey, don't look in the current directory for
this header file---look in the system-wide include directory instead."

Which, of course, implies that there must be a way to include local
files from the current directory. And there is: with double quotes:

``` {.c}
#include "myheader.h"
```

Or you can very probably look in relative directories using forward
slashes and dots, like this:

``` {.c}
#include "mydir/myheader.h"
#include "../someheader.py"
```

Don't use a backslash (`\`) for your path separators in your `#include`!
It's undefined behavior! Use forward slash (`/`) only, even on Windows.

In summary, used angle brackets (`<` and `>`) for the system includes,
and use double quotes (`"`) for your personal includes.

## Simple Macros

Now let's check out another cool feature of the preprocessor: the
ability to define constant values and substitute them in place.

We do this with `#define` (often read "pound define"). Here's an
example:

``` {.c .numberLines}
#include <stdio.h>

#define HELLO "Hello, world"
#define PI 3.14159

int main(void)
{
    printf("%s, %f\n", HELLO, PI);

    return 0;
}
```

On lines 3 and 4 we defined a couple macros. Wherever these appear
elsewhere in the code (line 8), they'll be substituted with the defined
values.

From the C compiler's perspective, it's exactly as if we'd written this,
instead:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    printf("%s, %f\n", "Hello, world", 3.14159);

    return 0;
}
```

Note that the macros aren't typed, _per se_. Really all that happens is
they get replaced wholesale with whatever they're `#define`d as. If the
resulting C code is invalid, the compiler will puke.

You can also define a macro with no value:

``` {.c}
#define EXTRA_HAPPY
```

in that case, the macro exists and is defined, but is defined to be
nothing. So anyplace it occurs in the text will just be replaced with
nothing. We'll see a use for this later.

It's conventional to write macro names in `ALL_CAPS` even though that's
not technically required.

Overall, this gives you a way to define constant values that are
effectively global and can be used _any_ place that a constant can be
used, e.g. in `switch` `case`s.

It can also be used to replace or modify keywords, a place a `const`
won't work at all, though this practice should be used sparingly.

## Conditional Compilation

It's possible to get the preprocessor to decide whether or not to
present certain blocks of code to the compiler, or just remove them
entirely before compilation.

We do that by basically wrapping up the code in conditional blocks,
similar to `if`-`else` statements.

### If Defined, `#ifdef` and `#endif`

First of all, let's try to compile specific code depending on whether or
not a macro is even defined.

``` {.c .numberLines}
#include <stdio.h>

#define EXTRA_HAPPY

int main(void)
{

#ifdef EXTRA_HAPPY
    printf("I'm extra happy!\n");
#endif

    printf("OK!\n");

    return 0;
}
```

In that example, we define `EXTRA_HAPPY` (to be nothing, but it _is_
defined), then on line 8 we check to see if it is defined with an
`#ifdef` directive. If it is defined, the subsequent code will be
included up until the `#endif`.

So because it is defined, the code will be included for compilation and
the output will be:

```
I'm extra happy!
OK!
```

If we were to comment out the `#define`, like so:

``` {.c}
//#define EXTRA_HAPPY
```

then it wouldn't be defined, and the code wouldn't be included in
compilation. And the output would just be:

```
OK!
```

It's important to remember that these decisions happen at compile time!
The code actually get compiled or removed depending on the condition.
This is in contrast to a standard `if` statement that gets evaluated
while the program is running.

### If Not Defined, `#ifndef`

There's also the negative sense of "if defined": "if not defined", or
`#ifndef`. We could change the previous example to read to output
different things based on whether or not something was defined:

``` {.c .numberLines startFrom="8"}
#ifdef EXTRA_HAPPY
    printf("I'm extra happy!\n");
#endif

#ifndef EXTRA_HAPPY
    printf("I'm just regular\n");
#endif
```

We'll see a cleaner way to do that in the next section.

Tying it all back in to header files, we've seen how we can cause header
files to only be included one time by wrapping them in preprocessor
directives like this:

``` {.c}
#ifndef MYHEADER_H  // First line of myheader.h
#define MYHEADER_H

int x = 12;

#endif  // Last line of myheader.h
```

This demonstrates how a macro persists across files and multiple
`#include`s. If it's not yet defined, let's define it and compile the
whole header file.

But the next time it's included, we see that `MYHEADER_H` _is_ defined,
so we don't send the header file to the compiler---it gets effectively
removed.


### `#else`

But that's not all we can do! There's also an `#else` that we can throw
in the mix.

Let's mod the previous example:

``` {.c .numberLines startFrom="8"}
#ifdef EXTRA_HAPPY
    printf("I'm extra happy!\n");
#else
    printf("I'm just regular\n");
#endif
```

Now if `EXTRA_HAPPY` is not defined, it'll hit the `#else` clause and
print:

```
I'm just regular
```

### General Conditional: `#if`, `#elif`

This works very much like the `#ifdef` and `#ifndef` directives in that
you can also have an `#else` and the whole thing wraps up with `#endif`.

The only difference is that the constant expression after the `#if` must
evaluate to true (non-zero) for the code in the `#if` to be compiled. So
instead of whether or not something is defined, we want an expression
that evaluates to true.

``` {.c .numberLines}
#include <stdio.h>

#define HAPPY_FACTOR 1

int main(void)
{

#if HAPPY_FACTOR == 0
    printf("I'm not happy!\n");
#elif HAPPY_FACTOR == 1
    printf("I'm just regular\n");
#else
    printf("I'm extra happy!\n");
#endif

    printf("OK!\n");

    return 0;
}
```

Again, for the unmatched `#if` clauses, the compiler won't even see
those lines. For the above code, after the preprocessor gets finished
with it, all the compiler sees is:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{

    printf("I'm just regular\n");

    printf("OK!\n");

    return 0;
}
```

One hackish thing this is used for is to comment out large numbers of
lines quickly^[You can't always just wrap the code in `/*` `*/` comments
because those won't nest.].

If you put an `#if 0` ("if false") at the front of the block to be
commented out and an `#endif` at the end, you can get this effect:

``` {.c}
#if 0
    printf("All this code"); /* is effectively */
    printf("commented out"); // by the #if 0
#endif
```

You might have noticed that there's no `#elifdef` or `#elifndef`
directives. How can we get the same effect with `#if`? That is, what if
I wanted this:

``` {.c}
#ifdef FOO
    x = 2;
#elifdef BAR  // ERROR: Not supported by standard C
    x = 3;
#endif
```

How could I do it?

Turns out there's a preprocessor operator called `defined` that we can
use with an `#if` statement.

These are equivalent:

``` {.c}
#ifdef FOO
#if defined FOO
#if defined(FOO)   // Parentheses optional
```

As are these:

``` {.c}
#ifndef FOO
#if !defined FOO
#if !defined(FOO)   // Parentheses optional
```

Notice how we can use the standard logical NOT operator (`!`) for "not
defined".

So now we're back in `#if` land and we can use `#elif` with impunity!

This broken code:

``` {.c}
#ifdef FOO
    x = 2;
#elifdef BAR  // ERROR: Not supported by standard C
    x = 3;
#endif
```

can be replaced with:

``` {.c}
#if defined FOO
    x = 2;
#elif defined BAR
    x = 3;
#endif
```

### Losing a Macro: `#undef`

If you've defined something but you don't need it any longer, you can
undefine it with `#undef`.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
#define GOATS

#ifdef GOATS
    printf("Goats detected!\n");  // prints
#endif

#undef GOATS  // Make GOATS no longer defined

#ifdef GOATS
    printf("Goats detected, again!\n"); // doesn't print
#endif

    return 0;
}
```

## Built-in Macros

The standard defines a lot of built-in macros that you can test and use
for conditional compilation. Let's look at those here.


### Mandatory Macros

These are all defined:

|Macro|Description|
|-|-|
|`__DATE__`|The date of compilation---like when you're compiling this file---in `Mmm dd yyyy` format|
|`__TIME__`|The time of compilation in `hh:mm:ss` format|
|`__FILE__`|A string containing this file's name|
|`__LINE__`|The line number of the file this macro appears on|
|`__func__`|The name of the function this appears in, as a string^[This isn't really a macro---it's technically an identifier. But it's the only predefined identifier and it feels very macro-like, so I'm including it here. Like a rebel.]|
|`__STDC__`|Defined with `1` if this is a standard C compiler|
|`__STDC_HOSTED__`|This will be `1` if the compiler is a _hosted implementation_^[A hosted implementation basically means you're running the full C standard, probably on an operating system of some kind. Which you probably are. If you're running on bare metal in some kind of embedded system, you're probably on a _standalone implementation_.], otherwise `0`|
|`__STDC_VERSION__`|This version of C, a constant `long int` in the form `yyyymmL`, e.g. `201710L`|

Let's put these together.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    printf("This function: %s\n", __func__);
    printf("This file: %s\n", __FILE__);
    printf("This line: %d\n", __LINE__);
    printf("Compiled on: %s %s\n", __DATE__, __TIME__);
    printf("C Version: %ld\n", __STDC_VERSION__);

    return 0;
}
```

The output on my system is:

```
This function: main
This file: foo.c
This line: 7
Compiled on: Nov 23 2020 17:16:27
C Version: 201710
```

`__FILE__`, `__func__` and `__LINE__` are particularly useful to report
error conditions in messages to developers. The `assert()` macro in
`<assert.h>` uses these to call out where in the code the assertion
failed.

### Optional Macros

Your implementation might define these, as well. Or it might not.

|Macro|Description|
|-|-|
|`__STDC_ISO_10646__`|If defined, `wchar_t` holds Unicode values, otherwise something else|
|`__STDC_MB_MIGHT_NEQ_WC__`|A `1` indicates that the values in multibyte characters might not map equally to values in wide characters|
|`__STDC_UTF_16__`|A `1` indicates that the system uses UTF-16 encoding in type `char16_t`|
|`__STDC_UTF_16__`|A `1` indicates that the system uses UTF-16 encoding in type `char16_t`|
|`__STDC_ANALYZABLE__`|A `1` indicates the code is analyzable^[OK, I know that was a cop-out answer. Basically there's an optional extension compilers can implement wherein they agree to limit certain types of undefined behavior so that the C code is more amenable to static code analysis. It is unlikely you'll need to use this.]|
|`__STDC_IEC_559__`|`1` if IEEE-754 (aka IEC 60559) floating point is supported|
|`__STDC_IEC_559_COMPLEX__`|`1` if IEC 60559 complex floating point is supported|
|`__STDC_LIB_EXT1__`|`1` if this implementation supports a variety of "safe" alternate standard library functions (they have `_s` suffixes on the name)|
|`__STDC_NO_ATOMICS__`|`1` if this implementation does **not** support `_Atomic` or `<stdatomic.h>`|
|`__STDC_NO_COMPLEX__`|`1` if this implementation does **not** support complex types or `<complex.h>`|
|`__STDC_NO_THREADS__`|`1` if this implementation does **not** support `<threads.h>`|
|`__STDC_NO_VLA__`|`1` if this implementation does **not** support variable-length arrays|


## The Null Directive

A `#` on a line by itself is ignored by the preprocessor. Now, to be
entirely honest, I don't know what the use case is for this.

I've seen examples like this:

``` {.c}
#ifdef FOO
    #
#else
    printf("Something");
#endif
```

which is just cosmetic; the line with the solitary `#` can be deleted
with no ill effect.

Or maybe for cosmetic consistency, like this:

``` {.c}
#
#ifdef FOO
    x = 2;
#endif
#
#if BAR == 17
    x = 12;
#endif
#
```

But, with respect to cosmetics, that's just ugly.

Another post mentions elimination of comments---that in GCC, a comment
after a `#` will not be seen by the compiler. Which I don't doubt, but
the specification doesn't seem to say this is standard behavior.

My searches for rationale aren't bearing much fruit. So I'm going to
just say this is some good ol' fashioned C esoterica.

<!--
TODO

https://stackoverflow.com/questions/1674032/static-const-vs-define-vs-enum

#define

##
#
Multiline
variadic
built-ins
#line
#error
#pragma
-->