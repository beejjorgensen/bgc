<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<wchar.h>` Wide Character Handling {#wchar}

|Function|Description|
|------|-------------------------|
|[`btowc()`](#man-btowc)|Convert a single byte character to a wide character|
|[`fgetwc()`](#man-getwc)|Get a wide character from a wide stream|
|[`fgetws()`](#man-fgetws)|Read a wide string from a wide stream|
|[`fputwc()`](#man-putwc)|Write a wide character to a wide stream|
|[`fputws()`](#man-fputws)|Write a wide string to a wide stream|
|[`fwide()`](#man-fwide)|Get or set the orientation of the stream|
|[`fwprintf()`](#man-wprintf)|Formatted wide output to a wide stream|
|[`fwscanf()`](#man-wscanf)|Formatted wide input from a wide stream|
|[`getwchar()`](#man-getwc)|Get a wide character from `stdin`|
|[`getwc()`](#man-getwc)|Get a wide character from `stdin`|
|[`mbrlen()`](#man-mbrlen)|Compute the number of bytes in a multibyte character restartably|
|[`mbrtowc()`](#man-mbrtowc)|Convert multibyte to wide characters restartably|
|[`mbsinit()`](#man-mbsinit)|Test if an `mbstate_t` is in the initial conversion state|
|[`mbsrtowcs()`](#man-mbsrtowcs)|Convert a multibyte string to a wide character string restartably|
|[`putwchar()`](#man-putwc)|Write a wide character to `stdout`|
|[`putwc()`](#man-putwc)|Write a wide character to `stdout`|
|[`swprintf()`](#man-wprintf)|Formatted wide output to a wide string|
|[`swscanf()`](#man-wscanf)|Formatted wide input from a wide string|
|[`ungetwc()`](#man-ungetwc)|Pushes a wide character back into the input stream|
|[`vfwprintf()`](#man-wprintf)|Variadic formatted wide output to a wide stream|
|[`vfwscanf()`](#man-wscanf)|Variadic formatted wide input from a wide stream|
|[`vswprintf()`](#man-wprintf)|Variadic formatted wide output to a wide string|
|[`vswscanf()`](#man-wscanf)|Variadic formatted wide input from a wide string|
|[`vwprintf()`](#man-wprintf)|Variadic formatted wide output|
|[`vwscanf()`](#man-wscanf)|Variadic formatted wide input|
|[`wcscat()`](#man-wcscat)|Concatenate wide strings dangerously|
|[`wcschr()`](#man-wcschr)|Find a wide character in a wide string|
|[`wcscmp()`](#man-wcscmp)|Compare wide strings|
|[`wcscoll()`](#man-wcscoll)|Compare two wide strings accounting for locale|
|[`wcscpy()`](#man-wcscpy)|Copy a wide string dangerously|
|[`wcscspn()`](#man-wcsspn)|Count characters not from a start at the front of a wide string|
|[`wcsftime()`](#man-wcsftime)|Formatted date and time output|
|[`wcslen()`](#man-wcslen)|Returns the length of a wide string|
|[`wcsncat()`](#man-wcscat)|Concatenate wide strings more safely|
|[`wcsncmp()`](#man-wcscmp)|Compare wide strings, length limited|
|[`wcsncpy()`](#man-wcscpy)|Copy a wide string more safely|
|[`wcspbrk()`](#man-wcspbrk)|Search a wide string for one of a set of wide characters|
|[`wcsrchr()`](#man-wcschr)|Find a wide character in a wide string from the end|
|[`wcsrtombs()`](#man-wcsrtombs)|Convert a wide character string to a multibyte string restartably|
|[`wcsspn()`](#man-wcsspn)|Count characters from a set at the front of a wide string|
|[`wcsstr()`](#man-wcsstr)|Find a wide string in another wide string|
|[`wcstod()`](#man-wcstod)|Convert a wide string to a `double`|
|[`wcstof()`](#man-wcstod)|Convert a wide string to a `float`|
|[`wcstok()`](#man-wcstok)|Tokenize a wide string|
|[`wcstold()`](#man-wcstod)|Convert a wide string to a `long double`|
|[`wcstoll()`](#man-wcstol)|Convert a wide string to a `long long`|
|[`wcstol()`](#man-wcstol)|Convert a wide string to a `long`|
|[`wcstoull()`](#man-wcstol)|Convert a wide string to an `unsigned long long`|
|[`wcstoul()`](#man-wcstol)|Convert a wide string to an `unsigned long`|
|[`wcsxfrm()`](#man-wcsxfrm)|Transform a wide string for comparing based on locale|
|[`wctob()`](#man-btowc)|Convert a wide character to a single byte character|
|[`wctombr()`](#man-wcrtomb)|Convert wide to multibyte characters restartably|
|[`wmemcmp()`](#man-wcscmp)|Compare wide characters in memory|
|[`wmemcpy()`](#man-wmemcpy)|Copy wide character memory|
|[`wmemmove()`](#man-wmemcpy)|Copy wide character memory, potentially overlapping|
|[`wprintf()`](#man-wprintf)|Formatted wide output|
|[`wscanf()`](#man-wscanf)|Formatted wide input|

These are the wide character variants of the functions found in
[`<stdio.h>`](#stdio).

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

## Restartable Functions

Finally, a note on the "restartable" functions that are included here.
When conversion is happening, some encodings require C to keep track of
some _state_ about the progress of the conversion so far.

For a lot of the functions, C uses an internal variable for the state
that is shared between function calls. The problem is if you're writing
multithreaded code, this state might get trampled by other threads.

To avoid this, each thread needs to maintain its own state in a variable
of the opaque type `mbstate_t`. And the "restartable" functions allow
you to pass in this state so that each thread can use their own.

[[manbreak]]
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

``` {.default}
We're all wide for multibyte and wide!
pi = 3.141593
```

### See Also {.unnumbered .unlisted}

[`printf()`](#man-printf),
[`vwprintf()`](#man-vwprintf)

[[manbreak]]
## `wscanf()` `fwscanf()` `swscanf()` {#man-wscanf}

Scan a wide stream or wide string for formatted input

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

``` {.default}
Enter "quantity: item"
12: apples
You entered: 12 apples
```

### See Also {.unnumbered .unlisted}

[`scanf()`](#man-scanf),
[`vwscanf()`](#man-vwscanf)

[[manbreak]]
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

``` {.default}
2021-03-30 04:25:49 : Hello!
2021-03-30 04:25:49 : x = 12 and y = 3.20
```

### See Also {.unnumbered .unlisted}

[`printf()`](#man-printf),
[`vprintf()`](#man-vprintf)


[[manbreak]]
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

[[manbreak]]
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

Reads all the characters from a file, outputting only the letter 'b's it
finds in the file:

``` {.c .numberLines}
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

[`fputwc`](#man-putwc),
[`fgetws`](#man-fgetws),
[`errno`](#errno)

[[manbreak]]
## `fgetws()` {#man-fgetws}

Read a wide string from a file

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

``` {.default}
0001: line 1
0002: line 2
0003: something
0004: line 4
```

### See Also {.unnumbered .unlisted}

[`fgetwc()`](#man-getwc),
[`fgets()`](#man-gets)

[[manbreak]]
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

Read all characters from a file, outputting only the letter 'b's it
finds in the file:

``` {.c .numberLines}
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

[[manbreak]]
## `fputws()` {#man-fputws}

Write a wide string to a file

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <stdio.h>
#include <wchar.h>

int fputws(const wchar_t * restrict s, FILE * restrict stream);
```

### Description {.unnumbered .unlisted}

This is the wide version of [`fputs()`](#man-puts).

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

[[manbreak]]
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

``` {.default}
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

``` {.default}
Hello world!
Stream is wide-oriented
```

<!--
### See Also {.unnumbered .unlisted}

[`example()`](#man-example),
-->

[[manbreak]]
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

[`fgetwc()`](#man-getwc),
[`ungetc()`](#man-ungetc)

[[manbreak]]
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

``` {.default}
Converted string to 123.456700
Encountered bad characters: beej
Ignoring bad chars: 987.654321
No bad chars: 11.223300
```

### See Also {.unnumbered .unlisted}

[`wcstol()`](#man-wcstol),
[`strtod()`](#man-strtod),
[`errno`](#errno)

[[manbreak]]
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

``` {.default}
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
[`strtol()`](#man-strtol),
[`errno`](#errno),
[`wcstoimax()`](#man-wcstoimax),
[`wcstoumax()`](#man-wcstoimax)

[[manbreak]]
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

[[manbreak]]
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

``` {.default}
s is "GoGoats"
t is "Goats"
```

### See Also {.unnumbered .unlisted}

[`wcscpy()`](#man-wcscpy),
[`wcsncpy()`](#man-wcscpy),
[`memcpy()`](#man-memcpy),
[`memmove()`](#man-memcpy)

[[manbreak]]
## `wcscat()` `wcsncat()` {#man-wcscat}

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

[[manbreak]]
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

These are the wide variants of [`memcmp()`](#man-strcmp),
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

``` {.default}
-1
1
This will print because s1 and s3 are the same
The strings are the same!
The first 6 characters of s1 and s2 are the same
```

### See Also {.unnumbered .unlisted}

[`wcscoll()`](#man-wcscoll),
[`memcmp()`](#man-strcmp),
[`strcmp()`](#man-strcmp),
[`strncmp()`](#man-strcmp)

[[manbreak]]
## `wcscoll()` {#man-wcscoll}

Compare two wide strings accounting for locale

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

int wcscoll(const wchar_t *s1, const wchar_t *s2);
```

### Description {.unnumbered .unlisted}

This is the wide version of [`strcoll()`](#man-strcoll). See [`that
reference page`](#man-strcoll) for details.

This is slower than `wcscmp()`, so only use it if you need the
locale-specific compare.

### Return Value {.unnumbered .unlisted}

Returns zero if both regions are equal in this locale.

Returns a negative number if the region pointed to by `s1` is less than
`s2` in this locale.

Returns a positive number if the region pointed to by `s1` is greater
than `s2` in this locale.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <locale.h>

int main(void)
{
    setlocale(LC_ALL, "");

    // If your source character set doesn't support "é" in a string
    // you can replace it with `\u00e9`, the Unicode code point
    // for "é".

    wprintf(L"%d\n", wcscmp(L"é", L"f"));   // Reports é > f, yuck.
    wprintf(L"%d\n", wcscoll(L"é", L"f"));  // Reports é < f, yay!
}
```

### See Also {.unnumbered .unlisted}

[`wcscmp()`](#man-wcscmp),
[`wcsxfrm()`](#man-wcsxfrm),
[`strcoll()`](#man-strcoll)

[[manbreak]]
## `wcsxfrm()` {#man-wcsxfrm}

Transform a wide string for comparing based on locale

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

size_t wcsxfrm(wchar_t * restrict s1,
               const wchar_t * restrict s2, size_t n);
```

### Description {.unnumbered .unlisted}

This is the wide variant of [`strxfrm()`](#man-strxfrm). See
[that reference page](#man-strxfrm) for details.

### Return Value {.unnumbered .unlisted}

Returns the length of the transformed wide string in wide characters.

If the return value is greater than `n`, all bets are off for the
result in `s1`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

// Transform a string for comparison, returning a malloc'd
// result
wchar_t *get_xfrm_str(wchar_t *s)
{
    int len = wcsxfrm(NULL, s, 0) + 1;
    wchar_t *d = malloc(len * sizeof(wchar_t));

    wcsxfrm(d, s, len);

    return d;
}

// Does half the work of a regular wcscoll() because the second
// string arrives already transformed.
int half_wcscoll(wchar_t *s1, wchar_t *s2_transformed)
{
    wchar_t *s1_transformed = get_xfrm_str(s1);

    int result = wcscmp(s1_transformed, s2_transformed);

    free(s1_transformed);

    return result;
}

int main(void)
{
    setlocale(LC_ALL, "");

    // Pre-transform the string to compare against
    wchar_t *s = get_xfrm_str(L"éfg");

    // Repeatedly compare against "éfg" 
    wprintf(L"%d\n", half_wcscoll(L"fgh", s));  // "fgh" > "éfg"
    wprintf(L"%d\n", half_wcscoll(L"àbc", s));  // "àbc" < "éfg"
    wprintf(L"%d\n", half_wcscoll(L"ĥij", s));  // "ĥij" > "éfg"
    
    free(s);
}
```

Output:

``` {.default}
1
-1
1
```

### See Also {.unnumbered .unlisted}

[`wcscmp()`](#man-wcscmp),
[`wcscoll()`](#man-wcscoll),
[`strxfrm()`](#man-strxfrm)

[[manbreak]]
## `wcschr()` `wcsrchr()` {#man-wcschr}

Find a wide character in a wide string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

wchar_t *wcschr(const wchar_t *s, wchar_t c);

wchar_t *wcsrchr(const wchar_t *s, wchar_t c);

wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n);
```

### Description {.unnumbered .unlisted}

These are the wide equivalents to [`strchr()`](#man-strchr),
[`strrchr()`](#man-strchr), and [`memchr()`](#man-strchr).

They search for wide characters in a wide string from the front
(`wcschr()`), the end (`wcsrchr()`) or for an arbitrary number of wide
characters (`wmemchr()`).

### Return Value {.unnumbered .unlisted}

All three functions return a pointer to the wide character found, or
`NULL` if the character, sadly, isn't found.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    // "Hello, world!"
    //       ^  ^   ^
    //       A  B   C

    wchar_t *str = L"Hello, world!";
    wchar_t *p;

    p = wcschr(str, ',');       // p now points at position A
    p = wcsrchr(str, 'o');      // p now points at position B

    p = wmemchr(str, '!', 13);   // p now points at position C

    // repeatedly find all occurrences of the letter 'B'
    str = L"A BIG BROWN BAT BIT BEEJ";

    for(p = wcschr(str, 'B'); p != NULL; p = wcschr(p + 1, 'B')) {
        wprintf(L"Found a 'B' here: %ls\n", p);
    }
}
```

Output:

``` {.default}
Found a 'B' here: BIG BROWN BAT BIT BEEJ
Found a 'B' here: BROWN BAT BIT BEEJ
Found a 'B' here: BAT BIT BEEJ
Found a 'B' here: BIT BEEJ
Found a 'B' here: BEEJ
```

### See Also {.unnumbered .unlisted}

[`strchr()`](#man-strchr),
[`strrchr()`](#man-strchr),
[`memchr()`](#man-strchr)

[[manbreak]]
## `wcsspn()` `wcscspn()` {#man-wcsspn}

Return the length of a wide string consisting entirely of a set of wide
characters, or of not a set of wide characters

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

size_t wcsspn(const wchar_t *s1, const wchar_t *s2);

size_t wcscspn(const wchar_t *s1, const wchar_t *s2);
```

### Description {.unnumbered .unlisted}

The are the wide character counterparts to [`strspn()`] (#man-strspn)and
[`strcspn()`](#man-strspn).

They compute the length of the string pointed to by `s1` consisting
entirely of the characters found in `s2`. Or, in the case of
`wcscspn()`, the characters _not_ found in `s2`.

### Return Value {.unnumbered .unlisted}

The length of the string pointed to by `s1` consisting solely of the
characters in `s2` (in the case of `wcsspn()`) or of the characters
_not_ in `s2` (in th ecase of `wcscspn()`).

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    wchar_t str1[] = L"a banana";
    wchar_t str2[] = L"the bolivian navy on maneuvers in the south pacific";
    int n;

    // how many letters in str1 until we reach something that's not a vowel?
    n = wcsspn(str1, L"aeiou");
    wprintf(L"%d\n", n);  // n == 1, just "a"

    // how many letters in str1 until we reach something that's not a, b,
    // or space?
    n = wcsspn(str1, L"ab ");
    wprintf(L"%d\n", n);  // n == 4, "a ba"

    // how many letters in str2 before we get a "y"?
    n = wcscspn(str2, L"y");
    wprintf(L"%d\n", n);  // n = 16, "the bolivian nav"
}
```

### See Also {.unnumbered .unlisted}

[`wcschr()`](#man-wcschr),
[`wcsrchr()`](#man-wcschr),
[`strspn()`](#man-strspn)

[[manbreak]]
## `wcspbrk()` {#man-wcspbrk}

Search a wide string for one of a set of wide characters

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

wchar_t *wcspbrk(const wchar_t *s1, const wchar_t *s2);
```

### Description {.unnumbered .unlisted}

This is the wide character variant of [`strpbrk()`](#man-strpbrk).

It finds the first occurrance of any of a set of wide characters in a
wide string.

### Return Value {.unnumbered .unlisted}

Returns a pointer to the first character in the string `s1` that exists
in the string `s2`.

Or `NULL` if none of the characters in `s2` can be found in `s1`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    //  p points here after wcspbrk
    //                  v
    wchar_t *s1 = L"Hello, world!";
    wchar_t *s2 = L"dow!";  // Match any of these chars

    wchar_t *p = wcspbrk(s1, s2);  // p points to the o

    wprintf(L"%ls\n", p);  // "o, world!"
}
```

### See Also {.unnumbered .unlisted}

[`wcschr()`](#man-wcschr),
[`wmemchr()`](#man-wcschr),
[`strpbrk()`](#man-strpbrk)

[[manbreak]]
## `wcsstr()` {#man-wcsstr}

Find a wide string in another wide string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

wchar_t *wcsstr(const wchar_t *s1, const wchar_t *s2);
```

### Description {.unnumbered .unlisted}

This is the wide variant of [`strstr()`](#man-strstr).

It locates a substring in a string.

### Return Value {.unnumbered .unlisted}

Returns a pointer to the location in `s1` that contains `s2`.

Or `NULL` if `s2` cannot be found in `s1`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    wchar_t *str = L"The quick brown fox jumped over the lazy dogs.";
    wchar_t *p;

    p = wcsstr(str, L"lazy");
    wprintf(L"%ls\n", p == NULL? L"null": p); // "lazy dogs."

    // p is NULL after this, since the string "wombat" isn't in str:
    p = wcsstr(str, L"wombat");
    wprintf(L"%ls\n", p == NULL? L"null": p); // "null"
}
```

### See Also {.unnumbered .unlisted}

[`wcschr()`](#man-wcschr),
[`wcsrchr()`](#man-wcschr),
[`wcsspn()`](#man-wcsspn),
[`wcscspn()`](#man-wcsspn),
[`strstr()`](#man-strstr)

[[manbreak]]
## `wcstok()` {#man-wcstok}

Tokenize a wide string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>
wchar_t *wcstok(wchar_t * restrict s1, const wchar_t * restrict s2,
                wchar_t ** restrict ptr); 
```

### Description {.unnumbered .unlisted}

This is the wide version of [`strtok()`](#man-strtok).

And, like that one, it modifies the string `s1`. So make a copy of it
first if you want to preserve the original.

One key difference is that `wcstok()` can be threadsafe because you pass
in the pointer `ptr` to the current state of the transformation. This
gets initializers for you when `s1` is initially passed in as
non-`NULL`. (Subsequent calls with a `NULL` `s1` cause the state to
update.)

### Return Value {.unnumbered .unlisted}

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    // break up the string into a series of space or
    // punctuation-separated words
    wchar_t str[] = L"Where is my bacon, dude?";
    wchar_t *token;
    wchar_t *state;

    // Note that the following if-do-while construct is very very
    // very very very common to see when using strtok().

    // grab the first token (making sure there is a first token!)
    if ((token = wcstok(str, L".,?! ", &state)) != NULL) {
        do {
            wprintf(L"Word: \"%ls\"\n", token);

            // now, the while continuation condition grabs the
            // next token (by passing NULL as the first param)
            // and continues if the token's not NULL:
        } while ((token = wcstok(NULL, L".,?! ", &state)) != NULL);
    }
}
```

Output:

``` {.default}
Word: "Where"
Word: "is"
Word: "my"
Word: "bacon"
Word: "dude"
```

### See Also {.unnumbered .unlisted}

[`strtok()`](#man-strtok)

[[manbreak]]
## `wcslen()` {#man-wcslen}

Returns the length of a wide string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

size_t wcslen(const wchar_t *s);
```

### Description {.unnumbered .unlisted}

This is the wide counterpart to [`strlen()`](#man-strlen).

### Return Value {.unnumbered .unlisted}

Returns the number of wide characters before the wide NUL terminator.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    wchar_t *s = L"Hello, world!"; // 13 characters

    // prints "The string is 13 characters long.":

    wprintf(L"The string is %zu characters long.\n", wcslen(s));
}
```

### See Also {.unnumbered .unlisted}

[`strlen()`](#man-strlen)

[[manbreak]]
## `wcsftime()` {#man-wcsftime}

Formatted date and time output

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <time.h>
#include <wchar.h>

size_t wcsftime(wchar_t * restrict s, size_t maxsize,
                const wchar_t * restrict format,
                const struct tm * restrict timeptr);
```

### Description {.unnumbered .unlisted}

This is the wide equivalent to [`strftime()`](#man-strftime). See that
reference page for details.

`maxsize` here refers to the maximum number of wide characters that can
be in the result string.

### Return Value {.unnumbered .unlisted}

If successful, returns the number of wide characters written.

If not successful because the result couldn't fit in the space alloted,
`0` is returned and the contents of the string could be anything.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>
#include <time.h>

#define BUFSIZE 128

int main(void)
{
    wchar_t s[BUFSIZE];
    time_t now = time(NULL);

    // %c: print date as per current locale
    wcsftime(s, BUFSIZE, L"%c", localtime(&now));
    wprintf(L"%ls\n", s);   // Sun Feb 28 22:29:00 2021

    // %A: full weekday name
    // %B: full month name
    // %d: day of the month
    wcsftime(s, BUFSIZE, L"%A, %B %d", localtime(&now));
    wprintf(L"%ls\n", s);   // Sunday, February 28

    // %I: hour (12 hour clock)
    // %M: minute
    // %S: second
    // %p: AM or PM
    wcsftime(s, BUFSIZE, L"It's %I:%M:%S %p", localtime(&now));
    wprintf(L"%ls\n", s);   // It's 10:29:00 PM

    // %F: ISO 8601 yyyy-mm-dd
    // %T: ISO 8601 hh:mm:ss
    // %z: ISO 8601 timezone offset
    wcsftime(s, BUFSIZE, L"ISO 8601: %FT%T%z", localtime(&now));
    wprintf(L"%ls\n", s);   // ISO 8601: 2021-02-28T22:29:00-0800
}
```

### See Also {.unnumbered .unlisted}

[`strftime()`](#man-strftime)

[[manbreak]]
## `btowc()` `wctob()` {#man-btowc}

Convert a single byte character to a wide character

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

wint_t btowc(int c);

int wctob(wint_t c);
```

### Description {.unnumbered .unlisted}

These functions convert between single byte characters and wide
characters, and vice-versa.

Even though `int`s are involved, don't let this mislead you; they're
effectively converted to `unsigned char`s internally.

The characters in the basic character set are guaranteed to be a single
byte.

### Return Value {.unnumbered .unlisted}

`btowc()` returns the single-byte character as a wide character. Returns
`WEOF` if `EOF` is passed in, or if the byte doesn't correspond to a
valid wide character.

`wctob()` returns the wide character as a single-byte character. Returns
`EOF` if `WEOF` is passed in, or if the wide character doesn't
correspond to a value single-byte character.

See [`mbtowc()`](#man-mbtowc) and [`wctomb()`](#man-wctomb) for
multibyte to wide character conversion.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <wchar.h>

int main(void)
{
    wint_t wc = btowc('B');    // Convert single byte to wide char

    wprintf(L"Wide character: %lc\n", wc);

    unsigned char c = wctob(wc);  // Convert back to single byte

    wprintf(L"Single-byte character: %c\n", c);
}
```

Output:

``` {.default}
Wide character: B
Single-byte character: B
```

### See Also {.unnumbered .unlisted}

[`mbtowc()`](#man-mbtowc),
[`wctomb()`](#man-wctomb)

[[manbreak]]
## `mbsinit()` {#man-mbsinit}

Test if an `mbstate_t` is in the initial conversion state

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

int mbsinit(const mbstate_t *ps);
```

### Description {.unnumbered .unlisted}

For a given conversion state in a `mbstate_t` variable, this function
determines if it's in the initial conversion state.

### Return Value {.unnumbered .unlisted}

Returns non-zero if the value pointed to by `ps` is in the initial
conversion state, or if `ps` is `NULL`.

Returns `0` if the value pointed to by `ps` is **not** in the initial
conversion state.

### Example {.unnumbered .unlisted}

For me, this example doesn't do anything exciting, saying that the
`mbstate_t` variable is always in the initial state. Yay.

But if have a stateful encoding like 2022-JP, try messing around with
this to see if you can get into an intermediate state.

This program has a bit of code at the top that reports if your locale's
encoding requires any state.

``` {.c .numberLines}
#include <locale.h>   // For setlocale()
#include <string.h>   // For memset()
#include <stdlib.h>   // For mbtowc()
#include <wchar.h>

int main(void)
{
    mbstate_t state;
    wchar_t wc[128];

    setlocale(LC_ALL, "");

    int is_state_dependent = mbtowc(NULL, NULL, 0);

    wprintf(L"Is encoding state dependent? %d\n", is_state_dependent);

    memset(&state, 0, sizeof state);  // Set to initial state

    wprintf(L"In initial conversion state? %d\n", mbsinit(&state));

    mbrtowc(wc, "B", 5, &state);

    wprintf(L"In initial conversion state? %d\n", mbsinit(&state));
}
```

### See Also {.unnumbered .unlisted}

[`mbtowc()`](#man-mbtowc),
[`wctomb()`](#man-wctomb),
[`mbrtowc()`](#man-mbrtowc),
[`wcrtomb()`](#man-wcrtomb)

[[manbreak]]
## `mbrlen()` {#man-mbrlen}

Compute the number of bytes in a multibyte character, restartably

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

size_t mbrlen(const char * restrict s, size_t n, mbstate_t * restrict ps);
```

### Description {.unnumbered .unlisted}

This is the restartable version of [`mblen()`](#man-mblen).

It inspects at most `n` bytes of the string `s` to see how many bytes in
this character.

The conversion state is stored in `ps`.

This function doesn't have the functionality of `mblen()` that allowed
you to query if this character encoding was stateful and to reset the
internal state.

### Return Value {.unnumbered .unlisted}

Returns the number of bytes required for this multibyte character.

Returns `(size_t)(-1)` if the data in `s` is not a valid multibyte
character.

Returns `(size_t)(-2)` if the data is `s` is a valid but not complete
multibyte character.

### Example {.unnumbered .unlisted}

If your character set doesn't support the Euro symbol "€", substitute
the Unicode escape sequence `\u20ac`, below.

``` {.c .numberLines}
#include <locale.h>   // For setlocale()
#include <string.h>   // For memset()
#include <wchar.h>

int main(void)
{
    mbstate_t state;
    int len;

    setlocale(LC_ALL, "");

    memset(&state, 0, sizeof state);  // Set to initial state

    len = mbrlen("B", 5, &state);

    wprintf(L"Length of 'B' is %d byte(s)\n", len);

    len = mbrlen("€", 5, &state);

    wprintf(L"Length of '€' is %d byte(s)\n", len);
}
```

Output:

``` {.default}
Length of 'B' is 1 byte(s)
Length of '€' is 3 byte(s)
```

### See Also {.unnumbered .unlisted}

[`mblen()`](#man-mblen)

[[manbreak]]
## `mbrtowc()` {#man-mbrtowc}

Convert multibyte to wide characters restartably

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

size_t mbrtowc(wchar_t * restrict pwc, const char * restrict s,
               size_t n, mbstate_t * restrict ps);
```

### Description {.unnumbered .unlisted}

This is the restartable counterpart to [`mbtowc()`](#man-mbtowc).

It converts individual characters from multibyte to wide, tracking the
 conversion state in the variable pointed to by `ps`.

At most `n` bytes are inspected for conversion to a wide character.

These two variants are identical and cause the state pointed to by `ps`
to be set to the initial conversion state:

``` {.c}
mbrtowc(NULL, NULL, 0, &state);
mbrtowc(NULL, "", 1, &state);
```

Also, if you're just interested in the length in bytes of the multibyte
character, you can pass `NULL` for `pwc` and nothing will be stored for
the wide character:

``` {.c}
int len = mbrtowc(NULL, "€", 5, &state);
```

This function doesn't have the functionality of `mbtowc()` that allowed
you to query if this character encoding was stateful and to reset the
internal state.

### Return Value {.unnumbered .unlisted}

On success, returns a positive number corresponding to the number of
bytes in the multibyte character.

Returns `0` if the character encoded is a wide NUL character.

Returns `(size_t)(-1)` if the data in `s` is not a valid multibyte
character.

Returns `(size_t)(-2)` if the data is `s` is a valid but not complete
multibyte character.

### Example {.unnumbered .unlisted}

If your character set doesn't support the Euro symbol "€", substitute
the Unicode escape sequence `\u20ac`, below.

``` {.c .numberLines}
#include <string.h>  // For memset()
#include <stdlib.h>  // For mbtowc()
#include <locale.h>  // For setlocale()
#include <wchar.h>

int main(void)
{
    mbstate_t state;

    memset(&state, 0, sizeof state);

    setlocale(LC_ALL, "");

    wprintf(L"State dependency: %d\n", mbtowc(NULL, NULL, 0));

    wchar_t wc;
    int bytes;

    bytes = mbrtowc(&wc, "€", 5, &state);

    wprintf(L"L'%lc' takes %d bytes as multibyte char '€'\n", wc, bytes);
}
```

Output on my system:

``` {.default}
State dependency: 0
L'€' takes 3 bytes as multibyte char '€'
```

### See Also {.unnumbered .unlisted}

[`mbtowc()`](#man-mbtowc),
[`wcrtomb()`](#man-wcrtomb)

[[manbreak]]
## `wcrtomb()` {#man-wcrtomb}

Convert wide to multibyte characters restartably

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

size_t wcrtomb(char * restrict s, wchar_t wc, mbstate_t * restrict ps);
```

### Description {.unnumbered .unlisted}

This is the restartable counterpart to [`wctomb()`](#man-wctomb).

It converts individual characters from wide to multibyte, tracking the
conversion state in the variable pointed to by `ps`.

The destination array `s` should be at least `MB_CUR_MAX`^[This is a
variable, not a macro, so if you use it to define an array, it'll be a
variable-length array.] bytes in size---you won't get anything bigger
back from this function.

Note that the values in this result array won't be NUL-terminated.

If you pass a wide NUL character in, the result will contain any bytes
needed to restore the conversion state to its initial state followed by
a NUL character, and the state pointed to by `ps` will be reset to its
initial state:

``` {.c}
// Reset state
wcrtomb(mb, L'\0', &state)
```

If you don't care about the results (i.e. you're just interested in
resetting the state or getting the return value), you can do this by
passing `NULL` for `s`:

``` {.c}
wcrtomb(NULL, L'\0', &state);                // Reset state

int byte_count = wctomb(NULL, "X", &state);  // Count bytes in 'X'
```

This function doesn't have the functionality of `wctomb()` that allowed
you to query if this character encoding was stateful and to reset the
internal state.

### Return Value {.unnumbered .unlisted}

On success, returns the number of bytes needed to encode this wide
character in the current locale.

If the input is an invalid wide character, `errno` will be set to
`EILSEQ` and the function returns `(size_t)(-1)`. If this happens, all
bets are off for the conversion state, so you might as well reset it.

### Example {.unnumbered .unlisted}

If your character set doesn't support the Euro symbol "€", substitute
the Unicode escape sequence `\u20ac`, below.

``` {.c .numberLines}
#include <string.h>  // For memset()
#include <stdlib.h>  // For mbtowc()
#include <locale.h>  // For setlocale()
#include <wchar.h>

int main(void)
{
    mbstate_t state;

    memset(&state, 0, sizeof state);

    setlocale(LC_ALL, "");

    wprintf(L"State dependency: %d\n", mbtowc(NULL, NULL, 0));

    char mb[10] = {0};
    int bytes = wcrtomb(mb, L'€', &state);

    wprintf(L"L'€' takes %d bytes as multibyte char '%s'\n", bytes, mb);
}
```

### See Also {.unnumbered .unlisted}

[`mbrtowc()`](#man-mbrtowc),
[`wctomb()`](#man-wctomb),
[`errno`](#errno)

[[manbreak]]
## `mbsrtowcs()` {#man-mbsrtowcs}

Convert a multibyte string to a wide character string restartably

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

size_t mbsrtowcs(wchar_t * restrict dst, const char ** restrict src,
                 size_t len, mbstate_t * restrict ps);
```

### Description {.unnumbered .unlisted}

This is the restartable version of [`mbstowcs()`](#man-mbstowcs).

It converts a multibyte string to a wide character string.

The result is put in the buffer pointed to by `dst`, and the pointer
`src` is updated to indicate how much of the string was consumed (unless
`dst` is `NULL`).

At most `len` wide characters will be stored.

This also takes a pointer to its own `mbstate_t` variable in `ps` for
holding the conversion state.

You can set `dst` to `NULL` if you only care about the return value.
This could be useful for getting the number of characters in a multibyte
string.

In the normal case, the `src` string will be consumed up to the NUL
character, and the results will be stored in the `dst` buffer, including
the wide NUL character. In this case, the pointer pointed to by `src`
will be set to `NULL`. And the conversion state will be set to the
initial conversion state.

If things go wrong because the source string isn't a valid sequence of
characters, conversion will stop and the pointer pointed to by `src`
will be set to the address just after the last successfully-translated
multibyte character.

### Return Value {.unnumbered .unlisted}

If successful, returns the number of characters converted, not including
any NUL terminator.

If the multibyte sequence is invalid, the function returns
`(size_t)(-1)` and `errno` is set to `EILSEQ`.

### Example {.unnumbered .unlisted}

Here we'll convert the string "€5 ± π" into a wide character string:

``` {.c .numberLines}
#include <locale.h>  // For setlocale()
#include <string.h>  // For memset()
#include <wchar.h>

#define WIDE_STR_SIZE 10

int main(void)
{
    const char *mbs = "€5 ± π";  // That's the exact price range

    wchar_t wcs[WIDE_STR_SIZE];

    setlocale(LC_ALL, "");
    
    mbstate_t state;
    memset(&state, 0, sizeof state);

    size_t count = mbsrtowcs(wcs, &mbs, WIDE_STR_SIZE, &state);

    wprintf(L"Wide string L\"%ls\" is %d characters\n", wcs, count);
}
```

Output:

``` {.default}
Wide string L"€5 ± π" is 6 characters
```

Here's another example of using `mbsrtowcs()` to get the length in
characters of a multibyte string even if the string is full of multibyte
characters. This is in contrast to `strlen()`, which returns the total
number of bytes in the string.

``` {.c .numberLines}
#include <stdio.h>   // For printf()
#include <locale.h>  // For setlocale()

#include <string.h>  // For memset()
#include <stdint.h>  // For SIZE_MAX
#include <wchar.h>

size_t mbstrlen(const char *mbs)
{
    mbstate_t state;

    memset(&state, 0, sizeof state);

    return mbsrtowcs(NULL, &mbs, SIZE_MAX, &state);
}

int main(void)
{
    setlocale(LC_ALL, "");
    
    char *mbs = "€5 ± π";  // That's the exact price range

    printf("\"%s\" is %zu characters...\n", mbs, mbstrlen(mbs)); 
    printf("but it's %zu bytes!\n", strlen(mbs));
}
```

Output on my system:

``` {.default}
"€5 ± π" is 6 characters...
but it's 10 bytes!
```

### See Also {.unnumbered .unlisted}

[`mbrtowc()`](#man-mbrtowc),
[`mbstowcs()`](#man-mbstowcs),
[`wcsrtombs()`](#man-wcsrtombs),
[`strlen()`](#man-strlen),
[`errno`](#errno)

[[manbreak]]
## `wcsrtombs()` {#man-wcsrtombs}

Convert a wide character string to a multibyte string restartably

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <wchar.h>

size_t wcsrtombs(char * restrict dst, const wchar_t ** restrict src,
                 size_t len, mbstate_t * restrict ps);
```

### Description {.unnumbered .unlisted}

If you have a wide character string, you can convert it to a multibyte
character string in the current locale using this function.

At most `len` bytes of data will be stored in the buffer pointed to by
`dst`. Conversion will stop just after the NUL terminator is copied, or
`len` bytes get copied, or some other error occurs.

If `dst` is a `NULL` pointer, no result is stored. You might do this if
you're just interested in the return value (nominally the number of
bytes this would use in a multibyte string, not including the NUL
terminator).

If `dst` is not a `NULL` pointer, the pointer pointed to by `src` will
get modified to indicate how much of the data was copied. If it contains
`NULL` at the end, it means everything went well. In this case, the
state `ps` will be set to the initial conversion state.

If `len` was reached or an error occurred, it'll point one address past
`dst+len`.

### Return Value {.unnumbered .unlisted}

If everything goes well, returns the number of bytes needed for the
multibyte string, not counting the NUL terminator.

If any character in the string doesn't correspond to a valid multibyte
character in the currently locale, it returns `(size_t)(-1)` and
`EILSEQ` is stored in `errno`.


### Example {.unnumbered .unlisted}

Here we'll convert the wide string "€5 ± π" into a multibyte character
string:

``` {.c .numberLines}
#include <locale.h>  // For setlocale()
#include <string.h>  // For memset()
#include <wchar.h>

#define MB_STR_SIZE 20

int main(void)
{
    const wchar_t *wcs = L"€5 ± π";  // That's the exact price range

    char mbs[MB_STR_SIZE];

    setlocale(LC_ALL, "");
    
    mbstate_t state;
    memset(&state, 0, sizeof state);

    size_t count = wcsrtombs(mbs, &wcs, MB_STR_SIZE, &state);

    wprintf(L"Multibyte string \"%s\" is %d bytes\n", mbs, count);
}
```

Here's another example helper function that `malloc()`s just enough
memory to hold the converted string, then returns the result. (Which
must later be freed, of course, to prevent leaking memory.)

``` {.c .numberLines}
#include <stdlib.h>  // For malloc()
#include <locale.h>  // For setlocale()
#include <string.h>  // For memset()
#include <stdint.h>  // For SIZE_MAX
#include <wchar.h>

char *get_mb_string(const wchar_t *wcs)
{
    setlocale(LC_ALL, "");

    mbstate_t state;
    memset(&state, 0, sizeof state);

    // Need a copy of this because wcsrtombs changes it
    const wchar_t *p = wcs;

    // Compute the number of bytes needed to hold the result
    size_t bytes_needed = wcsrtombs(NULL, &p, SIZE_MAX, &state);

    // If we didn't get a good full conversion, forget it
    if (bytes_needed == (size_t)(-1))
        return NULL;

    // Allocate space for result
    char *mbs = malloc(bytes_needed + 1);  // +1 for NUL terminator

    // Set conversion state to initial state
    memset(&state, 0, sizeof state);

    // Convert and store result
    wcsrtombs(mbs, &wcs, bytes_needed + 1, &state);

    // Make sure things went well
    if (wcs != NULL) {
        free(mbs);
        return NULL;
    }

    // Success!
    return mbs;
}

int main(void)
{
    char *mbs = get_mb_string(L"€5 ± π");

    wprintf(L"Multibyte result: \"%s\"\n", mbs);

    free(mbs);
}
```

### See Also {.unnumbered .unlisted}

[`wcrtomb()`](#man-wcrtomb),
[`wcstombs()`](#man-wcstombs),
[`mbsrtowcs()`](#man-mbsrtowcs),
[`errno`](#errno)

<!--
[[manbreak]]
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
