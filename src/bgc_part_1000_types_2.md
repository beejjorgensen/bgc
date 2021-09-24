<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Types II: Way More Types!

We're used to `char`, `int`, and `float` types, but it's now time to
take that stuff to the next level and see what else we have out there in
the types department!

## Signed and Unsigned Integers

So far we've used `int` as a _signed_ type, that is, a value that can be
either negative or positive. But C also has specific _unsigned_ integer
types that can only hold positive numbers.

These types are prefaced by the keyword `unsigned`.

``` {.c}
int a;           // signed
signed int a;    // signed
signed a;        // signed, "shorthand" for "int" or "signed int", rare
unsigned int b;  // unsigned
unsigned c;      // unsigned, shorthand for "unsigned int"
```

Why? Why would you decide you only wanted to hold positive numbers?

Answer: you can get larger numbers in an unsigned variable than you can
in a signed ones.

But why is that?

You can think of integers being represented by a certain number of
_bits_^["Bit" is short for _binary digit_. Binary is just another way of
representing numbers. Instead of digits 0-9 like we're used to, it's
digits 0-1.]. On my computer, an `int` is represented by 64 bits.

And each permutation of bits that are either `1` or `0` represents a
number. We can decide how to divvy up these numbers.

With signed numbers, we use (roughly) half the permutations to represent
negative numbers, and the other half to represent positive numbers.

With unsigned, we use _all_ the permutations to represent positive
numbers.

On my computer with 64-bit `int`s using [flw[two's
complement|Two%27s_complement]] to represent unsigned numbers, I have
the following limits on integer range:

|Type|Minimum|Maximum|
|:-|-:|-:|
|`int`|`-9,223,372,036,854,775,808`|`9,223,372,036,854,775,807`|
|`unsigned int`|`0`|`18,446,744,073,709,551,615`|

Notice that the largest positive `unsigned int` is approximately twice
as large as the largest positive `int`. So you can get some flexibility
there.

## Character Types

Remember `char`? The type we can use to hold a single character?

``` {.c}
char c = 'B';

printf("%c\n", c);  // "B"
```

I have a shocker for you: it's actually an integer.

``` {.c}
char c = 'B';

// Change this from %c to %d:
printf("%d\n", c);  // 66 (!!)
```

Deep down, `char` is just a small `int`, namely an integer that uses
just a single byte of space, limiting its range to...

Here the C spec gets just a little funky. It assures us that a `char` is
a single byte, i.e. `sizeof(char) == 1`. But then in C11 §3.6¶3 it goes
out of its way to say:

> A byte is composed of a contiguous sequence of bits, _the number of
> which is implementation-defined._

Wait---what? Some of you might be used to the notion that a byte is 8
bits, right? I mean, that's what it is, right? And the answer is,
"Almost certainly."^[The industry term for a sequence of exactly,
indisputably 8 bits is an _octet_.] But C is an old language, and
machines back in the day had, shall we say, a more _relaxed_ opinion
over how many bits were in a byte. And through the years, C has retained
this flexibility.

But assuming your bytes in C are 8 bits, like they are for virtually all
machines in the world that you'll ever see, the range of a `char` is...

---So before I can tell you, it turns out that `char`s might be signed
or unsigned depending on your compiler. Unless you explicitly specify.

In many cases, just having `char` is fine because you don't care about
the sign of the data. But if you need signed or unsigned `char`s, you
_must_ be specific:

``` {.c}
char a;           // Could be signed or unsigned
signed char b;    // Definitely signed
unsigned char c;  // Definitely unsigned
```

OK, now, finally, we can figure out the range of numbers if we assume
that a `char` is 8 bits and your system uses the virtually universal
two's complement representation for signed and unsigned^[In general, f
you have an $n$ bit two's complement number, the signed range is
$-2^{n-1}$ to $2^{n-1}-1$. And the unsigned range is $0$ to $2^n-1$.].

So, assuming those constraints, we can finally figure our ranges:

|`char` type|Minimum|Maximum|
|:-|-:|-:|
|`signed char`|`-128`|`127`|
|`unsigned char`|`0`|`255`|

And the ranges for `char` are implementation-defined.

Let me get this straight. `char` is actually a number, so can we do math
on it?

Yup! Just remember to keep things in the range of a `char`!

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    char a = 10, b = 20;

    printf("%d\n", a + b);  // 30!
}
```

What about those constant characters in single quotes, like `'B'`? How
does that have a numeric value?

The spec is also hand-wavey here, since C isn't designed to run on a
single type of underlying system.

But let's just assume for the moment that your character set is based on
[flw[ASCII|ASCII]] for at least the first 128 characters. In that case,
the character constant will be converted to a `char` whose value is the
same as the ASCII value of the character.

That was a mouthful. Let's just have an example:

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    char a = 10;
    char b = 'B';  // ASCII value 66

    printf("%d\n", a + b);  // 76!
}
```

This depends on your execution environment and the [flw[character set
used|List_of_information_system_character_sets]]. One of the most
popular character sets today is [flw[Unicode|Unicode]] (which is a
superset of ASCII), so for your basic 0-9, A-Z, a-z and punctuation,
you'll almost certainly get the ASCII values out of them.

## More Integer Types: `short`, `long`, `long long`

So far we've just generally been using two integer types:

* `char`
* `int`

and we recently learned about the unsigned variants of the integer
types. And we learned that `char` was secretly a small `int` in
disguise. So we know the `int`s can come in multiple bit sizes.

But there are a couple more integer types we should look at, and the
_minimum_ minimum and maximum values they can hold.

Yes, I said "minimum" twice. The spec says that these types will hold
numbers of _at least_ these sizes, so your implementation might be
different. The header file `<limits.h>` defines macros that hold the
minimum and maximum integer values; rely on that to be sure, and _never
hardcode or assume these values_.

These additional types are `short int`, `long int`, and `long long int`.
Commonly, when using these types, C developers leave the `int` part off
(e.g. `long long`), and the compiler is perfectly happy.

``` {.c}
// These two lines are equivalent:
long long int x;
long long x;

// And so are these:
short int x;
short x;
```

Let's take a look at the integer data types and sizes in ascending
order, grouped by signedness.

|Type|Minimum Bytes|Minimum Value|Maximum Value|
|:-|-:|-:|-:|
|`char`|1|-127 or 0|127 or 255^[Depends on if a `char` defaults to `signed char` or `unsigned char`]|
|`signed char`|1|-127|127|
|`short`|2|-32767|32767|
|`int`|2|-32767|32767|
|`long`|4|-2147483647|2147483647|
|`long long`|8|-9223372036854775807|9223372036854775807|
|`unsigned char`|1|0|255|
|`unsigned short`|2|0|65535|
|`unsigned int`|2|0|65535|
|`unsigned long`|4|0|44294967295|
|`unsigned long long`|8|0|9223372036854775807|

There is no `long long long` type. You can't just keep adding `long`s
like that. Don't be silly.

> Two's complement fans might have noticed something funny about those
> numbers. Why does, for example, the `signed char` stop at -127 instead
> of -128? Remember: these are only the minimums required by the spec.
> Some number representations (like [flw[sign and
> magnitude|Signed_number_representations#Signed_magnitude_representation]])
> top off at ±127.

Let's run the same table on my 64-bit, two's complement system and see
what comes out:

|Type|My Bytes|Minimum Value|Maximum Value|
|:-|-:|-:|-:|
|`char`|1|-128|127^[My `char` is signed.]|
|`signed char`|1|-128|127|
|`short`|2|-32768|32767|
|`int`|4|-2147483648|2147483647|
|`long`|8|-9223372036854775808|9223372036854775807|
|`long long`|8|-9223372036854775808|9223372036854775807|
|`unsigned char`|1|0|255|
|`unsigned short`|2|0|65535|
|`unsigned int`|4|0|4294967295|
|`unsigned long`|8|0|18446744073709551615|
|`unsigned long long`|8|0|18446744073709551615|

That's a little more sensible, but we can see how my system has larger
limits than the minimums in the specification.

[So what are the macros in `<limits.h>`?]{#limits-macros}

|Type|Min Macro|Max Macro|
|:-|:-|:-|
|`char`|`CHAR_MIN`|`CHAR_MAX`|
|`signed char`|`SCHAR_MIN`|`SCHAR_MAX`|
|`short`|`SHRT_MIN`|`SHRT_MAX`|
|`int`|`INT_MIN`|`INT_MAX`|
|`long`|`LONG_MIN`|`LONG_MAX`|
|`long long`|`LLONG_MIN`|`LLONG_MAX`|
|`unsigned char`|`0`|`UCHAR_MAX`|
|`unsigned short`|`0`|`USHRT_MAX`|
|`unsigned int`|`0`|`UINT_MAX`|
|`unsigned long`|`0`|`ULONG_MAX`|
|`unsigned long long`|`0`|`ULLONG_MAX`|

Notice there's a way hidden in there to determine if a system uses
signed or unsigned `char`s. If `CHAR_MAX == UCHAR_MAX`, it must be
unsigned.

Also notice there's no minimum macro for the `unsigned`
variants---they're just `0`.

## More Float: `double` and `long double`

Let's see what the  spec has to say about floating point numbers in
§5.2.4.2.2¶1-2:

>The following parameters are used to define the model for each
>floating-point type:
>
> |Parameter|Definition|
> |:-|:-|
> |$s$|sign ($\pm1$)|
> |$b$|base or radix of exponent representation (an integer $> 1$)|
> |$e$|exponent (an integer between a minimum $e_{min}$ and a maximum $e_{max}$)|
> |$p$|precision (the number of base-$b$ digits in the significand)|
> |$f_k$|nonnegative integers less than $b$ (the significand digits)|
>
> A _floating-point number_ ($x$) is defined by the following model:
>
>> $x=sb^e\sum\limits_{k=1}^p f_kb^{-k},$\ \ \ \ $e_{min}\le e\le e_{max}$

I hope that cleared it right up for you.

Okay, fine. Let's step back a bit and see what's practical.

Note: we refer to a bunch of macros in this section. They can be found
in the header `<float.h>`.

Floating point number are encoded in a specific sequence of bits
([flw[IEEE-754 format|IEEE_754]] is tremendously popular) in bytes.

Diving in a bit more, the number is basically represented as the
_significand_ (which is the number part---the significant digits
themselves, also sometimes referred to as the _mantissa_) and the
_exponent_, which is what power to raise the digits to. Recall that a
negative exponent can make a number smaller.

Imagine we're using $10$ as a number to raise by an exponent. We could
represent the following numbers by using a significand of $12345$, and
exponents of $-3$, $4$, and $0$ to encode the following floating point
values:

$12345\times10^{-3}=12.345$

$12345\times10^4=123450000$

$12345\times10^0=12345$

For all those numbers, the significand stays the same. The only
difference is the exponent.

On your machine, the base for the exponent is probably $2$, not $10$,
since computers like binary. You can check it by printing the
`FLT_RADIX` macro.

So we have a number that's represented by a number of bytes, encoded in
some way. Because there are a limited number of bit patterns, a limited
number of floating point numbers can be represented.

But more particularly, only a certain number of significant decimal
digits can be represented accurately.

How can you get more? You can use larger data types!

And we have a couple of them. We know about `float` already, but for
more precision we have `double`. And for even more precision, we have
`long double` (unrelated to `long int` except by name).

The spec doesn't go into how many bytes of storage each type should
take, but on my system, we can see the relative size increases:

|Type|`sizeof`|
|:-|-:|
|`float`|4|
|`double`|8|
|`long double`|16|

So each of the types (on my system) uses those additional bits for more
precision.

But _how much_ precision are we talking, here? How many decimal numbers
can be represented by these values?

Well, C provides us with a bunch of macros in `<float.h>` to help us
figure that out.

It gets a little wonky if you are using a base-2 (binary) system for
storing the numbers (which is virtually everyone on the planet, probably
including you), but bear with me while we figure it out.

### How Many Decimal Digits?

The million dollar question is, "How many significant decimal digits can
I store in a given floating point type so that I get out the same
decimal number when I print it?"

The number of decimal digits you can store in a floating point type and
surely get the same number back out when you print it is given by these
macros:

|Type|Decimal Digits You Can Store|Minimum|My System|
|:-|-:|-:|
|`float`|`FLT_DIG`|6|6|
|`double`|`DBL_DIG`|10|15|
|`long double`|`LDBL_DIG`|10|18|

On my system, `FLT_DIG` is 6, so I can be sure that if I print out a 6
digit `float`, I'll get the same thing back. (It could be more
digits---some numbers will come back correctly with more digits. But 6
is definitely coming back.)

For example, printing out `float`s following this pattern of increasing
digits, we apparently make it to 8 digits before something goes wrong,
but after that we're back to 7 correct digits.

``` {.default}
0.12345
0.123456
0.1234567
0.12345678
0.123456791  <-- Things start going wrong
0.1234567910
```

Let's do another demo. In this code we'll have two `float`s that both
hold numbers that have `FLT_DIG` significant decimal digits^[This
program runs as its comments indicate on a system with `FLT_DIG` of `6`
that uses IEEE-754 base-2 floating point numbers. Otherwise, you might
get different output.]. Then we add those together, for what should be
12 significant decimal digits. But that's more than we can store in a
`float` and correctly recover as a string---so we see when we print it
out, things start going wrong after the 7th significant digit.


``` {.c .numberLines}
#include <stdio.h>
#include <float.h>

int main(void)
{
    // Both these numbers have 6 significant digits, so they can be
    // stored accurately in a float:

    float f = 3.14159f;
    float g = 0.00000265358f;

    printf("%.5f\n", f);   // 3.14159       -- correct!
    printf("%.11f\n", g);  // 0.00000265358 -- correct!

    // Now add them up
    f += g;                // 3.14159265358 is what f _should_ be

    printf("%.11f\n", f);  // 3.14159274101 -- wrong!
}
```

(The above code has an `f` after the numeric constants---this indicates
that the constant is type `float`, as opposed to the default of
`double`. More on this later.)

Remember that `FLT_DIG` is the safe number of digits you can store in a
`float` and retrieve correctly.

Sometimes you might get one or two more out of it. But sometimes you'll
only get `FLT_DIG` digits back. The sure thing: if you store any number
of digits up to and including `FLT_DIG` in a `float`, you're sure to get
them back correctly.

So that's the story. `FLT_DIG`. The End.

...Or is it?

### Converting to Decimal and Back

But storing a base 10 number in a floating point number and getting it
back out is only half the story.

Turns out floating point numbers can encode numbers that require more
decimal places to print out completely. It's just that your big decimal
number might not map to one of those numbers.

That is, when you look at floating point numbers from one to the next,
there's a gap. If you try to encode a decimal number in that gap, it'll
use the closest floating point number. That's why you can only encode
`FLT_DIG` for a `float`.

But what about those floating point numbers that _aren't_ in the gap?
How many places do you need to print those out accurately?

Another way to phrase this question is for any given floating point
number, how many decimal digits do I have to preserve if I want to
convert the decimal number back into an identical floating point number?
That is, how many digits do I have to print in base 10 to recover
**all** the digits in base 2 in the original number?

Sometimes it might only be a few. But to be sure, you'll want to convert
to decimal with a certain safe number of decimal places. That number is
encoded in the following macros:

|Macro|Description|
|-|-|
|`FLT_DECIMAL_DIG`|Number of decimal digits encoded in a `float`.|
|`DBL_DECIMAL_DIG`|Number of decimal digits encoded in a `double`.|
|`LDBL_DECIMAL_DIG`|Number of decimal digits encoded in a `long double`.|
|`DECIMAL_DIG`|Same as the widest encoding, `LDBL_DECIMAL_DIG`.|

Let's see an example where `DBL_DIG` is 15 (so that's all we can have in
a constant), but `DBL_DECIMAL_DIG` is 17 (so we have to convert to 17
decimal numbers to preserve all the bits of the original `double`).

Let's assign the 15 significant digit number `0.123456789012345` to `x`,
and let's assign the 1 significant digit number `0.0000000000000006` to
`y`.

``` {.default}
x is exact: 0.12345678901234500    Printed to 17 decimal places
y is exact: 0.00000000000000060
```

But let's add them together. This should give `0.1234567890123456`, but
that's more than `DBL_DIG`, so strange things might happen... let's
look:

``` {.default}
x + y not quite right: 0.12345678901234559    Should end in 4560!
```

That's what we get for printing more than `DBL_DIG`, right? But check
this out... that number, above, is exactly representable as it is!

If we assign `0.12345678901234559` (17 digits) to `z` and print it, we
get:

``` {.default}
z is exact: 0.12345678901234559   17 digits correct! More than DBL_DIG!
```

If we'd truncated `z` down to 15 digits, it wouldn't have been the same
number. That's why to preserve all the bits of a `double`, we need
`DBL_DECIMAL_DIG` and not just the lesser `DBL_DIG`.

All that being said, it's clear that when we're messing with decimal
numbers in general, it's not safe to print more than `FLT_DIG`,
`DBL_DIG`, or `LDBL_DIG` digits to be sensible in relation to the
original base 10 numbers and any subsequent math.

But when converting from `float` to a decimal representation and _back_
to `float`, definitely use `FLT_DECIMAL_DIG` to do that so that all the
bits are preserved exactly.


## Constant Numeric Types

When you write down a constant number, like `1234`, it has a type. But
what type is it? Let's look at the how C decides what type the constant
is, and how to force it to choose a specific type.

### Hexadecimal and Octal

In addition to good ol' decimal like Grandma used to bake, C also
supports constants of different bases.

If you lead a number with `0x`, it is read as a hex number:

``` {.c}
int a = 0x1A2B;   // Hexadecimal
int b = 0x1a2b;   // Case doesn't matter for hex digits

printf("%x", a);  // Print a hex number, "1a2b"
```

If you lead a number with a `0`, it is read as an octal number:

``` {.c}
int a = 012;

printf("%o\n", a);  // Print an octal number, "12"
```

This is particularly problematic for beginner programmers who try to pad
decimal numbers on the left with `0` to line things up nice and pretty,
inadvertently changing the base of the number:

``` {.c}
int x = 11111;  // Decimal 11111
int y = 00111;  // Decimal 73 (Octal 111)
int z = 01111;  // Decimal 585 (Octal 1111)
```

#### A Note on Binary

An unofficial extension^[It's really surprising to me that C doesn't
have this in the spec yet. In the C99 Rationale document, they write, "A
proposal to add binary constants was rejected due to lack of precedent
and insufficient utility." Which seems kind of silly in light of some of
the other features they kitchen-sinked in there! I'll bet one of the
next releases has it.] in many C compilers allows you to represent a
binary number with a `0b` prefix:

``` {.c}
int x = 0b101010;    // Binary 101010

printf("%d\n", x);   // Prints 42 decimal
```

There's no `printf()` format specifier for printing a binary number. You
have to do it a character at a time with bitwise operators.

### Integer Constants

You can force a constant integer to be a certain type by appending a
suffix to it that indicates the type.

We'll do some assignments to demo, but most often devs leave off the
suffixes unless needed to be precise. The compiler is pretty good at
making sure the types are compatible.

``` {.c}
int           x = 1234;
long int      x = 1234L;
long long int x = 1234LL

unsigned int           x = 1234U;
unsigned long int      x = 1234UL;
unsigned long long int x = 1234ULL;
```

The suffix can be uppercase or lowercase. And the `U` and `L` or `LL`
can appear either one first.

|Type|Suffix|
|:-|:-|
|`int`|None|
|`long int`|`L`|
|`long long int`|`LL`|
|`unsigned int`|`U`|
|`unsigned long int`|`UL`|
|`unsigned long long int`|`ULL`|

I mentioned in the table that "no suffix" means `int`... but it's
actually more complex than that.

So what happens when you have an unsuffixed number like:

``` {.c}
int x = 1234;
```

What type is it?

What C will generally do is choose the smallest type from `int` up that
can hold the value.

But specifically, that depends on the number's base (decimal, hex, or
octal), as well.

The spec has a great table indicating which type gets used for what
unsuffixed value. In fact, I'm just going to copy it wholesale right
here.

C11 §6.4.4.1¶5 reads, "The type of an integer constant is the first of
the first of the corresponding list in which its value can be
represented."

And then goes on to show this table:

+----------------+------------------------+-------------------------+
|Suffix          |Decimal Constant        |Octal or Hexadecimal\    |
|                |                        |Constant                 |
+:===============+:=======================+:========================+
|none            |`int`\                  |`int`\                   |
|                |`long int`              |`unsigned int`\          |
|                |                        |`long int`\              |
|                |                        |`unsigned long int`\     |
|                |                        |`long long int`\         |
|                |                        |`unsigned long long int`\|
|                |                        |                         |
+----------------+------------------------+-------------------------+
|`u` or `U`      |`unsigned int`\         |`unsigned int`\          |
|                |`unsigned long int`\    |`unsigned long int`\     |
|                |`unsigned long long int`|`unsigned long long int`\|
|                |                        |                         |
+----------------+------------------------+-------------------------+
|`l` or `L`      |`long int`\             |`long int`\              |
|                |`long long int`         |`unsigned long int`\     |
|                |                        |`long long int`\         |
|                |                        |`unsigned long long int`\|
|                |                        |                         |
+----------------+------------------------+-------------------------+
|Both `u` or `U`\|`unsigned long int`\    |`unsigned long int`\     |
|and `l` or `L`  |`unsigned long long int`|`unsigned long long int`\|
|                |                        |                         |
+----------------+------------------------+-------------------------+
|`ll` or `LL`    |`long long int`         |`long long int`\         |
|                |                        |`unsigned long long int`\|
|                |                        |                         |
+----------------+------------------------+-------------------------+
|Both `u` or `U`\|`unsigned long long int`|`unsigned long long int` |
|and `ll` or `LL`|                        |                         |
+----------------+------------------------+-------------------------+

What that's saying is that, for example, if you specify a number like
`123456789U`, first C will see if it can be `unsigned int`. If it
doesn't fit there, it'll try `unsigned long int`. And then `unsigned
long long int`. It'll use the smallest type that can hold the number.

### Floating Point Constants

You'd think that a floating point constant like `1.23` would have a
default type of `float`, right?

Surprise! Turns out unsuffiexed floating point numbers are type
`double`! Happy belated birthday!

You can force it to be of type `float` by appending an `f` (or
`F`---it's case-insensitive). You can force it to be of type `long
double` by appending `l` (or `L`).

|Type|Suffix|
|:-|:-|
|`float`|`F`|
|`double`|None|
|`long double`|`L`|

For example:

``` {.c}
float x       = 3.14f;
double x      = 3.14;
long double x = 3.14L;
```

This whole time, though, we've just been doing this, right?

``` {.c}
float x = 3.14;
```

Isn't the left a `float` and the right a `double`? Yes! But C's pretty
good with automatic numeric conversions, so it's more common to have an
unsuffixed floating point constant than not. More on that later.

#### Scientific Notation

Remember earlier when we talked about how a floating point number can be
represented by a significand, base, and exponent?

Well, there's a common way of writing such a number, shown here followed
by it's more recognizable equivalent which is what you get when you
actually run the math:

$1.2345\times10^3 = 1234.5$

Writing numbers in the form $s\times b^e$ is called [flw[_scientific
notation_|Scientific_notation]]. In C, these are written using "E
notation", so these are equivalent:

|Scientific Notation|E notation|
|:-|:-|
|$1.2345\times10^{-3}=12.345$|`1.2345e-3`|
|$1.2345\times10^4=123450000$|`1.2345e+4`|

You can print a number in this notation with `%e`:

``` {.c}
printf("%e\n", 123456.0);  // Prints 1.234560e+05
```

A couple little fun facts about scientific notation:

* You don't have to write them with a single leading digit before the
  decimal point. Any number of numbers can go in front.

  ``` {.c}
  double x = 123.456e+3;  // 123456
  ```

  However, when you print it, it will change the exponent so there is
  only one digit in front of the decimal point.

* The plus can be left off the exponent, as it's default, but this is
  uncommon in practice from what I've seen.

  ``` {.c}
  1.2345e10 == 1.2345e+10
  ```
* You can apply the `F` or `L` suffixes to E-notation constants:

  ``` {.c}
  1.2345e10F
  1.2345e10L
  ```

#### Hexadecimal Floating Point Constants

But wait, there's more floating to be done!

Turns out there are hexadecimal floating point constants, as well!

These work similar to decimal floating point numbers, but they begin
with a `0x` just like integer numbers.

The catch is that you _must_ specify an exponent, and this exponent
produces a power of 2. That is: $2^x$.

And then you use a `p` instead of an `e` when writing the number:

So `0xa.1p3` is $10.0625\times2^3 == 80.5$.

When using floating point hex constants, 
We can print hex scientific notation with `%a`:

``` {.c}
double x = 0xa.1p3;

printf("%a\n", x);  // 0x1.42p+6
printf("%f\n", x);  // 80.500000
```
