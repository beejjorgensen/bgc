<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Hello, World!

## What to Expect from C

> _"Where do these stairs go?"_ \
> _"They go up."_
>
> ---Ray Stantz and Peter Venkman, Ghostbusters

C is a low-level language.

It didn't use to be. Back in the day when people carved punch cards out
of granite, C was an incredible way to be free of the drudgery of
lower-level languages like [flw[assembly|Assembly_language]].

But now in these modern times, current-generation languages offer all
kinds of features that didn't exist in 1972 when C was invented. This
means C is a pretty basic language with not a lot of features. It can do
_anything_, but it can make you work for it.

So why would we even use it today?

* As a learning tool: not only is C a venerable piece of computing
  history, but it is connected to the [flw[bare metal|Bare_machine]] in
  a way that present-day languages are not. When you learn C, you learn
  about how software interfaces with computer memory at a low level.
  There are no seatbelts. You'll write software that crashes, I assure
  you. And that's all part of the fun!

* As a useful tool: C still is used for certain applications, such as
  building [flw[operating systems|Operating_system]] or in [flw[embedded
  systems|Embedded_system]]. (Though the
  [flw[Rust|Rust_(programming_language)]] programming language is eyeing
  both these fields!)

If you're familiar with another language, a lot of things about C are
easy. C inspired many other languages, and you'll see bits of it in Go,
Rust, Swift, Python, JavaScript, Java, and all kinds of other languages.
Those parts will be familiar.

The one thing about C that hangs people up is _pointers_. Virtually
everything else is familiar, but pointers are the weird one. The concept
behind pointers is likely one you already know, but C forces you to be
explicit about it, using operators you've likely never seen before.

It's especially insidious because once you [flw[_grok_|Grok]] pointers,
they're suddenly easy. But up until that moment, they're slippery eels.

Everything else in C is just memorizing another way (or sometimes the
same way!) of doing something you've done already. Pointers are the
weird bit. And, arguably, even pointers are variations on a theme you're
probably familiar with.

So get ready for a rollicking adventure as close to the core of the
computer as you can get without assembly, in the most influential
computer language of all time^[I know someone will fight me on that,
but it's gotta be at least in the top three, right?]. Hang on!


## Hello, World!

[i[Hello, world]()]This is the canonical example of a C program.
Everyone uses it. (Note that the numbers to the left are for reader
reference only, and are not part of the source code.)

``` {.c .numberLines}
/* Hello world program */

#include <stdio.h>

int main(void)
{
    printf("Hello, World!\n");  // Actually do the work here
}
```

We're going to don our long-sleeved heavy-duty rubber gloves, grab a
scalpel, and rip into this thing to see what makes it tick. So, scrub
up, because here we go. Cutting very gently...

[i[Comments]<]Let's get the easy thing out of the way: anything between
the digraphs `/*` and `*/` is a comment and will be completely ignored
by the compiler. Same goes for anything on a line after a `//`. This
allows you to leave messages to yourself and others, so that when you
come back and read your code in the distant future, you'll know what the
heck it was you were trying to do. Believe me, you will forget; it
happens.[i[Comments]>]

[i[C Preprocessor]<][i[`#include` directive]<]Now, what is this
`#include`? GROSS! Well, it tells the C Preprocessor to pull the
contents of another file and insert it into the code right _there_.

Wait---what's a C Preprocessor? Good question. There are two
stages^[Well, technically there are more than two, but hey, let's
pretend there are two---ignorance is bliss, right?] to compilation: the
preprocessor and the compiler. [i[Octothorpe]<]Anything that starts with
pound sign, or "octothorpe", (`#`) is something the
preprocessor[i[Preprocessor]] operates on before the compiler even gets
started. Common _preprocessor directives_, as they're called, are
`#include` and `#define`.[i[`#define` directive]] More on that
later.[i[`#include` directive]>][i[C Preprocessor]>]

Before we go on, why would I even begin to bother pointing out that a
pound sign is called an octothorpe? The answer is simple: I think the
word octothorpe is so excellently funny, I have to gratuitously spread
its name around whenever I get the opportunity.  Octothorpe. Octothorpe,
octothorpe, octothorpe.[i[Octothorpe]>]

So _anyway_. After the C preprocessor has finished preprocessing
everything, the results are ready for the compiler to take them and
produce [flw[assembly code|Assembly_language]], [flw[machine
code|Machine_code]], or whatever it's about to do. Machine code is the
"language" the CPU understands, and it can understand it _very rapidly_.
This is one of the reasons C programs tend to be quick.

Don't worry about the technical details of compilation for now; just
know that your source runs through the preprocessor, then the output of
that runs through the compiler, then that produces an executable for you
to run.

What about the rest of the line? [i[`stdio.h` header file]<]What's
`<stdio.h>`? That is what is known as a _header file_. It's the dot-h at
the end that gives it away.  In fact it's the "Standard I/O" (`stdio`)
header file that you will grow to know and love. It gives us access to a
bunch of I/O functionality^[Technically, it contains preprocessor
directives and function prototypes (more on that later) for common input
and output needs.]. For our demo program, we're outputting the string
"Hello, World!", so we in particular need access to the
`printf()`[i[printf()]T] function to do this. The `<stdio.h>` file gives
us this access. Basically, if we tried to use [i[`printf()` function]<]
without `#include <stdio.h>`, the compiler would have complained to us
about it.

How did I know I needed to `#include <stdio.h>` for
`printf()`?[i[`printf()` function]>] Answer: it's in the documentation.
If you're on a Unix system, `man 3 printf` and it'll tell you right at
the top of the man page what header files are required. Or see the
reference section in this book. `:-)` [i[`stdio.h` header file]>]

Holy moly. That was all to cover the first line! But, let's face it, it
has been completely dissected. No mystery shall remain!

So take a breather...look back over the sample code. Only a couple easy
lines to go.

Welcome back from your break! I know you didn't really take a break; I
was just humoring you.

[i[`main()` function]<]The next line is `main()`. This is the definition
of the function `main()`; everything between the squirrelly braces (`{`
and `}`) is part of the function definition.

(How do you _call_ a different function, anyway? The answer lies in the
`printf()` line, but we'll get to that in a minute.)

Now, the main function is a special one in many ways, but one way stands
above the rest: it is the function that will be called automatically
when your program starts executing. Nothing of yours gets called before
`main()`. In the case of our example, this works fine since
all we want to do is print a line and exit.

Oh, that's another thing: once the program executes past the end of
`main()`, down there at the closing squirrelly brace, the
program will exit, and you'll be back at your command prompt.

So now we know that that program has brought in a header file,
`stdio.h`[i[stdio.h]T], and declared a `main()` function that will
execute when the program is started. What are the goodies in
`main()`[i[`main()` function]>]?

I am so happy you asked. Really! We only have the one goodie: a call to
the function [i[`printf()` function]<]`printf()`. You can tell this is a
function call and not a function definition in a number of ways, but one
indicator is the lack of squirrelly braces after it. And you end the
function call with a semicolon so the compiler knows it's the end of the
expression. You'll be putting semicolons after almost everything, as
you'll see.

You're passing one argument to the function `printf()`[i[`printf()`
function]>]: a string to be printed when you call it. Oh, yeah---we're
calling a function! We rock!  Wait, wait---don't get cocky. [i[`\n`
newline]<]What's that crazy `\n` at the end of the string? Well, most
characters in the string will print out just like they are stored. But
there are certain characters that you can't print on screen well that
are embedded as two-character backslash codes. One of the most popular
is `\n` (read "backslash-N") that corresponds to the _newline_
character. This is the character that causes further printing to
continue at the beginning of the next line instead of the current.  It's
like hitting return at the end of the line.[i[`\n` newline]>]

So copy that code into a file called `hello.c` and build it. On a
Unix-like platform (e.g. Linux, BSD, Mac, or WSL), from the command line
you'll build with a command like so:

[i[`gcc` compiler]]
``` {.zsh}
gcc -o hello hello.c
```

(This means "compile `hello.c`, and output an executable called
`hello`".)

After that's done, you should have a file called `hello` that you can
run with this command:

``` {.default}
./hello
```

(The leading `./` tells the shell to "run from the current directory".)

And see what happens:

``` {.default}
Hello, World! 
```

It's done and tested! Ship it![i[Hello, world]>]

## Compilation Details

[i[Compilation]<]Let's talk a bit more about how to build C programs,
and what happens behind the scenes there.

Like other languages, C has _source code_. But, depending on what
language you're coming from, you might never have had to _compile_ your
source code into an _executable_.

Compilation is the process of taking your C source code and turning it
into a program that your operating system can execute.

JavaScript and Python devs aren't used to a separate compilation step at
all--though behind the scenes it's happening! Python compiles your
source code into something called _bytecode_ that the Python virtual
machine can execute. Java devs are used to compilation, but that
produces bytecode for the Java Virtual Machine.

When compiling C, _machine code_ is generated. This is the 1s and 0s
that can be executed directly and speedily by the CPU.

> Languages that typically aren't compiled are called _interpreted_
> languages. But as we mentioned with Java and Python, they also have a
> compilation step. And there's no rule saying that C can't be
> interpreted. (There are C interpreters out there!) In short, it's a
> bunch of gray areas. Compilation in general is just taking source code
> and turning it into another, more easily-executed form.

The C compiler is the program that does the compilation.

As we've already said, `gcc` is a compiler that's installed on a lot of
[flw[Unix-like operating systems|Unix]]. And it's commonly run from the
command line in a terminal, but not always. You can run it from your
IDE, as well.

So how do we do command line builds?

## Building with `gcc`

[i[`gcc` compiler]<]If you have a source file called `hello.c` in the current
directory, you can build that into a program called `hello` with this
command typed in a terminal:

``` {.zsh}
gcc -o hello hello.c
```

The `-o` means "output to this file"^[If you don't give it an output
filename, it will export to a file called `a.out` by default---this
filename has its roots deep in Unix history.]. And there's `hello.c` at
the end, the name of the file we want to compile.

If your source is broken up into multiple files, you can compile them
all together (almost as if they were one file, but the rules are
actually more complex than that) by putting all the `.c` files on the
command line:

``` {.zsh}
gcc -o awesomegame ui.c characters.c npc.c items.c
```
[i[`gcc` compiler]>]

and they'll all get built together into a big executable.

That's enough to get started---later we'll talk details about multiple
source files, object files, and all kinds of fun stuff.[i[Compilation]>]

## Building with `clang`

On Macs, the stock compiler isn't `gcc`---it's `clang`[i[`clang`
compiler]]. But a wrapper is also installed so you can run `gcc` and
have it still work.

You can also install the `gcc`[i[`gcc` compiler]] compiler proper
through [fl[Homebrew|https://formulae.brew.sh/formula/gcc]] or some
other means.

## Building from IDEs

[i[Integrated Development Environment]<]If you're using an _Integrated
Development Environment_ (IDE), you probably don't have to build from
the command line. 

With Visual Studio, `CTRL-F7` will build, and `CTRL-F5` will run.

With VS Code, you can hit `F5` to run via the debugger. (You'll have to
install the C/C++ Extension.)

With XCode, you can build with `COMMAND-B` and run with `COMMAND-R`. To
get the command line tools, Google for "XCode command line tools" and
you'll find instructions for installing them.

For getting started, I encourage you to also try to build from the
command line---it's history![i[Integrated Development Environment]>]

## C Versions

[i[Language versions]<]C has come a long way over the years, and it had
many named version numbers to describe which dialect of
the language you're using.

These generally refer to the year of the specification.

The most famous are C89, C99, C11, and C2x. We'll focus on the latter in
this book.

But here's a more complete table:

|Version|Description|
|-----|--------------|
|K&R C|1978, the original. Named after Brian Kernighan and Dennis Ritchie. Ritchie designed and coded the language, and Kernighan co-authored the book on it. You rarely see original K&R code today. If you do, it'll look odd, like Middle English looks odd to modern English readers.|
|**C89**, ANSI C, C90|In 1989, the American National Standards Institute (ANSI) produced a C language specification that set the tone for C that persists to this day. A year later, the reins were handed to the International Organization for Standardization (ISO) that produced the identical C90.|
|C95|A rarely-mentioned addition to C89 that included wide character support.|
|**C99**|The first big overhaul with lots of language additions. The thing most people remember is the addition of `//`-style comments. This is the most popular version of C in use as of this writing.|
|**C11**|This major version update includes Unicode support and multi-threading. Be advised that if you start using these language features, you might be sacrificing portability with places that are stuck in C99 land. But, honestly, 1999 is getting to be a while back now.|
|C17, C18|Bugfix update to C11. C17 seems to be the official name, but the publication was delayed until 2018. As far as I can tell, these two are interchangeable, with C17 being preferred.|
|C2x|What's coming next! Expected to eventually become C21.|

[i[`gcc` compiler]<]You can force GCC to use one of these standards with the
`-std=` command line argument. If you want it to be picky about the
standard, add `-pedantic`.

For example:

``` {.zsh}
gcc -std=c11 -pedantic foo.c
```

For this book, I compile programs for C2x with all warnings set:

``` {.zsh}
gcc -Wall -Wextra -std=c2x -pedantic foo.c
```
[i[`gcc` compiler]>]
