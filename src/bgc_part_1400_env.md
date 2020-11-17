<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# The Outside Environment

When you run a program, it's actually you talking to the shell, saying,
"Hey, please run this thing." And the shell says, "Sure," and then tells
the operating system, "Hey, could you please make a new process and run
this thing?" And if all goes well, the OS complies and your program
runs.

But there's a whole world outside your program in the shell that can be
interacted with from within C. We'll look at a few of those in this
chapter.

## Command Line Arguments

Many command line utilities accept _command line arguments_. For
example, if we want to see all files that end in `.txt`, we can type
something like this on a Unix-like system:

```
ls *.txt
```

(or `dir` instead of `ls` on a Windows system).

In this case, the command is `ls`, but it arguments are all all files
that end with `.txt`^[Historially, MS-DOS and Windows programs would do
this differently than Unix. In Unix, the shell would _expand_ the
wildcard into all matching files before your program saw it, whereas the
Microsoft variants would pass the wildcard expression into the program
to deal with. In any case, there are arguments that get passed into the
program.].

So how can we see what is passed into program from the command line?

Say we have a program called `add` that adds all numbers passed on the
command line and prints the result:

```
./add 10 30 5
45
```

That's gonna pay the bills for sure!

But seriously, this is a great tool for seeing how to get those
arguments from the command line and break them down.

First, let's see how to get them at all. For this, we're going to need a
new `main()`!

Here's a program that prints out all the command line arguments. For
example, if we name the executable `foo`, we can run it like this:

```
./foo i like turtles
```

and we'll see this output:

```
arg 0: ./foo
arg 1: i
arg 2: like
arg 3: turtles
```

It's a little weird, because the zeroth argument is the name of the
executable, itself. But that's just something to get used to. The
arguments themselves follow directly.

Source:

``` {.c .numberLines}
#include <stdio.h>

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++) {
        printf("arg %d: %s\n", i, argv[i]);
    }

    return 0;
}
```

Whoa! What's going on with the `main()` function signature? What's
`argc` and `argv`^[Since they're just regular parameter names, you don't
actually have to call them `argc` and `argv`. But it's so very idiomatic
to use those names, if you get creative, other C programmers will look
at you with a suspicious eye, indeed!] (pronounced _arg-c_ and _arg-v_)?

Let's start with the easy one first: `argc`. This is the _argument count_,
including the program name, itself. If you think of all the arguments as
an array of strings, which is exactly what they are, then you can think of
`argc` as the length of that array, which is exactly what it is.


argv[argc] == NULL



## Exit Status

## Environment Variables