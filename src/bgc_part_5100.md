<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->
# String Manipulation {#stringref}

As has been mentioned earlier in the guide, a string in C is a
sequence of bytes in memory, terminated by a NUL character
('`\0`'). The NUL at the end is important, since it lets all
these string functions (and `printf()` and
`puts()` and everything else that deals with a string) know
where the end of the string actually is.

Fortunately, when you operate on a string using one of these many
functions available to you, they add the NUL terminator on for you, so
you actually rarely have to keep track of it yourself. (Sometimes you
do, especially if you're building a string from scratch a character at a
time or something.)

In this section you'll find functions for pulling substrings out of
strings, concatenating strings together, getting the length of a string,
and so forth and so on.

[[pagebreak]]
## `strlen()` {#man-strlen}

Returns the length of a string.

#### Synopsis

``` {.c}
#include <string.h>

size_t strlen(const char *s);
```

#### Description 

This function returns the length of the passed null-terminated string
(not counting the NUL character at the end). It does this by walking
down the string and counting the bytes until the NUL character, so it's
a little time consuming. If you have to get the length of the same
string repeatedly, save it off in a variable somewhere.

#### Return Value

Returns the number of characters in the string.

#### Example

``` {.c}
char *s = "Hello, world!"; // 13 characters

// prints "The string is 13 characters long.":

printf("The string is %d characters long.\n", strlen(s));
```

#### See Also

[[pagebreak]]
## `strcmp()`, `strncmp()` {#man-strcmp}

Compare two strings and return a difference.

#### Synopsis

``` {.c}
#include <string.h>

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
```

#### Description 

Both these functions compare two strings. `strcmp()`
compares the entire string down to the end, while `strncmp()`
only compares the first `n` characters of the strings.

It's a little funky what they return. Basically it's a difference of
the strings, so if the strings are the same, it'll return zero (since
the difference is zero). It'll return non-zero if the strings
differ; basically it will find the first mismatched character and return
less-than zero if that character in `s1` is less than the
corresponding character in `s2`. It'll return greater-than
zero if that character in `s1` is greater than that in
`s2`.

For the most part, people just check to see if the return value is
zero or not, because, more often than not, people are only curious if
strings are the same.

These functions can be used as comparison functions for [`qsort()`](#qsort) if you have an array of
`char*`s you want to sort.

#### Return Value

Returns zero if the strings are the same, less-than zero if the first
different character in `s1` is less than that in `s2`,
or greater-than zero if the first difference character in `s1`
is greater than than in `s2`.

#### Example

``` {.c}
char *s1 = "Muffin";
char *s2 = "Muffin Sandwich";
char *s3 = "Muffin";

strcmp("Biscuits", "Kittens"); // returns < 0 since 'B' < 'K'
strcmp("Kittens", "Biscuits"); // returns > 0 since 'K' > 'B'

if (strcmp(s1, s2) == 0)
    printf("This won't get printed because the strings differ");

if (strcmp(s1, s3) == 0)
    printf("This will print because s1 and s3 are the same");

// this is a little weird...but if the strings are the same, it'll
// return zero, which can also be thought of as "false". Not-false
// is "true", so (!strcmp()) will be true if the strings are the
// same. yes, it's odd, but you see this all the time in the wild
// so you might as well get used to it:

if (!strcmp(s1, s3))
    printf("The strings are the same!")

if (!strncmp(s1, s2, 6))
    printf("The first 6 characters of s1 and s2 are the same");
```

#### See Also

[`memcmp()`](#memcmp),
[`qsort()`](#qsort)

[[pagebreak]]
## `strcat()`, `strncat()` {#man-strcat}

Concatenate two strings into a single string.

#### Synopsis

``` {.c}
#include <string.h>

int strcat(const char *dest, const char *src);
int strncat(const char *dest, const char *src, size_t n);
```

#### Description 

"Concatenate", for those not in the know, means to "stick together".
These functions take two strings, and stick them together, storing the
result in the first string.

These functions don't take the size of the first string into account
when it does the concatenation. What this means in practical terms is
that you can try to stick a 2 megabyte string into a 10 byte space.
This will lead to unintended consequences, unless you intended to lead
to unintended consequences, in which case it will lead to intended
unintended consequences.

Technical banter aside, your boss and/or professor will be irate.

If you want to make sure you don't overrun the first string, be sure
to check the lengths of the strings first and use some highly technical
subtraction to make sure things fit.

You can actually only concatenate the first `n` characters
of the second string by using `strncat()` and specifying the
maximum number of characters to copy.

#### Return Value

Both functions return a pointer to the destination string, like most
of the string-oriented functions.

#### Example

``` {.c}
char dest[20] = "Hello";
char *src = ", World!";
char numbers[] = "12345678";

printf("dest before strcat: \"%s\"\n", dest); // "Hello"

strcat(dest, src);
printf("dest after strcat:  \"%s\"\n", dest); // "Hello, world!"

strncat(dest, numbers, 3); // strcat first 3 chars of numbers
printf("dest after strncat: \"%s\"\n", dest); // "Hello, world!123"
```

Notice I mixed and matched pointer and array notation there with
`src` and `numbers`; this is just fine with
string functions.

#### See Also

[`strlen()`](#strlen)

[[pagebreak]]
## `strchr()`, `strrchr()` {#man-strchr}

Find a character in a string.

#### Synopsis

``` {.c}
#include <string.h>

char *strchr(char *str, int c);
char *strrchr(char *str, int c);
```

#### Description 

The functions `strchr()` and `strrchr` find the
first or last occurance of a letter in a string, respectively. (The
extra "r" in `strrchr()` stands for "reverse"--it looks
starting at the end of the string and working backward.)  Each function
returns a pointer to the char in question, or `NULL` if the
letter isn't found in the string.

Quite straightforward.

One thing you can do if you want to find the next occurance of the
letter after finding the first, is call the function again with the
previous return value plus one. (Remember pointer arithmetic?)  Or
minus one if you're looking in reverse. Don't accidentally go off the
end of the string!

#### Return Value

Returns a pointer to the occurance of the letter in the string, or
`NULL` if the letter is not found.

#### Example

``` {.c}
// "Hello, world!"
//       ^  ^   
//       A  B

char *str = "Hello, world!";
char *p;

p = strchr(str, ','); // p now points at position A
p = strrchr(str, 'o'); // p now points at position B
```

``` {.c}
// repeatedly find all occurances of the letter 'B'
char *str = "A BIG BROWN BAT BIT BEEJ";
char *p;

for(p = strchr(str, 'B'); p != NULL; p = strchr(p + 1, 'B')) {
    printf("Found a 'B' here: %s\n", p);
}

// output is:
//
// Found a 'B' here: BIG BROWN BAT BIT BEEJ
// Found a 'B' here: BROWN BAT BIT BEEJ
// Found a 'B' here: BAT BIT BEEJ
// Found a 'B' here: BIT BEEJ
// Found a 'B' here: BEEJ
```

#### See Also

[[pagebreak]]
## `strcpy()`, `strncpy()` {#man-strcpy}

Copy a string

#### Synopsis

``` {.c}
#include <string.h>

char *strcpy(char *dest, char *src);
char *strncpy(char *dest, char *src, size_t n);
```

#### Description 

These functions copy a string from one address to another, stopping
at the NUL terminator on the `src`string.

`strncpy()` is just like `strcpy()`, except
only the first `n` characters are actually copied. Beware that
if you hit the limit, `n` before you get a NUL terminator on
the `src` string, your `dest` string won't be
NUL-terminated. Beware! BEWARE!

(If the `src` string has fewer than `n` characters,
it works just like `strcpy()`.)

You can terminate the string yourself by sticking the `'\0'`
in there yourself:

``` {.c}
char s[10];
char foo = "My hovercraft is full of eels."; // more than 10 chars

strncpy(s, foo, 9); // only copy 9 chars into positions 0-8
s[9] = '\0';        // position 9 gets the terminator
```

#### Return Value

Both functions return `dest` for your convenience, at no
extra charge.

#### Example

``` {.c}
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
```

#### See Also

[`memcpy()`](#memcpy),
[`strcat()`](#strcat),
[`strncat()`](#strcat)

[[pagebreak]]
## `strspn()`, `strcspn()` {#man-strspn}

Return the length of a string consisting entirely of a set of
characters, or of not a set of characters.

#### Synopsis

``` {.c}
#include <string.h>

size_t strspn(char *str, const char *accept);
size_t strcspn(char *str, const char *reject);
```

#### Description 

`strspn()` will tell you the length of a string consisting
entirely of the set of characters in `accept`. That is, it
starts walking down `str` until it finds a character that is
_not_ in the set (that is, a character that is not to be
accepted), and returns the length of the string so far.

`strcspn()` works much the same way, except that it walks
down `str` until it finds a character in the `reject`
set (that is, a character that is to be rejected.)  It then returns the
length of the string so far.

#### Return Value

The lenght of the string consisting of all characters in
`accept` (for `strspn()`), or the length of the
string consisting of all characters except `reject` (for
`strcspn()`

#### Example

``` {.c}
char str1[] = "a banana";
char str2[] = "the bolivian navy on manuvers in the south pacific";

// how many letters in str1 until we reach something that's not a vowel?
n = strspn(str1, "aeiou");  // n == 1, just "a"

// how many letters in str1 until we reach something that's not a, b,
// or space?
n = strspn(str1, "ab "); // n == 4, "a ba"

// how many letters in str2 before we get a "y"?
n = strcspn(str2, "y"); // n = 16, "the bolivian nav"
```

#### See Also

[`strchr()`](#strchr),
[`strrchr()`](#strchr)

[[pagebreak]]
## `strstr()` {#man-strstr}

Find a string in another string.

#### Synopsis

``` {.c}
#include <string.h>

char *strstr(const char *str, const char *substr);
```

#### Description 

Let's say you have a big long string, and you want to find a word, or
whatever substring strikes your fancy, inside the first string. Then
`strstr()` is for you! It'll return a pointer to the
`substr` within the `str`!

#### Return Value

You get back a pointer to the occurance of the `substr`
inside the `str`, or `NULL` if the substring can't be
found.

#### Example

``` {.c}
char *str = "The quick brown fox jumped over the lazy dogs.";
char *p;

p = strstr(str, "lazy");
printf("%s\n", p); // "lazy dogs."

// p is NULL after this, since the string "wombat" isn't in str:
p = strstr(str, "wombat");
```

#### See Also

[`strchr()`](#strchr),
[`strrchr()`](#strchr),
[`strspn()`](#strspn),
[`strcspn()`](#strspn)

[[pagebreak]]
## `strtok()` {#man-strtok}

Tokenize a string.

#### Synopsis

``` {.c}
#include <string.h>

char *strtok(char *str, const char *delim);
```

#### Description 

If you have a string that has a bunch of separators in it, and you
want to break that string up into individual pieces, this function can
do it for you.

The usage is a little bit weird, but at least whenever you see the
function in the wild, it's consistently weird.

Basically, the first time you call it, you pass the string,
`str` that you want to break up in as the first argument. For
each subsequent call to get more tokens out of the string, you pass
`NULL`. This is a little weird, but `strtok()`
remembers the string you originally passed in, and continues to strip
tokens off for you.

Note that it does this by actually putting a NUL terminator after the
token, and then returning a pointer to the start of the token. So the
original string you pass in is destroyed, as it were. If you need to
preserve the string, be sure to pass a copy of it to
`strtok()` so the original isn't destroyed.

#### Return Value

A pointer to the next token. If you're out of tokens,
`NULL` is returned.

#### Example

``` {.c}
// break up the string into a series of space or
// punctuation-separated words
char *str = "Where is my bacon, dude?";
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

// output is:
//
// Word: "Where"
// Word: "is"
// Word: "my"
// Word: "bacon"
// Word: "dude"
//
```

#### See Also

[`strchr()`](#strchr),
[`strrchr()`](#strchr),
[`strspn()`](#strspn),
[`strcspn()`](#strspn)


