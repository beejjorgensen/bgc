<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# File Input/Output

We've already seen a couple examples of I/O with `scanf()` and
`printf()` for doing I/O at the console (screen/keyboard).

But we'll push those concepts a little farther this chapter.

## The `FILE*` Data Type

When we do any kind of I/O in C, we do so through a piece of data that
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

``` {.zsh}
./foo > output.txt 2> errors.txt   # This command is Unix-specific
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
introduction of OS X, being Unix-based, reduced this number to two.].
Text files are logically a sequence of _lines_ separated by newlines. To
be portable, your input data should always end with a newline.

But the general rule is that if you're able to edit the file in a
regular text editor, it's a text file. Otherwise, it's binary. More on
binary later.

So let's get to work---how do we open a file for reading, and pull data
out of it?

Let's create a file called `hello.txt` that has just this in it:

``` {.default}
Hello, world!
```

And let's write a program to open the file, read a character out of it,
and then close the file when we're done. That's the game plan!

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;                      // Variable to represent open file

    fp = fopen("hello.txt", "r");  // Open file for reading

    int c = fgetc(fp);             // Read a single character
    printf("%c\n", c);             // Print char to stdout

    fclose(fp);                    // Close the file when done
}
```

See how when we opened the file with `fopen()`, it returned the `FILE*`
to us so we could use it later.

(I'm leaving it out for brevity, but `fopen()` will return `NULL` if
something goes wrong, like file-not-found, so you should really error
check it!)

Also notice the `"r"` that we passed in---this means "open a text stream
for reading". (There are various strings we can pass to `fopen()` with
additional meaning, like writing, or appending, and so on.)

After that, we used the `fgetc()` function to get a character from the
stream. You might be wondering why I've made `c` an `int` instead of a
`char`---hold that thought!

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

How about I share that Fun Fact™, now. Turns out `EOF` is the reason why
`fgetc()` and functions like it return an `int` instead of a `char`.
`EOF` isn't a character proper, and its value likely falls outside the
range of `char`. Since `fgetc()` needs to be able to return any byte
**and** `EOF`, it needs to be a wider type that can hold more values. so
`int` it is. But unless you're comparing the returned value against
`EOF`, you can know, deep down, it's a `char`.

All right! Back to reality! We can use this to read the whole file in a
loop.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    int c;

    fp = fopen("hello.txt", "r");

    while ((c = fgetc(fp)) != EOF)
        printf("%c", c);

    fclose(fp);
}
```

(If line 10 is too weird, just break it down starting with the
innermost-nested parens. The first thing we do is assign the result of
`fgetc()` into `c`, and _then_ we compare _that_ against `EOF`. We've
just crammed it into a single line. This might look hard to read, but
study it---it's idiomatic C.)

And running this, we see:

``` {.default}
Hello, world!
```

But still, we're operating a character at a time, and lots of text files
make more sense at the line level. Let's switch to that.

### Reading a Line at a Time

So how can we get an entire line at once? `fgets()` to the rescue! For
arguments, it takes a pointer to a `char` buffer to hold bytes, a
maximum number of bytes to read, and a `FILE*` to read from. It returns
`NULL` on end-of-file or error. `fgets()` is even nice enough to
NUL-terminate the string when its done^[If the buffer's not big enough
to read in an entire line, it'll just stop reading mid-line, and the
next call to `fgets()` will continue reading the rest of the line.].

Let's do a similar loop as before, except let's have a multiline file
and read it in a line at a time.

Here's a file `quote.txt`:

``` {.default}
A wise man can learn more from
a foolish question than a fool
can learn from a wise answer.
                  --Bruce Lee
```

And here's some code that reads that file a line at a time and prints
out a line number before each one:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    char s[1024];  // Big enough for any line this program will encounter
    int linecount = 0;

    fp = fopen("quote.txt", "r");

    while (fgets(s, sizeof s, fp) != NULL) 
        printf("%d: %s", ++linecount, s);

    fclose(fp);
}
```

Which gives the output:

``` {.default}
1: A wise man can learn more from
2: a foolish question than a fool
3: can learn from a wise answer.
4:                   --Bruce Lee
```

## Formatted Input

You know how you can get formatted output with `printf()` (and, thus,
`fprintf()` like we'll see, below)?

You can do the same thing with `fscanf()`.

Let's have a file with a series of data records in it. In this case,
whales, with name, length in meters, and weight in tonnes. `whales.txt`:

``` {.default}
blue 29.9 173
right 20.7 135
gray 14.9 41
humpback 16.0 30
```

Yes, we could read these with `fgets()` and then parse the string with
`sscanf()` (and in some ways that's more resilient against corrupted
files), but in this case, let's just use `fscanf()` and pull it in
directly.

The `fscanf()` function skips leading whitespace when reading, and returns `EOF`
on end-of-file or error.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    char name[1024];  // Big enough for any line this program will encounter
    float length;
    int mass;

    fp = fopen("whales.txt", "r");

    while (fscanf(fp, "%s %f %d", name, &length, &mass) != EOF)
        printf("%s whale, %d tonnes, %.1f meters\n", name, mass, length);

    fclose(fp);
}
```

Which gives the result:

``` {.default}
blue whale, 173 tonnes, 29.9 meters
right whale, 135 tonnes, 20.7 meters
gray whale, 41 tonnes, 14.9 meters
humpback whale, 30 tonnes, 16.0 meters
```

## Writing Text Files

In much the same way we can use `fgetc()`, `fgets()`, and `fscanf()` to
read text streams, we can use `fputc()`, `fputs()`, and `fprintf()` to
write text streams.

To do so, we have to `fopen()` the file in write mode by passing `"w"`
as the second argument. Opening an existing file in `"w"` mode will
instantly truncate that file to 0 bytes for a full overwrite.

We'll put together a simple program that outputs a file `output.txt`
using a variety of output functions.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    int x = 32;

    fp = fopen("output.txt", "w");

    fputc('B', fp);
    fputc('\n', fp);   // newline
    fprintf(fp, "x = %d\n", x);
    fputs("Hello, world!\n", fp);

    fclose(fp);
}
```

And this produces a file, `output.txt`, with these contents:

``` {.default}
B
x = 32
Hello, world!
```

Fun fact: since `stdout` is a file, you could replace line 8 with:

``` {.c}
fp = stdout;
```

and the program would have outputted to the console instead of to a
file. Try it!

## Binary File I/O

So far we've just been talking text files. But there's that other beast
we mentioned early on called _binary_ files, or binary streams.

These work very similarly to text files, except the I/O subsystem
doesn't perform any translations on the data like it might with a text
file. With binary files, you get a raw stream of bytes, and that's all.

The big difference in opening the file is that you have to add a `"b"`
to the mode. That is, to read a binary file, open it in `"rb"` mode. To
write a file, open it in `"wb"` mode.

Because it's streams of bytes, and streams of bytes can contain NUL
characters, and the NUL character is the end-of-string marker in C, it's
rare that people use the `fprintf()`-and-friends functions to operate on
binary files.

Instead the most common functions are `fread()` and `fwrite()`. The
functions read and write a specified number of bytes to the stream.

To demo, we'll write a couple programs. One will write a sequence of
byte values to disk all at once. And the second program will read a byte
at a time and print them out^[Normally the second program would read all
the bytes at once, and _then_ print them out in a loop. That would be
more efficient. But we're going for demo value, here.].

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    unsigned char bytes[6] = {5, 37, 0, 88, 255, 12};

    fp = fopen("output.bin", "wb");  // wb mode for "write binary"!

    // In the call to fwrite, the arguments are:
    //
    // * Pointer to data to write
    // * Size of each "piece" of data
    // * Count of each "piece" of data
    // * FILE*

    fwrite(bytes, sizeof(char), 6, fp);

    fclose(fp);
}
```

Those two middle arguments to `fwrite()` are pretty odd. But basically
what we want to tell the function is, "We have items that are _this_
big, and we want to write _that_ many of them." This makes it convenient
if you have a record of a fixed length, and you have a bunch of them in
an array. You can just tell it the size of one record and how many to
write. 

In the example above, we tell it each record is the size of a `char`,
and we have 6 of them.

Running the program gives us a file `output.bin`, but opening it in a
text editor doesn't show anything friendly! It's binary data---not text.
And random binary data I just made up, at that!

If I run it through a [flw[hex dump|Hex_dump]] program, we can see the
output as bytes:

``` {.default}
05 25 00 58 ff 0c
```

And those values in hex do match up to the values (in decimal) that we
wrote out.

But now let's try to read them back in with a different program. This
one will open the file for binary reading (`"rb"` mode) and will read
the bytes one at a time in a loop.

`fread()` has the neat feature where it returns the number of bytes
read, or `0` on EOF. So we can loop until we see that, printing numbers
as we go.

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    unsigned char c;

    fp = fopen("output.bin", "rb"); // rb for "read binary"!

    while (fread(&c, sizeof(char), 1, fp) > 0)
        printf("%d\n", c);
}
```

And, running it, we see our original numbers!

``` {.default}
5
37
0
88
255
12
```

Woo hoo!

### `struct` and Number Caveats

As we saw in the `struct`s section, the compiler is free to add padding
to a `struct` as it sees fit. And different compilers might do this
differently. And the same compiler on different architectures could do
it differently. And the same compiler on the same architectures could do
it differently.

What I'm getting at is this: it's not portable to just `fwrite()` an
entire `struct` out to a file when you don't know where the padding will
end up.

How do we fix this? Hold that thought---we'll look at some ways to do
this after looking at another related problem.

Numbers!

Turns out all architectures don't represent numbers in memory the same
way.

Let's look at a simple `fwrite()` of a 2-byte number. We'll write it in
hex so each byte is clear. The most significant byte will have the value
`0x12` and the least significant will have the value `0x34`.

``` {.c}
unsigned short v = 0x1234;  // Two bytes, 0x12 and 0x34

fwrite(&v, sizeof v, 1, fp);
```

What ends up in the stream?

Well, it seems like it should be `0x12` followed by `0x34`, right?

But if I run this on my machine and hex dump the result, I get:

``` {.default}
34 12
```

They're reversed! What gives?

This has something to do with what's called the
[flw[_endianess_|Endianess]] of the architecture. Some write the most
significant bytes first, and some the least significant bytes first.

This means that if you write a multibyte number out straight from
memory, you can't do it in a portable way^[And this is why I used
individual bytes in my `fwrite()` and `fread()` examples, above,
shrewdly.].

A similar problem exists with floating point. Most systems use the same
format for their floating point numbers, but some do not. No guarantees!

So... how can we fix all these problems with numbers and `struct`s to
get our data written in a portable way?

The summary is to _serialize_ the data, which is a general term that
means to take all the data and write it out in a format that you
control, that is well-known, and programmable to work the same way on
all platforms.

As you might imagine, this is a solved problem. There are a bunch of
serialization libraries you can take advantage of, such as Google's
[flw[_protocol buffers_|Protocol_buffers]], out there and ready to use.
They will take care of all the gritty details for you, and even will
allow data from your C programs to interoperate with other languages
that support the same serialization methods.

Do yourself and everyone a favor! Serialize your binary data when you
write it to a stream! This will keep things nice and portable, even if
you transfer data files from one architecture to another.
