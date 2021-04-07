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

[[pagebreak]]
## `fwide()` {#man-fwide}

Get or set the orientation of the stream

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>
#include <wchar.h>

int fwide(FILE *stream, int mode);
```

### Description {.unnumbered .unlisted}

Streams can be either wide-oriented (meaning the wide functions are in
use) or byte-oriented (that the regular multibyte functions are in use).
Or, before an orientation is chosen, unoriented.

There are two ways to set the orientation of an unoriented stream:

* Implicitly: just use a function like `printf()` (byte oriented) or
  `wprintf()` (wide oriented), and the orientation will be set.

* Explicitly: use this function to set it.

You can set the orientation for the stream by passing different numbers
to `mode`:

|`mode`|Description|
|-|-|
|`0`|Do not alter the orientation|
|`-1`|Set stream to byte-oriented|
|`1`|Set stream to wide-oriented|

(I said `-1` and `1` there, but really it could be any positive or
negative number.)

Most people choose the wide or byte functions (`printf()` or
`wprintf()`) and just start using them and never use `fwide()` to set
the orientation.

And once the orientation is set, you can't change it. So you can't use
`fwide()` for that, either.

So what can you use it for?

You can _test_ to see what orientation a stream is in by passing `0` as
the `mode` and checking the return value.

### Return Value {.unnumbered .unlisted}

Returns greater than zero if the stream is wide-oriented.

Returns less than zero if the stream is byte-oriented.

Returns zero if the stream is unoriented.

### Example {.unnumbered .unlisted}

Example setting to byte-oriented:

``` {.c .numberLines}
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    printf("Hello world!\n");  // Implicitly set to byte

    int mode = fwide(stdout, 0);

    printf("Stream is %s-oriented\n", mode < 0? "byte": "wide");
}
```

Output:

```
Hello world!
Stream is byte-oriented
```

Example setting to wide-oriented:

``` {.c .numberLines}
#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wprintf(L"Hello world!\n");  // Implicitly set to wide

    int mode = fwide(stdout, 0);

    wprintf(L"Stream is %ls-oriented\n", mode < 0? L"byte": L"wide");
}
```

Output:

```
Hello world!
Stream is wide-oriented
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[pagebreak]]

## `ungetwc()` {#man-ungetwc}

Pushes a wide character back into the input stream

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>
#include <wchar.h>

wint_t ungetwc(wint_t c, FILE *stream);
```

### Description {.unnumbered .unlisted}

This is the wide character variant of [`ungetc()`](#man-ungetc).

It performs the reverse operation of [`fgetwc()`](#man-getwc), pushing a
character back on the input stream.

The spec guarantees you can do this one time in a row. You can probably
do it more times, but it's up to the implementation. If you do too many
calls without an intervening read, an error could be returned.

Setting the file position discards any characters pushed by `ungetwc()`
without being subsequently read.

The end-of-file flag is cleared after a successful call.

### Return Value {.unnumbered .unlisted}

Returns the value of the pushed character on success, or `WEOF` on failure.

### Example {.unnumbered .unlisted}

This example reads a piece of punctuation, then everything after it up
to the next piece of punctuation. It returns the leading punctuation,
and stores the rest in a string.

``` {.c .numberLines}
#include <stdio.h>
#include <wctype.h>
#include <wchar.h>

wint_t read_punctstring(FILE *fp, wchar_t *s)
{
    wint_t origpunct, c;
    
    origpunct = fgetwc(fp);

    if (origpunct == WEOF)  // return EOF on end-of-file
        return WEOF;

    while (c = fgetwc(fp), !iswpunct(c) && c != WEOF)
        *s++ = c;  // save it in the string

    *s = L'\0'; // nul-terminate the string

    // if we read punctuation last, ungetc it so we can fgetc it next
    // time:
    if (iswpunct(c))
        ungetwc(c, fp);

    return origpunct;
}

int main(void)
{
    wchar_t s[128];
    wint_t c;

    while ((c = read_punctstring(stdin, s)) != WEOF) {
        wprintf(L"%lc: %ls\n", c, s);
    }
}
```

Sample Input:

```
!foo#bar*baz
```

Sample output:

```
!: foo
#: bar
*: baz
```

### See Also {.unnumbered .unlisted}

[`fgetwc()`](#man-getwc),
[`ungetc()`](#man-ungetc)

[[pagebreak]]
## `wcstod()` `wcstof()` `wcstold()` {#man-wcstod}

Convert a wide string to a floating point number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

double wcstod(const wchar_t * restrict nptr, wchar_t ** restrict endptr);

float wcstof(const wchar_t * restrict nptr, wchar_t ** restrict endptr);

long double wcstold(const wchar_t * restrict nptr, wchar_t ** restrict endptr);
```

### Description {.unnumbered .unlisted}

These are the wide counterparts to the [`strtod()`](#man-strtod) family
of functions. See [their reference pages for details](#man-strtod).

### Return Value {.unnumbered .unlisted}

Returns the string converted to a floating point value.

Returns `0` if there's no valid number in the string.

On overflow, returns an apporpriately-signed `HUGE_VAL`, `HUGE_VALF`. or
`HUGE_VALL` depending on the return type, and `errno` is set to
`ERANGE`.

On underflow, returns a number no greater than the smallest normalized
positive number, appropriately signed. The implemention _might_ set
`errno` to `ERANGE`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    wchar_t *inp = L"   123.4567beej";
    wchar_t *badchar;

    double val = wcstod(inp, &badchar);

    wprintf(L"Converted string to %f\n", val);
    wprintf(L"Encountered bad characters: %ls\n", badchar);

    val = wcstod(L"987.654321beej", NULL);
    wprintf(L"Ignoring bad chars: %f\n", val);

    val = wcstod(L"11.2233", &badchar);

    if (*badchar == L'\0')
        wprintf(L"No bad chars: %f\n", val);
    else
        wprintf(L"Found bad chars: %f, %ls\n", val, badchar);
}
```

Output:

```
Converted string to 123.456700
Encountered bad characters: beej
Ignoring bad chars: 987.654321
No bad chars: 11.223300
```

### See Also {.unnumbered .unlisted}

[`wcstol()`](#man-wcstol),
[`strtod()`](#man-strtod)

[[pagebreak]]
## `wcstol()` `wcstoll()` `wcstoul()` `wcstoull()` {#man-wcstol}

Convert a wide string to an integer value

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

long int wcstol(const wchar_t * restrict nptr,
                wchar_t ** restrict endptr, int base);

long long int wcstoll(const wchar_t * restrict nptr,
                      wchar_t ** restrict endptr, int base);

unsigned long int wcstoul(const wchar_t * restrict nptr,
                          wchar_t ** restrict endptr, int base);

unsigned long long int wcstoull(const wchar_t * restrict nptr,
                                wchar_t ** restrict endptr, int base);
```

### Description {.unnumbered .unlisted}

These are the wide counterparts to the [`strtol()`](#man-strtol) family
of functions, so see [their reference pages for the
details](#man-strtol).

### Return Value {.unnumbered .unlisted}

Returns the integer value of the string.

If nothing can be found, `0` is returned.

If the result is out of range, the value returned is one of `LONG_MIN`,
`LONG_MAX`, `LLONG_MIN`, `LLONG_MAX`, `ULONG_MAX` or `ULLONG_MAX`, as
appropriate. And `errno` is set to `ERANGE`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    // All output in decimal (base 10)

    wprintf(L"%ld\n", wcstol(L"123", NULL, 0));     // 123
    wprintf(L"%ld\n", wcstol(L"123", NULL, 10));    // 123
    wprintf(L"%ld\n", wcstol(L"101010", NULL, 2));  // binary, 42
    wprintf(L"%ld\n", wcstol(L"123", NULL, 8));     // octal, 83
    wprintf(L"%ld\n", wcstol(L"123", NULL, 16));    // hex, 291

    wprintf(L"%ld\n", wcstol(L"0123", NULL, 0));    // octal, 83
    wprintf(L"%ld\n", wcstol(L"0x123", NULL, 0));   // hex, 291

    wchar_t *badchar;
    long int x = wcstol(L"   1234beej", &badchar, 0);

    wprintf(L"Value is %ld\n", x);                  // Value is 1234
    wprintf(L"Bad chars at \"%ls\"\n", badchar);    // Bad chars at "beej"
}
```

Output:

```
123
123
42
83
291
83
291
Value is 1234
Bad chars at "beej"
```

### See Also {.unnumbered .unlisted}

[`wcstod()`](#man-wcstod),
[`strtol()`](#man-strtol)

[[pagebreak]]
## `wcscpy()` `wcsncpy()` {#man-wcscpy}

Copy a wide string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

wchar_t *wcscpy(wchar_t * restrict s1, const wchar_t * restrict s2);

wchar_t *wcsncpy(wchar_t * restrict s1,
                 const wchar_t * restrict s2, size_t n);
```

### Description {.unnumbered .unlisted}

These are the wide versions of [`strcpy()`](#man-strcpy) and
[`strncpy()`](#man-strcpy).

They'll copy a string up to a wide NUL. Or, in the case of the safer
`wcsncpy()`, until then or until `n` wide characters are copied.

If the string in `s1` is shorter than `n`, `wcsncpy()` will pad `s2`
with wide NUL characters until the `n`th wide character is reached.

Even though `wcsncpy()` is safer because it will never overrun the end
of `s2` (assuming you set `n` correctly), it's still unsafe a NUL is not
found in `s1` in the first `n` characters. In that case, `s2` will not
be NUL-terminated. Always make sure `n` is greater than the string
length of `s1`!

### Return Value {.unnumbered .unlisted}

Returns `s1`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    wchar_t *s1 = L"Hello!";
    wchar_t s2[10];

    wcsncpy(s2, s1, 10);

    wprintf(L"\"%ls\"\n", s2);  // "Hello!"
}
```

### See Also {.unnumbered .unlisted}

[`wmemcpy()`](#man-wmemcpy),
[`wmemmove()`](#man-wmemcpy)
[`strcpy()`](#man-strcpy),
[`strncpy()`](#man-strcpy)

[[pagebreak]]
## `wmemcpy()` `wmemmove()` {#man-wmemcpy}

Copy wide characters

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

wchar_t *wmemcpy(wchar_t * restrict s1,
                 const wchar_t * restrict s2, size_t n);

wchar_t *wmemmove(wchar_t *s1, const wchar_t *s2, size_t n);
```

### Description {.unnumbered .unlisted}

These are the wide versions of [`memcpy()`](#man-memcpy) and
[`memmove()`](#man-memcpy).

They copy `n` wide characters from `s2` to `s1`.

They're the same except that `wmemmove()` is guaranteed to work with
overlapping memory regions, and `wmemcpy()` is not.

### Return Value {.unnumbered .unlisted}

Both functions return the pointer `s1`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    wchar_t s[100] = L"Goats";
    wchar_t t[100];

    wmemcpy(t, s, 6);       // Copy non-overlapping memory

    wmemmove(s + 2, s, 6);  // Copy overlapping memory

    wprintf(L"s is \"%ls\"\n", s);
    wprintf(L"t is \"%ls\"\n", t);
}
```

Output:

```
s is "GoGoats"
t is "Goats"
```

### See Also {.unnumbered .unlisted}

[`wcscpy()`](#man-wcscpy),
[`wcsncpy()`](#man-wcscpy),
[`memcpy()`](#man-memcpy),
[`memmove()`](#man-memcpy)

[[pagebreak]]
## `wcscat()` `wcsncat()` {#man-example}

Concatenate wide strings

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

wchar_t *wcscat(wchar_t * restrict s1, const wchar_t * restrict s2);

wchar_t *wcsncat(wchar_t * restrict s1,
                 const wchar_t * restrict s2, size_t n);
```

### Description {.unnumbered .unlisted}

These are the wide variants of [`strcat()`](#man-strcat) and
[`strncat()`](#man-strcat).

They concatenate `s2` onto the end of `s1`.

They're the same except `wcsncat()` gives you the option to limit the
number of wide characters appended.

Note that `wcsncat()` always adds a NUL terminator to the end, even if
`n` characters were appended. So be sure to leave room for that.

### Return Value {.unnumbered .unlisted}

Both functions return the pointer `s1`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    wchar_t dest[30] = L"Hello";
    wchar_t *src = L", World!";
    wchar_t numbers[] = L"12345678";

    wprintf(L"dest before strcat: \"%ls\"\n", dest); // "Hello"

    wcscat(dest, src);
    wprintf(L"dest after strcat:  \"%ls\"\n", dest); // "Hello, world!"

    wcsncat(dest, numbers, 3); // strcat first 3 chars of numbers
    wprintf(L"dest after strncat: \"%ls\"\n", dest); // "Hello, world!123"
}
```

### See Also {.unnumbered .unlisted}

[`strcat()`](#man-strcat),
[`strncat()`](#man-strcat)

[[pagebreak]]
## `wcscmp()`, `wcsncmp()`, `wmemcmp()` {#man-wcscmp}

Compare wide strings or memory

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

int wcscmp(const wchar_t *s1, const wchar_t *s2);

int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);

int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n);

```

### Description {.unnumbered .unlisted}

These are the wide variants of [`memcmp()`](#man-memcmp),
[`strcmp()`](#man-strcmp), and [`strncmp()`](#man-strcmp).

`wcscmp()` and `wcsncmp()` both compare strings until a NUL character.

`wcsncmp()` also has the additional restriction that it will only
compare the first `n` characters.

`wmemcmp()` is like `wcsncmp()` except it won't stop at a NUL.

The comparison is done against the character value (which might (or
might not) be its Unicode code point).

### Return Value {.unnumbered .unlisted}

Returns zero if both regions are equal.

Returns a negative number if the region pointed to by `s1` is less than
`s2`.

Returns a positive number if the region pointed to by `s1` is greater
than `s2`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    wchar_t *s1 = L"Muffin";
    wchar_t *s2 = L"Muffin Sandwich";
    wchar_t *s3 = L"Muffin";

    wprintf(L"%d\n", wcscmp(L"Biscuits", L"Kittens")); // <0 since 'B' < 'K'
    wprintf(L"%d\n", wcscmp(L"Kittens", L"Biscuits")); // >0 since 'K' > 'B'

    if (wcscmp(s1, s2) == 0)
        wprintf(L"This won't get printed because the strings differ\n");

    if (wcscmp(s1, s3) == 0)
        wprintf(L"This will print because s1 and s3 are the same\n");

    // this is a little weird...but if the strings are the same, it'll
    // return zero, which can also be thought of as "false". Not-false
    // is "true", so (!wcscmp()) will be true if the strings are the
    // same. yes, it's odd, but you see this all the time in the wild
    // so you might as well get used to it:

    if (!wcscmp(s1, s3))
        wprintf(L"The strings are the same!\n");

    if (!wcsncmp(s1, s2, 6))
        wprintf(L"The first 6 characters of s1 and s2 are the same\n");
}
```

Output:

```
-1
1
This will print because s1 and s3 are the same
The strings are the same!
The first 6 characters of s1 and s2 are the same
```

### See Also {.unnumbered .unlisted}

[`memcmp()`](#man-memcmp),
[`strcmp()`](#man-strcmp),
[`strncmp()`](#man-strcmp)

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
