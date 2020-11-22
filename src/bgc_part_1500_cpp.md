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


<!--
TODO

#ifdef, ifndef
#if

defined
!defined

#define
#undef

##
#
Multiline
null directive
variadic
built-ins
#line
#error
#pragma
-->