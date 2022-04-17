<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Bitwise Operations

[i[Bitwise operations]<]

These numeric operations effectively allow you to manipulate individual
bits in variables, fitting since C is such a low-level langauge^[Not
that other languages don't do this---they do. It is interesting how many
modern languages use the same operators for bitwise that C does.].

If you're not familiar with bitwise operations, [flw[Wikipedia has a
good bitwise article|Bitwise_operation]].

## Bitwise AND, OR, XOR, and NOT

For each of these, the [usual arithmetic
conversions](#usual-arithmetic-conversions) take place on the operands
(which in this case must be an integer type), and then the appropriate
bitwise operation is performed.

|Operation|Operator|Example|
|-|:-:|-|
|[i[`&` bitwise AND]]AND|`&`|`a = b & c`|
|[i[`|` bitwise OR]]OR|`|`|`a = b | c`|
|[i[`^` bitwise XOR]]XOR|`^`|`a = b ^ c`|
|[i[`~` bitwise NOT]]NOT|`~`|`a = ~c`|

Note how they're similar to the Boolean operators `&&` and `||`.

These have assignment shorthand variants similar to `+=` and `-=`:

|Operator|Example|Longhand equivalent|
|-|-|-|
|[i[`&=` assignment]]`&=`|`a &= c`|`a = a & c`|
|[i[`|=` assignment]]`|=`|`a |= c`|`a = a | c`|
|[i[`^=` assignment]]`^=`|`a ^= c`|`a = a ^ c`|

## Bitwise Shift

For these, the [i[Integer promotions]] [integer
promotions](#integer-promotions) are performed on each operand (which
must be an integer type) and then a bitwise shift is executed. The type
of the result is the type of the promoted left operand.

New bits are filled with zeros, with a possible exception noted in the
implementation-defined behavior, below.

|Operation|Operator|Example|
|-|:-:|-|
|[i[`<<` shift left]]Shift left|`<<`|`a = b << c`|
|[i[`>>` shift right]]Shift right|`>>`|`a = b >> c`|

There's also the same similar shorthand for shifting:

|Operator|Example|Longhand equivalent|
|-|-|-|
|[i[`>>=` assignment]]`>>=`|`a >>= c`|`a = a >> c`|
|[i[`<<=` assignment]]`<<=`|`a <<= c`|`a = a << c`|

Watch for undefined behavior: no negative shifts, and no shifts that are
larger than the size of the promoted left operand.

Also watch for implementation-defined behavior: if you right-shift a
negative number, the results are implementation-defined. (It's perfectly
fine to right-shift a signed `int`, just make sure it's positive.)

[i[Bitwise operations]>]
