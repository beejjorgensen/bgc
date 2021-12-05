<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<iso646.h>` Alternative Operator Spellings {#iso646}

ISO-646 is a character encoding standard that's very similar to ASCII.
But it's missing a few notable characters, like `|`, `^`, and `~`.

Since these are operators or parts of operators in C, this header file
defines a number of macros you can use in case those characters aren't
found on your keyboard. (And also C++ can use these same alternate
spellings.)

|Operator|`<iso646.h>` equivalent|
|-|-|
|`&&`|`and`|
|`&=`|`and_eq`|
|`&`|`bitand`|
|`|`|`bitor`|
|`~`|`compl`|
|`!`|`not`|
|`!=`|`not_eq`|
|`||`|`or`|
|`|=`|`or_eq`|
|`^`|`xor`|
|`^=`|`xor_eq`|

Interestingly, there is no `eq` for `==`, and `&` and `!` are included
despite being in ISO-646.

Example usage:

``` {.c .numberLines}
#include <stdio.h>
#include <iso646.h>

int main(void)
{
    int x = 12;
    int y = 30;

    if (x == 12 and y not_eq 40)
        printf("Now we know.\n");
}
```

I've personally never seen this file included, but I'm sure it gets used
from time to time.