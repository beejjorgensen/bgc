<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<wchar.h>` Wide Character Handling {#wchar}

These are the wide character variants of the functions found in
[`<stdio.h>`](#man-stdio).

Remember that you can't mix-and-match multibyte output functions (like
[`printf()`](#man-printf)) with wide character output functions (like
[`wprintf()`](#man-wprintf)). The output stream has an
[_orientation_](#io-stream-orientation) to either multibyte or wide that
gets set on the first I/O call to that stream. (Or it can be set with
[`fwide()`](#man-fwide).)

So choose one or the other and stick with it.

And you can specify wide character constants and string literals by
prefixing `L` to the front of it:

``` {.c}
wchar_t *s = L"Hello, world!";
wchar_t c = L'B';
```

This header also introduces a type `wint_t` that is used by the
character I/O functions. It's a type that can hold any single wide
character, but _also_ the macro `WEOF` to indicate wide end-of-file.

[[pagebreak]]
## `wprintf()`, `fwprintf()`, `swprintf()` {#man-wprintf}

Formatted output with a wide string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>   // For fwprintf()
#include <wchar.h>

int wprintf(const wchar_t * restrict format, ...);

int fwprintf(FILE * restrict stream, const wchar_t * restrict format, ...);

int swprintf(wchar_t * restrict s, size_t n,
             const wchar_t * restrict format, ...);
```

### Description {.unnumbered .unlisted}

These are the wide versions of [`printf()`](#man-printf),
[`fprintf()](#man-printf), and [`sprintf()`](#man-printf).

See those pages for exact substantial usage.

These are the same except the `format` string is a wide character
string instead of a multibyte string.

And that `swprintf()` is analogous to `snprintf()` in that they both
take the size of the destination array as an argument.

And one more thing: the precision specified for a `%s` specifier
corresponds to the number of wide characters printed, not the number of
bytes. If you know of other difference, let me know.

### Return Value {.unnumbered .unlisted}

Returns the number of wide characters outputted, or `-1` if there's an
error.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    char *mbs = "multibyte";
    wchar_t *ws = L"wide";

    wprintf(L"We're all wide for %s and %ls!\n", mbs, ws);

    double pi = 3.14159265358979;
    wprintf(L"pi = %f\n", pi);
}
```

Output:

```
We're all wide for multibyte and wide!
pi = 3.141593
```

### See Also {.unnumbered .unlisted}

[`printf()`](#man-printf),
[`vwprintf()`](#man-vwprintf)

[[pagebreak]]
## `wscanf()` `fwscanf()` `swscanf()` {#man-wscanf}

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>  // for fwscanf()
#include <wchar.h>

int wscanf(const wchar_t * restrict format, ...);

int fwscanf(FILE * restrict stream, const wchar_t * restrict format, ...);

int swscanf(const wchar_t * restrict s, const wchar_t * restrict format, ...);
```

### Description {.unnumbered .unlisted}

These are the wide variants of [`scanf()`](#man-scanf),
[`fscanf()`](#man-scanf), and [`sscanf()`](#man-scanf).

See the [`scanf()`](#man-scanf) page for all the details.

### Return Value {.unnumbered .unlisted}

Returns the number of items successfully scanned, or `EOF` on some kind
of input failure.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    int quantity;
    wchar_t item[100];

    wprintf(L"Enter \"quantity: item\"\n");
    
    if (wscanf(L"%d:%99ls", &quantity, item) != 2)
        wprintf(L"Malformed input!\n");
    else
        wprintf(L"You entered: %d %ls\n", quantity, item);
}
```

Output (input of `12: apples`):

```
Enter "quantity: item"
12: apples
You entered: 12 apples
```

### See Also {.unnumbered .unlisted}

[`scanf()`](#man-scanf),
[`vwscanf()`](#man-vwscanf)

[[pagebreak]]
## `vwprintf()` `vfwprintf()` `vswprintf()` {#man-vwprintf}

`wprintf()` variants using variable argument lists (`va_list`)

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>   // For vfwprintf()
#include <stdarg.h>
#include <wchar.h>

int vwprintf(const wchar_t * restrict format, va_list arg);

int vswprintf(wchar_t * restrict s, size_t n,
              const wchar_t * restrict format, va_list arg); 

int vfwprintf(FILE * restrict stream, const wchar_t * restrict format,
              va_list arg);
```

### Description {.unnumbered .unlisted}

These functions are the wide character variants of the
[`vprintf()`](#man-vprintf), functions. You can [refer to that reference
page](#man-vprintf) for more details.

### Return Value {.unnumbered .unlisted}

Returns the number of wide characters stored, or a negative value on
error.

### Example {.unnumbered .unlisted}

In this example, we make our own version of `wprintf()` called
`wlogger()` that timestamps output. Notice how the calls to `wlogger()`
have all the bells and whistles of `wprintf()`.

``` {.c .numberLines}
#include <stdarg.h>
#include <wchar.h>
#include <time.h>

int wlogger(wchar_t *format, ...)
{
    va_list va;
    time_t now_secs = time(NULL);
    struct tm *now = gmtime(&now_secs);

    // Output timestamp in format "YYYY-MM-DD hh:mm:ss : "
    wprintf(L"%04d-%02d-%02d %02d:%02d:%02d : ",
        now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
        now->tm_hour, now->tm_min, now->tm_sec);

    va_start(va, format);
    int result = vwprintf(format, va);
    va_end(va);

    wprintf(L"\n");

    return result;
}

int main(void)
{
    int x = 12;
    float y = 3.2;

    wlogger(L"Hello!");
    wlogger(L"x = %d and y = %.2f", x, y);
}
```

Output:

```
2021-03-30 04:25:49 : Hello!
2021-03-30 04:25:49 : x = 12 and y = 3.20
```

### See Also {.unnumbered .unlisted}

[`printf()`](#man-printf),
[`vprintf()`](#man-vprintf)


[[pagebreak]]
## `vwscanf()`, `vfwscanf()`, `vswscanf()` {#man-vwscanf}

`wscanf()` variants using variable argument lists (`va_list`)

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>   // For vfwscanf()
#include <stdarg.h>
#include <wchar.h>

int vwscanf(const wchar_t * restrict format, va_list arg);

int vfwscanf(FILE * restrict stream, const wchar_t * restrict format,
             va_list arg); 

int vswscanf(const wchar_t * restrict s, const wchar_t * restrict format,
             va_list arg);
```

### Description {.unnumbered .unlisted}

These are the wide counterparts to the [`vscanf()`](#man-vscanf)
collection of functions. See their [reference page for
details](#man-vscanf).

### Return Value {.unnumbered .unlisted}

Returns the number of items successfully scanned, or `EOF` on some kind
of input failure.

### Example {.unnumbered .unlisted}

I have to admit I was wracking my brain to think of when you'd ever want
to use this. The best example I could find was [fl[one on Stack
Overflow|https://stackoverflow.com/questions/17017331/c99-vscanf-for-dummies/17018046#17018046]]
that error-checks the return value from `scanf()` against the expected.
A variant of that is shown below.

``` {.c .numberLines}
#include <stdarg.h>
#include <wchar.h>
#include <assert.h>

int error_check_wscanf(int expected_count, wchar_t *format, ...)
{
    va_list va;

    va_start(va, format);
    int count = vwscanf(format, va);
    va_end(va);

    // This line will crash the program if the condition is false:
    assert(count == expected_count);

    return count;
}

int main(void)
{
    int a, b;
    float c;

    error_check_wscanf(3, L"%d, %d/%f", &a, &b, &c);
    error_check_wscanf(2, L"%d", &a);
}
```

### See Also {.unnumbered .unlisted}

[`wscanf()`](#man-wscanf)

[[pagebreak]]
## `getwc()` `fgetwc()` `getwchar()` {#man-getwc}

Get a wide character from an input stream

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>   // For getwc() and fgetwc()
#include <wchar.h>

wint_t getwchar(void);

wint_t getwc(FILE *stream);

wint_t fgetwc(FILE *stream);
```

### Description {.unnumbered .unlisted}

These are the wide variants of [`fgetc()`](#man-getc).

`fgetwc()` and `getwc()` are identical except that `getwc()` might be
implemented as a macro and is allowed to evaluate `stream` multiple
times.

`getwchar()` is identical to `getwc()` with `stream` set to `stdin`.

I don't know why you'd ever use `getwc()` instead of `fgetwc()`, but if
anyone knows, drop me a line.

### Return Value {.unnumbered .unlisted}

Returns the next wide character in the input stream. Return `WEOF` on
end-of-file or error.

If an I/O error occurs, the error flag is also set on the stream.

If an invalid byte sequence is encountered, `errno` is set to `ILSEQ`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
// read all characters from a file, outputting only the letter 'b's
// it finds in the file

#include <stdio.h>
#include <wchar.h>

int main(void)
{
    FILE *fp;
    wint_t c;

    fp = fopen("datafile.txt", "r"); // error check this!

    // this while-statement assigns into c, and then checks against EOF:

    while((c = fgetc(fp)) != WEOF) 
        if (c == L'b')
            fputwc(c, stdout);

    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[`fputwc`](#man-fputwc),
[`fgetws`](#man-fgetws),
[`errno`](#errno)

[[pagebreak]]
## `fgetws()` {#man-fgetws}

Read a wide string from console or file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>
#include <wchar.h>

wchar_t *fgetws(wchar_t * restrict s, int n, FILE * restrict stream);
```

### Description {.unnumbered .unlisted}

This is the wide version of [`fgets()`](#man-gets).  See [its reference
page for details](#man-gets).

A wide `NUL` character is used to terminate the string.

### Return Value {.unnumbered .unlisted}

Returns `s` on success, or a `NULL` pointer on end-of-file or error.

### Example {.unnumbered .unlisted}

The following example reads lines from a file and prepends them with
numbers:

``` {.c .numberLines}
#include <stdio.h>
#include <wchar.h>

#define BUF_SIZE 1024

int main(void)
{
    FILE *fp;
    wchar_t buf[BUF_SIZE];

    fp = fopen("textfile.txt", "r"); // error check this!

    int line_count = 0;

    while ((fgetws(buf, BUF_SIZE, fp)) != NULL) 
        wprintf(L"%04d: %ls", ++line_count, buf);

    fclose(fp);
}
```

Example output for a file with these lines in them (without the
prepended numbers):

```
0001: line 1
0002: line 2
0003: something
0004: line 4
```

### See Also {.unnumbered .unlisted}

[`fgetwc()`](#man-fgetwc),
[`fgets()`](#man-gets)

[[pagebreak]]
## `putwchar()` `putwc()` `fputwc()` {#man-putwc}

Write a single wide character to the console or to a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>   // For putwc() and fputwc()
#include <wchar.h>

wint_t putwchar(wchar_t c);

wint_t putwc(wchar_t c, FILE *stream);

wint_t fputwc(wchar_t c, FILE *stream);
```

### Description {.unnumbered .unlisted}

These are the wide character equivalents to the ['fputc()'](#man-putc)
group of functions. You can find more information ['in that reference
section'](#man-putc).

`fputwc()` and `putwc()` are identical except that `putwc()` might be
implemented as a macro and is allowed to evaluate `stream` multiple
times.

`putwchar()` is identical to `putwc()` with `stream` set to `stdin`.

I don't know why you'd ever use `putwc()` instead of `fputwc()`, but if
anyone knows, drop me a line.

### Return Value {.unnumbered .unlisted}

Returns the wide character written, or `WEOF` on error.

If it's an I/O error, the error flag will be set for the stream.

If it's an encoding error, `errno` will be set to `EILSEQ`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
// read all characters from a file, outputting only the letter 'b's
// it finds in the file

#include <stdio.h>
#include <wchar.h>

int main(void)
{
    FILE *fp;
    wint_t c;

    fp = fopen("datafile.txt", "r"); // error check this!

    // this while-statement assigns into c, and then checks against EOF:

    while((c = fgetc(fp)) != WEOF) 
        if (c == L'b')
            fputwc(c, stdout);

    fclose(fp);
}
```

### See Also {.unnumbered .unlisted}

[`fgetwc()`](#man-getwc),
[`fputc()`](#man-putc),
[`errno`](#errno)

[[pagebreak]]
## `fputws()` {#man-example}

Write a wide string to the console or to a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>
#include <wchar.h>

int fputws(const wchar_t * restrict s, FILE * restrict stream);
```

### Description {.unnumbered .unlisted}

This is the wide version of [`fputs()`](#man-fputs).

Pass in a wide string and an output stream, and it will so be written.

### Return Value {.unnumbered .unlisted}

Returns a non-negative value on success, or `EOF` on error.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    fputws(L"Hello, world!\n", stdout);
}
```

### See Also {.unnumbered .unlisted}

[`fputwc()`](#man-putwc)
[`fputs()`](#man-puts)

<!--
[[pagebreak]]
## `example()` {#man-example}

### Synopsis {.unnumbered .unlisted}

``` {.c}
```

### Description {.unnumbered .unlisted}

### Return Value {.unnumbered .unlisted}

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
```

### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->
