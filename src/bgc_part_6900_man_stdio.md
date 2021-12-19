<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdio.h>` Standard I/O Library {#stdio}

|Function|Description|
|-|-|
|[`clearerr()`](#man-feof)|Clear the `feof` and `ferror` status flags|
|[`fclose()`](#man-fclose)|Close an open file|
|[`feof()`](#man-feof)|Return the file end-of-file status|
|[`ferror()`](#man-feof)|Return the file error status|
|[`fflush()`](#man-fflush)|Flush all buffered output to a file|
|[`fgetc()`](#man-getc)|Read a character in a file|
|[`fgetpos()`](#man-fgetpos)|Get the file I/O position|
|[`fgets()`](#man-gets)|Read a line from a file|
|[`fopen()`](#man-fopen)|Open a file|
|[`fprintf()`](#man-printf)|Print formatted output to a file|
|[`fputc()`](#man-putc)|Print a character to a file|
|[`fputs()`](#man-puts)|Print a string to a file|
|[`fread()`](#man-fread)|Read binary data from a file|
|[`freopen()`](#man-freopen)|Change file associated with a stream|
|[`fscanf()`](#man-scanf)|Read formatted input from a file|
|[`fseek()`](#man-fseek)|Set the file I/O position|
|[`fsetpos()`](#man-fgetpos)|Set the file I/O position|
|[`ftell()`](#man-ftell)|Get the file I/O position|
|[`fwrite()`](#man-fwrite)|Write binary data to a file|
|[`getc()`](#man-getc)|Get a character from `stdin`|
|[`getchar()`](#man-getc)|Get a character from `stdin`|
|[`gets()`](#man-gets)|Get a string from `stdin` (removed in C11)|
|[`perror()`](#man-perror)|Print a human-formatted error message|
|[`printf()`](#man-printf)|Print formatted output to `stdout`|
|[`putc()`](#man-putc)|Print a character to `stdout`|
|[`putchar()`](#man-putc)|Print a character to `stdout`|
|[`puts()`](#man-puts)|Print a string to `stdout`|
|[`remove()`](#man-remove)|Delete a file from disk|
|[`rename()`](#man-rename)|Rename or move a file on disk|
|[`rewind()`](#man-fseek)|Set the I/O position to the beginning of a file|
|[`scanf()`](#man-scanf)|Read formatted input from `stdin`|
|[`setbuf()`](#man-setbuf)|Configure buffering for I/O operations|
|[`setvbuf()`](#man-setbuf)|Configure buffering for I/O operations|
|[`snprintf()`](#man-printf)|Print length-limited formatted output to a string|
|[`sprintf()`](#man-printf)|Print formatted output to a string|
|[`sscanf()`](#man-scanf)|Read formatted input from a string|
|[`tmpfile()`](#man-tmpfile)|Create a temporary file|
|[`tmpnam()`](#man-tmpnam)|Generate a unique name for a temporary file|
|[`ungetc()`](#man-ungetc)|Push a character back on the input stream|
|[`vfprintf()`](#man-vprintf)|Variadic print formatted output to a file|
|[`vfscanf()`](#man-vscanf)|Variadic read formatted input from a file|
|[`vprintf()`](#man-vprintf)|Variadic print formatted output to `stdout`|
|[`vscanf()`](#man-vscanf)|Variadic read formatted input from `stdin`|
|[`vsnprintf()`](#man-vprintf)|Variadic length-limited print formatted output to a string|
|[`vsprintf()`](#man-vprintf)|Variadic print formatted output to a string|
|[`vsscanf()`](#man-vscanf)|Variadic read formatted input to a string|

The most basic of all libraries in the whole of the standard C library
is the standard I/O library. It's used for reading from and writing to
files. I can see you're very excited about this.

So I'll continue. It's also used for reading and writing to the console,
as we've already often seen with the `printf()` function.

(A little secret here---many many things in various operating systems
are secretly files deep down, and the console is no exception.
"_Everything in Unix is a file!_" `:-)`)

You'll probably want some prototypes of the functions you can use,
right? To get your grubby little mittens on those, you'll want to
include `stdio.h`.

Anyway, so we can do all kinds of cool stuff in terms of file I/O. LIE
DETECTED. Ok, ok. We can do all kinds of stuff in terms of file I/O.
Basically, the strategy is this:

1. Use `fopen()` to get a pointer to a file structure of type `FILE*`.
   This pointer is what you'll be passing to many of the other file I/O
   calls.

2. Use some of the other file calls, like `fscanf()`, `fgets()`,
   `fprintf()`, or etc. using the `FILE*` returned from `fopen()`.

3. When done, call `fclose()` with the `FILE*`. This let's the operating
   system know that you're truly done with the file, no take-backs.

What's in the `FILE*`? Well, as you might guess, it points to a `struct`
that contains all kinds of information about the current read and write
position in the file, how the file was opened, and other stuff like
that. But, honestly, who cares. No one, that's who. The `FILE` structure
is _opaque_ to you as a programmer; that is, you don't need to know
what's in it, and you don't even _want_ to know what's in it. You just
pass it to the other standard I/O functions and they know what to do.

This is actually pretty important: try to not muck around in the `FILE`
structure. It's not even the same from system to system, and you'll end
up writing some really non-portable code.

One more thing to mention about the standard I/O library: a lot of the
functions that operate on files use an "f" prefix on the function name.
The same function that is operating on the console will leave the "f"
off. For instance, if you want to print to the console, you use
`printf()`, but if you want to print to a file, use `fprintf()`, see?

Wait a moment! If writing to the console is, deep down, just like
writing to a file, since everything in Unix is a file, why are there two
functions? Answer: it's more convenient. But, more importantly, is there
a `FILE*` associated with the console that you can use? Answer: YES!

There are, in fact, _three_ (count 'em!) special `FILE*`s you have at
your disposal merely for just including `stdio.h`. There is one for
input, and two for output.

That hardly seems fair---why does output get two files, and input only
get one?

That's jumping the gun a bit---let's just look at them:

|Stream|Description|
|-|-|
|`stdin`|Input from the console.|
|`stdout`|Output to the console.|
|`stderr`|Output to the console on the error file stream.|

So standard input (`stdin`) is by default just what you type at the
keyboard. You can use that in `fscanf()` if you want, just like this:

``` {.c}
/* this line: */
scanf("%d", &x);

/* is just like this line: */
fscanf(stdin, "%d", &x);
```

And `stdout` works the same way:

``` {.c}
printf("Hello, world!\n");
fprintf(stdout, "Hello, world!\n"); /* same as previous line! */
```

So what is this `stderr` thing? What happens when you output to that?
Well, generally it goes to the console just like `stdout`, but people
use it for error messages, specifically. Why? On many systems you can
redirect the output from the program into a file from the command
line...and sometimes you're interested in getting just the error output.
So if the program is good and writes all its errors to `stderr`, a user
can redirect just `stderr` into a file, and just see that. It's just a
nice thing you, as a programmer, can do.

Finally, a lot of these functions return `int` where you might expect
`char`. This is because the function can return a character _or_
end-of-file (`EOF`), and `EOF` is potentially an integer. If you don't
get `EOF` as a return value, you can safely store the result in a
`char`.

[[manbreak]]

## `remove()` {#man-remove}

Delete a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int remove(const char *filename); 
```

### Description {.unnumbered .unlisted}

Removes the specified file from the filesystem. It just deletes it.
Nothing magical. Simply call this function and sacrifice a small chicken
and the requested file will be deleted.

### Return Value {.unnumbered .unlisted}

Returns zero on success, and `-1` on error, setting `errno`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    char *filename = "evidence.txt";

    remove(filename);
}
```

### See Also {.unnumbered .unlisted}

[`rename()`](#man-rename)

[[manbreak]]
## `rename()` {#man-rename}

Renames a file and optionally moves it to a new location

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int rename(const char *old, const char *new);
```

### Description {.unnumbered .unlisted}

Renames the file `old` to name `new`. Use this function if you're tired
of the old name of the file, and you are ready for a change. Sometimes
simply renaming your files makes them feel new again, and could save you
money over just getting all new files!

One other cool thing you can do with this function is actually move a
file from one directory to another by specifying a different path for
the new name.

### Return Value {.unnumbered .unlisted}

Returns zero on success, and `-1` on error, setting `errno`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    // Rename a file
    rename("foo", "bar");

    // Rename and move to another directory:
    rename("/home/beej/evidence.txt", "/tmp/nothing.txt");
}
```

### See Also {.unnumbered .unlisted}

[`remove()`](#man-remove)

[[manbreak]]

## `tmpfile()` {#man-tmpfile}

Create a temporary file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

FILE *tmpfile(void);
```

### Description {.unnumbered .unlisted}

This is a nifty little function that will create and open a temporary
file for you, and will return a `FILE*` to it that you can use. The file
is opened with mode "`r+b`", so it's suitable for reading, writing, and
binary data.

By using a little magic, the temp file is automatically deleted when it
is `close()`'d or when your program exits. (Specifically, in Unix terms,
`tmpfile()`
[fl[_unlinks_|https://man.archlinux.org/man/unlinkat.2.en#DESCRIPTION]]
the file right after it opens it. This means that it's primed to be
deleted from disk, but still exists because your process still has it
open. As soon as your process exits, all open files are closed, and the
temp file vanishes into the ether.)

### Return Value {.unnumbered .unlisted}

This function returns an open `FILE*` on success, or `NULL` on failure.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *temp;
    char s[128];

    temp = tmpfile();

    fprintf(temp, "What is the frequency, Alexander?\n");

    rewind(temp); // back to the beginning

    fscanf(temp, "%s", s); // read it back out

    fclose(temp); // close (and magically delete)
}
```

### See Also {.unnumbered .unlisted}

[`fopen()`](#man-fopen),
[`fclose()`](#man-fclose),
[`tmpnam()`](#man-tmpnam)

[[manbreak]]

## `tmpnam()` {#man-tmpnam}

Generate a unique name for a temporary file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

char *tmpnam(char *s);
```

### Description {.unnumbered .unlisted}

This function takes a good hard look at the existing files on your
system, and comes up with a unique name for a new file that is suitable
for temporary file usage.

Let's say you have a program that needs to store off some data for a
short time so you create a temporary file for the data, to be deleted
when the program is done running. Now imagine that you called this file
`foo.txt`. This is all well and good, except what if a user already has
a file called `foo.txt` in the directory that you ran your program from?
You'd overwrite their file, and they'd be unhappy and stalk you forever.
And you wouldn't want that, now would you?

Ok, so you get wise, and you decide to put the file in `/tmp` so that it
won't overwrite any important content. But wait! What if some other user
is running your program at the same time and they both want to use that
filename? Or what if some other program has already created that file?

See, all of these scary problems can be completely avoided if you just
use `tmpnam()` to get a safe-ready-to-use filename.

So how do you use it? There are two amazing ways. One, you can declare
an array (or `malloc()` it---whatever) that is big enough to hold the
temporary file name. How big is that? Fortunately there has been a macro
defined for you, `L_tmpnam`, which is how big the array must be.

And the second way: just pass `NULL` for the filename. `tmpnam()` will
store the temporary name in a static array and return a pointer to that.
Subsequent calls with a `NULL` argument will overwrite the static array,
so be sure you're done using it before you call `tmpnam()` again.

Again, this function just makes a file name for you. It's up to you to
later `fopen()` the file and use it.

One more note: some compilers warn against using `tmpnam()` since some
systems have better functions (like the Unix function `mkstemp()`.)  You
might want to check your local documentation to see if there's a better
option. Linux documentation goes so far as to say, "Never use this
function. Use `mkstemp()` instead."

I, however, am going to be a jerk and not talk about
[flm[`mkstemp()`|mkstemp.3.en]] because it's not in the standard I'm
writing about. Nyaah.

The macro `TMP_MAX` holds the number of unique filenames that can be
generated by `tmpnam()`. Ironically, it is the _minimum_ number of such
filenames.

### Return Value {.unnumbered .unlisted}

Returns a pointer to the temporary file name. This is either a pointer
to the string you passed in, or a pointer to internal static storage if
you passed in `NULL`. On error (like it can't find any temporary name
that is unique), `tmpnam()` returns `NULL`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    char filename[L_tmpnam];
    char *another_filename;

    if (tmpnam(filename) != NULL)
        printf("We got a temp file name: \"%s\"\n", filename);
    else
        printf("Something went wrong, and we got nothing!\n");

    another_filename = tmpnam(NULL);

    printf("We got another temp file name: \"%s\"\n", another_filename);
    printf("And we didn't error check it because we're too lazy!\n");
}

```

On my Linux system, this generates the following output:

```
We got a temp file name: "/tmp/filew9PMuZ"
We got another temp file name: "/tmp/fileOwrgPO"
And we didn't error check it because we're too lazy!
```
    

### See Also {.unnumbered .unlisted}

[`fopen()`](#man-fopen),
[`tmpfile()`](#man-tmpfile)

[[manbreak]]

## `fclose()` {#man-fclose}

The opposite of `fopen()`---closes a file when you're done with it so
that it frees system resources

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int fclose(FILE *stream);
```

### Description {.unnumbered .unlisted}

When you open a file, the system sets aside some resources to maintain
information about that open file. Usually it can only open so many files
at once. In any case, the Right Thing to do is to close your files when
you're done using them so that the system resources are freed.

Also, you might not find that all the information that you've written to
the file has actually been written to disk until the file is closed.
(You can force this with a call to `fflush()`.)

When your program exits normally, it closes all open files for you. Lots
of times, though, you'll have a long-running program, and it'd be better
to close the files before then. In any case, not closing a file you've
opened makes you look bad. So, remember to `fclose()` your file when
you're done with it!

### Return Value {.unnumbered .unlisted}

On success, `0` is returned. Typically no one checks for this. On error
`EOF` is returned. Typically no one checks for this, either.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;

    fp = fopen("spoon.txt", "r");

    if (fp == NULL) {
        printf("Error opening file\n");
    } else {
        printf("Opened file just fine!\n");
        fclose(fp);  // All done!
    }
}
```

### See Also {.unnumbered .unlisted}

[`fopen()`](#man-fopen)

[[manbreak]]

## `fflush()` {#man-fflush}

Process all buffered I/O for a stream right now

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int fflush(FILE *stream);
```

### Description {.unnumbered .unlisted}

When you do standard I/O, as mentioned in the section on the
[`setvbuf()`](#man-setbuf) function, it is usually stored in a buffer until
a line has been entered or the buffer is full or the file is closed.
Sometimes, though, you really want the output to happen _right this
second_, and not wait around in the buffer. You can force this to happen
by calling `fflush()`.

The advantage to buffering is that the OS doesn't need to hit the disk
every time you call `fprintf()`. The disadvantage is that if you look at
the file on the disk after the `fprintf()` call, it might not have
actually been written to yet. ("I called `fputs()`, but the file is
still zero bytes long! Why?!") In virtually all circumstances, the
advantages of buffering outweigh the disadvantages; for those other
circumstances, however, use `fflush()`.

Note that `fflush()` is only designed to work on output streams
according to the spec. What will happen if you try it on an input
stream? Use your spooky voice: _who knooooows!_

### Return Value {.unnumbered .unlisted}

On success, `fflush()` returns zero. If there's an error, it returns
`EOF` and sets the error condition for the stream (see
[`ferror()`](#man-feof).)

### Example {.unnumbered .unlisted}

In this example, we're going to use the carriage return, which is
`'\r'`. This is like newline (`'\n'`), except that it doesn't move to
the next line. It just returns to the front of the current line.

What we're going to do is a little text-based status bar like so many
command line programs implement. It'll do a countdown from 10 to 0
printing over itself on the same line.

What is the catch and what does this have to do with `fflush()`? The
catch is that the terminal is most likely "line buffered" (see the
section on [`setvbuf()`](#man-setbuf) for more info), meaning that it
won't actually display anything until it prints a newline. But we're not
printing newlines; we're just printing carriage returns, so we need a
way to force the output to occur even though we're on the same line.
Yes, it's `fflush()!`

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

void sleep_seconds(int s)
{
    thrd_sleep(&(struct timespec){.tv_sec=s}, NULL);
}

int main(void)
{
    int count;

    for(count = 10; count >= 0; count--) {
        printf("\rSeconds until launch: ");  // lead with a CR
        if (count > 0)
            printf("%2d", count);
        else
            printf("blastoff!\n");

        // force output now!!
        fflush(stdout);

        sleep_seconds(1);
    }
}
```

### See Also {.unnumbered .unlisted}

[`setbuf()`](#man-setbuf),
[`setvbuf()`](#man-setbuf)

[[manbreak]]

## `fopen()` {#man-fopen}

Opens a file for reading or writing

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

FILE *fopen(const char *path, const char *mode);
```

### Description {.unnumbered .unlisted}

The `fopen()` opens a file for reading or writing.

Parameter `path` can be a relative or fully-qualified path and file name
to the file in question.

Parameter `mode` tells `fopen()` how to open the file (reading, writing,
or both), and whether or not it's a binary file. Possible modes are:

|Mode|Description|
|-|-|
|`r`|Open the file for reading (read-only).|
|`w`|Open the file for writing (write-only). The file is created if it doesn't exist.|
|`r+`|Open the file for reading and writing. The file has to already exist.|
|`w+`|Open the file for writing and reading. The file is created if it doesn't already exist.|
|`a`|Open the file for append. This is just like opening a file for writing, but it positions the file pointer at the end of the file, so the next write appends to the end. The file is created if it doesn't exist.|
|`a+`|Open the file for reading and appending. The file is created if it doesn't exist.|

Any of the modes can have the letter "`b`" appended to the end, as is
"`wb`" ("write binary"), to signify that the file in question is a
_binary_ file. ("Binary" in this case generally means that the file
contains non-alphanumeric characters that look like garbage to human
eyes.)  Many systems (like Unix) don't differentiate between binary and
non-binary files, so the "`b`" is extraneous. But if your data is
binary, it doesn't hurt to throw the "`b`" in there, and it might help
someone who is trying to port your code to another system.

The macro `FOPEN_MAX` tells you how many streams (at least) you can have
open at once.

The macro `FILENAME_MAX` tells you what the longest valid filename can
be. Don't go crazy, now.

### Return Value {.unnumbered .unlisted}

`fopen()` returns a `FILE*` that can be used in subsequent file-related
calls.

If something goes wrong (e.g. you tried to open a file for read that
didn't exist), `fopen()` will return `NULL`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;

    fp = fopen("spoon.txt", "r");

    if (fp == NULL) {
        printf("Error opening file\n");
    } else {
        printf("Opened file just fine!\n");
        fclose(fp);  // All done!
    }
}
```

### See Also {.unnumbered .unlisted}

[`fclose()`](#man-fclose),
[`freopen()`](#man-freopen)

[[manbreak]]

## `freopen()` {#man-freopen}

Reopen an existing `FILE*`, associating it with a new path

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

FILE *freopen(const char *filename, const char *mode, FILE *stream);
```

### Description {.unnumbered .unlisted}

Let's say you have an existing `FILE*` stream that's already open, but
you want it to suddenly use a different file than the one it's using.
You can use `freopen()` to "re-open" the stream with a new file.

Why on Earth would you ever want to do that? Well, the most common
reason would be if you had a program that normally would read from
`stdin`, but instead you wanted it to read from a file. Instead of
changing all your `scanf()`s to `fscanf()`s, you could simply reopen
`stdin` on the file you wanted to read from.

Another usage that is allowed on some systems is that you can pass
`NULL` for `filename`, and specify a new `mode` for `stream`. So you
could change a file from "`r+`" (read and write) to just "`r`" (read),
for instance. It's implementation dependent which modes can be changed.

When you call `freopen()`, the old `stream` is closed. Otherwise, the
function behaves just like the standard `fopen()`.

### Return Value {.unnumbered .unlisted}

`freopen()` returns `stream` if all goes well.

If something goes wrong (e.g. you tried to open a file for read that
didn't exist), `freopen()` will return `NULL`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int i, i2;

    scanf("%d", &i); // read i from stdin

    // now change stdin to refer to a file instead of the keyboard
    freopen("someints.txt", "r", stdin);

    scanf("%d", &i2); // now this reads from the file "someints.txt"

    printf("Hello, world!\n"); // print to the screen

    // change stdout to go to a file instead of the terminal:
    freopen("output.txt", "w", stdout);

    printf("This goes to the file \"output.txt\"\n");

    // this is allowed on some systems--you can change the mode of a file:
    freopen(NULL, "wb", stdout); // change to "wb" instead of "w"
}
```

### See Also {.unnumbered .unlisted}

[`fclose()`](#man-fclose),
[`fopen()`](#man-fopen)

[[manbreak]]

## `setbuf()`, `setvbuf()` {#man-setbuf}

Configure buffering for standard I/O operations

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

void setbuf(FILE *stream, char *buf);

int setvbuf(FILE *stream, char *buf, int mode, size_t size);
```

### Description {.unnumbered .unlisted}

Now brace yourself because this might come as a bit of a surprise to
you: when you `printf()` or `fprintf()` or use any I/O functions like
that, _it does not normally work immediately_. For the sake of
efficiency, and to irritate you, the I/O on a `FILE*` stream is buffered
away safely until certain conditions are met, and only then is the
actual I/O performed. The functions `setbuf()` and `setvbuf()` allow you
to change those conditions and the buffering behavior.

So what are the different buffering behaviors? The biggest is called
"full buffering", wherein all I/O is stored in a big buffer until it is
full, and then it is dumped out to disk (or whatever the file is). The
next biggest is called "line buffering"; with line buffering, I/O is
stored up a line at a time (until a newline (`'\n'`) character is
encountered) and then that line is processed. Finally, we have
"unbuffered", which means I/O is processed immediately with every
standard I/O call.

You might have seen and wondered why you could call `putchar()` time and
time again and not see any output until you called `putchar('\n')`;
that's right---`stdout` is line-buffered!

Since `setbuf()` is just a simplified version of `setvbuf()`, we'll talk
about `setvbuf()` first.

The `stream` is the `FILE*` you wish to modify. The standard says you
_must_ make your call to `setvbuf()` _before_ any I/O operation is
performed on the stream, or else by then it might be too late.

The next argument, `buf` allows you to make your own buffer space (using
[`malloc()`](#man-malloc) or just a `char` array) to use for buffering.
If you don't care to do this, just set `buf` to `NULL`.

Now we get to the real meat of the function: `mode` allows you to choose
what kind of buffering you want to use on this `stream`. Set it to one
of the following:

|Mode|Description|
|-|-|
|`_IOFBF`|`stream` will be fully buffered.|
|`_IOLBF`|`stream` will be line buffered.|
|`_IONBF`|`stream` will be unbuffered.|

Finally, the `size` argument is the size of the array you passed in for
`buf`...unless you passed `NULL` for `buf`, in which case it will resize
the existing buffer to the size you specify.

Now what about this lesser function `setbuf()`? It's just like calling
`setvbuf()` with some specific parameters, except `setbuf()` doesn't
return a value. The following example shows the equivalency:

``` {.c}
// these are the same:
setbuf(stream, buf);
setvbuf(stream, buf, _IOFBF, BUFSIZ); // fully buffered

// and these are the same:
setbuf(stream, NULL);
setvbuf(stream, NULL, _IONBF, BUFSIZ); // unbuffered
```

### Return Value {.unnumbered .unlisted}

`setvbuf()` returns zero on success, and nonzero on failure.
`setbuf()` has no return value.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    char lineBuf[1024];

    fp = fopen("somefile.txt", "w");
    setvbuf(fp, lineBuf, _IOLBF, 1024);  // set to line buffering
    fprintf(fp, "You won't see this in the file yet. ");
    fprintf(fp, "But now you will because of this newline.\n");
    fclose(fp);

    fp = fopen("anotherfile.txt", "w");
    setbuf(fp, NULL); // set to unbuffered
    fprintf(fp, "You will see this in the file now.");
    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[`fflush()`](#man-fflush)

[[manbreak]]

## `printf()`, `fprintf()`, `sprintf()`, `snprintf()` {#man-printf}

Print a formatted string to the console or to a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int printf(const char *format, ...);

int fprintf(FILE *stream, const char *format, ...);

int sprintf(char * restrict s, const char * restrict format, ...);

int snprintf(char * restrict s, size_t n, const char * restrict format, ...);
```

### Description {.unnumbered .unlisted}

These functions print formatted output to a variety of destinations.

|Function|Output Destination|
|-|-|
|`printf()`|Print to console (screen by default, typically).|
|`fprintf()`|Print to a file.|
|`sprintf()`|Print to a string.|
|`snprintf()`|Print to a string (safely).|

The only differences between these is are the leading parameters that
you pass to them before the `format` string.

|Function|What you pass before `format`|
|-|-|
|`printf()`|Nothing comes before `format`.|
|`fprintf()`|Pass a `FILE*`.|
|`sprintf()`|Pass a `char*` to a buffer to print into.|
|`snprintf()`|Pass a `char*` to the buffer and a maximum buffer length.|

The `printf()` function is legendary as being one of the most flexible
outputting systems ever devised. It can also get a bit freaky here or
there, most notably in the `format` string. We'll take it a step at a
time here.

The easiest way to look at the format string is that it will print
everything in the string as-is, _unless_ a character has a percent sign
(`%`) in front of it. That's when the magic happens: the next argument
in the `printf()` argument list is printed in the way described by the
percent code. These percent codes are called _format specifiers_.

Here are the most common format specifiers.

|Specifier|Description|
|:--:|--------------------------|
|`%d`|Print the next argument as a signed decimal number, like `3490`. The argument printed this way should be an `int`, or something that gets promoted to `int`.|
|`%f`|Print the next argument as a signed floating point number, like `3.14159`. The argument printed this way should be a `double`, or something that gets promoted to a `double`.|
|`%c`|Print the next argument as a character, like `'B'`. The argument printed this way should be a `char` variant.|
|`%s`|Print the next argument as a string, like `"Did you remember your mittens?"`. The argument printed this way should be a `char*` or `char[]`.|
|`%%`|No arguments are converted, and a plain old run-of-the-mill percent sign is printed. This is how you print a '%' using `printf()`.|

So those are the basics. I'll give you some more of the format
specifiers in a bit, but let's get some more breadth before then.
There's actually a lot more that you can specify in there after the
percent sign.

For one thing, you can put a field width in there---this is a number
that tells `printf()` how many spaces to put on one side or the other of
the value you're printing. That helps you line things up in nice
columns. If the number is negative, the result becomes left-justified
instead of right-justified. Example:

``` {.c}
printf("%10d", x);  /* prints X on the right side of the 10-space field */
printf("%-10d", x); /* prints X on the left side of the 10-space field */
```

If you don't know the field width in advance, you can use a little
kung-foo to get it from the argument list just before the argument
itself. Do this by placing your seat and tray tables in the fully
upright position. The seatbelt is fastened by placing the---_*cough*_. I
seem to have been doing way too much flying lately. Ignoring that
useless fact completely, you can specify a dynamic field width by
putting a `*` in for the width. If you are not willing or able to
perform this task, please notify a flight attendant and we will reseat
you.

``` {.c}
int width = 12;
int value = 3490;

printf("%*d\n", width, value);
```

You can also put a "0" in front of the number if you want it to be
padded with zeros:

``` {.c}
int x = 17;
printf("%05d", x);  /* "00017" */
```

When it comes to floating point, you can also specify how many decimal
places to print by making a field width of the form "`x.y`" where `x` is
the field width (you can leave this off if you want it to be just wide
enough) and `y` is the number of digits past the decimal point to print:

``` {.c}
float f = 3.1415926535;

printf("%.2f", f);  /* "3.14" */
printf("%7.3f", f); /* "  3.141" <-- 7 spaces across */
```

Ok, those above are definitely the most common uses of `printf()`, but
let's get _total coverage_.

#### Format Specifier Layout

Technically, the layout of the format specifier is these things in this
order:

1. `%`, followed by...
2. Optional: zero or more flags, left justify, leading zeros, etc.
3. Optional: Field width, how wide the output field should be.
4. Optional: Precision, or how many decimal places to print.
5. Optional: Length modifier, for printing things bigger than `int` or
   `double`.
6. Conversion specifier, like `d`, `f`, etc.

In short, the whole format specifier is laid out like this:

```
%[flags][fieldwidth][.precision][lengthmodifier]conversionspecifier
```

What could be easier?

#### Conversion Specifiers

Let's talk conversion specifiers first. Each of the following specifies
what type it can print, but it can also print anything that gets
promoted to that type. For example, `%d` can print `int`, `short`, and
`char`.

|Conversion Specifier|Description|
|:--:|--------------------------|
|`d`|Print an `int` argument as a decimal number.|
|`i`|Identical to `d`.|
|`o`|Print an `unsigned int` in octal (base 8).|
|`u`|Print an `unsigned int` in decimal.|
|`x`|Print an `unsigned int` in hexadecimal with lowercase letters.|
|`X`|Print an `unsigned int` in hexadecimal with uppercase letters.|
|`f`|Print a `double` in decimal notation. Infinity is printed as `infinity` or `inf`, and NaN is printed as `nan`, any of which could have a leading minus sign.|
|`F`|Same as `f`, except it prints out `INFINITY`, `INF`, or `NAN` in all caps.|
|`e`|Print a number in scientific notation, e.g. `1.234e56`. Does infinity and NaN like `f`.|
|`E`|Just like `e`, except prints the exponent `E` (and infinity and NaN) in uppercase.|
|`g`|Print small numbers like `f` and large numbers like `e`. See note below.|
|`G`|Print small numbers like `F` and large numbers like `E`. See note below.|
|`a`|Print a `double` in hexadecimal form `0xh.hhhhpd` where `h` is a lowercase hex digit and `d` is a decimal exponent of 2. Infinity and NaN in the form of `f`. More below.|
|`A`|Like `a` except everything's uppercase.|
|`c`|Convert `int` argument to `unsigned char` and print as a character.|
|`s`|Print a string starting at the given `char*`.|
|`p`|Print a `void*` out as a number, probably the numeric address, possibly in hex.|
|`n`|Store the number of characters written so far in the given `int*`. Doesn't print anything. See below.|
|`%`|Print a literal percent sign.|

##### Note on `%a` and `%A`

When printing floating point numbers in hex form, there is one number
before the decimal point, and the rest of are out to the precision.

``` {.c}
double pi = 3.14159265358979;

printf("%.3a\n", pi);  // 0x1.922p+1
```

C can choose the leading number in such a way to ensure subsequent
digits align to 4-bit boundaries.

If the precision is left out and the macro `FLT_RADIX` is a power of 2,
enough precision is used to represent the number exactly. If `FLT_RADIX`
is not a power of two, enough precision is used to be able to tell any
two floating values apart.

If the precision is `0` and the `#` flag isn't specified, the decimal
point is omitted.

##### Note on `%g` and `%G`

The gist of this is to use scientific notation when the number gets too
"extreme", and regular decimal notation otherwise.

The exact behavior for whether these print as `%f` or `%e` depends on a
number of factors:

If the number's exponent is greater than or equal to -4 **and** the
precision is greater than the exponent, we use `%f`. In this case, the
precision is converted according to $p=p-(x+1)$, where $p$ is the
specified precision and $x$ is the exponent.

Otherwise we use `%e`, and the precision becomes $p-1$.

Trailing zeros in the decimal portion are removed. And if there are none
left, the decimal point is removed, too. All this unless the `#` flag is
specified.

##### Note on `%n`

This specifier is cool and different, and rarely needed. It doesn't
actually print anything, but stores the number of characters printed so
far in the next pointer argument in the list.

``` {.c}
int numChars;
float a = 3.14159;
int b = 3490;

printf("%f %d%n\n", a, b, &numChars);
printf("The above line contains %d characters.\n", numChars);
```

The above example will print out the values of `a` and `b`, and then
store the number of characters printed so far into the variable
`numChars`. The next call to `printf()` prints out that result.

``` {.default}
3.141590 3490
The above line contains 13 characters
```

#### Length Modifiers

You can stick a _length_ modifier in front of each of the conversion
specifiers, if you want. most of those format specifiers work on `int`
or `double` types, but what if you want larger or smaller types? That's
what these are good for.

For example, you could print out a long long int with the `ll` modifier:

``` {.c}
long long int x = 3490;

printf("%lld\n", x);  // 3490
```

|Length Modifier|Conversion Specifier|Description|
|:--:|:-----:|------------------|
|`hh`|`d`, `i`, `o`, `u`, `x`, `X`|Convert argument to `char` (signed or unsigned as appropriate) before printing.|
|`h`|`d`, `i`, `o`, `u`, `x`, `X`|Convert argument to `short int` (signed or unsigned as appropriate) before printing.|
|`l`|`d`, `i`, `o`, `u`, `x`, `X`|Argument is a `long int` (signed or unsigned as appropriate).|
|`ll`|`d`, `i`, `o`, `u`, `x`, `X`|Argument is a `long long int` (signed or unsigned as appropriate).|
|`j`|`d`, `i`, `o`, `u`, `x`, `X`|Argument is a `intmax_t` or `uintmax_t` (as appropriate).|
|`z`|`d`, `i`, `o`, `u`, `x`, `X`|Argument is a `size_t`.|
|`t`|`d`, `i`, `o`, `u`, `x`, `X`|Argument is a `ptrdiff_t`.|
|`L`|`a`, `A`, `e`, `E`, `f`, `F`, `g`, `G`|Argument is a `long double`.|
|`l`|`c`|Argument is in a `wint_t`,  a wide character.|
|`l`|`s`|Argument is in a `wchar_t*`,  a wide character string.|
|`hh`|`n`|Store result in `signed char*` argument.|
|`h`|`n`|Store result in `short int*` argument.|
|`l`|`n`|Store result in `long int*` argument.|
|`ll`|`n`|Store result in `long long int*` argument.|
|`j`|`n`|Store result in `intmax_t*` argument.|
|`z`|`n`|Store result in `size_t*` argument.|
|`t`|`n`|Store result in `ptrdiff_t*` argument.|

#### Precision

In front of the length modifier, you can put a precision, which
generally means how many decimal places you want on your floating point
numbers.

To do this, you put a decimal point (`.`) and the decimal places
afterward.

For example, we could print π rounded to two decimal places like this:

``` {.c}
double pi = 3.14159265358979;

printf("%.2f\n", pi);  // 3.14
```

|Conversion Specifier|Precision Value Meaning|
|:-------:|---------------------|
|`d`, `i`, `o`, `u`, `x`, `X`|For integer types, minimum number of digits (will pad with leading zeros)|
|`a`, `e`, `f`, `A`, `E`, `F`|For floating types, the precision is the number of digits past the decimal.|
|`g`, `G`|For floating types, the precision is the number of significant digits printed.|
|`s`|The maximum number of bytes (not multibyte characters!) to be written.|

If no number is specified in the precision after the decimal point, the
precision is zero.

If an `*` is specified after the decimal, something amazing happens! It
means the `int` argument to `printf()` before the number to be printed
holds the precision. You can use this if you don't know the precision at
compile time.

``` {.c}
int precision;
double pi = 3.14159265358979;

printf("Enter precision: "); fflush(stdout);
scanf("%d", &precision);

printf("%.*f\n", precision, pi);
```

Which gives:

``` {.default}
Enter precision: 4
3.1416
```

#### Field Width

In front of the optional precision, you can indicate a field width. This
is a decimal number that indicates how wide the region should be in
which the argument is printed. The region is padding with leading (or
trailing) spaces to make sure it's wide enough.

If the field width specified is too small to hold the output, it is
ignored.

As a preview, you can give a negative field width to justify the item
the other direction.

So let's print a number in a field of width 10. We'll put some angle
brackets around it so we can see the padding spaces in the output.

``` {.c}
printf("<<%10d>>\n", 3490);   // right justified
printf("<<%-10d>>\n", 3490);  // left justified
```

``` {.default}
<<      3490>>
<<3490      >>
```

Like with the precision, you can use an asterisk (`*`) as the field
width 

``` {.c}
int field_width;
int val = 3490;

printf("Enter field_width: "); fflush(stdout);
scanf("%d", &field_width);

printf("<<%*d>>\n", field_width, val);
```

#### Flags

Before the field width, you can put some optional flags that further
control the output of the subsequent fields. We just saw that the `-`
flag can be used to left- or right-justify fields. But there are plenty
more!

|Flag|Description|
|:--:|--------------------------|
|`-`|For a field width, left justify in the field (right is default).|
|`+`|If the number is signed, always prefix a `+` or `-` on the front.|
|[SPACE]|If the number is signed, prefix a space for positive, or a `-` for negative.|
|`0`|Pad the right-justified field with leading zeros instead of leading spaces.
|`#`|Print using an alternate form. See below.|

For example, we could pad a hexadecimal number with leading zeros to a
field width of 8 with:

``` {.c}
printf("%08x\n", 0x1234);  // 00001234
```

The `#` "alternate form" result depends on the conversion specifier.

|Conversion Specifier|Alternate Form (`#`) Meaning|
|:---------:|---------------------------------------|
|`o`|Increase precision of a non-zero number just enough to get one leading `0` on the octal number.|
|`x`|Prefix a non-zero number with `0x`.|
|`X`|Same as `x`, except capital `0X`.|
|`a`, `e`, `f`|Always print a decimal point, even if nothing follows it.|
|`A`, `E`, `F`|Identical to `a`, `e`, `f`.|
|`g`, `G`|Always print a decimal point, even if nothing follows it, and keep trailing zeros.|


#### `sprintf()` and `snprintf()` Details

Both `sprintf()` and `snprintf()` have the quality that if you pass in
`NULL` as the buffer, nothing is written---but you can still check the
return value to see how many characters _would_ have been written.

`snprintf()` **always** terminates the string with a `NUL` character. So
if you try to write out more than the maximum specified characters, the
universe ends.

Just kidding. If you do, `snprintf()` will write $n-1$ characters so
that it has enough room to write the terminator at the end.

### Return Value {.unnumbered .unlisted}

Returns the number of characters outputted, or a negative number on
error.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int a = 100;
    float b = 2.717;
    char *c = "beej!";
    char d = 'X';
    int e = 5;

    printf("%d\n", a); /* "100"      */
    printf("%f\n", b); /* "2.717000" */
    printf("%s\n", c); /* "beej!"    */
    printf("%c\n", d); /* "X"        */
    printf("110%%\n"); /* "110%"     */

    printf("%10d\n", a);   /* "       100" */
    printf("%-10d\n", a);  /* "100       " */
    printf("%*d\n", e, a); /* "  100"      */
    printf("%.2f\n", b);   /* "2.72"       */

    printf("%hhd\n", d); /* "88" <-- ASCII code for 'X' */

    printf("%5d %5.2f %c\n", a, b, d); /* "  100  2.72 X" */
}
```

### See Also {.unnumbered .unlisted}

[`sprintf()`](#man-printf),
[`vprintf()`](#man-vprintf)

[[manbreak]]

## `scanf()`, `fscanf()`, `sscanf()` {#man-scanf}

Read formatted string, character, or numeric data from the
console or from a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int scanf(const char *format, ...);

int fscanf(FILE *stream, const char *format, ...);

int sscanf(const char * restrict s, const char * restrict format, ...);
```

### Description {.unnumbered .unlisted}

These functions read formatted output from a variety of sources.

|Function|Input Source|
|-|-|
|`scanf()`|Read from the console (keyboard by default, typically).|
|`fscanf()`|Read from a file.|
|`sscanf()`|Read from a string.|

The only differences between these is are the leading parameters that
you pass to them before the `format` string.

|Function|What you pass before `format`|
|-|-|
|`scanf()`|Nothing comes before `format`.|
|`fscanf()`|Pass a `FILE*`.|
|`sscanf()`|Pass a `char*` to a buffer to read from.|

The `scanf()` family of functions reads data from the console or from a
`FILE` stream, parses it, and stores the results away in variables you
provide in the argument list.

The format string is very similar to that in `printf()` in that you can
tell it to read a `"%d"`, for instance for an `int`. But it also has
additional capabilities, most notably that it can eat up other
characters in the input that you specify in the format string.

But let's start simple, and look at the most basic usage first before
plunging into the depths of the function. We'll start by reading an
`int` from the keyboard:

``` {.c}
int a;

scanf("%d", &a);
```

`scanf()` obviously needs a pointer to the variable if it is going to
change the variable itself, so we use the address-of operator to get the
pointer.

In this case, `scanf()` walks down the format string, finds a "`%d`",
and then knows it needs to read an integer and store it in the next
variable in the argument list, `a`.

Here are some of the other format specifiers you can put in the format
string:

|Format Specifier|Description|
|-|-|
|`%d`|Reads an integer to be stored in an `int`. This integer can be signed.|
|`%u`|Reads an integer to be stored in an `unsigned int`.|
|`%f`|Reads a floating point number, to be stored in a `float`.|
|`%s`|Reads a string up to the first whitespace character.|
|`%c`|Reads a `char`.|

And that's the end of the story!

Ha! Just kidding. If you've just arrived from the `printf()` page, you
know there's a near-infinite amount of additional material.

#### Consuming Other Characters

`scanf()` will move along the format string matching any characters you
include.

For example, you could read a hyphenated date like so:

``` {.c}
scanf("%u-%u-%u", &yyyy, &mm, &dd);
```

In that case, `scanf()` will attempt to consume an unsigned decimal
number, then a hyphen, then another unsigned number, then another hypen,
then another unsigned number.

If it fails to match at any point (e.g. the user entered "foo"),
`scanf()` will bail without consuming the offending characters.

And it will return the number of variables successfully converted. In
the example above, if the user entered a valid string, `scanf()` would
return `3`, one for each variable successfully read.

#### Problems with `scanf()`

I (and the C FAQ and a lot of people) recommend _against_ using
`scanf()` to read directly from the keyboard. It's too easy for it to
stop consuming characters when the user enters some bad data.

If you have data in a file and you're confident it's in good shape,
`fscanf()` can be really useful.

But in the case of the keyboard or file, you can always use `fgets()` to
read a complete line into a buffer, and then use `sscanf()` to scan
things out of the buffer. This gives you the best of both worlds.

#### Problems with `sscanf()`

A while back, a third-party programmer rose to fame for figuring out
[fl[how to cut _GTA Online_ load times by
70%|https://nee.lv/2021/02/28/How-I-cut-GTA-Online-loading-times-by-70/]].

What they'd discovered was that the implementation of `sscanf()` first
effectively calls `strlen()`... so even if you're just using `sscanf()`
to peel the first few characters off the string, it still runs all the
way out to the end of the string first.

On small strings, no big deal, but on large strings with repeated calls
(which is what was happening in _GTA_) it got _sloooooooooowwwww_...

So if you're just converting a string to a number, consider
[`atoi()`](#man-atoi), [`atof()`](#man-atof), or the
[`strtol()`](#man-strtol) and [`strtod()`](#man-strtod) families of
functions, instead.

(The programmer [collected a $10,000 bug
bounty](https://www.polygon.com/2021/3/16/22334214/gta-online-loading-times-t0st-update-bug-bounty)
for the effort.)

#### The Deep Details

Let's check out what a `scanf()` 

And here are some more codes, except these don't tend to be used as
often. You, of course, may use them as often as you wish!

First, the format string. Like we mentioned, it can hold ordinary
characters as well as `%` format specifiers. And whitespace characters.

Whitespace characters have a special role: a whitespace character will
cause `scanf()` to consume as many whitespace characters as it can up to
the next non-whitespace character. You can use this to ignore all
leading or trailing whitespace.

Also, all format specifiers except for `s`, `c`, and `[` automatically
consume leading whitespace.

But I know what you're thinking: the meat of this function is in the
format specifiers. What do those look like?

These consist of the following, in sequence:

1. A `%` sign
2. Optional: an `*` to suppress assignment---more later
3. Optional: a field width---max characters to read
4. Optional: length modifier, for specifying longer or shorter types
5. A conversion specifier, like `d` or `f` indicating the type to read

#### The Conversion Specifier

Let's start with the best and last: the _conversion specifier_. 

This is the part of the format specifier that tells us what type of
variable `scanf()` should be reading into, like `%d` or `%f`.

|Conversion Specifier|Description|
|:--:|--------------------------|
|`d`|Matches a decimal `int`. Can have a leading sign.|
|`i`|Like `d`, except will handle it if you put a leading `0x` (hex) or `0` (octal) on the number.|
|`o`|Matches an octal (base 8) `unsigned int`. Leading zeros are ignored.|
|`u`|Matches a decimal `unsigned int`.|
|`x`|Matches a hex (base 16) `unsigned int`.|
|`f`|Match a floating point number (or scientific notation, or anything `strtod()` can handle).|
|`c`|Match a `char`, or mutiple `char`s if a field width is given.|
|`s`|Match a sequence of non-whitespace `char`s.|
|`[`|Match a sequence of characters from a set. The set ends with `]`. More below.|
|`p`|Match a pointer, the opposite of `%p` for `printf()`.|
|`n`|Store the number of characters written so far in the given `int*`. Doesn't consume anything.|
|`%`|Match a literal percent sign.|

All of the following are equivalent to the `f` specifier: `a`, `e`, `g`,
`A`, `E`, `F`, `G`.

And capital `X` is equivalent to lowercase `x`.

##### The Scanset `%[]` Conversion Specifier

This is about the weirdest format specifier there is. It allows you to
specify a set of characters (the _scanset_) to be stored away (likely in
an array of `char`s). Conversion stops when a character that is not in
the set is matched.

For example, `%[0-9]` means "match all numbers zero through nine."  And
`%[AD-G34]` means "match A, D through G, 3, or 4".

Now, to convolute matters, you can tell `scanf()` to match characters
that are _not_ in the set by putting a caret (`^`) directly after the
`%[` and following it with the set, like this: `%[^A-C]`, which means
"match all characters that are _not_ A through C."

To match a close square bracket, make it the first character in the set,
like this: `%[]A-C]` or `%[^]A-C]`. (I added the "`A-C`" just so it was
clear that the "`]`" was first in the set.)

To match a hyphen, make it the last character in the set, e.g. to match
A-through-C or hyphen: `%[A-C-]`.

So if we wanted to match all letters _except_ "%", "^", "]", "B", "C",
"D", "E", and "-", we could use this format string: `%[^]%^B-E-]`.

Got it? Now we can go onto the next func---no wait! There's more! Yes,
still more to know about `scanf()`. Does it never end? Try to imagine
how I feel writing about it!

#### The Length Modifier

So you know that "`%d`" stores into an `int`. But how do you store into
a `long`, `short`, or `double`?

Well, like in `printf()`, you can add a modifier before the type
specifier to tell `scanf()` that you have a longer or shorter type. The
following is a table of the possible modifiers:

|Length Modifier|Conversion Specifier|Description|
|:--:|:-----:|------------------|
|`hh`|`d`, `i`, `o`, `u`, `x`, `X`|Convert input to `char` (signed or unsigned as appropriate) before printing.|
|`h`|`d`, `i`, `o`, `u`, `x`, `X`|Convert input to `short int` (signed or unsigned as appropriate) before printing.|
|`l`|`d`, `i`, `o`, `u`, `x`, `X`|Convert input to `long int` (signed or unsigned as appropriate).|
|`ll`|`d`, `i`, `o`, `u`, `x`, `X`|Convert input to `long long int` (signed or unsigned as appropriate).|
|`j`|`d`, `i`, `o`, `u`, `x`, `X`|Convert input to `intmax_t` or `uintmax_t` (as appropriate).|
|`z`|`d`, `i`, `o`, `u`, `x`, `X`|Convert input to `size_t`.|
|`t`|`d`, `i`, `o`, `u`, `x`, `X`|Convert input to `ptrdiff_t`.|
|`L`|`a`, `A`, `e`, `E`, `f`, `F`, `g`, `G`|Convert input to `long double`.|
|`l`|`c`,`s`,`[`|Convert input to `wchar_t`,  a wide character.|
|`l`|`s`|Argument is in a `wchar_t*`,  a wide character string.|
|`hh`|`n`|Store result in `signed char*` argument.|
|`h`|`n`|Store result in `short int*` argument.|
|`l`|`n`|Store result in `long int*` argument.|
|`ll`|`n`|Store result in `long long int*` argument.|
|`j`|`n`|Store result in `intmax_t*` argument.|
|`z`|`n`|Store result in `size_t*` argument.|
|`t`|`n`|Store result in `ptrdiff_t*` argument.|

#### Field Widths

The field width generally allows you to specify a maximum number of
characters to consume. If the thing you're trying to match is shorter
than the field width, that input will stop being processed before the
field width is reached.

So a string will stop being consumed when whitespace is found, even if
fewer than the field width characters are matched.

And a float will stop being consumed at the end of the number, even if
fewer characters than the field width are matched.

But `%c` is an interesting one---it doesn't stop consuming characters on
anything. So it'll go exactly to the field width. (Or 1 character if no
field width is given.)

#### Skip Input with `*`

If you put an `*` in the format specifier, it tells `scanf()` do to the
conversion specified, but not store it anywhere. It simply discards the
data as it reads it. This is what you use if you want `scanf()` to eat
some data but you don't want to store it anywhere; you don't give
`scanf()` an argument for this conversion.

``` {.c}
// Read 3 ints, but discard the middle one
scanf("%d %*d %d", &int1, &int3);
```

### Return Value {.unnumbered .unlisted}

`scanf()` returns the number of items assigned into variables. Since
assignment into variables stops when given invalid input for a certain
format specifier, this can tell you if you've input all your data
correctly.

Also, `scanf()` returns `EOF` on end-of-file.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int a;
    long int b;
    unsigned int c;
    float d;
    double e;
    long double f;
    char s[100];

    scanf("%d", &a);  // store an int
    scanf(" %d", &a); // eat any whitespace, then store an int
    scanf("%s", s); // store a string
    scanf("%Lf", &f); // store a long double

    // store an unsigned, read all whitespace, then store a long int:
    scanf("%u %ld", &c, &b);

    // store an int, read whitespace, read "blendo", read whitespace,
    // and store a float:
    scanf("%d blendo %f", &a, &d);

    // read all whitespace, then store all characters up to a newline
    scanf(" %[^\n]", s);

    // store a float, read (and ignore) an int, then store a double:
    scanf("%f %*d %lf", &d, &e);

    // store 10 characters:
    scanf("%10c", s);
}
```

### See Also {.unnumbered .unlisted}

[`sscanf()`](#man-scanf),
[`vscanf()`](#man-vscanf),
[`vsscanf()`](#man-vscanf),
[`vfscanf()`](#man-vscanf)

[[manbreak]]
## `vprintf()`, `vfprintf()`, `vsprintf()`, `vsnprintf()` {#man-vprintf}

`printf()` variants using variable argument lists (`va_list`)

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>
#include <stdarg.h>

int vprintf(const char * restrict format, va_list arg);

int vfprintf(FILE * restrict stream, const char * restrict format,
             va_list arg);

int vsprintf(char * restrict s, const char * restrict format, va_list arg);

int vsnprintf(char * restrict s, size_t n, const char * restrict format,
              va_list arg);
```

### Description {.unnumbered .unlisted}

These are just like the `printf()` variants except instead of taking an
actual variable number of arguments, they take a fixed number---the last
of which is a `va_list` that refers to the variable arguments.

Like with `printf()`, the different variants send output different
places.

|Function|Output Destination|
|-|-|
|`vprintf()`|Print to console (screen by default, typically).|
|`vfprintf()`|Print to a file.|
|`vsprintf()`|Print to a string.|
|`vsnprintf()`|Print to a string (safely).|

Both `vsprintf()` and `vsnprintf()` have the quality that if you pass in
`NULL` as the buffer, nothing is written---but you can still check the
return value to see how many characters _would_ have been written.

If you try to write out more than the maximum number of characters,
`vsnprintf()` will graciously write only $n-1$ characters so that it has
enough room to write the terminator at the end.

As for why in the heck would you ever want to do this, the most common
reason is to create your own specialized versions of `printf()`-type
functions, piggybacking on all that `printf()` functionality goodness.

See the example for an example, predictably.

### Return Value {.unnumbered .unlisted}

`vprintf()` and `vfprintf()` return the number of characters printed, or
a negative value on error.

`vsprintf()` returns the number of characters printed to the buffer, not
counting the NUL terminator, or a negative value if an error occurred.

`vnsprintf()` returns the number of characters printed to the buffer. Or
the number that _would_ have been printed if the buffer had been large
enough.

### Example {.unnumbered .unlisted}

In this example, we make our own version of `printf()` called `logger()`
that timestamps output. Notice how the calls to `logger()` have all the
bells and whistles of `printf()`.

``` {.c .numberLines}
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

int logger(char *format, ...)
{
    va_list va;
    time_t now_secs = time(NULL);
    struct tm *now = gmtime(&now_secs);

    // Output timestamp in format "YYYY-MM-DD hh:mm:ss : "
    printf("%04d-%02d-%02d %02d:%02d:%02d : ",
        now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
        now->tm_hour, now->tm_min, now->tm_sec);

    va_start(va, format);
    int result = vprintf(format, va);
    va_end(va);

    printf("\n");

    return result;
}

int main(void)
{
    int x = 12;
    float y = 3.2;

    logger("Hello!");
    logger("x = %d and y = %.2f", x, y);
}
```

Output:

``` {.default}
2021-03-30 04:25:49 : Hello!
2021-03-30 04:25:49 : x = 12 and y = 3.20
```

### See Also {.unnumbered .unlisted}
[`printf()`](#man-printf)

[[manbreak]]
## `vscanf()`, `vfscanf()`, `vsscanf()` {#man-vscanf}

`scanf()` variants using variable argument lists (`va_list`)

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>
#include <stdarg.h>

int vscanf(const char * restrict format, va_list arg);

int vfscanf(FILE * restrict stream, const char * restrict format,
            va_list arg);

int vsscanf(const char * restrict s, const char * restrict format,
            va_list arg);
```

### Description {.unnumbered .unlisted}

These are just like the `scanf()` variants except instead of taking an
actual variable number of arguments, they take a fixed number---the last
of which is a `va_list` that refers to the variable arguments.

|Function|Input Source|
|-|-|
|`vscanf()`|Read from the console (keyboard by default, typically).|
|`vfscanf()`|Read from a file.|
|`vsscanf()`|Read from a string.|

Like with the `vprintf()` functions, this would be a good way to add
additional functionality that took advantage of the power `scanf()` has
to offer.

### Return Value {.unnumbered .unlisted}

Returns the number of items successfully scanned, or `EOF` on
end-of-file or error.

### Example {.unnumbered .unlisted}

I have to admit I was wracking my brain to think of when you'd ever want
to use this. The best example I could find was [fl[one on Stack
Overflow|https://stackoverflow.com/questions/17017331/c99-vscanf-for-dummies/17018046#17018046]]
that error-checks the return value from `scanf()` against the expected.
A variant of that is shown below.

``` {.c .numberLines}
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

int error_check_scanf(int expected_count, char *format, ...)
{
    va_list va;

    va_start(va, format);
    int count = vscanf(format, va);
    va_end(va);

    // This line will crash the program if the condition is false:
    assert(count == expected_count);

    return count;
}

int main(void)
{
    int a, b;
    float c;

    error_check_scanf(3, "%d, %d/%f", &a, &b, &c);
    error_check_scanf(2, "%d", &a);
}
```

### See Also {.unnumbered .unlisted}
[`scanf()`](#man-scanf)

[[manbreak]]
## `getc()`, `fgetc()`, `getchar()` {#man-getc}

Get a single character from the console or from a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int getc(FILE *stream);

int fgetc(FILE *stream);

int getchar(void);
```

### Description {.unnumbered .unlisted}

All of these functions in one way or another, read a single character
from the console or from a `FILE`. The differences are fairly minor, and
here are the descriptions:

`getc()` returns a character from the specified `FILE`. From a usage
standpoint, it's equivalent to the same `fgetc()` call, and `fgetc()` is
a little more common to see. Only the implementation of the two
functions differs.

`fgetc()` returns a character from the specified `FILE`. From a usage
standpoint, it's equivalent to the same `getc()` call, except that
`fgetc()` is a little more common to see. Only the implementation of the
two functions differs.

Yes, I cheated and used cut-n-paste to do that last paragraph.

`getchar()` returns a character from `stdin`. In fact, it's the same as
calling `getc(stdin)`.

### Return Value {.unnumbered .unlisted}

All three functions return the `unsigned char` that they read, except
it's cast to an `int`.

If end-of-file or an error is encountered, all three functions return
`EOF`.

### Example {.unnumbered .unlisted}

This example reads all the characters from a file, outputting only the
letter 'b's it finds..

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    int c;

    fp = fopen("spoon.txt", "r"); // error check this!

    // this while-statement assigns into c, and then checks against EOF:

    while((c = fgetc(fp)) != EOF) {
        if (c == 'b') {
            putchar(c);
        }
    }

    putchar('\n');

    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[[manbreak]]
## `gets()`, `fgets()` {#man-gets}

Read a string from console or file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

char *fgets(char *s, int size, FILE *stream);
char *gets(char *s);
```

### Description {.unnumbered .unlisted}

These are functions that will retrieve a newline-terminated string from
the console or a file. In other normal words, it reads a line of text.
The behavior is slightly different, and, as such, so is the usage. For
instance, here is the usage of `gets()`:

Don't use `gets()`. In fact, as of C11, it ceases to exist! This is one
of the rare cases of a function being _removed_ from the standard.

Admittedly, rationale would be useful, yes? For one thing, `gets()`
doesn't allow you to specify the length of the buffer to store the
string in. This would allow people to keep entering data past the end of
your buffer, and believe me, this would be Bad News.

And that's what the `size` parameter in `fgets()` is for. `fgets()` will
read at most `size-1` characters and then stick a `NUL` terminator on
after that.

I was going to add another reason, but that's basically the primary and
only reason not to use `gets()`. As you might suspect, `fgets()` allows
you to specify a maximum string length.

One difference here between the two functions: `gets()` will devour and
throw away the newline at the end of the line, while `fgets()` will
store it at the end of your string (space permitting).

Here's an example of using `fgets()` from the console, making it behave
more like `gets()` (with the exception of the newline inclusion):

``` {.c}
char s[100];
gets(s);  // don't use this--read a line (from stdin)
fgets(s, sizeof(s), stdin); // read a line from stdin
```

In this case, the `sizeof()` operator gives us the total size of the
array in bytes, and since a `char` is a byte, it conveniently gives us
the total size of the array.

Of course, like I keep saying, the string returned from `fgets()`
probably has a newline at the end that you might not want. You can write
a short function to chop the newline off---in fact, let's just roll that
into our own version of `gets()`

``` {.c}
#include <stdio.h>
#include <string.h>

char *ngets(char *s, int size)
{
    char *rv = fgets(s, size, stdin);

    if (rv == NULL)
        return NULL;

    char *p = strchr(s, '\n');  // Find a newline

    if (p != NULL)  // if there's a newline
        *p = '\0';  // truncate the string there

    return s;
}
```

So, in summary, use `fgets()` to read a line of text from the keyboard
or a file, and don't use `gets()`.

### Return Value {.unnumbered .unlisted}

Both `gets()` and `fgets()` return a pointer to the string passed.

On error or end-of-file, the functions return `NULL`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    char s[100];

    gets(s); // read from standard input (don't use this--use fgets()!)

    fgets(s, sizeof s, stdin); // read 100 bytes from standard input

    fp = fopen("spoon.txt", "r"); // (you should error-check this)
    fgets(s, 100, fp); // read 100 bytes from the file datafile.dat
    fclose(fp);

    fgets(s, 20, stdin); // read a maximum of 20 bytes from stdin
}
```

### See Also {.unnumbered .unlisted}

[`getc()`](#man-getc),
[`fgetc()`](#man-getc),
[`getchar()`](#man-getc),
[`puts()`](#man-puts),
[`fputs()`](#man-puts),
[`ungetc()`](#man-ungetc)

[[manbreak]]
## `putc()`, `fputc()`, `putchar()` {#man-putc}

Write a single character to the console or to a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int putc(int c, FILE *stream);

int fputc(int c, FILE *stream);

int putchar(int c);
```

### Description {.unnumbered .unlisted}

All three functions output a single character, either to the console or
to a `FILE`.

`putc()` takes a character argument, and outputs it to the specified
`FILE`. `fputc()` does exactly the same thing, and differs from `putc()`
in implementation only. Most people use `fputc()`.

`putchar()` writes the character to the console, and is the same as
calling `putc(c, stdout)`.

### Return Value {.unnumbered .unlisted}

All three functions return the character written on success, or `EOF` on
error.

### Example {.unnumbered .unlisted}

Print the alphabet:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    char i;

    for(i = 'A'; i <= 'Z'; i++)
        putchar(i);

    putchar('\n'); // put a newline at the end to make it pretty
}
```

### See Also {.unnumbered .unlisted}

[[manbreak]]
## `puts()`, `fputs()` {#man-puts}

Write a string to the console or to a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int puts(const char *s);

int fputs(const char *s, FILE *stream);
```

### Description {.unnumbered .unlisted}

Both these functions output a NUL-terminated string. `puts()` outputs to
the console, while `fputs()` allows you to specify the file for output.

### Return Value {.unnumbered .unlisted}

Both functions return non-negative on success, or `EOF` on error.

### Example {.unnumbered .unlisted}

Read strings from the console and save them in a file:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;
    char s[100];

    fp = fopen("somefile.txt", "w"); // error check this!

    while(fgets(s, sizeof(s), stdin) != NULL) { // read a string
        fputs(s, fp);  // write it to the file we opened
    }

    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[[manbreak]]
## `ungetc()` {#man-ungetc}

Pushes a character back into the input stream

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int ungetc(int c, FILE *stream);
```

### Description {.unnumbered .unlisted}

You know how `getc()` reads the next character from a file stream? Well,
this is the opposite of that---it pushes a character back into the file
stream so that it will show up again on the very next read from the
stream, as if you'd never gotten it from `getc()` in the first place.

Why, in the name of all that is holy would you want to do that? Perhaps
you have a stream of data that you're reading a character at a time, and
you won't know to stop reading until you get a certain character, but
you want to be able to read that character again later. You can read the
character, see that it's what you're supposed to stop on, and then
`ungetc()` it so it'll show up on the next read.

Yeah, that doesn't happen very often, but there we are.

Here's the catch: the standard only guarantees that you'll be able to
push back _one character_. Some implementations might allow you to push
back more, but there's really no way to tell and still be portable.

### Return Value {.unnumbered .unlisted}

On success, `ungetc()` returns the character you passed to it. On
failure, it returns `EOF`.

### Example {.unnumbered .unlisted}

This example reads a piece of punctuation, then everything after it up
to the next piece of punctuation. It returns the leading punctuation,
and stores the rest in a string.

``` {.c .numberLines}
#include <stdio.h>
#include <ctype.h>

int read_punctstring(FILE *fp, char *s)
{
    int origpunct, c;
    
    origpunct = fgetc(fp);

    if (origpunct == EOF)  // return EOF on end-of-file
        return EOF;

    while (c = fgetc(fp), !ispunct(c) && c != EOF)
        *s++ = c;  // save it in the string

    *s = '\0'; // nul-terminate the string

    // if we read punctuation last, ungetc it so we can fgetc it next
    // time:
    if (ispunct(c))
        ungetc(c, fp);

    return origpunct;
}

int main(void)
{
    char s[128];
    char c;

    while((c = read_punctstring(stdin, s)) != EOF) {
        printf("%c: %s\n", c, s);
    }
}
```

Sample Input:

``` {.default}
!foo#bar*baz
```

Sample output:

``` {.default}
!: foo
#: bar
*: baz
```

### See Also {.unnumbered .unlisted}

[`fgetc()`](#man-getc)

[[manbreak]]
## `fread()` {#man-fread}

Read binary data from a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

size_t fread(void *p, size_t size, size_t nmemb, FILE *stream);
```

### Description {.unnumbered .unlisted}

You might remember that you can call [`fopen()`](#man-fopen) with the
"`b`" flag in the open mode string to open the file in "binary" mode.
Files open in not-binary (ASCII or text mode) can be read using standard
character-oriented calls like [`fgetc()`](#man-getc) or
[`fgets()`](#man-gets). Files open in binary mode are typically read
using the `fread()` function.

All this function does is says, "Hey, read this many things where each
thing is a certain number of bytes, and store the whole mess of them in
memory starting at this pointer."

This can be very useful, believe me, when you want to do something like
store 20 `int`s in a file.

But wait---can't you use [`fprintf()`](#man-printf) with the "`%d`" format
specifier to save the `int`s to a text file and store them that way?
Yes, sure. That has the advantage that a human can open the file and
read the numbers. It has the disadvantage that it's slower to convert
the numbers from `int`s to text and that the numbers are likely to take
more space in the file. (Remember, an `int` is likely 4 bytes, but the
string "12345678" is 8 bytes.)

So storing the binary data can certainly be more compact and faster to
read.

### Return Value {.unnumbered .unlisted}

This function returns the number of items successfully read. If all
requested items are read, the return value will be equal to that of the
parameter `nmemb`. If EOF occurs, the return value will be zero.

To make you confused, it will also return zero if there's an error. You
can use the functions [`feof()`](#man-feof) or [`ferror()`](#man-feof)
to tell which one really happened.

### Example {.unnumbered .unlisted}

Read 10 numbers from a file and store them in an array:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int i;
    int n[10]
    FILE *fp;

    fp = fopen("numbers.dat", "rb");
    fread(n, sizeof(int), 10, fp);  // read 10 ints
    fclose(fp);

    // print them out:
    for(i = 0; i < 10; i++)
        printf("n[%d] == %d\n", i, n[i]);
}
```

### See Also {.unnumbered .unlisted}

[`fopen()`](#man-fopen),
[`fwrite()`](#man-fwrite),
[`feof()`](#man-feof),
[`ferror()`](#man-feof)

[[manbreak]]
## `fwrite()` {#man-fwrite}

Write binary data to a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

size_t fwrite(const void *p, size_t size, size_t nmemb, FILE *stream);
```

### Description {.unnumbered .unlisted}

This is the counterpart to the [`fread()`](#man-fread) function. It
writes blocks of binary data to disk. For a description of what this
means, see the entry for [`fread()`](#man-fread).

### Return Value {.unnumbered .unlisted}

`fwrite()` returns the number of items successfully written, which
should hopefully be `nmemb` that you passed in. It'll return zero on
error.

### Example {.unnumbered .unlisted}

Save 10 random numbers to a file:

``` {.c .numberLines}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i;
    int n[10];
    FILE *fp;

    // populate the array with random numbers:
    for(i = 0; i < 10; i++) {
        n[i] = rand();
        printf("n[%d] = %d\n", i, n[i]);
    }

    // save the random numbers (10 ints) to the file
    fp = fopen("numbers.dat", "wb");
    fwrite(n, sizeof(int), 10, fp); // write 10 ints
    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[`fopen()`](#man-fopen),
[`fread()`](#man-fread)

[[manbreak]]
## `fgetpos()`, `fsetpos()` {#man-fgetpos}

Get the current position in a file, or set the current position in a
file. Just like `ftell()` and `fseek()` for most systems

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int fgetpos(FILE *stream, fpos_t *pos);

int fsetpos(FILE *stream, fpos_t *pos);
```

### Description {.unnumbered .unlisted}

These functions are just like `ftell()` and `fseek()`, except instead of
counting in bytes, they use an _opaque_ data structure to hold
positional information about the file. (Opaque, in this case, means
you're not supposed to know what the data type is made up of.)

On virtually every system (and certainly every system that I know of),
people don't use these functions, using `ftell()` and `fseek()` instead.
These functions exist just in case your system can't remember file
positions as a simple byte offset.

Since the `pos` variable is opaque, you have to assign to it
using the `fgetpos()` call itself. Then you save the value
for later and use it to reset the position using
`fsetpos()`.

### Return Value {.unnumbered .unlisted}

Both functions return zero on success, and `-1` on error.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    char s[100];
    fpos_t pos;
    FILE *fp;

    fp = fopen("spoon.txt", "r");

    fgets(s, sizeof(s), fp); // read a line from the file
    printf("%s", s);

    fgetpos(fp, &pos);   // save the position after the read

    fgets(s, sizeof(s), fp); // read another line from the file
    printf("%s", s);

    fsetpos(fp, &pos);   // now restore the position to where we saved

    fgets(s, sizeof(s), fp); // read the earlier line again
    printf("%s", s);

    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[`fseek()`](#man-fseek),
[`ftell()`](#man-ftell),
[`rewind()`](#man-fseek)

[[manbreak]]
## `fseek()`, `rewind()` {#man-fseek}

Position the file pointer in anticipition of the next read or write

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int fseek(FILE *stream, long offset, int whence);

void rewind(FILE *stream);
```

### Description {.unnumbered .unlisted}

When doing reads and writes to a file, the OS keeps track of where you
are in the file using a counter generically known as the file pointer.
You can reposition the file pointer to a different point in the file
using the `fseek()` call. Think of it as a way to randomly access you
file.

The first argument is the file in question, obviously. `offset` argument
is the position that you want to seek to, and `whence` is what that
offset is relative to.

Of course, you probably like to think of the offset as being from the
beginning of the file. I mean, "Seek to position 3490, that should be
3490 bytes from the beginning of the file."  Well, it _can_ be, but it
doesn't have to be. Imagine the power you're wielding here. Try to
command your enthusiasm.

You can set the value of `whence` to one of three things:

|`whence`|Description|
|--------|---------------------------------------------|
|`SEEK_SET`|`offset` is relative to the beginning of the file. This is probably what you had in mind anyway, and is the most commonly used value for `whence`.|
|`SEEK_CUR`|`offset` is relative to the current file pointer position. So, in effect, you can say, "Move to my current position plus 30 bytes," or, "move to my current position minus 20 bytes."|
|`SEEK_END`|`offset` is relative to the end of the file. Just like `SEEK_SET` except from the other end of the file. Be sure to use negative values for `offset` if you want to back up from the end of the file, instead of going past the end into oblivion.|

Speaking of seeking off the end of the file, can you do it? Sure thing.
In fact, you can seek way off the end and then write a character; the
file will be expanded to a size big enough to hold a bunch of zeros way
out to that character.

Now that the complicated function is out of the way, what's this
`rewind()` that I briefly mentioned? It repositions the file pointer at
the beginning of the file:

``` {.c}
fseek(fp, 0, SEEK_SET); // same as rewind()
rewind(fp);             // same as fseek(fp, 0, SEEK_SET)
```

### Return Value {.unnumbered .unlisted}

For `fseek()`, on success zero is returned; `-1` is returned on failure.

The call to `rewind()` never fails.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    FILE *fp;

    fp = fopen("spoon.txt", "r");

    fseek(fp, 100, SEEK_SET); // seek to the 100th byte of the file
    printf("100: %c\n", fgetc(fp));

    fseek(fp, -31, SEEK_CUR); // seek backward 30 bytes from the current pos
    printf("31 back: %c\n", fgetc(fp));

    fseek(fp, -12, SEEK_END); // seek to the 10th byte before the end of file
    printf("12 from end: %c\n", fgetc(fp));

    fseek(fp, 0, SEEK_SET);   // seek to the beginning of the file
    rewind(fp);               // seek to the beginning of the file, too
    printf("Beginning: %c\n", fgetc(fp));

    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[`ftell()`](#man-ftell),
[`fgetpos()`](#man-fgetpos),
[`fsetpos()`](#man-fgetpos)

[[manbreak]]
## `ftell()` {#man-ftell}

Tells you where a particular file is about to read from or write to

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

long ftell(FILE *stream);
```

### Description {.unnumbered .unlisted}

This function is the opposite of [`fseek()`](#man-fseek). It tells you
where in the file the next file operation will occur relative to the
beginning of the file.

It's useful if you want to remember where you are in the file, `fseek()`
somewhere else, and then come back later. You can take the return value
from `ftell()` and feed it back into `fseek()` (with `whence` parameter
set to `SEEK_SET`) when you want to return to your previous position.

### Return Value {.unnumbered .unlisted}

Returns the current offset in the file, or `-1` on error.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    char c[6];
    FILE *fp;

    fp = fopen("spoon.txt", "r");

    long pos;

    // seek ahead 10 bytes:
    fseek(fp, 10, SEEK_SET);

    // store the current position in variable "pos":
    pos = ftell(fp);

    // Read some bytes
    fread(c, sizeof c  - 1, 1, fp);
    c[5] = '\0';
    printf("Read: \"%s\"\n", c);

    // and return to the starting position, stored in "pos":
    fseek(fp, pos, SEEK_SET);

    // Read the same bytes again
    fread(c, sizeof c  - 1, 1, fp);
    c[5] = '\0';
    printf("Read: \"%s\"\n", c);

    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[`fseek()`](#man-fseek),
[`rewind()`](#man-fseek),
[`fgetpos()`](#man-fgetpos),
[`fsetpos()`](#man-fgetpos)

[[manbreak]]
## `feof()`, `ferror()`, `clearerr()` {#man-feof}

Determine if a file has reached end-of-file or if an error has occurred

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>

int feof(FILE *stream);

int ferror(FILE *stream);

void clearerr(FILE *stream);
```

### Description {.unnumbered .unlisted}

Each `FILE*` that you use to read and write data from and to a file
contains flags that the system sets when certain events occur. If you
get an error, it sets the error flag; if you reach the end of the file
during a read, it sets the EOF flag. Pretty simple really.

The functions `feof()` and `ferror()` give you a simple way to test
these flags: they'll return non-zero (true) if they're set.

Once the flags are set for a particular stream, they stay that way until
you call `clearerr()` to clear them.

### Return Value {.unnumbered .unlisted}

`feof()` and `ferror()` return non-zero (true) if the file has reached
EOF or there has been an error, respectively.

### Example {.unnumbered .unlisted}

Read binary data, checking for EOF or error:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    int a;
    FILE *fp;

    fp = fopen("numbers.dat", "r");

    // read single ints at a time, stopping on EOF or error:

    while(fread(&a, sizeof(int), 1, fp), !feof(fp) && !ferror(fp)) {
        printf("Read %d\n", a);
    }

    if (feof(fp))
        printf("End of file was reached.\n");

    if (ferror(fp))
        printf("An error occurred.\n");

    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[`fopen()`](#man-fopen),
[`fread()`](#man-fread)

[[manbreak]]
## `perror()` {#man-perror}

Print the last error message to `stderr`

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>
#include <errno.h>  // only if you want to directly use the "errno" var

void perror(const char *s);
```

### Description {.unnumbered .unlisted}

Many functions, when they encounter an error condition for whatever
reason, will set a global variable called `errno` (in `<errno.h>`) for
you. `errno` is just an interger representing a unique error.

But to you, the user, some number isn't generally very useful. For this
reason, you can call `perror()` after an error occurs to print what
error has actually happened in a nice human-readable string.

And to help you along, you can pass a parameter, `s`, that will be
prepended to the error string for you.

One more clever trick you can do is check the value of the `errno` (you
have to include `errno.h` to see it) for specific errors and have your
code do different things. Perhaps you want to ignore certain errors but
not others, for instance.

The standard only defines three values for `errno`, but your system
undoubtedly defines more. The three that are defined are:

|`errno`|Description|
|-|-|
|`EDOM`|Math operation outside domain.|
|`EILSEQ`|Invalid sequence in multibyte to wide character encoding.|
|`ERANGE`|Result of operation doesn't fit in specified type.|

The catch is that different systems define different values for `errno`,
so it's not very portable beyond the above 3. The good news is that at
least the values are _largely_ portable between Unix-like systems, at
least.

### Return Value {.unnumbered .unlisted}

Returns nothing at all! Sorry!

### Example {.unnumbered .unlisted}

[`fseek()`](#man-fseek) returns `-1` on error, and sets `errno`, so
let's use it. Seeking on `stdin` makes no sense, so it should generate
an error:

``` {.c .numberLines}
#include <stdio.h>
#include <errno.h> // must include this to see "errno" in this example

int main(void)
{
    if (fseek(stdin, 10L, SEEK_SET) < 0)
        perror("fseek");

    fclose(stdin); // stop using this stream

    if (fseek(stdin, 20L, SEEK_CUR) < 0) {

        // specifically check errno to see what kind of
        // error happened...this works on Linux, but your
        // mileage may vary on other systems!

        if (errno == EBADF) {
            perror("fseek again, EBADF");
        } else {
            perror("fseek again");
        }
    }
}
```

And the output is:

``` {.default}
fseek: Illegal seek
fseek again, EBADF: Bad file descriptor
```

### See Also {.unnumbered .unlisted}

[`feof()`](#man-feof),
[`ferror()`](#man-feof),
[`strerror()`](#man-strerror)


<!--
[[manbreak]]
## `vprintf()`, `vfprintf()`, `vsprintf()`, `vsnprintf()` {#man-vprintf}

### Synopsis {.unnumbered .unlisted}
### Description {.unnumbered .unlisted}
### Return Value {.unnumbered .unlisted}
### Example {.unnumbered .unlisted}
### See Also {.unnumbered .unlisted}
[`sprintf()`](#man-printf),
-->
