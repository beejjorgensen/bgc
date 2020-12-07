<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Enumerated Types: `enum`

C offers us another way to have constant integer values by name: `enum`.

In some ways, it can be better than using a `#define`. Key differences:

* `enum`s can only be integer types.
* `#define` can define anything at all.
* `enum`s are often shown by their symbolic identifier name in a debugger.
* `#define`d numbers just show as raw numbers which are harder to know
  the meaning of while debugging.

Let's see a simple example:



