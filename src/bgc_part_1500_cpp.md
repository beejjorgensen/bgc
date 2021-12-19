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

A _macro_ is an identifier that gets _expanded_ to another piece of code
before the compiler even sees it. Think of it like a placeholder---when
the preprocessor sees one of those identifiers, it replaces it with
another value that you've defined.

We do this with `#define` (often read "pound define"). Here's an
example:

``` {.c .numberLines}
#include <stdio.h>

#define HELLO "Hello, world"
#define PI 3.14159

int main(void)
{
    printf("%s, %f\n", HELLO, PI);
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
}
```

See how `HELLO` was replaced with `"Hello, world"` and `PI` was replaced
with `3.14159`? From the compiler's perspective, it's just like those
values had appeared right there in the code.

Note that the macros don't have a specific type, _per se_. Really all
that happens is they get replaced wholesale with whatever they're
`#define`d as. If the resulting C code is invalid, the compiler will
puke.

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
effectively global and can be used _any_ place. Even in those places
where a `const` variable won't work, e.g. in `switch` `case`s and fixed
array lengths.

That said, the debate rages online whether a typed `const` variable is
better than `#define` macro in the general case.

It can also be used to replace or modify keywords, a concept completely
foreign to `const`, though this practice should be used sparingly.

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
}
```

In that example, we define `EXTRA_HAPPY` (to be nothing, but it _is_
defined), then on line 8 we check to see if it is defined with an
`#ifdef` directive. If it is defined, the subsequent code will be
included up until the `#endif`.

So because it is defined, the code will be included for compilation and
the output will be:

``` {.default}
I'm extra happy!
OK!
```

If we were to comment out the `#define`, like so:

``` {.c}
//#define EXTRA_HAPPY
```

then it wouldn't be defined, and the code wouldn't be included in
compilation. And the output would just be:

``` {.default}
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

``` {.default}
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
}
```

The output on my system is:

``` {.default}
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

#### `__STDC_VERSION__`s

In case you're wondering, here are the version numbers for different
major releases of the C Language Spec:

|Release|ISO/IEC version|`__STDC_VERSION__`|
|-|-|-|
|C89|ISO/IEC 9899:1990|undefined|
|**C89**|ISO/IEC 9899:1990/Amd.1:1995|`199409L`|
|**C99**|ISO/IEC 9899:1999|`199901L`|
|**C11**|ISO/IEC 9899:2011/Amd.1:2012|`201112L`|

Note the macro did not exist originally in C89.

Also note that the plan is that the version numbers will strictly
increase, so you could always check for, say, "at least C99" with:

``` {.c}
#if __STDC_VERSION__ >= 1999901L
```

### Optional Macros

Your implementation might define these, as well. Or it might not.

|Macro|Description|
|-|-|
|`__STDC_ISO_10646__`|If defined, `wchar_t` holds Unicode values, otherwise something else|
|`__STDC_MB_MIGHT_NEQ_WC__`|A `1` indicates that the values in multibyte characters might not map equally to values in wide characters|
|`__STDC_UTF_16__`|A `1` indicates that the system uses UTF-16 encoding in type `char16_t`|
|`__STDC_UTF_32__`|A `1` indicates that the system uses UTF-32 encoding in type `char32_t`|
|`__STDC_ANALYZABLE__`|A `1` indicates the code is analyzable^[OK, I know that was a cop-out answer. Basically there's an optional extension compilers can implement wherein they agree to limit certain types of undefined behavior so that the C code is more amenable to static code analysis. It is unlikely you'll need to use this.]|
|`__STDC_IEC_559__`|`1` if IEEE-754 (aka IEC 60559) floating point is supported|
|`__STDC_IEC_559_COMPLEX__`|`1` if IEC 60559 complex floating point is supported|
|`__STDC_LIB_EXT1__`|`1` if this implementation supports a variety of "safe" alternate standard library functions (they have `_s` suffixes on the name)|
|`__STDC_NO_ATOMICS__`|`1` if this implementation does **not** support `_Atomic` or `<stdatomic.h>`|
|`__STDC_NO_COMPLEX__`|`1` if this implementation does **not** support complex types or `<complex.h>`|
|`__STDC_NO_THREADS__`|`1` if this implementation does **not** support `<threads.h>`|
|`__STDC_NO_VLA__`|`1` if this implementation does **not** support variable-length arrays|

## Macros with Arguments

Macros are more powerful than simple substitution, though. You can set
them up to take arguments that are substituted in, as well.

A question often arises for when to use parameterized macros versus
functions. Short answer: use functions. But you'll see lots of macros in
the wild and in the standard library. People tend to use them for short,
mathy things, and also for features that might change from platform to
platform. You can define different keywords for one platform or another.

### Macros with One Argument

Let's start with a simple one that squares a number:

``` {.c .numberLines}
#include <stdio.h>

#define SQR(x) x * x  // Not quite right, but bear with me

int main(void)
{
    printf("%d\n", SQR(12));  // 144
}
```

What that's saying is "everywhere you see `SQR` with some value, replace
it with that value times itself".

So line 7 will be changed to:

``` {.c .numberLines startFrom="7"}
    printf("%d\n", 12 * 12);  // 144
```

which C comfortably converts to 144.

But we've made an elementary error in that macro, one that we need to
avoid.

Let's check it out. What if we wanted to compute `SQR(3 + 4)`? Well,
$3+4=7$, so we must want to compute $7^2=49$. That's it; `49`---final
answer.

Let's drop it in our code and see that we get... 19?

``` {.c .numberLines startFrom="7"}
    printf("%d\n", SQR(3 + 4));  // 19!!??
```

What happened?

If we follow the macro expansion, we get 

``` {.c .numberLines startFrom="7"}
    printf("%d\n", 3 + 4 * 3 + 4);  // 19!
```

Oops! Since multiplication takes precedence, we do the $4\times3=12$
first, and get $3+12+4=19$. Not what we were after.

So we have to fix this to make it right.

**This is so common that you should automatically do it every time you
make a parameterized math macro!**

The fix is easy: just add some parentheses!

``` {.c .numberLines startFrom="3"}
#define SQR(x) (x) * (x)   // Better... but still not quite good enough!
```

And now our macro expands to:

``` {.c .numberLines startFrom="7"}
    printf("%d\n", (3 + 4) * (3 + 4));  // 49! Woo hoo!
```

But we actually still have the same problem which might manifest if we
have a higher-precedence operator than multiply (`*`) nearby.

So the safe, proper way to put the macro together is to wrap the whole
thing in additional parentheses, like so:

``` {.c .numberLines startFrom="3"}
#define SQR(x) ((x) * (x))   // Good!
```

Just make it a habit to do that when you make a math macro and you can't
go wrong.

### Macros with More than One Argument

You can stack these things up as much as you want:

``` {.c}
#define TRIANGLE_AREA(w, h) (0.5 * (w) * (h))
```

Let's do some macros that solve for $x$ using the quadratic formula.
Just in case you don't have it on the top of your head, it says for
equations of the form:

$ax^2+bx+c=0$

you can solve for $x$ with the quadratic formula:

$x=\cfrac{-b\pm\sqrt{b^2-4ac}}{2a}$

Which is crazy. Also notice the plus-or-minus ($\pm$) in there,
indicating that there are actually two solutions.

So let's make macros for both:

``` {.c}
#define QUADP(a, b, c) ((-(b) + sqrt((b) * (b) - 4 * (a) * (c))) / (2 * (a)))
#define QUADM(a, b, c) ((-(b) - sqrt((b) * (b) - 4 * (a) * (c))) / (2 * (a)))
```

So that gets us some math. But let's define one more that we can use as
arguments to `printf()` to print both answers.

``` {.c}
//          macro              replacement
//      |-----------| |----------------------------|
#define QUAD(a, b, c) QUADP(a, b, c), QUADM(a, b, c)
```

That's just a couple values separated by a comma---and we can use that
as a "combined" argument of sorts to `printf()` like this:

``` {.c}
printf("x = %f or x = %f\n", QUAD(2, 10, 5));
```

Let's put it together into some code:

``` {.c .numberLines}
#include <stdio.h>
#include <math.h>  // For sqrt()

#define QUADP(a, b, c) ((-(b) + sqrt((b) * (b) - 4 * (a) * (c))) / (2 * (a)))
#define QUADM(a, b, c) ((-(b) - sqrt((b) * (b) - 4 * (a) * (c))) / (2 * (a)))
#define QUAD(a, b, c) QUADP(a, b, c), QUADM(a, b, c)

int main(void)
{
    printf("2*x^2 + 10*x + 5 = 0\n");
    printf("x = %f or x = %f\n", QUAD(2, 10, 5));
}
```

And this gives us the output:

``` {.default}
2*x^2 + 10*x + 5 = 0
x = -0.563508 or x = -4.436492
```

Plugging in either of those values gives us roughly zero (a bit off
because the numbers aren't exact):

$2\times-0.563508^2+10\times-0.563508+5\approx0.000003$

### Macros with Variable Arguments

There's also a way to have a variable number of arguments passed to a
macro, using ellipses (`...`) after the known, named arguments. When the
macro is expanded, all of the extra arguments will be in a
comma-separated list in the `__VA_ARGS__` macro, and can be replaced
from there:

``` {.c .numberLines}
#include <stdio.h>

// Combine the first two arguments to a single number,
// then have a commalist of the rest of them:

#define X(a, b, ...) (10*(a) + 20*(b)), __VA_ARGS__

int main(void)
{
    printf("%d %f %s %d\n", X(5, 4, 3.14, "Hi!", 12));
}
```

The substitution that takes place on line 10 would be:

``` {.c .numberLines startFrom="10"}
    printf("%d %f %s %d\n", (10*(5) + 20*(4)), 3.14, "Hi!", 12);
```

for output:

``` {.default}
130 3.140000 Hi! 12
```

You can also "stringify" `__VA_ARGS__` by putting a `#` in front of it:

``` {.c}
#define X(...) #__VA_ARGS__

printf("%s\n", X(1,2,3));  // Prints "1, 2, 3"
```

### Stringification

Already mentioned, just above, you can turn any argument into a string
by preceding it with a `#` in the replacement text.

For example, we could print anything as a string with this macro and
`printf()`:

``` {.c}
#define STR(x) #x

printf("%s\n", STR(3.14159));
```

In that case, the substitution leads to:

``` {.c}
printf("%s\n", "3.14159");
```

Let's see if we can use this to greater effect so that we can pass any
`int` variable name into a macro, and have it print out it's name and
value.

``` {.c .numberLines}
#include <stdio.h>

#define PRINT_INT_VAL(x) printf("%s = %d\n", #x, x)

int main(void)
{
    int a = 5;

    PRINT_INT_VAL(a);  // prints "a = 5"
}
```

On line 9, we get the following macro replacement:

``` {.c .numberLines startFrom="9"}
    printf("%s = %d\n", "a", 5);
```

### Concatenation

We can concatenate two arguments together with `##`, as well. Fun times!

``` {.c}
#define CAT(a, b) a ## b

printf("%f\n", CAT(3.14, 1592));   // 3.141592
```

## Multiline Macros

It's possible to continue a macro to multiple lines if you escape the
newline with a backslash (`\`).

Let's write a multiline macro that prints numbers from `0` to the
product of the two arguments passed in.

``` {.c .numberLines}
#include <stdio.h>

#define PRINT_NUMS_TO_PRODUCT(a, b) do { \
    int product = (a) * (b); \
    for (int i = 0; i < product; i++) { \
        printf("%d\n", i); \
    } \
} while(0)

int main(void)
{
    PRINT_NUMS_TO_PRODUCT(2, 4);  // Outputs numbers from 0 to 7
}
```

A couple things to note there:

* Escapes at the end of every line except the last one to indicate that
  the macro continues.
* The whole thing is wrapped in a `do`-`while(0)` loop with squirrley
  braces.

The latter point might be a little weird, but it's all about absorbing
the trailing `;` the coder drops after the macro.

At first I thought that just using squirrely braces would be enough, but
there's a case where it fails if the coder puts a semicolon after the
macro. Here's that case:

``` {.c .numberLines}
#include <stdio.h>

#define FOO(x) { (x)++; }

int main(void)
{
    int i = 0;

    if (i == 0)
        FOO(i);
    else
        printf(":-(\n");

    printf("%d\n", i);
}
```

Looks simple enough, but it won't build without a syntax error:

``` {.default}
foo.c:11:5: error: ‘else’ without a previous ‘if’  
```

Do you see it?

Let's look at the expansion:

``` {.c}

    if (i == 0) {
        (i)++;
    };             // <-- Trouble with a capital-T!

    else
        printf(":-(\n");
```

The `;` puts an end to the `if` statement, so the `else` is just
floating out there illegally^[_Breakin' the law... breakin' the
law..._].

So wrap that multiline macro with a `do`-`while(0)`.

## Example: An Assert Macro {#my-assert}

Adding asserts to your code is a good way to catch conditions that you
think shouldn't happen. C provides `assert()` functionality. It checks a
condition, and if it's false, the program bombs out telling you the
file and line number on which the assertion failed.

But this is wanting.

1. First of all, you can't specify an additional message with the
   assert.
2. Secondly, there's no easy on-off switch for all the asserts.

We can address the first with macros.

Basically, when I have this code:

``` {.c}
ASSERT(x < 20, "x must be under 20");
```

I want something like this to happen (assuming the `ASSERT()` is on line
220 of `foo.c`):

``` {.c}
if (!(x < 20)) {
    fprintf(stderr, "foo.c:220: assertion x < 20 failed: ");
    fprintf(stderr, "x must be under 20\n");
    exit(1);
}
```

We can get the filename out of the `__FILE__` macro, and the line number
from `__LINE__`. The message is already a string, but `x < 20` is not,
so we'll have to stringify it with `#`. We can make a multiline macro by
using backslash escapes at the end of the line.

``` {.c}
#define ASSERT(c, m) \
do { \
    if (!(c)) { \
        fprintf(stderr, __FILE__ ":%d: assertion %s failed: %s\n", \
                        __LINE__, #c, m); \
        exit(1); \
    } \
} while(0)
```

(It looks a little weird with `__FILE__` out front like that, but
remember it is a string literal, and string literals next to each other
are automagically concatenated. `__LINE__` on the other hand, it's just
an `int`.)

And that works! If I run this:

``` {.c}
int x = 30;

ASSERT(x < 20, "x must be under 20");
```

I get this output:

```
foo.c:23: assertion x < 20 failed: x must be under 20
```

Very nice!

The only thing left is a way to turn it on and off, and we could do that
with conditional compilation.

Here's the complete example:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

#define ASSERT_ENABLED 1

#if ASSERT_ENABLED
#define ASSERT(c, m) \
do { \
    if (!(c)) { \
        fprintf(stderr, __FILE__ ":%d: assertion %s failed: %s\n", \
                        __LINE__, #c, m); \
        exit(1); \
    } \
} while(0)
#else
#define ASSERT(c, m)  // Empty macro if not enabled
#endif

int main(void)
{
    int x = 30;

    ASSERT(x < 20, "x must be under 20");
}
```

This has the output:

``` {.default}
foo.c:23: assertion x < 20 failed: x must be under 20
```

## The `#error` Directive

This directive causes the compiler to error out as soon as it sees it.

Commonly, this is used inside a conditional to prevent compilation
unless some prerequisites are met:

``` {.c}
#ifndef __STDC_IEC_559__
    #error I really need IEEE-754 floating point to compile. Sorry!
#endif
```

Some compilers have a non-standard complementary `#warning` directive
that will output a warning but not stop compilation, but this is not in
the C11 spec.

## The `#pragma` Directive {#pragma}

This is one funky directive, short for "pragmatic". You can use it to
do... well, anything your compiler supports you doing with it.

Basically the only time you're going to add this to your code is if some
documentation tells you to do so.

### Non-Standard Pragmas

Here's one non-standard example of using `#pragma` to cause the compiler
to execute a `for` loop in parallel with multiple threads (if the
compiler supports the [fl[OpenMP|https://www.openmp.org/]] extension):

``` {.c}
#pragma omp parallel for
for (int i = 0; i < 10; i++) { ... }
```

There are all kinds of `#pragma` directives documented across all four
corners of the globe.

All unrecognized `#pragma`s are ignored by the compiler.

### Standard Pragmas

There are also a few standard ones, and these start with `STDC`, and
follow the same form:

``` {.c}
#pragma STDC pragma_name on-off
```

The `on-off` portion can be either `ON`, `OFF`, or `DEFAULT`.

And the `pragma_name` can be one of these:

|Pragma Name|Description|
|-|-|
|`FP_CONTRACT`|Allow floating point expressions to be contracted into a single operation to avoid rounding errors that might occur from multiple operations.|
|`FENV_ACCESS`|Set to `ON` if you plan to access the floating point status flags. If `OFF`, the compiler might perform optimizations that cause the values in the flags to be inconsistent or invalid.|
|`CX_LIMITED_RANGE`|Set to `ON` to allow the compiler to skip overflow checks when performing complex arithmetic. Defaults to `OFF`.|

For example:

``` {.c}
#pragma STDC FP_CONTRACT OFF
#pragma STDC CX_LIMITED_RANGE ON
```

As for `CX_LIMITED_RANGE`, the spec points out:

> The purpose of the pragma is to allow the implementation to use the
> formulas:
>
> $(x+iy)\times(u+iv) = (xu-yv)+i(yu+xv)$
>
> $(x+iy)/(u+iv) = [(xu+yv)+i(yu-xv)]/(u^2+v^2)$
>
> $|x+iy|=\sqrt{x^2+y^2}$
>
> where the programmer can determine they are safe.

### `_Pragma` Operator

This is another way to declare a pragma that you could use in a macro.

These are equivalent:

``` {.c}
#pragma "Unnecessary" quotes
_Pragma("\"Unnecessary\" quotes")
```

This can be used in a macro, if need be:

``` {.c}
#define PRAGMA(x) _Pragma(#x)
```

## The `#line` Directive

This allows you to override the values for `__LINE__` and `__FILE__`. If
you want.

I've never wanted to do this, but in K&R2, they write:

> For the benefit of other preprocessors that generate C programs [...]

So maybe there's that.

To override the line number to, say 300:

``` {.c}
#line 300
```

and `__LINE__` will keep counting up from there.

To override the line number and the filename:

``` {.c}
#line 300 "newfilename"
```

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
