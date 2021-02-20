<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `<stdlib.h>` Standard Library Functions {#stdlib}

The `<stdlib.h>` header has all kinds of---dare I say---miscellaneous
functions bundled into it. This functionality includes:

* Conversions from numbers to strings
* Conversions from strings to numbers
* Pseudorandom number generation
* Dynamic memory allocation
* Various ways to exit the program
* Ability to run external programs
* Binary search (or some fast search)
* Quicksort (or some fast sort)
* Integer arithmetic functions
* Multibyte and wide character and string conversions

So, you know... a little of everything.

## `<stdlib.h>` Types and Macros

A couple new types and macros are introduced, though some of these might
also be defined elsewhere:

|Type|Description|
|-|-|
|`size_t`|Returned from `sizeof` and used elsewhere
|`wchar_t`|For wide character operations
|`div_t`|For the `div()` function|
|`ldiv_t`|For the `ldiv()` function|
|`lldiv_t`|for the `lldiv()` function|

And some macros:

* `NULL`, our good friend