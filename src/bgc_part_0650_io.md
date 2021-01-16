<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# File Input/Output

We've already seen a couple examples of I/O with `scanf()` and
`printf()` for doing I/O at the console (screen/keyboard).

But we'll push those concepts a little farther this chapter.

## The `FILE*` Data Type

When we do any kind of I/O in C, we do so though a piece of data that
you get in the form of a `FILE*` type. This `FILE*` holds all the
information needed to communicate with the I/O subsystem about which
file you have open, where you are in the file, and so on.

The spec refers to these as _streams_, i.e. a stream of data from a file
or from any source. I'm going to use "files" and "streams"
interchangeably, but really you should think of a "file" as a special
case of a "stream". There are other ways to stream data into a program
than just reading from a file.

We'll see in a moment how to go from having a filename to getting an
open `FILE*` for it, but first I want to mention three streams that are
already open for you and ready for use.

|`FILE*` name|Description|
|-|-|
|`stdin`|Standard Input, generally the keyboard by default|
|`stdout`|Standard Output, generally the screen by default|
|`stderr`|Standard Error, generally the screen by default, as well|

We've actually been using these implicitly already, it turns out. For
example, these two calls are the same:

``` {.c}
printf("Hello, world!\n");
fprintf(stdout, "Hello, world!\n");  // printf to a file
```

But more on that later.

Also you'll notice that both `stdout` and `stderr` go to the screen.
While this seems at first either like an oversight or redundancy, it
actually isn't. Typical operating systems allow you to _redirect_ the
output of either of those into different files, and it can be convenient
to be able to separate error messages from regular non-error output.

For example, in a POSIX shell (like sh, ksh, bash, zsh, etc.) on a
Unix-like system, we could run a program and send just the non-error
(`stdout`) output to one file, and all the error (`stderr`) output to
another file.

```
$ ./foo > output.txt 2> errors.txt   # This command is Unix-specific
```

For this reason, you should send serious error messages to `stderr`
instead of `stdout`.

More on how to do that later.

## Reading Text Files

Streams are largely categorized two different ways: _text_ and _binary_.

Text streams are allowed to do significant translation of the data, most
notably translations of newlines to their different representations^[We
used to have three different newlines in broad effect: Carriage Return
(CR, used on old Macs), Linefeed (LF, used on Unix systems), and
Carriage Return/Linefeed (CRLF, used on Windows systems). Thankfully the
introduction of OSX, being Unix-based, reduced this number to two.].
Text files are logically a sequence of _lines_ separated by newlines. To
be portable, your input data should always end with a newline.

But the general rule is that if you're able to edit the file in a
regular text editor, it's a text file. Otherwise, it's binary. More on
binary later.

So let's get to work---how do we open a file for reading, and pull data
out of it?

Let's have a file called `hello.txt` that has just this in it:

```
Hello, world!
```

And let's write a program to open the file, read a character out of it,
and then close the file when we're done. That's the gameplan!

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;                      // Variable to represent open file

    fp = fopen("hello.txt", "r");  // Open file for reading

    char c = fgetc(fp);            // Read a single character
    printf("%c\n", c);             // Print char to stdout

    fclose(fp);                    // Close the file when done
}
```

See how when we opened the file with `fopen()`, it returned the `FILE*`
to us so we could use it later.

Also notice the `"r"` that we passed in---this means "open a text stream
for reading". (There are various strings we can pass to `fopen()` with
additional meaning, like writing, or appending, and so on.)

After that, we used the `fgetc()` function to get a character from the
stream.

Finally, we close the stream when we're done with it. All streams are
automatically closed when the program exits, but it's good form and good
housekeeping to explicitly close any files yourself when done with them.

The `FILE*` keeps track of our position in the file. So subsequent calls
to `fgetc()` would get the next character in the file, and then the
next, until the end.

But that sounds like a pain. Let's see if we can make it easier.

## End of File: `EOF`

There is a special character defined as a macro: `EOF`. This is what
`fgetc()` will return when the end of the file has been reached and
you've attempted to read another character.

We can use this to read the whole file in a loop.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;

    fp = fopen("hello.txt", "r");
    char c;

    while ((c = fgetc(fp)) != EOF)
        printf("%c", c);

    fclose(fp);
}
```

(If line 10 is too weird, just break it down starting with the
most-nested parens. The first thing we do is assign the result of
`fgets()` into `c`, and _then_ we compare that against `EOF`. We've just
crammed it into a single line. This might look hard to read, but study
it---it's idiomatic C.)

And running this, we see:

```
Hello, world!
```

But still, we're operating a character at a time, and lots of text files
make more sense at the line level. Let's switch to that.

### Reading a Line at a Time

So how can we get 

## Writing Text Files

## Binary File I/O

