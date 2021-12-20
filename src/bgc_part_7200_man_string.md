<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->
# `<string.h>` String Manipulation {#stringref}

|Function|Description|
|-|-|
|[`memchr()`](#man-strchr)|Find the first occurrence of a character in memory.|
|[`memcmp()`](#man-strcmp)|Compare two regions of memory.|
|[`memcpy()`](#man-memcpy)|Copy a region of memory to another.|
|[`memmove()`](#man-memcpy)|Move a (potentially overlapping) region of memory.|
|[`memset()`](#man-memset)|Set a region of memory to a value.|
|[`strcat()`](#man-strcat)|Concatenate (join) two strings together.|
|[`strchr()`](#man-strchr)|Find the first occurrence of a character in a string.|
|[`strcmp()`](#man-strcmp)|Compare two strings.|
|[`strcoll()`](#man-strcoll)|Compare two strings accounting for locale.|
|[`strcpy()`](#man-strcpy)|Copy a string.|
|[`strcspn()`](#man-strspn)|Find length of a string not consisting of a set of characters.|
|[`strerror()`](#man-strerror)|Return a human-readable error message for a given code.|
|[`strlen()`](#man-strlen)|Return the length of a string.|
|[`strncat()`](#man-strcat)|Concatenate (join) two strings, length-limited.|
|[`strncmp()`](#man-strcmp)|Compare two strings, length-limited.|
|[`strncpy()`](#man-strcpy)|Copy two strings, length-limited.|
|[`strpbrk()`](#man-strpbrk)|Search a string for one of a set of character.|
|[`strrchr()`](#man-strchr)|Find the last occurrence of a character in a string.|
|[`strspn()`](#man-strspn)|Find length of a string consisting of a set of characters.|
|[`strstr()`](#man-strstr)|Find a substring in a string.|
|[`strtok()`](#man-strtok)|Tokenize a string.|
|[`strxfrm()`](#man-strxfrm)|Prepare a string for comparison as if by `strcoll()`.|

As has been mentioned earlier in the guide, a string in C is a sequence
of bytes in memory, terminated by a NUL character ('`\0`'). The NUL at
the end is important, since it lets all these string functions (and
`printf()` and `puts()` and everything else that deals with a string)
know where the end of the string actually is.

Fortunately, when you operate on a string using one of these many
functions available to you, they add the NUL terminator on for you, so
you actually rarely have to keep track of it yourself. (Sometimes you
do, especially if you're building a string from scratch a character at a
time or something.)

In this section you'll find functions for pulling substrings out of
strings, concatenating strings together, getting the length of a string,
and so forth and so on.

[[manbreak]]
## `memcpy()`, `memmove()` {#man-memcpy}

Copy bytes of memory from one location to another

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

void *memcpy(void * restrict s1, const void * restrict s2, size_t n);

void *memmove(void *s1, const void *s2, size_t n);
```

### Description {.unnumbered .unlisted}

These functions copy memory---as many bytes as you want! From source to
destination!

The main difference between the two is that `memcpy()` cannot safely
copy overlapping memory regions, whereas `memmove()` can.

On the one hand, I'm not sure why you'd want to ever use `memcpy()`
instead of `memmove()`, but I'll bet it's possibly more performant.

The parameters are in a particular order: destination first, then
source. I remember this order because it behaves like an "`=`"
assignment: the destination is on the left.

### Return Value {.unnumbered .unlisted}

Both functions return whatever you passed in for parameter `s1` for your
convenience.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <string.h>

int main(void)
{
    char s[100] = "Goats";
    char t[100];

    memcpy(t, s, 6);       // Copy non-overlapping memory

    memmove(s + 2, s, 6);  // Copy overlapping memory
}
```

### See Also {.unnumbered .unlisted}

[`strcpy()`](#man-strcpy),
[`strncpy()`](#man-strcpy)

[[manbreak]]
## `strcpy()`, `strncpy()` {#man-strcpy}

Copy a string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

char *strcpy(char *dest, char *src);

char *strncpy(char *dest, char *src, size_t n);
```

### Description {.unnumbered .unlisted}

These functions copy a string from one address to another, stopping at
the NUL terminator on the `src`string.

`strncpy()` is just like `strcpy()`, except only the first `n`
characters are actually copied. Beware that if you hit the limit, `n`
before you get a NUL terminator on the `src` string, your `dest` string
won't be NUL-terminated. Beware! BEWARE!

(If the `src` string has fewer than `n` characters, it works just like
`strcpy()`.)

You can terminate the string yourself by sticking the `'\0'` in there
yourself:

``` {.c}
char s[10];
char foo = "My hovercraft is full of eels."; // more than 10 chars

strncpy(s, foo, 9); // only copy 9 chars into positions 0-8
s[9] = '\0';        // position 9 gets the terminator
```

### Return Value {.unnumbered .unlisted}

Both functions return `dest` for your convenience, at no extra charge.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <string.h>

int main(void)
{
    char *src = "hockey hockey hockey hockey hockey hockey hockey hockey";
    char dest[20];

    int len;

    strcpy(dest, "I like "); // dest is now "I like "

    len = strlen(dest);

    // tricky, but let's use some pointer arithmetic and math to append
    // as much of src as possible onto the end of dest, -1 on the length to
    // leave room for the terminator:
    strncpy(dest+len, src, sizeof(dest)-len-1);

    // remember that sizeof() returns the size of the array in bytes
    // and a char is a byte:
    dest[sizeof(dest)-1] = '\0'; // terminate

    // dest is now:       v null terminator
    // I like hockey hocke 
    // 01234567890123456789012345
}
```

### See Also {.unnumbered .unlisted}

[`memcpy()`](#man-memcpy),
[`strcat()`](#man-strcat),
[`strncat()`](#man-strcat)

[[manbreak]]
## `strcat()`, `strncat()` {#man-strcat}

Concatenate two strings into a single string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

int strcat(const char *dest, const char *src);

int strncat(const char *dest, const char *src, size_t n);
```

### Description {.unnumbered .unlisted}

"Concatenate", for those not in the know, means to "stick together".
These functions take two strings, and stick them together, storing the
result in the first string.

These functions don't take the size of the first string into account
when it does the concatenation. What this means in practical terms is
that you can try to stick a 2 megabyte string into a 10 byte space. This
will lead to unintended consequences, unless you intended to lead to
unintended consequences, in which case it will lead to intended
unintended consequences.

Technical banter aside, your boss and/or professor will be irate.

If you want to make sure you don't overrun the first string, be sure to
check the lengths of the strings first and use some highly technical
subtraction to make sure things fit.

You can actually only concatenate the first `n` characters of the second
string by using `strncat()` and specifying the maximum number of
characters to copy.

### Return Value {.unnumbered .unlisted}

Both functions return a pointer to the destination string, like most of
the string-oriented functions.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    char dest[30] = "Hello";
    char *src = ", World!";
    char numbers[] = "12345678";

    printf("dest before strcat: \"%s\"\n", dest); // "Hello"

    strcat(dest, src);
    printf("dest after strcat:  \"%s\"\n", dest); // "Hello, world!"

    strncat(dest, numbers, 3); // strcat first 3 chars of numbers
    printf("dest after strncat: \"%s\"\n", dest); // "Hello, world!123"
}
```

Notice I mixed and matched pointer and array notation there with `src`
and `numbers`; this is just fine with string functions.

### See Also {.unnumbered .unlisted}

[`strlen()`](#man-strlen)

[[manbreak]]
## `strcmp()`, `strncmp()`, `memcmp()` {#man-strcmp}

Compare two strings or memory regions and return a difference

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);
```

### Description {.unnumbered .unlisted}

All these functions compare chunks of bytes in memory.

`strcmp()` and `strncmp()` operate on NUL-terminated strings, whereas
`memcmp()` will compare the number of bytes you specify, brazenly
ignoring any NUL characters it finds along the way.

`strcmp()` compares the entire string down to the end, while `strncmp()`
only compares the first `n` characters of the strings.

It's a little funky what they return. Basically it's a difference of the
strings, so if the strings are the same, it'll return zero (since the
difference is zero). It'll return non-zero if the strings differ;
basically it will find the first mismatched character and return
less-than zero if that character in `s1` is less than the corresponding
character in `s2`. It'll return greater-than zero if that character in
`s1` is greater than that in `s2`.

So if they return `0`, the comparison was equal (i.e. the difference was
`0`.)

These functions can be used as comparison functions for
[`qsort()`](#man-qsort) if you have an array of `char*`s you want to
sort.

### Return Value {.unnumbered .unlisted}

Returns zero if the strings or memory are the same, less-than zero if
the first different character in `s1` is less than that in `s2`, or
greater-than zero if the first difference character in `s1` is greater
than than in `s2`.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *s1 = "Muffin";
    char *s2 = "Muffin Sandwich";
    char *s3 = "Muffin";

    int r1 = strcmp("Biscuits", "Kittens");
    printf("%d\n", r1); // prints < 0 since 'B' < 'K'

    int r2 = strcmp("Kittens", "Biscuits");
    printf("%d\n", r2); // prints > 0 since 'K' > 'B'

    if (strcmp(s1, s2) == 0)
        printf("This won't get printed because the strings differ\n");

    if (strcmp(s1, s3) == 0)
        printf("This will print because s1 and s3 are the same\n");

    // this is a little weird...but if the strings are the same, it'll
    // return zero, which can also be thought of as "false". Not-false
    // is "true", so (!strcmp()) will be true if the strings are the
    // same. yes, it's odd, but you see this all the time in the wild
    // so you might as well get used to it:

    if (!strcmp(s1, s3))
        printf("The strings are the same!\n");

    if (!strncmp(s1, s2, 6))
        printf("The first 6 characters of s1 and s2 are the same\n");
}
```

### See Also {.unnumbered .unlisted}

[`memcmp()`](#man-strcmp),
[`qsort()`](#man-qsort)

[[manbreak]]
## `strcoll()` {#man-strcoll}

Compare two strings accounting for locale

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

int strcoll(const char *s1, const char *s2);
```

### Description {.unnumbered .unlisted}

This is basically `strcmp()`, except that it handles accented characters
better depending on the locale.

For example, my `strcmp()` reports that the character "é" (with accent)
is greater than "f". But that's hardly useful for alphabetizing.

By setting the `LC_COLLATE` locale value (either by name or via
`LC_ALL`), you can have `strcoll()` sort in a way that's more meaningful
by the current locale. For example, by having "é" appear sanely _before_
"f".

It's also a lot slower than `strcmp()` so use it only if you have to.
See [`strxfrm()`](#man-strxfrm) for a potential speedup.

### Return Value {.unnumbered .unlisted}

Like the other string comparison functions, `strcoll()` returns a
negative value if `s1` is less than `s2`, or a positive value if `s1` is
greater than `s2`. Or `0` if they are equal.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>
#include <locale.h>

int main(void)
{
    setlocale(LC_ALL, "");

    // If your source character set doesn't support "é" in a string
    // you can replace it with `\u00e9`, the Unicode code point
    // for "é".

    printf("%d\n", strcmp("é", "f"));   // Reports é > f, yuck.
    printf("%d\n", strcoll("é", "f"));  // Reports é < f, yay!
}
```

### See Also {.unnumbered .unlisted}

[`strcmp()`](#man-strcmp)

[[manbreak]]
## `strxfrm()` {#man-strxfrm}

Transform a string for comparing based on locale

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

size_t strxfrm(char * restrict s1, const char * restrict s2, size_t n);
```

### Description {.unnumbered .unlisted}

This is a strange little function, so bear with me.

Firstly, if you haven't done so, get familiar with
[`strcoll()`](#man-strcoll) because this is closely related to that.

OK! Now that you're back, you can think of `strxfrm()` as the first part
of the `strcoll()` internals. Basically, `strcoll()` has to transform a
string into a form that can be compared with `strcmp()`. And it does
this with `strxfrm()` for both strings every time you call it.

`strxform()` takes string `s2` and transforms it (readies it for
`strcmp()`) storing the result in `s1`. It writes no more than `n`
bytes, protecting us from terrible buffer overflows.

But hang on---there's another mode! If you pass `NULL` for `s1` and `0`
for `n`, it will return the number of bytes that the transformed string
_would have_ used^[It always returns the number of bytes the transformed
string took, but in this case because `s1` was `NULL`, it doesn't
actually write a transformed string.]. This is useful if you need to
allocate some space to hold the transformed string before you `strcmp()`
it against another.

What I'm getting at, not to be too blunt, is that `strcoll()` is
slow compared to `strcmp()`. It does a lot of extra work running
`strxfrm()` on all its strings.

In fact, we can see how it works by writing our own like this:

``` {.c .numberLines}
int my_strcoll(char *s1, char *s2)
{
    // Use n = 0 to just get the lengths of the transformed strings
    int len1 = strxfrm(NULL, s1, 0) + 1;
    int len2 = strxfrm(NULL, s2, 0) + 1;

    // Allocate enough room for each
    char *d1 = malloc(len1);
    char *d2 = malloc(len2);

    // Transform the strings for comparison
    strxfrm(d1, s1, len1);
    strxfrm(d2, s2, len2);

    // Compare the transformed strings
    int result = strcmp(d1, d2);

    // Free up the transformed strings
    free(d2);
    free(d1);

    return result;
}
```

You see on lines 12, 13, and 16, above how we transform the two input
strings and then call `strcmp()` on the result.

So why do we have this function? Can't we just call `strcoll()` and be
done with it?

The idea is that if you have one string that you're going to be
comparing against a whole lot of other ones, maybe you just want to
transform that string one time, then use the faster `strcmp()` saving
yourself a bunch of the work we had to do in the function, above.

We'll do that in the example.

### Return Value {.unnumbered .unlisted}

Returns the number of bytes in the transformed sequence. If the value is
greater than `n`, the results in `s1` are meaningless.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

// Transform a string for comparison, returning a malloc'd
// result
char *get_xfrm_str(char *s)
{
    int len = strxfrm(NULL, s, 0) + 1;
    char *d = malloc(len);

    strxfrm(d, s, len);

    return d;
}

// Does half the work of a regular strcoll() because the second
// string arrives already transformed.
int half_strcoll(char *s1, char *s2_transformed)
{
    char *s1_transformed = get_xfrm_str(s1);

    int result = strcmp(s1_transformed, s2_transformed);

    free(s1_transformed);

    return result;
}

int main(void)
{
    setlocale(LC_ALL, "");

    // Pre-transform the string to compare against
    char *s = get_xfrm_str("éfg");

    // Repeatedly compare against "éfg" 
    printf("%d\n", half_strcoll("fgh", s));  // "fgh" > "éfg"
    printf("%d\n", half_strcoll("àbc", s));  // "àbc" < "éfg"
    printf("%d\n", half_strcoll("ĥij", s));  // "ĥij" > "éfg"
    
    free(s);
}
```

### See Also {.unnumbered .unlisted}
[`strcoll()`](#man-strcoll)

[[manbreak]]
## `strchr()`, `strrchr()`, `memchr()` {#man-strchr}

Find a character in a string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

char *strchr(char *str, int c);

char *strrchr(char *str, int c);

void *memchr(const void *s, int c, size_t n);
```

### Description {.unnumbered .unlisted}

The functions `strchr()` and `strrchr` find the first or last occurrence
of a letter in a string, respectively. (The extra "r" in `strrchr()`
stands for "reverse"--it looks starting at the end of the string and
working backward.)  Each function returns a pointer to the char in
question, or `NULL` if the letter isn't found in the string.

`memchr()` is similar, except that instead of stopping on the first NUL
character, it continues searching for however many bytes you specify.

Quite straightforward.

One thing you can do if you want to find the next occurrence of the
letter after finding the first, is call the function again with the
previous return value plus one. (Remember pointer arithmetic?)  Or minus
one if you're looking in reverse. Don't accidentally go off the end of
the string!

### Return Value {.unnumbered .unlisted}

Returns a pointer to the occurrence of the letter in the string, or
`NULL` if the letter is not found.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    // "Hello, world!"
    //       ^  ^   ^
    //       A  B   C

    char *str = "Hello, world!";
    char *p;

    p = strchr(str, ',');       // p now points at position A
    p = strrchr(str, 'o');      // p now points at position B

    p = memchr(str, '!', 13);   // p now points at position C

    // repeatedly find all occurrences of the letter 'B'
    str = "A BIG BROWN BAT BIT BEEJ";

    for(p = strchr(str, 'B'); p != NULL; p = strchr(p + 1, 'B')) {
        printf("Found a 'B' here: %s\n", p);
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

<!--
### See Also {.unnumbered .unlisted}
-->

[[manbreak]]
## `strspn()`, `strcspn()` {#man-strspn}

Return the length of a string consisting entirely of a set of
characters, or of not a set of characters

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

size_t strspn(char *str, const char *accept);

size_t strcspn(char *str, const char *reject);
```

### Description {.unnumbered .unlisted}

`strspn()` will tell you the length of a string consisting entirely of
the set of characters in `accept`. That is, it starts walking down `str`
until it finds a character that is _not_ in the set (that is, a
character that is not to be accepted), and returns the length of the
string so far.

`strcspn()` works much the same way, except that it walks down `str`
until it finds a character in the `reject` set (that is, a character
that is to be rejected.)  It then returns the length of the string so
far.

### Return Value {.unnumbered .unlisted}

The length of the string consisting of all characters in `accept` (for
`strspn()`), or the length of the string consisting of all characters
except `reject` (for `strcspn()`).

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    char str1[] = "a banana";
    char str2[] = "the bolivian navy on maenuvers in the south pacific";
    int n;

    // how many letters in str1 until we reach something that's not a vowel?
    n = strspn(str1, "aeiou");
    printf("%d\n", n);  // n == 1, just "a"

    // how many letters in str1 until we reach something that's not a, b,
    // or space?
    n = strspn(str1, "ab ");
    printf("%d\n", n);  // n == 4, "a ba"

    // how many letters in str2 before we get a "y"?
    n = strcspn(str2, "y");
    printf("%d\n", n);  // n = 16, "the bolivian nav"
}
```

### See Also {.unnumbered .unlisted}

[`strchr()`](#man-strchr),
[`strrchr()`](#man-strchr)

[[manbreak]]
## `strpbrk()` {#man-strpbrk}

Search a string for one of a set of characters

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

char *strpbrk(const char *s1, const char *s2);
```

### Description {.unnumbered .unlisted}

This function searches string `s1` for any of the characters that are
found in string `s2`.

It's just like how `strchr()` searches for a specific character in a
string, except it will match _any_ of the characters found in `s2`.

Think of the power!

### Return Value {.unnumbered .unlisted}

Returns a pointer to the first character matched in `s1`, or NULL if the
string isn't found.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    //  p points here after strpbrk
    //              v
    char *s1 = "Hello, world!";
    char *s2 = "dow!";  // Match any of these chars

    char *p = strpbrk(s1, s2);  // p points to the o

    printf("%s\n", p);  // "o, world!"
}
```

### See Also {.unnumbered .unlisted}

[`strchr()`](#man-strchr),
[`memchr()`](#man-strchr)

[[manbreak]]
## `strstr()` {#man-strstr}

Find a string in another string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

char *strstr(const char *str, const char *substr);
```

### Description {.unnumbered .unlisted}

Let's say you have a big long string, and you want to find a word, or
whatever substring strikes your fancy, inside the first string. Then
`strstr()` is for you! It'll return a pointer to the `substr` within the
`str`!

### Return Value {.unnumbered .unlisted}

You get back a pointer to the occurrence of the `substr` inside the
`str`, or `NULL` if the substring can't be found.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *str = "The quick brown fox jumped over the lazy dogs.";
    char *p;

    p = strstr(str, "lazy");
    printf("%s\n", p == NULL? "null": p); // "lazy dogs."

    // p is NULL after this, since the string "wombat" isn't in str:
    p = strstr(str, "wombat");
    printf("%s\n", p == NULL? "null": p); // "null"
}
```

### See Also {.unnumbered .unlisted}

[`strchr()`](#man-strchr),
[`strrchr()`](#man-strchr),
[`strspn()`](#man-strspn),
[`strcspn()`](#man-strspn)

[[manbreak]]
## `strtok()` {#man-strtok}

Tokenize a string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

char *strtok(char *str, const char *delim);
```

### Description {.unnumbered .unlisted}

If you have a string that has a bunch of separators in it, and you want
to break that string up into individual pieces, this function can do it
for you.

The usage is a little bit weird, but at least whenever you see the
function in the wild, it's consistently weird.

Basically, the first time you call it, you pass the string, `str` that
you want to break up in as the first argument. For each subsequent call
to get more tokens out of the string, you pass `NULL`. This is a little
weird, but `strtok()` remembers the string you originally passed in, and
continues to strip tokens off for you.

Note that it does this by actually putting a NUL terminator after the
token, and then returning a pointer to the start of the token. So the
original string you pass in is destroyed, as it were. If you need to
preserve the string, be sure to pass a copy of it to `strtok()` so the
original isn't destroyed.

### Return Value {.unnumbered .unlisted}

A pointer to the next token. If you're out of tokens, `NULL` is
returned.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    // break up the string into a series of space or
    // punctuation-separated words
    char str[] = "Where is my bacon, dude?";
    char *token;

    // Note that the following if-do-while construct is very very
    // very very very common to see when using strtok().

    // grab the first token (making sure there is a first token!)
    if ((token = strtok(str, ".,?! ")) != NULL) {
        do {
            printf("Word: \"%s\"\n", token);

            // now, the while continuation condition grabs the
            // next token (by passing NULL as the first param)
            // and continues if the token's not NULL:
        } while ((token = strtok(NULL, ".,?! ")) != NULL);
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

[`strchr()`](#man-strchr),
[`strrchr()`](#man-strchr),
[`strspn()`](#man-strspn),
[`strcspn()`](#man-strspn)


[[manbreak]]
## `memset()` {#man-memset}

Set a region of memory to a certain value

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

void *memset(void *s, int c, size_t n);
```
### Description {.unnumbered .unlisted}

This function is what you use to set a region of memory to a particular
value, namely `c` converted into `unsigned char`.

The most common usage is to zero out an array or `struct`.

### Return Value {.unnumbered .unlisted}

`memset()` returns whatever you passed in as `s` for happy convenience.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    struct banana {
        float ripeness;
        char *peel_color;
        int grams;
    };

    struct banana b;

    memset(&b, 0, sizeof b);

    printf("%d\n", b.ripeness == 0.0);     // True
    printf("%d\n", b.peel_color == NULL);  // True
    printf("%d\n", b.grams == 0);          // True
}
```

### See Also {.unnumbered .unlisted}

[`memcpy()`](#man-memcpy),
[`memmove()`](#man-memcpy)

[[manbreak]]
## `strerror()` {#man-strerror}

Get a string version of an error number

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

char *strerror(int errnum);
```

### Description {.unnumbered .unlisted}

This function ties closely into `perror()` (which prints a
human-readable error message corresponding to `errno`). But instead of
printing, `strerror()` returns a pointer to the locale-specific error
message string.

So if you ever need that string back for some reason (e.g. you're going
to `fprintf()` it to a file or something), this function will give it to
you. All you need to do is pass in `errno` as an argument. (Recall that
`errno` gets set as an error status by a variety of functions.)

You can actually pass in any integer for `errnum` you want. The function
will return _some_ message, even if the number doesn't correspond to any
known value for `errno`.

The values of `errno` and the strings returned by `strerror()` are
system-dependent.

### Return Value {.unnumbered .unlisted}

A string error message corresponding to the given error number.

You are not allowed to modify the returned string.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(void)
{
    FILE *fp = fopen("NONEXISTENT_FILE.TXT", "r");

    if (fp == NULL) {
        char *errmsg = strerror(errno);
        printf("Error %d opening file: %s\n", errno, errmsg);
    }
}
```

Output:

``` {.default}
Error 2 opening file: No such file or directory
```

### See Also {.unnumbered .unlisted}

[`perror()`](#man-perror)

[[manbreak]]
## `strlen()` {#man-strlen}

Returns the length of a string

### Synopsis {.unnumbered .unlisted}

``` {.c}
#include <string.h>

size_t strlen(const char *s);
```

### Description {.unnumbered .unlisted}

This function returns the length of the passed null-terminated string
(not counting the NUL character at the end). It does this by walking
down the string and counting the bytes until the NUL character, so it's
a little time consuming. If you have to get the length of the same
string repeatedly, save it off in a variable somewhere.

### Return Value {.unnumbered .unlisted}

Returns the number of bytes in the string. Note that this might be
different than the number of characters in a multibyte string.

### Example {.unnumbered .unlisted}

``` {.c .numberLines}
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *s = "Hello, world!"; // 13 characters

    // prints "The string is 13 characters long.":

    printf("The string is %zu characters long.\n", strlen(s));
}
```

### See Also {.unnumbered .unlisted}

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
