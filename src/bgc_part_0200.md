<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Variables, Expressions, and Statements (Oh My)

> _"It takes all kinds to make a world, does it not, Padre?"_<br>
> _"So it does, my son, so it does."_<br>
>
> Pirate Captain Thomas Bartholomew Red to the Padre, Pirates

There sure can be lotsa stuff in a C program.

Yup.

And for various reasons, it'll be easier for all of us if we classify
some of the types of things you can find in a program, so we can be
clear what we're talking about.

## Variables

It's said that "variables hold values". But another way to think about
it is that a variable is a human-readable name that refers to some data
in memory.

We're going to take a second here and take a peek down the rabbit hole
that is pointers. Don't worry about it.

You can think of memory as a big array of [bytes]^(A "byte" is an 8-bit
binary number. Think of it as an integer that can only hold the values
from 0 to 255, inclusive.) Data is stored in this ["array"]^(I'm
seriously oversimplifying how modern memory works, here. But the mental
model works, so please forgive me.). If a number is larger than a single
byte, it is stored in multiple bytes. Because memory is like an array,
each byte of memory can be referred to by its index. This index into
memory is also called an _address_, or a _location_, or a _pointer_.

When you have a variable in C, the value of that variable is in memory
_somewhere_, at some address. Of course. After all, where else would it
be? But it's a pain to refer to a value by its numeric address, so we
make a name for it instead, and that's what the variable is.

The reason I'm bringing all this up is twofold:

1. It's going to make it easier to understand pointers later.
2. Also, it's going to make it easier to understand pointers later.

So a variable is a name for some data that's stored in memory at some
address.


### Variable Types

Depending on which languages you already have in your toolkit, you might
or might not be familiar with the idea of types. But C's kinda picky
about them, so we should do a refresher.

Some example types:

* Integer: `3490`
* Floating point: `3.14159`
* [String]^(C purists will correctly point out that there's really no
  such thing as a string type in C, but we'll get to that when we get to
  pointers.): `"Hello, world!"`. Constant strings in C are held in
  double quotes.

If you need to convert between the types in C, you generally have to be
explicit about it; it won't autoconvert for you. It's C. It barely
auto-_anythings_ for you! (It makes a best effort converting between the
floating point and integer types without you needing to call a
conversion function.)

Almost all of the types in C are variants on these types.

Before you can use a variable, you have to _declare_ that variable and
tell C what type the variable holds. Once declared, the type of variable
cannot be changed later at runtime. What you set it to is what it is
until it falls out of scope and is reabsorbed into the universe.

Let's take our previous "Hello, world" code and add a couple variables
to it:

``` {.c}
#include <stdio.h>

int main(void)
{
    int i;    /* holds signed integers, e.g. -3, -2, 0, 1, 10 */
    float f;  /* holds signed floating point numbers, e.g. -3.1416 */

    printf("Hello, World!\n"); /* ah, blessed familiarity */

    return 0;
}
```

There! We've declared a couple of variables. We haven't used them yet,
and they're both uninitialized. One holds an integer number, and the
other holds a floating point number (a real number, basically, if you
have a math background).

Uninitialized variables have [indeterminate value]^(Colloquially, we say
they have "random" values, but they aren't truly---or even
pseudo-truly---random numbers.). They have to be initalized or else you
must assume they contain some nonsense number.

> This is one of the places C can "get you". Much of the time, in my
> experience, the indeterminate value is zero... but it can vary from
> run to run! Never assume the value will be zero, even if you see it
> is. _Always_ explicitly initialize variables to some value before you
> use them!

What's this? You want to store some numbers in those variables? Insanity!

Let's go ahead and do that:

``` {.c}
int main(void)
{
    int i;

    i = 2; // Assign the value 2 into the variable i

    printf("Hello, World!\n");

    return 0;
}
```

Killer. We've stored a value. Let's print it.

We're going to do that by passing _two_ amazing parameters to the
`printf()` function. The first argument is a string that describes what
to print and how to print it (called the _format string_), and the
second is the value to print, namely whatever is in the variable `i`.

`printf()` hunts through the format string for a variety of special
sequences which start with a percent sign (`%`) that tell it what to
print. For example, if it finds a `%d`, it looks to the next parameter
that was passed, and prints it out as an integer. If it finds a `%f`, it
prints the value out as a float.

As such, we can print out the value of `i` like so:

``` {.c}
int main(void)
{
    int i;

    i = 2; /* assign the value 2 into the variable i */

    printf("Hello, World! The value of i is %d, okay?\n", i);

    return 0;
}
```

And the output will be:

```shell
Hello, World! The value of i is 2, okay?
```

In this way, `printf()` might be similar to various types of format or
parameterized strings in other languages you're familiar with.

### Boolean Types

C has Boolean types, true or false?

1!

Historically, C didn't have a Boolean type, and some might argue it
still doesn't.

In C, `0` means "false", and non-zero means "true".

So `1` is true. And `37` is true. And `0` is false.

You can just declare Boolean types as `int`s:

``` {.c}
int x = 1;

if (x) {
    printf("x is true!\n");
}
```

If you `#include <stdbool.h>`, you also get access to some symbolic
names that might make things look more familiar, namely a `bool` type
and `true` and `false` values:

``` {.c}
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    bool x = true;

    if (x) {
        printf("x is true!\n");
    }

    return 0;
}
```

But these are identical to using integer values for true and false.
They're just a facade to make things look nice.


## Operators and Expressions {#operators}

C operators should be familiar to you from other languages. Let's blast
through some of them here.

(There are a bunch more details than this, but we're going to do enough
in this section to get started.)

### Arithmetic

Hopefully these are familiar:

``` {.c}
i = i + 3;  // addition (+) and assignment (=) operators, add 3 to i
i = i - 8;  // subtraction, subtract 8 from i
i = i * 9;  // multiplication
i = i / 2;  // division
i = i % 5;  // modulo (division remainder)
```

There are shorthand variants for all of the above. Each of those lines
could more tersely be written as:

``` {.c}
i += 3;  // Same as "i = i + 3", add 3 to i
i -= 8;  // Same as "i = i - 8"
i *= 9;  // Same as "i = i * 9"
i /= 2;  // Same as "i = i / 2"
i %= 5;  // Same as "i = i % 5"
```

There is no exponentiation. You'll have to use one of the `pow()`
function variants from `math.h`.

Let's get into some of the weirder stuff you might not have in your
other languages!

### Ternary Operator

C also includes the _ternary operator_. This is an expression
whose value depends on the result of a conditional embedded in it.

``` {.c}
// If x > 10, add 17 to y. Otherwise add 37 to y.

y += x > 10? 17: 37;
```

What a mess! You'll get used to it the more you read it. To help out a
bit, I'll rewrite the above expression using `if` statements:

``` {.c}
// This expression:

y += x > 10? 17: 37;

// is equivalent to this non-expression:

if (x > 10)
    y += 17;
else
    y += 37;
```

Or, another example that prints if a number stored in `x` is odd or
even:

```
printf("The number %d is %s.\n", x, x % 2 == 0?"even": "odd")
```

The `%s` format specifier in `printf()` means print a string. If the
expression `x % 2` evaluates to `0`, the value of the entire ternary
expression evaluates to the string `"even"`. Otherwise it evaluates to
the string `"odd"`. Pretty cool!

It's important to note that the ternary operator isn't flow control like
the `if` statement is. It's just an expression that evaluates to a value.


### Pre-and-Post Increment-and-Decrement

Now, let's mess with another thing that you might not have seen.

These are the legendary post-increment and post-decrement operators:

``` {.c}
i++;        // Add one to i (post-increment)
i--;        // Subtract one from i (post-decrement)
```

Very commonly, these are just used as shorter versions of:

``` {.c}
i += 1;        // Add one to i
i -= 1;        // Subtract one from i
```

but they're more subtly different than that, the clever scoundrels.

Let's take a look at this variant, pre-increment and pre-decrement:

``` {.c}
++i;        // Add one to i (pre-increment)
--i;        // Subtract one from i (pre-decrement)
```

With pre-increment and pre-decrement, the value of the variable is
incremented or decremented _before_ the expression is evaluated. Then
the expression is evaluated with the new value.

With post-increment and post-decrement, the value of the expression is
first computed with the value as-is, and _then_ the value is incremented
or decremented after the value of the expression has been determined.

You can actually embed them in expressions, like this:

``` {.c}
i = 10;
j = 5 + i++;  // Compute 5 + i, _then_ increment i

printf("%d, %d\n", i, j);  // Prints 11, 15
```

Let's compare this to the pre-increment operator:

``` {.c}
i = 10;
j = 5 + ++i;  // Increment i, _then_ compute 5 + i

printf("%d, %d\n", i, j);  // Prints 11, 16
```

This technique is used frequently with array and pointer access and
manipulation. It gives you a way to use the value in a variable, and
also increment or decrement that value before or after it is used.

But by far the most common place you'll see this is in a `for` loop:

``` {.c}
for (i = 0; i < 10; i++)
    printf("i is %d\n");
```

But more on that later.

### The Comma Operator

This is an uncommonly-used way to separated expressions that will run
left to right:

``` {.c}
x = 10, y = 20;  // First assign 10 to x, then 20 to y
```

Seems a bit silly, since you could just replace the comma with a
semicolon, right?

``` {.c}
x = 10; y = 20;  // First assign 10 to x, then 20 to y
```

But that's a little different. The latter is two separate expressions,
while the former is a single expression!

With the comma operator, the value of the comma expression is the value
of the rightmost expression:

``` {.c}
x = 1, 2, 3;

printf("x is %d\n", x);  // Prints 3, because 3 is rightmost in the comma list
```

But even that's pretty contrived. One common place the comma operator is
used is in `for` loops to do multiple things in each section of the
statement:

``` {.c}
for (i = 0, j = 10; i < 100; i++, j++)
    printf("%d, %d\n", i, j);
```

We'll revisit that later.


### Conditional Operators

For Boolean values, we have a raft of standard operators:

``` {.c}
a == b;  // True if a is equivalent to b
a != b;  // True if a is not equivalent to b
a < b;   // True if a is less than b
a > b;   // True if a is greater than b
a <= b;  // True if a is less than or equal to b
a >= b;  // True if a is greater than or equal to b
```

Don't mix up assignment `=` with comparison `==`! Use two equals to
compare, one to assign.

We can use the comparison expressions with `if` statements:

```
if (a <= 10)
    printf("Success!\n");
```

### Boolean Operators

We can chain together or alter conditional expressions with Boolean
operators for _and_, _or_, and _not_.

|Operator|Boolean meaning|
|:------:|:-------------:|
|`&&`|and|
|`||`|or|
|`!`|not|

An example of Boolean "and":

``` {.c}
// Do something if x less than 10 and y greater than 20:

if (x < 10 && y > 20)
    printf("Doing something!\n");
```

An example of Boolean "not":

``` {.c}
if (!(x < 12))
    printf("x is not less than 12\n");
```

`!` has higher precedence than the other Boolean operators, so we have
to use parentheses in that case.

Of course, that's just the same as:

``` {.c}
if (x >= 12)
    printf("x is not less than 12\n");
```

but I needed the example!


## Flow Control

Booleans are all good, but of course we're nowhere if we can't control
program flow. Let's take a look at a number of constructs: `if`, `for`,
`while`, and `do-while`.

First, a general forward-looking note about statements and blocks of
statements brought to you by your local friendly C developer:

After something like an `if` or `while` statement, you can either put a single
statement to be executed, or a block of statements to all be executed in
sequence.

Let's start with a single statement:

``` {.c}
if (x == 10) printf("x is 10");
```

This is also sometimes written on a separate line. (Whitespace is largely
irrelevant in C---it's not like Python.)

``` {.c}
if (x == 10)
    printf("x is 10\n");
```

But what if you want multiple things to happen due to the conditional?
You can use squirrelly braces to mark a _block_ or _compound statement_.

``` {.c}
if (x == 10) {
    printf("x is 10\n");
    printf("And also this happens when x is 10\n");
}
```

It's a really common style to _always_ use squirrelly braces even if
they aren't necessary:

``` {.c}
if (x == 10) {
    printf("x is 10\n");
}
```

Some devs feel the code is easier to read and avoids errors like this
where things visually look like they're in the `if` block, but actually
they aren't.

``` {.c}
// BAD ERROR EXAMPLE

if (x == 10)
    printf("x is 10\n");
    printf("And also this happens ALWAYS\n");  // Surprise!! Unconditional!
```

`while` and `for` and the other looping constructs work the same way as
the examples above. If you want to do multiple things in a loop or after
an `if`, wrap them up in squirrelly braces.

In other words, the `if` is going to run the one thing after the `if`.
And that one thing can be a single statement or a block of statements.


### The `if` statement {#ifstat}

We've already been using `if` for multiple examples, since it's likely
you've seen it in a language before, but here's another:

``` {.c}
int i = 10;

if (i > 10) {
    printf("Yes, i is greater than 10.\n");
    printf("And this will also print if i is greater than 10.\n");
}

if (i <= 10) printf("i is less than or equal to 10.\n");
```

In the example code, the message will print if `i` is greater than 10,
otherwise execution continues to the next line. Notice the squirrley
braces after the `if` statement; if the condition is true, either the
first statement or expression right after the if will be executed, or
else the collection of code in the squirlley braces after the `if` will
be executed. This sort of _code block_ behavior is common to all
statements.


### The `while` statement {#whilestat}

`while` is your average run-of-the-mill looping construct. Do a thing
while a condition expression is true.

Let's do one!

``` {.c}
// print the following output:
//
//   i is now 0!
//   i is now 1!
//   [ more of the same between 2 and 7 ]
//   i is now 8!
//   i is now 9!

i = 0;

while (i < 10) {
    printf("i is now %d!\n", i);
    i++;
}

printf("All done!\n");
```

That gets you a basic loop. C also has a `for` loop which would have
been cleaner for that example.

A not-uncommon use of `while` is for infinite loops where you repeat
while true:

``` {.c}
while (1) {
    printf("1 is always true, so this repeats forever.\n");
}
```


### The `do-while` statement {#dowhilestat}

So now that we've gotten the `while` statement under control, let's take
a look at its closely related cousin, `do-while`.

They are basically the same, except if the continuation condition is
false on the first pass, `do-while` will execute once, but `while` won't
execute at all. Let's see by example:

``` {.c}
/* using a while statement: */

i = 10;

// this is not executed because i is not less than 10:
while(i < 10) {
    printf("while: i is %d\n", i);
    i++;
}

/* using a do-while statement: */

i = 10;

// this is executed once, because the continuation condition is
// not checked until after the body of the loop runs:
do {
    printf("do-while: i is %d\n", i);
    i++;
} while (i < 10);

printf("All done!\n");
```

Notice that in both cases, the continuation condition is false right
away. So in the `while`, the condition fails, and the following block of
code is never executed. With the `do-while`, however, the condition is
checked _after_ the block of code executes, so it always executes at
least once. In this case, it prints the message, increments `i`, then
fails the condition, and continues to the "All done!" output.

The moral of the story is this: if you want the loop to execute at least
once, no matter what the continuation condition, use `do-while`.

All these examples might have been better done with a `for` loop. Let's
do something less deterministic---repeat until a certain random number
comes up!

``` {.c}
#include <stdio.h>   // For printf
#include <stdlib.h>  // For rand

int main(void)
{
    int r;

    do {
        r = rand() % 100; // Get a random number between 0 and 99
        printf("%d\n", r);
    } while (r != 37);    // Repeat until 37 comes up

    return 0;
}
```

### The `for` statement {#forstat}

Now you're starting to feel more comfortable with these looping
statements, huh! Well, listen up! It's time for something a little more
complicated: the `for` statement. This is another looping construct that
gives you a cleaner syntax than `while` in many cases, but does
basically the same thing. Here are two pieces of equivalent code:

``` {.c}
// Using a while statement:

// Print numbers between 0 and 9, inclusive:

i = 0;
while (i < 10) {
    printf("i is %d\n", i);
    i++;
}

// Do the same thing with a for-loop:

for (i = 0; i < 10; i++) {
    printf("i is %d\n", i);
}
```

That's right, kids---they do exactly the same thing. But you can see how
the `for` statement is a little more compact and easy on the eyes.
(JavaScript users will fully appreciate its C origins at this point.)

It's split into three parts, separated by semicolons. The first is
the initialization, the second is the continuation condition, and the
third is what should happen at the end of the block if the contination
condition is true. All three of these parts are optional. And empty
`for` will run forever:

``` {.c}
for(;;) {  // "forever"
    printf("I will print this again and again and again\n" );
    printf("for all eternity until the cold-death of the universe.\n");

}
```

## Types Revisited

### Unsigned and Signed Integer Types

Among the integers in C, there are two big classifications: _signed_
versus _unsigned_.

The basic idea here is that signed types can be both positive and
negative, while unsigned can only be positive.

Why would you ever use an unsigned number, then?

### Longer Types

### Types of Specific Sizes

## Casting and Conversions


# Functions {#functions}

With the previous section on building blocks fresh in your head,
let's imagine a freaky world where a program is so complicated, so
insidiously large, that once you shove it all into your
`main()`, it becomes rather unwieldy.

What do I mean by that? The best analogy I can think of is that
programs are best read, modified, and understood by humans when they are
split into convenient pieces, like a book is most conveniently read when
it is split into paragraphs.

Ever try to read a book with no paragraph breaks? It's tough, man,
believe me. I once read through _Captain Singleton_ by Daniel
Defoe since I was a fan of his, but Lord help me, the man didn't put a
single paragraph break in there. It was a brutal novel.

But I digress. What we're going to do to help us along is to put
some of those building blocks in their own functions when they become
too large, or when they do a different thing than the rest of the code.
For instance, the assignment might call for a number to be read, then
the sum of all number between 1 and it calculated and printed. It would
make sense to put the code for calculating the sum into a separate
function so that the main code a) looks cleaner, and b) the function can
be _reused_ elsewhere.

Reuse is one of the main reasons for having a function. Take the
`printf()` for instance. It's pretty complicated down there,
parsing the format string and knowing how to actually output characters
to a device and all that. Imagine if you have to rewrite all that code
every single time you wanted to output a measly string to the console?
No, no---far better to put the whole thing in a function and let
you just call it repeatedly, see?

You've already seen a few functions called, and you've even seen one
_defined_, namely the almighty `main()` (the
definition is where you actually put the code that does the work of the
function.)  But the `main()` is a little bit incomplete
in terms of how it is defined, and this is allowed for purely historical
reasons. More on that later. Here we'll define and call a normal
function called `plus_one()` that take an integer parameter
and returns the value plus one:

``` {.c}
int plus_one(int n) /* THE DEFINITION */
{
    return n + 1;
}

int main(void)
{
    int i = 10, j;
    
    j = plus_one(i); /* THE CALL */

    printf("i + 1 is %d\n", j);

    return 0;
}
```

(Before I forget, notice that I defined the function before I used it.
If hadn't done that, the compiler wouldn't know about it yet when it
compiles `main()` and it would have given an unknown function
call error. There is a more proper way to do the above code with
function prototypes, but we'll talk about that later.)

So here we have a function definition for `plus_one()`.
The first word, `int`, is the return type of the function.
That is, when we come back to use it in `main()`, the value
of the expression (in the case of the call, the expression is merely the
call itself) will be of this type. By wonderful coincidence, you'll
notice that the type of `j`, the variable that is to hold the
return value of the function, is of the same type, `int`.
This is completely on purpose.

Then we have the function name, followed by a _parameter
list_ in parenthesis. These correspond to the values in
parenthesis in the call to the function...but _they don't have to
have the same names_. Notice we call it with `i`, but the
variable in the function definition is named `n`. This is ok,
since the compiler will keep track of it for you.

Inside the `plus_one()` itself, we're doing a couple
things on one line here. We have an expression `n + 1` that is
evaluated before the `return` statement. So if we
pass the value 10 into the function, it will evaluate `10 + 1`,
which, in this universe, comes to 11, and it will return that.

Once returned to the call back in `main()`, we do the
assignment into `j`, and it takes on the return value, which
was 11. Hey look! `j` is now `i` plus one! Just
like the function was supposed to do! This calls for a celebration!

[_GENERIC PARTY SOUNDS._]

Ok, that's enough of that. Now, a couple paragraphs back, I
mentioned that the names in the parameter list of the function
definition correspond to the _values_ passed into the function.
In the case of `plus_one()`, you can call it any way you
like, as long as you call it with an `int`-type parameter.
For example, all these calls are valid:

``` {.c}
int a = 5, b = 10;

plus_one(a);     /* the type of a is int */
plus_one(10);   /* the type of 10 is int */
plus_one(1+10);  /* the type of the whole expression is still int */
plus_one(a+10);  /* the type of the whole expression is still int */
plus_one(a+b);   /* the type of the whole expression is still int */
plus_one(plus_one(a));  /* oooo! return value is int, so it's ok! */
```
    

If you're having trouble wrapping your head around that last line
there, just take it one expression at a time, starting at the innermost
parentheses (because the innermost parentheses are evaluated first,
rememeber?)  So you start at `a` and think, that's a valid
`int` to call the function `plus_one()` with, so
we call it, and that returns an `int`, and that's a valid
type to call the next outer `plus_one()` with, so we're
golden.

Hey! What about the return value from all of these? We're not
assigning it into anything! Where is it going? Well, on the last line,
the innermost call to `plus_one()` is being used to call
`plus_one()` again, but aside from that, you're
right---they are being discarded completely. This is legal, but
rather pointless unless you're just writing sample code for
demonstration purposes.

It's like we wrote "5" down on a slip of paper and passed it to the
`plus_one()` function, and it went through the trouble of
adding one, and writing "6" on a slip of paper and passing it back to
us, and then we merely just throw it in the trash without looking at it.
We're such bastards.

I have said the word "value" a number of times, and there's a good
reason for that. When we pass parameters to functions, we're doing
something commonly referred to as _passing by value_. This
warrants its own subsection.


## Passing by Value {#passvalue}

When you pass a value to a function, _a copy of that value_
gets made in this magical mystery world known as _the stack_.
(The stack is just a hunk of memory somewhere that the program allocates
memory on. Some of the stack is used to hold the copies of values that
are passed to functions.)

(Here is where the Pedant Police come and get me and say that the C
specification doesn't say anything about a stack. It's true. Your
system might not use a stack deep-down for function calls. But it's
going to look like it does, and every single C programmer on the planet
will know what you're talking about when you talk about "the stack". It
would be just mean for me to keep you in the dark. Plus, the stack
analogy is excellent for describing how recursion works.)

For now, the important part is that _a copy_ of the variable
or value is being passed to the function. The practical upshot of
this is that since the function is operating on a copy of the value, you
can't affect the value back in the calling function directly. Like if
you wanted to increment a value by one, this would NOT work:

``` {.c}
void increment(int a)
{
    a++;
}

int main(void)
{
    int i = 10;

    increment(i);

    return 0;
}
```

Wait a minute, wait a minute---hold it, hold it! What's this
`void` return type on this function? Am I trying to pull a
fast one on you? Not at all. This just means that the function doesn't
return any value. Relax!

So anyway, if I might be allowed to get on with it, you might think
that the value of `i` after the call would be 11, since that's
what the `++` does, right? This would be incorrect.
What is really happening here?

Well, when you pass `i` to the `increment()`
function, a copy gets made on the stack, right? It's the copy that
`increment()` works on, not the original; the original
`i` is unaffected. We even gave the copy a name: `a`,
right? It's right there in the parameter list of the function
definition. So we increment `a`, sure enough, but what
good does that do us? None! Ha!

That's why in the previous example with the `plus_one()`
function, we `return`ed the locally modified value
so that we could see it again in `main()`.

Seems a little bit restrictive, huh? Like you can only get one piece
of data back from a function, is what you're thinking. There is,
however, another way to get data back; people call it _passing by
reference_. But no fancy-schmancy name will distract you from the
fact that _EVERYTHING_ you pass to a function _WITHOUT
EXCEPTION_ is copied onto the stack and the function operates on
that local copy, _NO MATTER WHAT_. Remember that, even when
we're talking about this so-called passing by reference.

But that's a story for another time.


## Function Prototypes {#prototypes}

So if you recall back in the ice age a few sections ago, I mentioned
that you had to define the function before you used it, otherwise the
compiler wouldn't know about it ahead of time, and would bomb out with
an error.

This isn't quite strictly true. You can notify the compiler in
advance that you'll be using a function of a certain type that has a
certain parameter list and that way the function can be defined
anywhere at all, as long as the _function prototype_ has been
declared first.

Fortunately, the function prototype is really quite easy. It's
merely a copy of the first line of the function definition with a
semicolon tacked on the end for good measure. For example, this code
calls a function that is defined later, because a prototype has been
declared first:

``` {.c}
int foo(void); /* this is the prototype! */

int main(void)
{
    int i;
    
    i = foo();

    return 0;
}

int foo(void) /* this is the definition, just like the prototype! */
{
    return 3490;
}
```

You might notice something about the sample code we've been
using...that is, we've been using the good old `printf()`
function without defining it or declaring a prototype! How do we get
away with this lawlessness? We don't, actually. There is a prototype;
it's in that header file `stdio.h` that we included with
`#include`, remember? So we're still legit, officer!


# Variables, The Sequel {#vars2}

Just when you thought it was safe to know everything there was to
know about variables, this section of the guide lunges at you from the
darkness! What?! There's more?

Yes, I'm sorry, but I'm afraid there is. We're going to talk about a
couple things in this section that increase the power you have over
variables _TO THE EXTREME_. Yes, by now you realize that
melodrama is a well-respected part of this guide, so you probably
weren't even taken off-guard by that one, ironically.

Where was I? Oh, yes; let's talk about variable _scope_ and
_storage classes_.


## "Up Scope" {#varscope}

You recall how in some of those functions that we previously defined
there were variables that were visible from some parts of the program
but not from others? Well, if you can use a variable from a certain
part of a program, it's said to be _in scope_ (as opposed to
_out of scope_.)  A variable will be in scope if it is declared
inside the block (that is, enclosed by squirrley braces) that is
currently executing.

Take a look at this example:

``` {.c}
int frotz(int a)
{
    int b;

    b = 10; /* in scope (from the local definition) */
    a = 20; /* in scope (from the parameter list) */
    c = 30; /* ERROR, out of scope (declared in another block, in main()) */
}

int main(void)
{
    int c;

    c = 20; /* in scope */
    b = 30; /* ERROR, out of scope (declared above in frotz()) */

    return 0;
}
```

So you can see that you have to have variables declared locally for
them to be in scope. Also note that the parameter `a` is also
in scope for the function `frotz()`

What do I mean by _local variables_, anyway? These are
variable that exist and are visible only in a single basic block of code
(that is, code that is surrounded by squirrley braces) and, basic blocks
of code within them. For instance:

``` {.c}
int main(void)
{  /* start of basic block */
    int a = 5; /* local to main() */

    if (a != 0) {
        int b = 10; /* local to if basic block */
        
        a = b; /* perfectly legal--both a and b are visible here */
    }

    b = 12; /* ERROR -- b is not visible out here--only in the if */

    { /* notice I started a basic block with no statement--this is legal */
        int c = 12;
        int a; /* Hey! Wait! There was already an "a" out in main! */

        /* the a that is local to this block hides the a from main */
        a = c; /* this modified the a local to this block to be 12 */
    }

    /* but this a back in main is still 10 (since we set it in the if): */
    printf("%d\n", a);

    return 0;
}
```

There's a lot of stuff in that example, but all of it is basically a
presentation of a simple rule: when it comes to local variables, you can
only use them in the basic block in which they are declared, or in basic
blocks within that. Look at the "ERROR" line in the example to see
exactly what _won't_ work.

Let's digress for a second and take into account the special case of
parameters passed to functions. These are in scope for the entire
function and you are free to modify them to your heart's content. They
are just like local variables to the function, except that they have
copies of the data you passed in, obviously.

``` {.c}
void foo(int a)
{
    int b;

    a = b; /* totally legal */
}
```


### Global variables {#globals}

There are other types of variables besides locals. There are
_global variables_, for instance. Sounds grand, huh. Though
they're aren't exactly the chicken's pajamas for a number of reasons,
they're still a powerful piece of the language. Wield this power with
care, since you can make code that's very hard to maintain if you abuse
it.

A global variable is visible thoughout the entire file that it is
defined in (or declared in---more on that later). So it's just
like a local, except you can use it from anyplace. I guess, then, it's
rather not like a local at all. But here's an example:

``` {.c}
#include <stdio.h>

/* this is a global variable. We know it's global, because it's */
/* been declared in "global scope", and not in a basic block somewhere */
int g = 10;

void afunc(int x)
{
    g = x; /* this sets the global to whatever x is */
}

int main(void)
{
    g = 10; /* global g is now 10 */
    afunc(20); /* but this function will set it to 20 */
    printf("%d\n", g); /* so this will print "20" */

    return 0;
}
```

Remember how local variables go on the stack? Well, globals go on
_the heap_, another chunk of memory. And never the twain shall
meet. You can think of the heap as being more "permanent" than the
stack, in many ways.

(Dang it---the C Standard doesn't say anything about the heap,
either. But, like the stack, all C programmers use the terminology
and will know what you're talking about.)

Now, I mentioned that globals can be dangerous. How is that? Well,
one thing you could imagine is a large-scale project in which there were
a bazillion globals declared by a bazillion different programmers. What
if they named them the same thing? What if you thought you were using a
local, but you had forgotten to declare it, so you were using the
global instead?

(Ooo. That's a good side note: if you declare a local with the same
name as a global, it hides the global and all operations in the scope
will take place on the local variable.)

What else can go wrong? Sometimes using global variables encourages
people to not structure their code as well as they might have otherwise.
It's a good idea to not use them until there simply is no other
reasonable way to move data around.

Another thing to consider is this: does it actually make sense to
have this data stored globally for all to see? For example, if you have
a game where you use "the temperature of the world" in a lot of
different places, that might be a good candidate for a global varible.
Why? Because it's a pain to pass it around, and everyone has the same
need for it.

On the other hand, "the temperature of this guy's little finger"
might not be of so much interest to the rest of the universe. It'd be
better to store that data in a way that's more associated with the guy
than globally. We'll talk more later about associating data with things
(nice and vague, huh?) later.


## Storage Classes {#stclasses}

What is a storage class? It's a class for storing variables.

You're welcome.

Don't get this confused with any C++ class, either, since it's not at
all the same thing.

So what does a storage class declaration do? It tells the compiler
where to store the data, such as on the stack or on the heap, or if
variable data storage is already declared elsewhere.

"What?"

Let's just get some examples in there and it'll make more sense.


### Gimme some static! { #static}

Ready? Here's an example: I'm sitting on BART (the Bay Area Rapid
Transit subway) as I type this on my way home from work. There is a
young couple happily canoodling each other in the seat in front of me,
and it's completely distracting.

...Er, what. No, an example! Yes! Ok, here it is:

``` {.c}
void print_plus_one(void)
{
    static int a=0; /* static storage class! */

    printf("%d\n", a);

    a++;  /* increment the static value */
}

int main(void)
{
    print_plus_one(); /* prints "0" */
    print_plus_one(); /* prints "1" */
    print_plus_one(); /* prints "2" */
    print_plus_one(); /* prints "3" */
    print_plus_one(); /* prints "4" */

    return 0;
}
```

What have we here? How can this magic be? Isn't that a local
variable in `print_plus_one()` and doesn't it get allocated
on the stack and doesn't it go away after the function returns? How can
it possibly remember the value from the last call?

The answer: it uses the modern day magic that is the
`static` keyword. This directive (given before the type)
tells the compiler to actually store this data someplace permanent
instead of the stack! Ooooo! Well, the value gets initialized once (in
the definition), and it never gets initialized again, so all operations
on it are cumulative.

Also, if you simply define a local variable as static, it'll be
initialized automatically to zero unless you specify otherwise.

You'll probably see more use for this later, but it's common enough
that you should know about it.


### Other Storage Classes {#otherstor}

There are other storage classes, yes. The default is
`auto`, which you never see in the wild since it's
default.

Another is `extern` which tells the compiler that the
definition of the variable is in a different file. This allows you to
reference a global variable from a file, even if its definition is
somewhere else. It would be illegal to define it twice, and since it's
global it'd be nice to have it available across different source
files.

I know, I know. It's hard to imagine now, but programs do get big
enough to span multiple files eventually. `:-)`

 


# Pointers---Cower In Fear! { #pointers}

Pointers are one of the most feared things in the C language. In
fact, they are the one thing that makes this language challenging at
all. But why?

Because they, quite honestly, can cause electric shocks to come up
through the keyboard and physically _weld_ your arms permantly
in place, cursing you to a life at the keyboard.

Well, not really. But they can cause huge headaches if you don't
know what you're doing when you try to mess with them.


## Memory and Variables {#ptmem}

Computer memory holds data of all kinds, right? It'll hold
`float`s, `int`s, or whatever you have. To make
memory easy to cope with, each byte of memory is identified by an
integer. These integers increase sequentially as you move up through
memory. You can think of it as a bunch of numbered boxes, where each
box holds a byte of data. The number that represents each box is called
its _address_.

Now, not all data types use just a byte. For instance, a
`long` is often four bytes, but it really depends on the
system. You can use the `sizeof()` operator to
determine how many bytes of memory a certain type uses. (I know,
`sizeof()` looks more like a function than an
operator, but there we are.)

``` {.c}
printf("a long uses %d bytes of memory\n", sizeof(long));
```

When you have a data type that uses more than a byte of memory, the
bytes that make up the data are always adjacent to one another in
memory. Sometimes they're in order, and sometimes they're not, but
that's platform-dependent, and often taken care of for you without you
needing to worry about pesky byte orderings.

So _anyway_, if we can get on with it and get a drum roll
and some forboding music playing for the definition of a pointer, _a
pointer is the address of some data in memory_. Imagine the
classical score from 2001: A Space Odessey at this point. Ba bum ba
bum ba bum BAAAAH!

Ok, so maybe a bit overwrought here, yes? There's not a lot of
mystery about pointers. They are the address of data. Just like an
`int` can be `12`, a pointer can be the address of
data.

Often, we like to make a pointer to some data that we have stored in
a variable, as opposed to any old random data out in memory whereever.
Having a pointer to a variable is often more useful.

So if we have an `int`, say, and we want a
pointer to it, what we want is some way to get the address of that
`int`, right? After all, the pointer is just the
_address of_ the data. What operator do you suppose we'd use
to find the _address of_ the `int`?

Well, by a shocking suprise that must come as something of a shock to
you, gentle reader, we use the `address-of` operator (which
happens to be an ampersand: "`&`") to find the
address of the data. Ampersand.

So for a quick example, we'll introduce a new _format
specifier_ for `printf()` so you can print a pointer.
You know already how `%d` prints a decimal integer, yes? Well,
`%p` prints a pointer. Now, this pointer is going to look like
a garbage number (and it might be printed in hexidecimal instead of
decimal), but it is merely the number of the box the data is stored in.
(Or the number of the box that the first byte of data is stored in, if
the data is multi-byte.)  In virtually all circumstances, including this
one, the actual value of the number printed is unimportant to you, and I
show it here only for demonstration of the `address-of`
operator.

``` {.c}
#include <stdio.h>

int main(void)
{
    int i = 10;

    printf("The value of i is %d, and its address is %p\n", i, &i);

    return 0;
}
```

On my laptop, this prints:

```shell
The value of i is 10, and its address is 0xbffff964
```

(I can show off a bit here, and say that from experience the address
to me looks like its on the stack...and it is, since it's a local
variable, and all locals go on the stack. Am I cool or what.
Don't answer that.)


## Pointer Types {#pttypes}

Well, this is all well and good. You can now successfully take the
address of a variable and print it on the screen. There's a little
something for the ol' resume, right? Here's where you grab me by the
scruff of the neck and ask politely what the frick pointers are good
for.

Excellent question, and we'll get to that right after these messages
from our sponsor.

> 
`ACME ROBOTIC HOUSING UNIT CLEANING SERVICES. YOUR
HOMESTEAD WILL BE DRAMATICALLY IMPROVED OR YOU WILL BE TERMINATED.
MESSAGE ENDS.`

Welcome back to another installment of Beej's Guide to Whatever.
When we met last we were talking about how to make use of pointers.
Well, what we're going to do is store a pointer off in a variable so
that we can use it later. You can identify the _pointer type_
because there's an asterisk (`*`) before the variable name
and after its type:

``` {.c}
int main(void)
{
    int i;  /* i's type is "int" */
    int *p; /* p's type is "pointer to an int", or "int-pointer" */

    return 0;
}
```

Hey, so we have here a variable that is a pointer itself, and it can
point to other `int`s. We know it points to
`int`s, since it's of type `int*` (read
"int-pointer").

When you do an assignment into a pointer variable, the type of the
right hand side of the assignment has to be the same type as the
pointer variable. Fortunately for us, when you take the
`address-of` a variable, the resultant type is a
pointer to that variable type, so assignments like the following are
perfect:

``` {.c}
int i;
int *p; /* p is a pointer, but is uninitialized and points to garbage */

p = &i; /* p now "points to" i */
```

Get it? I know is still doesn't quite make much sense since you
haven't seen an actual use for the pointer variable, but we're taking
small steps here so that no one gets lost. So now, let's introduce you
to the anti-address-of, operator. It's kind of like what
`address-of` would be like in Bizarro World.


## Dereferencing {#defer}

A pointer, also known as an address, is sometimes also called a
_reference_. How in the name of all that is holy can there be
so many terms for exactly the same thing? I don't know the answer to
that one, but these things are all equivalent, and can be used
interchangably.

The only reason I'm telling you this is so that the name of this
operator will make any sense to you whatsoever. When you have a pointer
to a variable (AKA "a reference to a variable"), you can use the
original variable through the pointer by _dereferencing_ the
pointer. (You can think of this as "de-pointering" the pointer, but no
one ever says "de-pointering".)

What do I mean by "get access to the original variable"? Well, if you
have a variable called `i`, and you have a pointer to
`i` called `p`, you can use the dereferenced pointer
`p` _exactly as if it were the original variable
`i`_!

You almost have enough knowledge to handle an example. The last
tidbit you need to know is actually this: what is the dereference
operator? It is the asterisk, again: `*`. Now,
don't get this confused with the asterisk you used in the pointer
declaration, earlier. They are the same character, but they have
different meanings in different contexts.

Here's a full-blown example:

``` {.c}
#include <stdio.h>

int main(void)
{
    int i;
    int *p;  // this is NOT a dereference--this is a type "int*"

    p = &i; // p now points to i

    i = 10;  // i is now 10
    *p = 20; // i (yes i!) is now 20!!

    printf("i is %d\n", i);   // prints "20"
    printf("i is %d\n", *p);  // "20"! dereference-p is the same as i!

    return 0;
}
```

Remember that `p` holds the address of `i`, as you
can see where we did the assignment to `p`. What the
dereference operator does is tells the computer to _use the variable
the pointer points to_ instead of using the pointer itself. In
this way, we have turned `*p` into an alias of sorts for
`i`.


## Passing Pointers as Parameters {#ptpass}

Right about now, you're thinking that you have an awful lot of
knowledge about pointers, but absolutely zero application, right? I
mean, what use is `*p` if you could just simply say
`i` instead?

Well, my feathered friend, the real power of pointers comes into play
when you start passing them to functions. Why is this a big deal? You
might recall from before that you could pass all kinds of parameters to
functions and they'd be dutifully copied onto the stack, and then you
could manipulate local copies of those variables from within the
function, and then you could return a single value.

What if you wanted to bring back more than one single piece of data
from the function? What if I answered that question with another
question, like this:

What happens when you pass a pointer as a parameter to a function?
Does a copy of the pointer get put on the stack? _You bet your
sweet peas it does._  Remember how earlier I rambled on and on about
how _EVERY SINGLE PARAMETER_ gets copied onto the stack and the
function uses a copy of the parameter? Well, the same is true here.
The function will get a copy of the pointer.

But, and this is the clever part: we will have set up the pointer in
advance to point at a variable...and then the function can
dereference its copy of the pointer to get back to the original
variable! The function can't see the variable itself, but it can
certainly dereference a pointer to that variable! Example!

``` {.c}
#include <stdio.h>

void increment(int *p) /* note that it accepts a pointer to an int */
{
    *p = *p + 1; /* add one to p */
}

int main(void)
{
    int i = 10;

    printf("i is %d\n", i); /* prints "10" */

    increment(&i); /* note the address-of; turns it into a pointer */

    printf("i is %d\n", i); /* prints "11"! */

    return 0;
}
```

Ok! There are a couple things to see here...not the least of which
is that the `increment()` function takes an `int*`
as a parameter. We pass it an `int*` in the call by changing
the `int` variable `i` to an `int*` using
the `address-of` operator. (Remember, a pointer is
an address, so we make pointers out of variables by running them through
the `address-of` operator.)

The `increment()` function gets a copy of the pointer on
the stack. Both the original pointer `&i` (in
`main()`) and the copy of the pointer `p` (in
`increment()`) point to the same address. So dereferencing
either will allow you to modify the original variable `i`!
The function can modify a variable in another scope! Rock on!

Pointer enthusiasts will recall from early on in the guide, we used a
function to read from the keyboard, `scanf()`...and, although
you might not have recognized it at the time, we used the
`address-of` to pass a pointer to a value to
`scanf()`. We had to pass a pointer, see, because
`scanf()` reads from the keyboard and stores the result in a
variable. The only way it can see that variable that is local to that
calling function is if we pass a pointer to that variable:

``` {.c}
int i = 0;

scanf("%d", &i);        /* pretend you typed "12" */
printf("i is %d\n", i); /* prints "i is 12" */
```

See, `scanf()` dereferences the pointer we pass it in
order to modify the variable it points to. And now you know why you
have to put that pesky ampersand in there!


# Structures and Unions {#structs}

You've been messing with variables for quite some time now, and
you've made a bunch of them here and there to do stuff, yes? Usually
you bundle them into the same basic block and let it go at that.

Sometimes, though, it makes more sense to put them together into a
_structure_. This is a construct that allows you to logically
(or even illogically, if the fancy takes you) group variables into, uh,
groups. You can then reference the group as a whole. One place this
comes in really handily is if you want to pass 13 bazillion parameters
to a function, but don't want the function declaration to be that long.
You just put all the variables into a structure (or `struct`,
as it is normally called), and then pass that structure to the function.
(Actually, people usually pass a pointer to the structure, but we'll get
to that later.)

So how do you use one of these things? First off, the
`struct` itself is a new type. If you make a variable that
is a `struct foo`, its type is "`struct foo`",
just as the number 12 is of type "`int`". This is a little
bit spooky because this could be the first time you've created a new
type, and it might be unclear how that works.

Adding to the confusion, it turns out there are multiple ways to
create and use a new `struct` type, and barely any of them
are particularly intuitive. We'll have an example of one way to declare
a new `struct` here:

``` {.c}
#include <stdio.h>

/* Here we declare the type so we can use it later: */
struct stuff {
    int val;
    float b;
};

/* Note that we don't actually have any variables of that type, yet. */

int main(void)
{
    /* ok, now let's declare a variable "s" of type "struct stuff" */
    struct stuff s;

    s.val = 3490;  /* assignment into a struct! */
    s.b = 3.14159;

    printf("The val field in s is: %d\n", s.val);

    return 0;
}
```

The compiler allows us to predeclare a `struct` like in
the example. We can then use it later, like we do in
`main()` to assign values into it. When we say things like
`s.val = 3490`, we are using a special operator to access the
`val` field, known as the _dot operator_
(`.`).


## Pointers to `struct`s {#structpt}

Now let's talk a bit about how to pass these `struct`s
around to other functions and stuff. I mentioned before that you
probably want to pass a pointer to the `struct` instead of
the `struct` itself. Why?

Don't you hate it when the professor asks you a question like that,
but you're too tired in lecture to care to begin to think about it?
"Just tell me and we'll get on with it," is what you're thinking, isn't
it?

Fine! Be that way! Well, remember that when you pass parameters to
functions, and I'll clear my throat here in preparation to say again,
_EVERY PARAMETER WITHOUT FAIL GETS COPIED ONTO THE STACK when you
call a function!_  So if you have a huge `struct` that's
like 80,000 bytes in size, it's going to copy that onto the stack when
you pass it. That takes time.

Instead, why not pass a pointer to the `struct`? I hear
you---doesn't the pointer have to get copied on the stack then?
Sure does, but a pointer is only 4 or 8 bytes these days, so it's much
easier on the machine, and works faster.

And there's even a little bit of _syntactic sugar_ to help
access the fields in a pointer to a `struct`. For those that
aren't aware, syntactic sugar is a feature of a compiler that simplifies
the code even though there is another way to accomplish the same thing.
For instance, I've already mentioned the `+=` a
number of times...what does it do? Did I ever tell you? To be honest,
in all the excitement, I've forgotten myself. Here is an example that
shows how it works like another two operators, but is a little bit
easier to use. It is syntactic sugar:

``` {.c}
i = i + 12;  /* add 12 to i */
i += 12;     /* <-- does exactly the same thing as "i = i + 12" */
```

You see? It's not a necessary operator because there's another way
to do the same thing, but people like the shortcut.

But we are way off course, buster. I was talking about pointers to
`struct`s, and here we are talking about how to access them.
Here's an example wherein we have a `struct` variable, and
another variable that is a pointer to that `struct` type, and
some usage for both (this will use `struct stuff` from above):

``` {.c}
#include <stdio.h>

/* Here we declare the type so we can use it later: */
struct antelope {
    int val;
    float something;
};

int main(void)
{
    struct antelope a;
    struct antelope *b; /* this is a pointer to a struct antelope */

    b = &a; /* let's point b at a for laughs and giggles */

    a.val = 3490; /* normal struct usage, as we've already seen */

    /* since b is a pointer, we have to dereference it before we can */
    /* use it:                                                       */

    (*b).val = 3491;

    /* but that looks kinda bad, so let's do the exact same thing  */
    /* except this time we'll use the "arrow operator", which is a */
    /* bit of syntactic sugar:                                     */

    b->val = 3491; /* EXACTLY the same as (*b).val = 3491; */

    return 0;
}
```

So here we've seen a couple things. For one, we have the manner with
which we dereference a pointer to a `struct` and then use the
dot operator (`.`) to access it. This is kind of the
classic way of doing things: we have a pointer, so we dereference it to
get at the variable it points to, then we can treat it as if it
_is_ that variable.

But, syntactic sugar to the rescue, we can use the _arrow
operator_ (`->`) instead of the dot operator!
Saves us from looking ugly in the code, doesn't? The arrow operator has
a built-in dereference, so you don't have to mess with that syntax when
you have a pointer to a `struct`. So the rule is this: if
you have a `struct`, use the dot operator; if you have a
_pointer_ to a `struct`, use the arrow operator
(`->`).


## Passing `struct` pointers to functions {#ptsfunc}

This is going to be a very short section about passing pointers to
structs to functions. I mean, you already know how to pass variables as
paramters to functions, and you already know how to make and use
pointers to `struct`s, so there's really not much to be said
here. An example should show it straightforwardly enough.
("Straightforwardly"--there's an oxymoronic word if ever I saw one.)

Grrrr. Why do I always sit on this side of the bus? I mean, I know
that the sun streams in here, and I can barely read my screen with the
bright light blazing away against it. You think I'd learn by now. So
_anyway_...

``` {.c}
#include <stdio.h>

struct mutantfrog {
    int num_legs;
    int num_eyes;
};

void build_beejs_frog(struct mutantfrog *f)
{
    f->num_legs = 10;
    f->num_eyes = 1;
}

int main(void)
{
    struct mutantfrog rudolph;

    build_beejs_frog(&rudolph);  /* passing a pointer to the struct */

    printf("leg count: %d\n", rudolph.num_legs); /* prints "10" */
    printf("eye count: %d\n", rudolph.num_eyes); /* prints "1" */

    return 0;
}
```

Another thing to notice here: if we passed the `struct`
instead of a pointer to the `struct`, what would happen in
the function `build_beejs_frog()` when we changed the values?
That's right: they'd only be changed in the local copy, and not back at
out in `main()`. So, in short, pointers to
`struct`s are the way to go when it comes to passing
`struct`s to functions.


## Unions {#unions}

Now that we've covered `struct`s to a certain extent, we
can talk about a related entity called a `union`.
Syntactically, `union`s are very much the same as
`struct`s; take the above examples and replace the word
`struct` with the word `union` and you're there.
But you need to be aware of the important difference between the two.

And the important difference is this: `union`s share the
same memory space for all their members. For example, consider this
`union`:

``` {.c}
union misc_stuff{
    int train_type;
    char *crab_name;
    float cups_of_coffee;
};
```

If you store something in the `train_type` field, it's going
to munge what's in the `crab_name` field and the
`cups_of_coffee` field. That's because all three fields are
stored in the same place in memory.

The reason `union` exists is to allow you to save memory in a
`struct`-like construct when you know you won't be using the
fields at the same time. A more practical example might be if you
wanted to store data and its type, you could store the type as an
`int` (or an `[enum](#enum)`,
talked about later), and then store the data in a `union`,
below that.

``` {.c}
#include <stdio.h>

struct variable {
    // 0 = int, 1 = float, 2 = string
    int type;

    union data {
        int i;
        float f;
        char *s;
    } data;
};

void print_variable(struct variable *v)
{
    switch(v->type) {
    case 0: // int
        printf("%d\n", v->data.i);
        break;
    case 1: // float
        printf("%f\n", v->data.f);
        break;
    case 2: // string
        printf("%s\n", v->data.s);
        break;
    }
}

int main(void)
{
    struct variable v;

    v.type = 1;
    v.data.f = 0.8;

    print_variable(&v);

    return 0;
}
```

Just a quick note here to get you thinking about how you're going to
be breaking stuff down into basic blocks. You now have a lot of
different tools at your disposal: loops, conditionals,
`struct`s, and especially functions. Remember back on how
you learned to break down projects into little pieces and see what you'd
use these individual tools for.


# Arrays {#arrays}

What a wide array of information we have for you in this section.
_*BLAM*!_  We're sorry---that pun has been taken out and
shot.

An array: a linear collection of related data. Eh? It's a
continuous chunk of memory that holds a number of identical data types.
Why would we want to do that? Well, here's an example without
arrays:

``` {.c}
int age0;
int age1;
int age2;
int age3;
int age4;

age1 = 10;
printf("age 1 is %d\n", age1);
```

And here is that same example using the magical power of arrays:

``` {.c}
int age[5];

age[1] = 10;
printf("age 1 is %d\n", age[1]);
```

Ooooo! See how much prettier that is? You use the square-brackets
notation (`[]`) to access _elements_ in the array by
putting an `int` variable in there or, like we did in the
example, a constant number. Since you can _index_ the array
using a variable, it means you can do things in loops and stuff. Here's
a better example:

``` {.c}
int i;
int age[5] = {10,20,25,8,2};

for(i = 0; i < 5; i++) {
    printf("age %d is %d\n", i, age[i]);
}
```

Whoa---we've done a couple new things here. For one, you'll
notice in the array definition that we've initialized the entire array
at once. This is allowed _in the definition only_. Once you
get down to the code, it's too late to initialize the array like this,
and it must be done an element at a time.

The other thing to notice is how we've accessed the array element
inside the `for` loop: using `age[i]`. So
what happens in this `for` is that `i`
runs from 0 to 4, and so each `age` is printed out in turn,
like so:

```shell
age 0 is 10
age 1 is 20
age 2 is 25
age 3 is 8
age 4 is 2
```

Why does it run from 0 to 4 instead of 1 to 5? Good question. The
easiest answer is that the array index numbers are _zero-based_
instead of one-based. That's not really an answer, I know...well, it
turns out that most (if not all) processors use zero-based indexing in
their machine code for doing memory access, so this maps very well to
that.

You can make arrays of any type you desire, including
`struct`s, and pointers.


## Passing arrays to functions {#array2fun}

It's pretty effortless to pass an array to a function, but things get
a little wierd. The weirdest part is that when you pass the "whole"
array by putting its name in as a parameter to the function, it's
actually a _pointer to the first element of the array_ that
gets passed in for you.

Now, inside the function, you can still access the array using array
notation (`[]`). Here's an example:

``` {.c}
void init_array(int a[], int count)
{
    int i;

    /* for each element, set it equal to its index number times 10: */

    for(i = 0; i < count; i++)
        a[i] = i * 10;
}

int main(void)
{
    int mydata[10];

    init_array(mydata, 10);  /* note lack of [] notation */

    return 0;
}
```

A couple things to note here are that we didn't have to specify the
array _dimension_ (that is, how many elements are in the array)
in the declaration for `init_array()`. We could have, but we
didn't have to.

Also, since an array in C has no built-in concept of how big it is
(i.e. how many elements it holds), we have to be nice and cooperative
and actually pass in the array size separately into the function. We
use it later in the `for` to know how many elements
to initialize.

Hey! We didn't use the address-of operator in the call! Wouldn't
that make a copy of the array onto the stack? Isn't that bad? Well,
no.

When you have an array, leaving off the and square brackets gives you
a _pointer_ to the first element of the array. (You can use
the address-of operator if you want, but it actually results in a
different pointer type, so it might not be what you expect.)  So it is,
in fact, a pointer to the array that's getting copied onto the stack for
the function call, not the entire array.

Right about now, you should be recognizing that you can use arrays to
hold a lot of things, and that could serve you quite well in your
projects which often involve collections of data. For instance, let's
say:

> 
We have a virtual world where we have a number of virtual
creatures that run around doing virtual things. Each creature has a
real X and Y coordinate. There are 12 creatures. Each step of the
simulation, the creatures will process their behavior algorithm and move
to a new position. The new position should be
displayed.

Uh oh! Time for building blocks! What do we have? Ok, we need an
X and Y coordinate for the creatures. We need 12 creatures. We need a
construct that repeatedly processes the behavior for each. And we need
output.

So the coordinates of the creature. There are a few ways to do this.
You could have two arrays of 12 elements, one to hold the X and one to
hold the Y coordinate. (These are known as _parallel arrays_.)
But let's instead try to think of a way that we could bundle those data
together. I mean, they're both related to the same virtual creature, so
wouldn't it be nice to have them somehow logically related? If only
there were a way...but wait! That's right! A `struct`!

``` {.c}
struct creature {
    float x;
    float y;
};
```

There we go. Now we need 12 of them; 12 items of type `struct
creature`. What's a good way of holding a collection of identical
types? Yes, yes! Array!

``` {.c}
struct creature guys[12];
```

So what else---we need to be able to repeatedly execute the
behavior of these creatures. Like looping over and over and over...yes,
a loop would be good for that. But how many times should we loop? I
mean, the specification didn't tell us that, so it's an excellent
question. Often we'll loop until some exit condition is true (like the
user presses ESCAPE or something like that), but since the spec writer
didn't say, let's just loop forever.

``` {.c}
for(;;) {  /* loop forever */
```

Now what? We need to write the behavior of the creatures and put it
in the loop, right? Since this is a lot of self-contained code we're
about to write, we might as well stick it in a function and call that
function for each of the creatures. But I'll leave the actual
implementation of that function up to you, the reader. `:-)`

``` {.c}
for(i = 0; i < 12; i++) {
            execute_behavior(&(guys[i]));
        }
```

You notice that we did use the address-of operator there in the call.
In this case, we're not passing the whole array; we're just passing a
pointer to a single element in the array. It's not always necessary to
do that (you could have it copy the single element in the call), but
since this is a `struct`, I pass a pointer to keep the memory
overhead low.

The last thing to do was to output the information. How this is done
should be in the spec, but isn't. It would be cool to do a
hi-resolution screen with little icons for where the creatures are, but
that is currently beyond the scope of what we're doing here, so we'll
just write a simple thing to print out the creatures.

One final note---it's always a good idea to initialize the data
before using it, right? So I'll write a function that initializes the
creatures, too, before we use it. How it initializes them is also
undefined in the spec, so I'll arbitrarily set them up in a diagonal
line.

Completed (except for the behavior) code:

``` {.c}
#include <stdio.h>

struct creature {
    float x;
    float y;
};

void execute_behavior(struct creature *c)
{
    /* does nothing now */
    /* --you'll have to code it if you want them to ever move! */
}

main(int main(void)
{
    int i;
    struct creature guys[12];

    /* initialize them--could be its own function: */

    for(i = 0; i < 12; i++) {
        guys[i].x = (float)i; /* (float) is a "cast"--it changes the type! */
        guys[i].y = (float)i;
    }

    /* main loop */

    for(;;) {   /* loop forever */

        /* have them do their thing: */

        for(i = 0; i < 12; i++) {
            execute_behavior(&(guy[i]));
        }

        /* output the result */
        for(i = 0; i < 12; i++) {
            printf("creature %d: (%.2f, %.2f)\n", i, guys[i].x, guys[i].y);
        }
    }

    return 0;
}
```

I threw in a _cast_ there in the code: `(float)`.
See `i` is an `int`, but each of the fields
`guys[i].x` and `guys[i].y` is a `float`.
The cast changes the expression right after it, in this case
"`i`" to the specified type. It's always a good idea to have
the same types on both sides of the assignment operator
(`=`).

Another new thing is the "`%.2f`" in the `printf()`
format string. A plain "`%f`" means to print a
`float`, which is what we're passing it. The addtional
"`.2`" means print it using two decimal places. You can leave
the "`.2`" off and see what happens. `:-)`


# Strings {#strings}

A string is a theoretical construct in modern physics that is used to
help explain the very fabric of the universe itself.

This is _exactly_ the same as a C string, except that it is,
in fact, completely different.

A string in C is a sequence of bytes in memory that usually contains
a bunch of letters. Constant strings in C are surrounded by
double quotes (`"`). You may have seen strings before in such
programming blockbusters, such as Hello World:

``` {.c}
#include <stdio.h>

int main(void)
{
    printf("Hello, World!\n");

    return 0;
}
```

You've spotted the string `"Hello, World!\n"` in there, haven't
you?

What type is that string, anyway? Well, it turns out that a constant
string (that is, one in double quotes) is of type `char*`.
But you can also put a string in a `char` array, if you so
desire. The `char*` points at the first character in the
string. Examples: 

``` {.c}
char *s = "Hello!";

printf("%s\n", s);    /* prints "Hello!" */
printf("%c\n", *s);   /* prints 'H' */
printf("%c\n", s[0]); /* prints 'H' */
printf("%c\n", s[1]); /* prints 'e' */
printf("%c\n", s[4]); /* prints 'o' */
```

(Note the two new format specifiers for `printf()` here:
`%c` for printing a single `char`, and `%s` for
printing a string! Ain't that exciting!)

And look here, we're accessing this string in a whole variety of
different ways. We're printing the whole thing, and we're printing
single characters.

You can also initialize `char` arrays during their
definition, just like other arrays:

``` {.c}
char s[20] = "The aliens are coming!";
```

And you can change the array elements on the fly, too by simply
assigning into it:

``` {.c}
char s[20] = "Give me $10!";

printf("%s\n", s);  /* prints "Give me $10!" */

s[9] = '8';
printf("%s\n", s);  /* prints "Give me $80!" */
```

In this case, note that we've put a constant `char` on the
right side of the assignment. Constant `char`s use the
single quote (`'`).

One more thing to remember is that when you specify an array by name
without the square brackets, it's just like using a pointer to the
beginning of the array. So you can actually do assignments like
this:

``` {.c}
char a[20] = "Cats are better.";
char *p;

p = a;  /* p now points to the first char in array a */
```

One more thing: strings in C end with a NUL character. That is, a
zero. It is most properly written as '\0'. You can truncate a string
to zero length by assigning '\0' to the first byte in the string. Also,
all the standard string functions assume the string ends with a NUL
character.

Standard string functions, did I say? Yes, I did.

C provides a whole metric slew of functions that you can use to
modify strings, stick them together, parse them apart, and so on. Check
out the reference section for all the brilliant _string processing
power_ at your disposal. It is your responsibility as a citizen of
the Planet Earth to wield this ultimate power wisely! (And pay me
$80.)


# Dynamic Memory {#dydydynamic}

Well, aren't you looking dynamic this morning, reader? Or is it
evening? I always lose track of these things.

Up until now, we've been talking about memory that pretty much is set
up at the beginning of the program run. You have constant strings here
and there, arrays of predeclared length, and variables all declared
ahead of time. But what if you have something like the following?

> 
**Assignment:** Implement a program that will read an arbitrary
number of integers from the keyboard. The first line the user enters
will be the number of `int`s to read. The `int`s
themselves will appear on subsequent lines, one `int` per
line.

Yes, it's that time again: break it up into component parts that you
can implement. You'll need to read lines of text from the keyboard
(there's a cool little function called `fgets()` that can help
here), and the first line you'll need to convert to an integer so you
know how many more lines to read. (You can use `atoi()`,
read "ascii-to-integer" to do this conversion.)  Then you'll need to
read that many more strings and store them...where?

Here's where dynamic memory can help out---we need to store a
bunch of `int`s, but we don't know how many until after the
program has already started running. What we do is find out how many
`int`s we need, then we calculate how many bytes we need for
each, multiply those two numbers to get the total number of bytes we
need to store everything, and then ask the OS to allocate that many
bytes for us on the heap for us to use in the manner we choose. In this
case, we're choosing to store `int`s in there.

There are three functions we're going to talk about here. Well, make
that four functions, but one is just a variant of another:
`malloc()` (allocate some memory for us to use),
`free()` (release some memory that `malloc()` gave
us earlier), `realloc()` (change the size of some previously
allocated memory), and `calloc()` (just like
`malloc()`, except clears the memory to zero.)

Using these functions in unison results in a beautifully intricate
dance of data, ebbing and flowing with the strong tidal pull of the
dedicated user's will.

Yeah. Let's cut the noise and get on with it here.


## `malloc()` {#mallocg}

This is the big one: he's the guy that gives you memory when you ask
for it. It returns to you a pointer to a chunk of memory of a specified
number of bytes, or `NULL` if there is some kind of error
(like you're out of memory). The return type is actually
`void*`, so it can turn into a pointer to whatever you
want.

Since `malloc()` operates in bytes of memory and you often
operate with other data types (e.g. "Allocate for me 12
`int`s."), people often use the `sizeof()`
operator to determine how many bytes to allocate, for example:

``` {.c}
int *p;

p = malloc(sizeof(int) * 12); // allocate for me 12 ints! 
```

Oh, and that was pretty much an example of how to use
`malloc()`, too. You can reference the result using pointer
arithmetic or array notation; either is fine since it's a pointer. But
you should really check the result for errors:

``` {.c}
int *p;

p = malloc(sizeof(float) * 3490); // allocate 3490 floats!

if (p == NULL) {
    printf("Horsefeathers! We're probably out of memory!\n");
    exit(1);
}
```

More commonly, people pack this onto one line:

``` {.c}
if ((p = malloc(100)) == NULL) { // allocate 100 bytes
    printf("Ooooo! Out of memory error!\n");
    exit(1);
}
```

Now remember this: you're allocating memory on the heap and there are
only two ways to ever get that memory back: 1) your program can exit, or
2) you can call `free()` to free a `malloc()`'d
chunk. If your program runs for a long time and keeps
`malloc()`ing and never `free()`ing when it
should, it's said to "leak" memory. This often manifests itself in a
way such as, "Hey, Bob. I started your print job monitor program a week
ago, and now it's using 13 terabytes of RAM. Why is that?"

Be sure to avoid memory leaks! `free()` that memory when
you're done with it!


## `free()` {#gfree}

Speaking of how to free memory that you've allocated, you do it with
the implausibly-named `free()` function.

This function takes as its argument a pointer that you've picked up
using `malloc()` (or `calloc()`). And it releases
the memory associated with that data. You really should never use
memory after it has been `free()`'d. It would be Bad.

So how about an example:

``` {.c}
int *p;

p = malloc(sizeof(int) * 37); // 37 ints!

free(p);  // on second thought, never mind! 
```

Of course, between the `malloc()` and the
`free()`, you can do anything with the memory your twisted
little heart desires.


## `realloc()` {#grealloc}

`realloc()` is a fun little function that takes a chunk of
memory you allocated with `malloc()` (or
`calloc()`) and changes the size of the memory chunk. Maybe
you thought you only needed 100 `int`s at first, but now you
need 200. You can `realloc()` the block to give you the
space you need.

This is all well and good, except that `realloc()` might
have to _move your data_ to another place in memory if it
can't, for whatever reason, increase the size of the current block.
It's not omnipotent, after all.

What does this mean for you, the mortal? Well in short, it means you
should use `realloc()` sparingly since it could be an
expensive operation. Usually the procedure is to keep track of how much
room you have in the memory block, and then add another big chunk to it
if you run out. So first you allocate what you'd guess is enough room
to hold all the data you'd require, and then if you happened to run out,
you'd reallocate the block with the next best guess of what you'd
require in the future. What makes a good guess depends on the program.
Here's an example that just allocates more "buckets" of space as
needed:

``` {.c}
#include <stdlib.h>

#define INITIAL_SIZE 10
#define BUCKET_SIZE 5

static int data_count; // how many ints we have stored
static int data_size;  // how many ints we *can* store in this block
static int *data;      // the block of data, itself

int main(void)
{
    void add_data(int new_data); // function prototype
    int i;

    // first, initialize the data area:
    data_count = 0;
    data_size = INITIAL_SIZE;
    data = malloc(data_size * sizeof(int)); // allocate initial area

    // now add a bunch of data
    for(i = 0; i < 23; i++) {
        add_data(i);
    }

    return 0;
}

void add_data(int new_data)
{
    // if data_count == data_size, the area is full and
    // needs to be realloc()'d before we can add another:

    if (data_count == data_size) {
        // we're full up, so add a bucket
        data_size += BUCKET_SIZE;
        data = realloc(data, data_size * sizeof(int));
    }

    // now store the data
    *(data+data_count) = new_data;

    // ^^^ the above line could have used array notation, like so:
    //  data[data_count] = new_data;

    data_count++;
}
```

In the above code, you can see that a potentially expensive
`realloc()` is only done after the first 10 `int`s
have been stored, and then again only after each block of five after
that. This beats doing a `realloc()` every time you add a
number, hands down.

(Yes, yes, in that completely contrived example, since I know I'm
adding 23 numbers right off the get-go, it would make much more sense to
set `INITIAL_SIZE` to 25 or something, but that defeats the
whole purpose of the example, now, doesn't it?)


## `calloc()` {#gcalloc}

Since you've already read the section on `malloc()` (you
have, right?), this part will be easy! Yay! Here's the scoop:
`calloc()` is just like `malloc()`, except that it
1) clears the memory to zero for you, and 2) it takes two parameters
instead of one.

The two parameters are the number of elements that are to be in the
memory block, and the size of each element. Yes, this is exactly like
we did in `malloc()`, except that `calloc()` is
doing the multiply for you:

``` {.c}
// this:
p = malloc(10 * sizeof(int));

// is just like this:
p = calloc(10, sizeof(int));
// (and the memory is cleared to zero when using calloc())
```

The pointer returned by `calloc()` can be used with
`realloc()` and `free()` just as if you had used
`malloc()`.

The drawback to using `calloc()` is that it takes time to
clear memory, and in most cases, you don't need it clear since you'll
just be writing over it anyway. But if you ever find yourself
`malloc()`ing a block and then setting the memory to zero
right after, you can use `calloc()` to do that in one
call.

I wish this section on `calloc()` were more exciting, with
plot, passion, and violence, like any good Hollywood picture, but...this
is C programming we're talking about. And that should be exciting in
its own right. Sorry!


# More Stuff! { #morestuff}

This is the section where we flesh out a bunch of the stuff we'd done
before, except in more detail. We even throw a couple new things in
there for good measure. You can read these sections in any order you
want and as you feel you need to.


## Pointer Arithmetic {#ptarith}

Pointer _what_? Yeah, that's right: you can perform math on
pointers. What does it mean to do that, though? Well, pay attention,
because people use _pointer arithmetic_ all the time to
manipulate pointers and move around through memory.

You can add to and subtract from pointers. If you have a pointer to
a `char`, incrementing that pointer moves to the next
`char` in memory (one byte up). If you have a pointer to an
`int`, incrementing that pointer moves to the next
`int` in memory (which might be four bytes up, or some other
number depending on your CPU architecture.)  It's important to know that
the number of bytes of memory it moves differs depending on the type of
pointer, but that's actually all taken care of for you.

``` {.c}
/* This code prints: */
/*  50               */
/*  99               */
/*  3490             */

int main(void)
{
    int a[4] = { 50, 99, 3490, 0 };
    int *p;

    p = a;
    while(*p > 0) {
        printf("%i\n", *p);
        p++; /* go to the next int in memory */
    }

    return 0;
}
```

What have we done! How does this print out the values in the array?
First of all, we point `p` at the first element of the array.
Then we're going to loop until what `p` points at is less than
or equal to zero. Then, inside the loop, we print what `p` is
pointing at. Finally, _and here's the tricky part_, we
_increment the pointer_. This causes the pointer to move to
the next `int` in memory so we can print it.

In this case, I've arbitrarily decided (yeah, it's shockingly true: I
just make all this stuff up) to mark the end of the array with a zero
value so I know when to stop printing. This is known as a _sentinel
value_...that is, something that lets you know when some data ends.
If this sounds familiar, it's because you just saw it in the section on
strings. Remember---strings end in a zero character
(`'\0'`) and the string functions use this as a sentinel value to
know where the string ends.

Lots of times, you see a `for` loop used to go
through pointer stuff. For instance, here's some code that copies a
string:

``` {.c}
char *source = "Copy me!";
char dest[20];  /* we'll copy that string into here */

char *sp;  /* source pointer */
char *dp;  /* destination pointer */

for(sp = source, dp = dest; *sp != '\0'; sp++, dp++) {
    *dp = *sp;
}

printf("%s\n", dest); /* prints "Copy me!" */
```

Looks complicated! Something new here is the _comma
operator_ (`,`). The comma operator allows you
to stick expressions together. The total value of the expression is the
rightmost expression after the comma, but all parts of the expression
are evaluated, left to right.

So let's take apart this `for` loop and see
what's up. In the initialization section, we point `sp` and
`dp` to the source string and the destination area we're going
to copy it to.

In the body of the loop, the actual copy takes place. We copy, using
the assignment operator, the character that the source pointer points
to, to the address that the destination pointer points to. So in this
way, we're going to copy the string a letter at a time.

The middle part of the `for` loop is the
continuation condition---we check here to see if the source pointer
points at a NUL character which we know exists at the end of the source
string. Of course, at first, it's pointing at `'C'` (of the
"Copy me!" string), so we're free to continue.

At the end of the `for` statement we'll
increment both `sp` and `dp` to move to the next
character to copy. Copy, copy, copy!


## `typedef` {#typedef}

This one isn't too difficult to wrap your head around, but there are
some strange nuances to it that you might see out in the wild.
Basically `typedef` allows you to make up an alias
for a certain type, so you can reference it by that name instead.

Why would you want to do that? The most common reason is that the
other name is a little bit too unwieldy and you want something more
concise...and this most commonly occurs when you have a
`struct` that you want to use.

``` {.c}
struct a_structure_with_a_large_name {
    int a;
    float b;
};

typedef struct a_structure_with_a_large_name NAMESTRUCT;

int main(void)
{
    /* we can make a variable of the structure like this: */
    struct a_structure_with_a_large_name one_variable;

    /* OR, we can do it like this: */
    NAMESTRUCT another_variable;

    return 0;
}
```

In the above code, we've defined a type, `NAMESTRUCT`,
that can be used in place of the other type, `struct
a_structure_with_a_large_name`. Note that this is now a
full-blown type; you can use it in function calls, or whereever you'd
use a "normal" type. (Don't tell `typedef`'d types
they're not normal---it's impolite.)

You're probably also wondering why the new type name is in all caps.
Historically, `typedef`'d types have been all caps
in C by convention (it's certainly not necessary.)  In C++, this is no
longer the case and people use mixed case more often. Since this is a C
guide, we'll stick to the old ways.

(One thing you might commonly see is a `struct` with an
underscore before the `struct` tag name in the
`typedef`. Though technically illegal, many
programmers like to use the same name for the `struct` as
they do for the new type, and putting the underscore there
differentiates the two visually. But you shouldn't do it.)

You can also typedef "anonymous" `struct`s, like this

``` {.c}
typedef struct {
    int a;
    float b;
} someData;
```

So then you can define variables as type `someData`. Very
exciting.


## `enum` {#enum}

Sometimes you have a list of numbers that you want to use to
represent different things, but it's easier for the programmer to
represent those things by name instead of number. You can use an
`enum` to make symbolic names for integer numbers
that programmers can use later in their code in place of
`int`s.

(I should note that C is more relaxed than C++ is here about
interchanging `int`s and `enum`s.
We'll be all happy and C-like here, though.)

Note that an `enum` is a type, too. You can
`typedef` it, you can pass them into functions, and
so on, again, just like "normal" types.

Here are some enums and their usage. Remember---treat them just
like `int`s, more or less.

``` {.c}
enum fishtypes {
    HALIBUT,
    TUBESNOUT,
    SEABASS,
    ROCKFISH
};

int main(void)
{
    enum fishtypes fish1 = SEABASS;
    enum fishtypes fish2;

    if (fish1 == SEABASS) {
        fish2 = TUBESNOUT;
    }

    return 0;
}
```

Nothing to it---they're just symbolic names for unique numbers.
Basically it's easier for other programmers to read and maintain.

Now, you can print them out using `%d` in
`printf()`, if you want. For the most part, though, there's
no reason to know what the actual number is; usually you just want the
symbolic representation.

But, since I know you're dying of curiosity, I might as well tell you
that the `enum`s start at zero by default, and increase from
there. So in the above example, `HALIBUT` would be 0,
`TUBESNOUT` would be 1, and `ROCKFISH` would be 3.

If you want, though, you can override any or all of these:

``` {.c}
enum frogtypes {
    THREELEGGED=3,
    FOUREYED,
    SIXHEADED=6
};
```

In the above case, two of the enums are explicitly defined. For
`FOUREYED` (which isn't defined), it just increments one from the
last defined value, so its value is 4. You can, if you're curious, have
duplicate values, but why would you want to, sicko?


## More `struct` declarations {#morestruct}

Remember how, many moons ago, I mentioned that there were a number of
ways to declare `struct`s and not all of them made a whole
lot of sense. We've already seen how to declare a `struct`
globally to use later, as well as one in a
`typedef` situation, _comme ca_:

``` {.c}
/* standalone: */

struct antelope {
    int legcount;
    float angryfactor;
};

/* or with typedef: */

typedef struct _goatcheese {
    char victim_name[40];
    float cheesecount;
} GOATCHEESE;
```

But you can also declare variables along with the `struct`
declaration by putting them directly afterward:

``` {.c}
struct breadtopping {
    enum toppingtype type; /* BUTTER, MARGARINE or MARMITE */
    float amount;
} mytopping;

/* just like if you'd later declared: */

struct breadtopping mytopping;
```

So there we've kinda stuck the variable defintion on the tail end
of the `struct` definition. Pretty sneaky, but you see that
happen from time to time in that so-called _Real Life_ thing
that I hear so much about.

And, just when you thought you had it all, you can actually omit the
`struct` name in many cases. For example:

``` {.c}
typedef struct { /* <--Hey! We left the name off! */
    char name[100];
    int num_movies;
} ACTOR_PRESIDENT;
```

It's more _right_ to name all your `struct`s,
even if you don't use the proper name and only use the
`typedef`'d name, but you still see those naked
`struct`s here and there.


## Command Line Arguments {#clargs}

I've been lying to you this whole time, I must admit. I thought I
could hide it from you and not get caught, but you realized that
something was wrong...why doesn't the `main()` have a return
type or argument list?

Well, back in the depths of time, for some reason, !!!TODO
research!!! it was perfectly acceptable to do that. And it persists to
this day. Feel free to do that, in fact,  But that's not telling you
the whole story, and it's time you knew the _whole truth_!

Welcome to the real world:

``` {.c}
int main(int argc, char **argv)
```

Whoa! What is all that stuff? Before I tell you, though, you have
to realize that programs, when executed from the command line, accept
arguments from the command line program, and return a result to the
command line program. Using many Unix shells, you can get the return
value of the program in the shell variable `$?`. (This doesn't
even work in the windows command shell---use !!!TODO look up
windows return variable!!! instead.)  And you specify parameters to the
program on the command line after the program name. So if you have a
program called "makemoney", you can run it with parameters, and then
check the return value, like this:

```shell
$ makemoney fast alot
$ echo $?
2
```

In this case, we've passed two command line arguments, "fast" and
"alot", and gotten a return value back in the variable `$?`,
which we've printed using the Unix `echo` command. How
does the program read those arguments, and return that value?

Let's do the easy part first: the return value. You've noticed that
the above prototype for `main()` returns an `int`.
Swell! So all you have to do is either `return`
that value from `main()` somewhere, or, alternatively, you
can call the function `exit()` with an exit value as the
parameter:

``` {.c}
int main(void)
{
    int a = 12;

    if (a == 2) {
        exit(3); /* just like running (from main()) "return 3;" */
    }

    return 2; /* just like calling exit(2); */
}
```

For historical reasons, an exit status of `0` has meant
success, while nonzero means failure. Other programs can check your
program's exit status and react accordingly.

Ok that's the return status. What about those arguments? Well, that
whole definition of `argv` looks too intimidating to start
with. What about this `argc` instead? It's just an
`int`, and an easy one at that. It contains the total count
of arguments on the command line, _including the name of the program
itself_. For example:

```shell
$ makemoney fast alot         # <-- argc == 3
$ makemoney                   # <-- argc == 1
$ makemoney 1 2 3 4 5         # <-- argc == 6
```

(The dollar sign, above, is a common Unix command shell prompt. And
that hash mark (`#`) is the command shell comment character in
Unix. I'm a Unix-dork, so you'll have to deal. If you have a problem,
talk to those friendly Stormtroopers over there.)

Good, good. Not much to that `argc business`, either. Now
the biggie: `argv`. As you might have guessed, this is where
the arguments themselves are stored. But what about that
`char**` type? What do you do with that? Fortunately, you
can often use array notation in the place of a dereference, since you'll
recall arrays and pointers are related beasties. In this case, you can
think of `argv` as an array of pointers to strings, where each
string pointed to is one of the command line arguments:

```shell
$ makemoney   somewhere  somehow
$ # argv[0]    argv[1]   argv[2]   (and argc is 3)
```

Each of these array elements, `argv[0]`, `argv[1]`,
and so on, is a string. (Remember a string is just a pointer to a
`char` or an array of `char`s, the name of which
is a pointer to the first element of the array.)

I haven't told you much of what you can do with strings yet, but
check out the reference section for more information. What you do know
is how to `printf()` a string using the "`%s`" format
specifier, and you do know how to do a loop. So let's write a program
that simply prints out its command line arguments, and then sets an exit
status value of 4:

``` {.c}
/* showargs.c */

#include <stdio.h>

int main(int argc, char **argv)
{
    int i;

    printf("There are %d things on the command line\n", argc);
    printf("The program name is \"%s\"\n", argv[0];

    printf("The arguments are:\n");
    
    for(i = 1; i < argc; i++) {
        printf("    %s\n", argv[i]);
    }

    return 4; /* exit status of 4 */
}
```

Note that we started printing arguments at index 1, since we already
printed `argv[0]` before that. So sample runs and output
(assuming we compiled this into a program called
`showargs`):

```shell
$ showargs alpha bravo
There are 3 things on the command line
The program name is "showargs"
The arguments are:
    alpha
    bravo

$ showargs
There are 1 things on the command line
The program name is "showargs"
The arguments are:

$ showargs 12
There are 2 things on the command line
The program name is "showargs"
The arguments are:
    12
```

(The actual thing in `argv[0]` might differ from system to
system. Sometimes it'll contain some path information or other
stuff.)

So that's the secret for getting stuff into your program from the
command line!


## Multidimensional Arrays {#multiarray}

Welcome to...the _Nth Dimension!_  Bet you never thought
you'd see that. Well, here we are. Yup. The Nth Dimension.

Ok, then. Well, you've seen how you can arrange sequences of data in
memory using an array. It looks something like this:

!!!TODO image of 1d array

Now, imagine, if you will, a _grid_ of elements instead of
just a single row of them:

This is an example of a _two-dimensional_ array, and can be
indexed by giving a row number and a column number as the index, like
this: `a[2][10]`. You can have as many dimensions in an array
that you want, but I'm not going to draw them because 2D is already past
the limit of my artistic skills.

So check this code out---it makes up a two-dimensional array,
initializes it in the definition (see how we nest the squirrelly braces
there during the init), and then uses a _nested loop_ (that is,
a loop inside another loop) to go through all the elements and
pretty-print them to the screen.

``` {.c}
#include <stdio.h>

int main(void)
{
    int a[2][5] = { { 10, 20, 30, 40, 55 },  /* [2][5] == [rows][cols] */
                    { 10, 18, 21, 30, 44 } };

    int i, j;

    for(i = 0; i < 2; i++) {     /* for all the rows... */
        for(j = 0; j < 5; j++) { /* print all the columns! */
            printf("%d ", a[i][j]);
        }

        /* at the end of the row, print a newline for the next row */
        printf("\n"); 
    }

    return 0;
}
```

As you might well imagine, since there really is no surprise ending
for a program so simple as this one, the output will look something like
this:

```shell
10 20 30 40 55
10 18 21 30 44
```

Hold on for a second, now, since we're going to take this concept for
a spin and learn a little bit more about how arrays are stored in
memory, and some of tricks you can use to access them. First of all,
you need to know that in the previous example, even though the array has
two rows and is multidimensional, the data is stored sequentially in
memory in this order: 10, 20, 30, 40, 55, 10, 18, 21, 30, 44.

See how that works? The compiler just puts one row after the next
and so on.

But hey! Isn't that just like a one-dimensional array, then? Yes,
for the most part, it technically is! A lot of programmers don't even
bother with multidimensional arrays at all, and just use single
dimensional, doing the math by hand to find a particular row and
column. You can't technically just switch dimensions whenever you feel
like it, Buccaroo Bonzai, because the types are different. And it'd be
bad form, besides.

For instance...nevermind the "for instance". Let's do the same
example again using a single dimensional array:

``` {.c}
#include <stdio.h>

int main(void)
{
    int a[10] = {   10, 20, 30, 40, 55,  /* 10 elements (2x5) */
                    10, 18, 21, 30, 44 };

    int i, j;

    for(i = 0; i < 2; i++) {     /* for all the rows... */
        for(j = 0; j < 5; j++) { /* print all the columns! */
            int index = i*5 + j; /* calc the index */
            printf("%d ", a[index]);
        }

        /* at the end of the row, print a newline for the next row */
        printf("\n"); 
    }

    return 0;
}
```

So in the middle of the loop we've declared a local variable
`index` (yes, you can do that---remember local variables
are local to their block (that is, local to their surrounding squirrley
braces)) and we calculate it using `i` and `j`. Look
at that calculation for a bit to make sure it's correct. This is
technically what the compiler does behind your back when you accessed
the array using multidimensional notation.


## Casting and promotion {#cast}

Sometimes you have a type and you want it to be a different type.
Here's a great example:

``` {.c}
int main(void)
{
    int a = 5;
    int b = 10;
    float f;

    f = a / b;  /* calculate 5 divided by 10 */
    printf("%.2f\n", f);

    return 0;
}
```

And this prints:

``` {.c}
0
```

...What? Five divided by 10 is zero? Since when? I'll tell you:
since we entered the world of integer-only division. When you divide
one `int` by another `int`, the result is an
`int`, and any fractional part is thrown away. What do we do
if we want the result to become a `float` somewhere along the
way so that the result is correct?

Turns out, either integer (or both) in the divide can be made into a
`float`, and then the result of the divide will be also be a
`float`. So just change one and everything should work
out.

"Get on with it! How do you cast?"  Oh yeah---I guess I should
actually do it. You might recall the cast from other parts of this
guide, but just in case, we'll show it again:

``` {.c}
f = (float)a / b;  /* calculate 5 divided by 10 */
```

Bam! There is is! Putting the new type in parens in front of the
expression to be converted, and it magically becomes that type!

You can cast almost anything to almost anything else, and if you mess
it up somehow, it's entirely your fault since the compiler will blindly
do whatever you ask. `:-)`


## Incomplete types {#incomplete}

This topic is a little bit more advanced, but bear with it for a bit.
An incomplete type is simply the declaration of the name of a particular
`struct`, put there so that you can use pointers to the
`struct` without actually knowing the fields stored therein.
It most often comes up when people don't want to
`#include` another header file, which can happen
for a variety of different reasons.

For example, here we use a pointer to a type without actually having
it defined anywhere in `main()`. (It is defined elsewhere,
though.)

``` {.c}
struct foo;  /* incomplete type! Notice it's, well, incomplete. */

int main(void)
{
    struct foo *w;

    w = get_next_wombat(); /* grab a wombat */
    process_wombat(w);     /* use it somewhere */

    return 0;
}
```

I'm telling you this in case you find yourself trying to include a
header that includes another header that includes the same header, or if
your builds are taking forever because you're including too many
headers, or...more likely you'll see an error along the lines of "cannot
reference incomplete type". This error means you've tried to do too
much with the incomplete type (like you tried to dereference it or use a
field in it), and you need to `#include` the right
header file with the full complete declaration of the
`struct`.


## `void` pointers {#voidp}

Welcome to _THE VOID_! As Neo Anderson would say,
"...Whoa."  What is this `void` thing?

Stop! Before you get confused, a `void` pointer isn't the
same thing as a `void` return value from a function or a
`void` argument list. I know that can be confusing, but
there it is. Just wait until we talk about all the ways you can use the
`static` keyword.

A `void` pointer is a _pointer to any type_. It
is automatically cast to whatever type you assign into it, or copy from
it. Why would you want to ever use such a thing? I mean, if you're
going to dereference a pointer so that you can get to the original
value, doesn't the compiler need to know what type the pointer is so
that it can use it properly?

Yes. Yes, it does. Because of that, you cannot dereference a
`void` pointer. It's against the law, and the C Police will
be at your door faster than you can say Jack Robinson. Before you can
use it, you have to cast it to another pointer type.

How on Valhalla is this going to be of any use then? Why would you
even want a pointer you didn't know the type of?

> 
**The Specification:** Write a function that can append
pointers _of any type_ to an array. Also write a function that
can return a particular pointer for a certain index.

So in this case, we're going to write a couple useful little
functions for storing off pointers, and returning them later. The
function has be to _type-agnostic_, that is, it must be able to
store pointers of any type. This is something of a fairly common
feature to libraries of code that manipulate data---lots of them
take `void` pointers so they can be used with any type of
pointer the programmer might fancy.

Normally, we'd write a linked list or something to hold these, but
that's outside the scope of this book. So we'll just use an array,
instead, in this superficial example. Hmmm. Maybe I should write a
beginning data structures book...

Anyway, the specification calls for two functions, so let's pound
those puppies out right here:

``` {.c}
#include <stdio.h>

void *pointer_array[10]; /* we can hold up to 10 void-pointers */
int index=0;

void append_pointer(void *p)
{
    pointer_array[index++] = p;
}

void *get_pointer(int i)
{
    return pointer_array[i];
}
```

Since we need to store those pointers somewhere, I went ahead and
made a global array of them that can be accessed from within both
functions. Also, I made a global index variable to remember where to
store the next appended pointer in the array.

So check the code out for `append_pointer()` there. How
is all that crammed together into one line? Well, we need to do two
things when we append: store the data at the current index, and move the
index to the next available spot. We copy the data using the assignment
operator, and then notice that we use the _post-increment_
operator (`++`) to increment the index. Remember
what _post_-increment means? It means the increment is done
_after_ the rest of the expression is evaluated, including that
assignment.

The other function, `get_pointer`, simply returns the
`void*` at the specified index, `i`. What you want
to watch for here is the subtle difference between the return types of
the two functions. One of them is declared `void`, which
means it doesn't return anything, and the other one is declared with a
return type of `void*`, which means it returns a
`void` pointer. I know, I know, the duplicate usage is a
little troublesome, but you get used to it in a big hurry. Or else!

Finally, we have that code all written---now how can we actually
use it? Let's write a `main()` function that will use these
functions:

``` {.c}
int main(void)
{
    char *s = "some data!";  /* s points to a constant string (char*) */
    int a = 10;
    int *b;

    char *s2;  /* when we call get_pointer(), we'll store them back here */
    int *b2;

    b = &a; /* b is a pointer to a */

    /* now let's store them both, even though they're different types */
    append_pointer(s);
    append_pointer(b);

    /* they're stored! let's get them back! */
    s2 = get_pointer(0); /* this was at index 0 */
    b2 = get_pointer(1); /* this was at index 1 */

    return 0;
}
```

See how the pointer types are interchangable through the
`void*`? C will let you convert the `void*` into
any other pointer with impunity, and it really is up to you to make sure
you're getting them back to the type they were originally. Yes, you can
make mistakes here that crash the program, you'd better believe it.
Like they say, "C gives you enough rope to hang yourself."


## `NULL` pointers {#nullp}

I think I have just enough time before the plane lands to talk about
`NULL` when it comes to pointers.

`NULL` simply means a pointer to nothing. Sometimes it's
useful to know if the pointer is valid, or if it needs to be
initialized, or whatever. `NULL` can be used as a sentinel
value for a variety of different things. Remember: it means "this
pointer points to nothing"! Example:

``` {.c}
int main(void)
{
    int *p = NULL;

    if (p == NULL) {
        printf("p is uninitialized!\n");
    } else {
        printf("p points to %d\n", *p);
    }

    return 0;
}
```

Note that pointers aren't preinitialized to `NULL` when
you declare them---you have to explicitly do it. (No non-static
local variables are preinitialized, pointers included.)


## More Static {#static2}

Modern technology has landed me safely here at LAX, and I'm free to
continue writing while I wait for my plane to Ireland. Tell me you're
not jealous at least on some level, and I won't believe you.

But enough about me; let's talk about programming. (How's that for a
geek pick-up line? If you use it, do me a favor and don't credit
me.)

You've already seen how you can use the `static` keyword
to make a local variable persist between calls to its function. But
there are other exciting completely unrelated uses for
`static` that probably deserve to have their own keyword, but
don't get one. You just have to get used to the fact that
`static` (and a number of other things in C) have different
meanings depending on context.

So what about if you declare something as `static` in the
global scope, instead of local to a function? After all, global
variables already persist for the life of the program, so
`static` can't mean the same thing here. Instead, at the
global scope, `static` means that the variable or function
declared `static` _is only visible in this particular
source file_, and cannot be referenced from other source files.
Again, this definition of `static` only pertains to the
global scope. `static` still means the same old thing in the
local scope of the function.

You'll find that your bigger projects little bite-sized pieces
themselves fit into larger bite-sized pieces (just like that picture of
the little fish getting eaten by the larger fish being eaten by the
fish that's larger, still.)  When you have enough related smaller
bite-sized pieces, it often makes sense to put them in their own source
file.

I'm going to rip the example from the section on [`void` pointers](#voidp) wherein we have a couple
functions that can be used to store any types of pointers.

One of the many issues with that example program (there are all kinds
of shortcomings and bugs in it)  is that we've declared a global
variable called `index`. Now, "index" is a pretty common word,
and it's entirely likely that somewhere else in the project, someone
will make up their own variable and name it the same thing as yours.
This could cause all kinds of problems, not the least of which is they
can modify your value of `index`, something that is very
important to you.

One solution is to put all your stuff in one source file, and then
declare `index` to be `static` global. That way, no
one from outside your source file is allowed to use it. You are King!
`static` is a way of keeping the implementation details of
your portion of the code out of the hands of others. Believe me, if you
let other people meddle in your code, they will do so with playful
abandon! Big Hammer Smash!

So here is a quick rewrite of the code to be stuck it its own
file:

``` {.c}
/** file parray.c **/

static void *pointer_array[10]; /* now no one can see it except this file! */
static int index=0; /* same for this one! */

/* but these functions are NOT static, */
/* so they can be used from other files: */

void append_pointer(void *p)
{
    pointer_array[index++] = p;
}

void *get_pointer(int i)
{
    return pointer_array[i];
}

/** end of file parray.c **/
```

What would be proper at this point would be to make a file called
`parray.h` that has the function prototypes for the two
functions in it. Then the file that has `main()` in it can
`#include` `parray.h` and use the
functions when it is all linked together.


## Typical Multifile Projects {#multifile}

Like I'm so fond of saying, projects generally grow too big for a
single file, very similarly to how The Blob grew to be enormous and had
to be defeated by Steve McQueen. Unfortunately, McQueen has already
made his great escape to Heaven, and isn't here to help you with your
code. Sorry.

So when you split projects up, you should try to do it in bite-sized
modules that make sense to have in individual files. For instance, all
the code responsible for calculating Fast Fourier Transforms (a
mathematical construct, for those not in the know), would be a good
candidate for its own source file. Or maybe all the code that controls
a particular AI bot for a game could be in its own file. It's more of a
guideline than a rule, but if something's not at least in some way
related to what you have in a particular source file already, maybe it
should go elsewhere. A perfect illustrative question for this scenario
might be, "What is the 3D rendering code doing in the middle of the
sound driver code?"

When you do move code to its own source file, there is almost always
a header file that you should write to go along with it. The code in
the new source file (which will be a bunch of functions) will need to
have prototypes and other things made visible to the other files so they
can be used. The way the other source files use other files is to
`#include` their header files.

So for example, let's make a small set of functions and stick them in
a file called `simplemath.c`:

``` {.c}
/** file simplemath.c **/

int plusone(int a)
{
    return a+1;
}

int minusone(int a)
{
    return a-1;
}

/** end of file simplemath.c **/
```

A couple simple functions, there. Nothing too complex, yes? But by
themselves, they're not much use, and for other people to use them we
need to distribute their prototypes in a header file. Get ready to
absorb this...you should recognize the prototypes in there, but I've
added some new stuff:

``` {.c}
/** file simplemath.h **/

#ifndef _SIMPLEMATH_H_
#define _SIMPLEMATH_H_

/* here are the prototypes: */

int plusone(int a);
int minusone(int a);

#endif

/** end of file simplemath.h **/
```

Icky. What is all that `#ifndef` stuff? And
the `#define` and the
`#endif`? They are _boilerplate code_
(that is, code that is more often than not stuck in a file) that
prevents the header file from being included multiple times.

The short version of the logic is this: if the symbol
`_SIMPLEMATH_H_` isn't defined then define it, and then do all
the normal header stuff. Else if the symbol `_SIMPLEMATH_H_`
_is_ already defined, do nothing. In this way, the bulk of the
header file is included only once for the build, no matter how many
other files try to include it. This is a good idea, since at best it's
a redundant waste of time to re-include it, and at worst, it can cause
compile-time errors.

Well, we have the header and the source files, and now it's time to
write a file with `main()` in it so that we can actually use
these things:

``` {.c}
/** file main.c **/

#include "simplemath.h"

int main(void)
{
    int a = 10, b;

    b = plusone(a);  /* make that processor work! */

    return 0;
}

/** end of file main.c **/
```

Check it out! We used double-quotes in the
`#include` instead of angle brackets! What this
tells the preprocessor to do is, "include this file from the current
directory instead of the standard system directory."  I'm assuming that
you're putting all these files in the same place for the purposes of
this exercise.

Recall that including a file is exactly like bringing it into your
source at that point, so, as such, we bring the prototypes into the
source right there, and then the functions are free to be used in
`main()`. Huzzah!

One last question! How do we actually build this whole thing? From
the command line:

```shell
$ cc -o main main.c simplemath.c
```

You can lump all the sources on the command line, and it'll build
them together, nice and easy.


## The Almighty C Preprocessor {#cpp}

Remember back about a million years ago when you first started
reading this guide and I mentioned something about spinach? That's
right---you remember how spinach relates to the whole computing
process?

Of course you don't remember. I just made it up just now; I've never
mentioned spinach in this guide. I mean, c'mon. What does spinach have
to do with anything? Sheesh!

Let me steer you to a less leafy topic: the C preprocessor. As the
name suggests, this little program will process source code before the C
compiler sees it. This gives you a little bit more control over what
gets compiled and how it gets compiled.

You've already seen one of the most common preprocessor directives:
`#include`. Other sections of the guide have
touched upon various other ones, but we'll lay them all out here for
fun.


### `#include` {#cppinclude}

The well-known `#include` directive pulls in
source from another file. This other file should be a header file in
virtually every single case.

On each system, there are a number of standard include files that you
can use for various tasks. Most popularly, you've seen
`stdio.h` used. How did the system know where to find it?
Well, each compiler has a set of directories it looks in for header
files when you specify the file name in angle brackets. (On Unix
systems, it commonly searches the `/usr/include`
directory.)

If you want to include a file from the same directory as the source,
use double quotes around the name of the file. Some examples:

``` {.c}
/* include from the system include directory: */

#include <stdio.h>
#include <sys/types.h>

/* include from the local directory: */

#include "mutants.h"
#include "fishies/halibut.h"
```

As you can see from the example, you can also specify a
_relative path_ into subdirectories out of the main directory.
(Under Unix, again, there is a file called `types.h` in the
directory `/usr/include/sys`.)


### `#define` {#cppdefine}

The `#define` is one of the more powerful C
preprocessor directives. With it you can declare constants to be
substituted into the source code before the compiler even sees them.
Let's say you have a lot of constants scattered all over your program
and each number is _hard-coded_ (that is, the number is written
explicitly in the code, like "2").

Now, you thought it was a constant when you wrote it because the
people you got the specification swore to you up and down on pain of
torture that the number would be "2", and it would never change in 100
million years so strike them blind right now.

Hey---sounds good. You even have someone to blame if it did
change, and it probably won't anyway since they seem so sure.

_Don't be a fool._

The spec will change, and it will do so right after you have put the
number "2" in approximately three hundred thousand places throughout
your source, they're going to say, "You know what? Two just isn't going
to cut it---we need three. Is that a hard change to make?"

Blame or not, you're going to be the one that has to change it. A
good programmer will realize that hard-coding numbers like this isn't a
good idea, and one way to get around it is to use a
`#define` directive. Check out this example:

``` {.c}
#define PI 3.14159265358979  /* more pi than you can handle */

int main(void)
{
    float r =10.0;

    printf("pi: %f\n", PI);
    printf("pi/2: %f\n", PI/2);
    printf("area: %f\n", PI*r*r);
    printf("circumference: %f\n", 2*PI*r);

    return 0;
}
```

(Typically `#define`s are all capitals, by
convention.)  So, hey, we just printed that thing out as if it was a
float. Well, it _is_ a float. Remember---the C
preprocessor substitutes the value of `PI` _before the
compiler even sees it_. It is just as if you had typed it there
yourself.

Now let's say you've used `PI` all over the place and now
you're just about ready to ship, and the designers come to you and say,
"So, this whole pi thing, um, we're thinking it needs to be four instead
of three-point-one-whatever. Is that a hard change?"

No problem in this case, no matter how deranged the change request.
All you have to do is change the one `#define` at
the top, and it's therefore automatically changed all over the code
when the C preprocessor runs through it:

``` {.c}
#define PI 4.0  /* whatever you say, boss */
```

Pretty cool, huh. Well, it's perhaps not as good as to be "cool",
but you have not yet witnessed the destructive power of this fully
operational preprocessor directive! You can actually use
`#define` to write little _macros_ that
are like miniature functions that the preprocessor evaluates, again,
before the C compiler sees the code. To make a macro like this, you
give an argument list (without types, because the preprocessor knows
nothing about types, and then you list how that is to be used. For
instance, if we want a macro that evaluates to a number you pass it
times 3490, we could do the following:

``` {.c}
#define TIMES3490(x) ((x)*3490)    /* no semicolon, notice! */

void evaluate_fruit(void)
{
    printf("40 * 3490 = %d\n", TIMES3490(40));
}
```

In that example, the preprocessor will take the macro and
_expand_ it so that it looks like this to the compiler:

``` {.c}
void evaluate_fruit(void)
{
    printf("40 * 3490 = %d\n", ((40)*3490));
}
```

(Actually the preprocessor can do basic math, so it'll probably
reduce it directly to "139600". But this is my example and I'll do as I
please!)

Now here's a question for you: are you taking it on blind faith that
you need all those parenthesis in the macro, like you're some kind of
LISP superhero, or are you wondering, "Why am I wasting precious moments
of my life to enter all these parens?"

Well, you _should_ be wondering! It turns out there are
cases where you can really generate some _evil_ code with a
macro without realizing it. Take a look at this example which builds
without a problem:

``` {.c}
#define TIMES3490(x) x*3490

void walrus_discovery_unit(void)
{
    int tuskcount = 7;

    printf("(tuskcount+2) * 3490 = %d\n", TIMES3490(tuskcount + 2));
}
```

What's wrong here? We're calculating `tuskcount+2`, and then
passing that through the `TIMES3490()` macro. But look at
what it expands to:

``` {.c}
printf("(tuskcount+2) * 3490 = %d\n", tuskcount+2*3490);
```

Instead of calculating `(tuskcount+2)*3490`, we're calculating
`tuskcount+(2*3490)`, because multiplication comes before
addition in the order of operations! See, adding all those extra parens
to the macro prevents this sort of thing from happening. So programmers
with good practices will automatically put a set of parens around each
usage of the parameter variable in the macro, as well as a set of
parens around the outside of the macro itself.


### `#if` and {#macif}
`#ifdef`

There are some _conditionals_ that the C preprocessor can
use to discard blocks of code so that the compiler never sees them. The
`#if` directive operates like the C
`if`, and you can pass it an expression to be
evaluated. It is most common used to block off huge chunks of code like
a comment, when you don't want it to get built:

``` {.c}
void set_hamster_speed(int warpfactor)
{
#if 0
    uh this code isn't written yet. someone should really write it.
#endif
}
```

You can't nest comments in C, but you can nest
`#if` directives all you want, so it can be very
helpful for that.

The other if-statement, `#ifdef` is true if the
subsequent macro is already defined. There's a negative version of this
directive called `#ifndef` ("if not defined").
`#ifndef` is very commonly used with header files
to keep them from being included multiple times:

``` {.c}
/** file aardvark.h **/

#ifndef _AARDVARK_H_
#define _AARDVARK_H_

int get_nose_length(void);
void set_nose_length(int len);

#endif

/** end of file aardvark.h **/
```

The first time this file is included, `_AARDVARK_H_` is not
yet defined, so it goes to the next line, and defines it, and then does
some function prototypes, and you'll see there at the end, the whole
`#if`-type directive is culminated with an
`#endif` statement. Now if the file is included
again (which can happen when you have a lot of header files are
including other header files _ad
infininininini---_cough!), the macro `_AARDVARK_H_`
will already be defined, and so the `#ifndef` will
fail, and the file up to the `#endif` will be
discarded by the preprocessor.

Another extremely useful thing to do here is to have certain code
compile for a certain platform, and have other code compile for a
different platform. Lots of people like to build software with a macro
defined for the type of platform they're on, such as `LINUX` or
`WIN32`. And you can use this to great effect so that your
code will compile and work on different types of systems:

``` {.c}
void run_command_shell(void)
{
#ifdef WIN32
    system("COMMAND.EXE");
#elifdef LINUX
    system("/bin/bash");
#else
#error We don't have no steenkin shells!
#endif
}
```

A couple new things there, most notable
`#elifdef`. This is the contraction of "else
ifdef", which must be used in that case. If you're using
`#if`, then you'd use the corresponding
`#elif`.

Also I threw in an `#error` directive, there.
This will cause the preprocessor to bomb out right at that point with
the given message.


## Pointers to pointers {#pt2pt}

You've already seen how you can have a pointer to a variable...and
you've already seen how a pointer _is_ a variable, so is it
possible to have a _pointer to a pointer_?

No, it's not.

I'm kidding---of course it's possible. Would I have this
section of the guide if it wasn't?

There are a few good reasons why we'd want to have a pointer to a
pointer, and we'll give you the simple one first: you want to pass a
pointer as a parameter to a function, have the function modify it, and
have the results reflected back to the caller.

Note that this is exactly the reason why we use pointers in function
calls in the first place: we want the function to be able to modify the
thing the pointer points to. In this case, though, the thing we want it
to modify is another pointer. For example:

``` {.c}
void get_string(int a, char **s)
{
    switch(a) {
        case 0:
            *s = "everybody";
            break;

        case 1:
            *s = "was";
            break;

        case 2:
            *s = "kung-foo fighting";
            break;

        default:
            *s = "errrrrrnt!";
    }
}

int main(void)
{
    char *s;

    get_string(2, &s);

    printf("s is \"%s\"\n", s);  /* 's is "kung-foo fighting"' */

    return 0;
}
```

What we have, above, is some code that will deliver a string (pointer
to a `char`) back to the caller via pointer to a pointer.
Notice that we pass the _address of_ the pointer `s`
in `main()`. This gives the function
`get_string()` a pointer to `s`, and so it can
dereference that pointer to change what it is pointing at, namely
`s` itself.

There's really nothing mysterious here. You have a pointer to a
thing, so you can dereference the pointer to change the thing. It's
just like before, except for that fact that we're operating on a pointer
now instead of just a plain base type.

What else can we do with pointers to pointers? You can dynamically
make a construction similar to a two-dimensional array with them. The
following example relies on your knowledge that the function call
`malloc()` returns a chunk of sequential bytes of memory that
you can use as you will. In this case, we'll use them to create a
number of `char*`s. And we'll have a pointer to that, as
well, which is therefore of type `char**`.

``` {.c}
int main(void)
{
    char **p;

    p = malloc(sizeof(char*) * 10);  // allocate 10 char*s

    return 0;
}
```

Swell. Now what can we do with those? Well, they don't point to
anything yet, but we can call `malloc()` for each of them in
turn and then we'll have a big block of memory we can store strings
in.

``` {.c}
int main(void)
{
    char **p;
    int i;

    p = malloc(sizeof(char*) * 10);  // allocate 10 char*s-worth of bytes

    for(i = 0; i < 10; i++) {
        *(p+i) = malloc(30); // 30 bytes for each pointer

        // alternatively we could have written, above:
        //   p[i] = malloc(30);
        // but we didn't.

        sprintf(*(p+i), "this is string #%d", i);
    }

    for(i = 0; i < 10; i++) {
        printf("%d: %s\n", i, p[i]); // p[i] same as *(p+i)
    }

    return 0;
}
```

Ok, as you're probably thinking, this is where things get completely
wacko-jacko. Let's look at that second `malloc()` line and
dissect it one piece at a time.

You know that `p` is a pointer to a pointer to a
`char`, or, put another way, it's a pointer to a
`char*`. Keep that in mind.

And we know this `char*` is the first of a solid block of
10, because we just `malloc()`'d that many before the for
loop. With that knowledge, we know that we can use some pointer
arithmetic to hop from one to the next. We do this by adding the
value of `i` onto the `char**` so that when we
dereference it, we are pointing at the next `char*` in the
block. In the first iteration of the loop `i` is zero, so
we're just referring to the first `char*`.

And what do we do with that `char*` once we have it? We
point it at the return value of `malloc()` which will point
it at a fresh ready-to-use 30 bytes of memory.

And what do we use that memory for (sheesh, this could go on
forever!)--well, we use a variant of `printf()` called
`sprintf()` that writes the result into a string instead of
to the console.

And there you have it. Finally, for fun, we print out the results
using array notation to access the strings instead of pointer
arithmetic.


## Pointers to Functions {#ptfunc}

You've completely mastered all that pointer stuff, right? I mean,
you are the _Pointer Master_! No, really, I insist!

So, with that in mind, we're going to take the whole pointer and
address idea to the next phase and learn a little bit about the machine
code that the compiler produces. I know this seems like it has nothing
to do with this section, Pointers to Functions, but it's background that
will only make you stronger. (Provided, that is, it doesn't kill you
first. Admittedly, the chances of death from trying to understand this
section are slim, but you might want to read it in a padded room just as
a precautionary measure.)

Long ago I mentioned that the compiler takes your C source code and
produces _machine code_ that the processor can execute. These
machine code instructions are small (taking between one and four bytes
or memory, typically, with optionally up to, say, 32 bytes of arguments
per instruction---these numbers vary depending on the processor in
question). This isn't so important as the fact that these instructions
have to be stored somewhere. Guess where.

You thought that was a rhetorical command, but no, I really do want
you to guess where, generally, the instructions are stored.

You have your guess? Good. Is it animal, vegetable, or mineral?
Can you fly in it? Is it a rocketship? Yay!

But, cerebral digression aside, yes, you are correct, the
instructions are stored in memory, just like variables are stored in
memory. Instructions themselves have addresses, and a special variable
in the CPU (generally known as a "register" in CPU-lingo) points to the
address of the currently executing instruction.

Whatwhat? I said "points to" and "address-of"! Suddenly we have a
connection back to pointers and all that...familiar ground again. But
what did I just say? I said: instructions are held in addresses, and,
therefore, you have have a pointer to a block of instructions. A block
of instructions in C is held in a function, and, therefore, you can have
a pointer to a function. _Voila_!

Ok, so if you have a function, how do you get the address of the
function? Yes, you can use the `&`, but most people
don't. It's similar to the situation with arrays, where the name of the
array without square brackets is a pointer to the first element in the
array; the name of the function without parens is a pointer to the first
instruction in the function. That's the easy part.

The hard part is declaring a variable to be of type "pointer to
function". It's hard because the syntax is funky:

``` {.c}
// declare p as a pointer to a function that takes two int
// parameters, and returns a float:

float (*p)(int, int);
```

Again, note that this is a _declaration of a pointer_ to a
function. It doesn't yet point to anything in particular. Also notice
that you don't have to put dummy parameter names in the declaration of
the pointer variable. All right, let's make a function, point to it,
and call it:

``` {.c}
int deliver_fruit(char *address, float speed)
{
    printf("Delivering fruit to %s at speed %.2f\n", address, speed);

    return 3490;
}

int main(void)
{
    int (*p)(char*,float);  // declare a function pointer variable

    p = deliver_fruit; // p now points to the deliver_fruit() function

    deliver_fruit("My house", 5280.0); // a normal call

    p("My house", 5280.0); // the same call, but using the pointer

    return 0;
}
```

What the heck good is this? The usual reasons are these:

* You want to change what function is called at runtime.

* You have a big array of data including pointers to functions.

* You don't know the function at compile-time; maybe it's in a shared
library that you load a runtime and query to find a function, and that
query returns a pointer to the function. I know this is a bit beyond
the scope of the section, but bear with me.

For example, long ago a friend of mine and I wrote a program that
would simulate a bunch of creatures running around a grid. The
creatures each had a `struct` associated with them that held
their position, health, and other information. The `struct`
also held a pointer to a function that was their behavior, like
this:

``` {.c}
struct creature {
    int xpos;
    int ypos;
    float health;
    int (*behavior)(struct useful_data*);
};
```

So for each round of the simulation, we'd walk through the list of
creatures and call their behavior function (passing a pointer to a bunch
of useful data so the function could see other creatures, know about
itself, etc.)  In this way, it was easy to code bugs up as having
different behaviors.

Indeed, I wrote a creature called a "brainwasher" that would, when it
got close to another creature, change that creature's behavior pointer to
point to the brainwasher's behavior code! Of course, it didn't take
long before they were all brainwashers, and then starved and
cannibalized themselves to death. Let that be a lesson to you.


## Variable Argument Lists {#varargs}

Ever wonder, in your spare time, while you lay awake at night
thinking about the C Programming Language, how functions like
`printf()` and `scanf()` seem to take an arbitrary
number of arguments and other functions take a specific number? How do
you even write a function prototype for a function that takes a variable
number of arguments?

(Don't get confused over terminology here---we're not talking
about variables. In this case, "variable" retains its usual boring old
meaning of "an arbitrary number of".)

Well, there are some little tricks that have been set up for you in
this case. Remember how all the arguments are pushed onto the stack
when passed to a function? Well, some macros have been set up to help
you walk along the stack and pull arguments off one at a time. In this
way, you don't need to know at compile-time what the argument list will
look like---you just need to know how to parse it.

For instance, let's write a function that averages an arbitrary
number of positive numbers. We can pull numbers off the stack one at a
time and average them all, but we need to know when to stop pulling
stuff off the stack. One way to do this is to have a sentinel value
that you watch for---when you hit it, you stop. Another way is to
put some kind of information in the mandatory first argument. Let's do
option B and put the count of the number of arguments to be averaged in
as the first argument to the function.

Here's the prototype for the function---this is how we declare a
variable argument list. The first argument (at least) must be
specified, but that's all:

``` {.c}
float average(int count, ...);
```

It's the magical "`...`" that does it, see? This lets the
compiler know that there can be more arguments after the first one, but
doesn'r require you to say what they are. So _this_ is how we
are able to pass many or few (but at least one, the first argument)
arguments to the function.

But if we don't have names for the variables specified in the
function header, how do we use them in the function? Well, aren't we
the demanding ones, actually wanting to _use_ our function!
Ok, I'll tell you!

There is a special type declared in the header `stdarg.h`
called `va_list`. It holds data about the stack and about
what arguments have been parsed off so far. But first you have to tell
it where the stack for this function starts, and fortunately we have a
variable right there at the beginning of our `average()`
function: `a`.

We operate on our `va_list` using a number of preprocessor
macros (which are like mini-functions if you haven't yet read the
section on macros.)  First of all, we use `va_start()` to tell
our `va_list` where the stack starts. Then we use
`va_arg()` repeatedly to pull arguments off the stack. And
finally we use `va_end()` to tell our `va_list` that
we're done with it. (The language specification says we _must_
call `va_end()`, and we must call it in the same function from
which we called `va_start()`. This allows the compiler to do
any cleanup that is necessary, and keeps the Vararg Police from knocking
on your door.

So an example! Let's write that `average()` function.
Remember: `va_start()`, `va_arg()`,
`va_arg()`, `va_arg()`, etc., and then
`va_end()`!

``` {.c}
float average(int count, ...)
{
    float ave = 0;
    int i;
    va_list args; // here's our va_list!

    va_start(args, count); // tell it the stack starts with "count"

    // inside the for(), pull int args off the stack:

    for(i = 0; i < count; i++) {
        int val = va_arg(args, int); // get next int argument
        ave += (float)val;  // cast the value to a float and add to total
    }

    va_end(args);  // clean this up

    return ave / count; // calc and return the average
}
```

So there you have it. As you see, the `va_arg()` macro
pulls the next argument off the stack of the given type. So you have to
know in advance what type the thing is. We know for our
`average()` function, all the types are `int`s, so
that's ok. But what if they're different types mixed all together? How
do you tell what type is coming next?

Well, if you'll notice, this is exactly what our old friend
`printf()` does! It knows what type to call
`va_arg()` with, since it says so right in the format
string.


### `vprintf()` and its ilk {#varargsvp}

There are a number of functions that helpfully accept a
`va_list` as an argument that you can pass. This enables you
to wrap these functions up easily in your own functions that take a
variable number of arguments themselves. For instance:

> 
**Assignment:** Implement a version of
`printf()` called `timestamp_printf()` that works
_exactly_ like `printf()` except it prints the time
followed by a newline followed by the data output specified by the
`printf()`-style format string.

Holy cow! At first glance, it looks like you're going to have to
implement a clone of `printf()` just to get a timestamp out
in front of it! And `printf()` is, as we say in the
industry, "nontrivial"! See you next year!

Wait, though---wait, wait...there _must_ be a way to do
it easily, or this author is complete insane to give you this
assignment, and that couldn't be. Fruit! Where is my cheese!?
Blalalauugh!!

Ahem. I'm all right, really, Your Honor. I'm looking into my crystal
ball and I'm seeing...a type `va_list` in your future. In
fact, if we took our variable argument list, processed it with
`va_start()` and got our `va_list` back, we could,
if such a thing existed, just pass it to an already-written version of
`printf()` that accepted just that thing.

Welcome to the world of `vprintf()`! It does exactly
that, by Jove! Here's a lovely prototype:

``` {.c}
int vprintf(const char *format, va_list args);
```

All righty, so what building blocks do we need for this assignment?
The spec says we need to do something just like `printf()`,
so our function, like `printf()` is going to accept a format
string first, followed by a variable number of arguments, something like
this:

``` {.c}
int timestamp_printf(char *format, ...);
```

But before it prints its stuff, it needs to output a timestamp
followed by a newline. The exact format of the timestamp wasn't
specified in the assignment, so I'm going to assume something in the
form of "weekday month day hh:mm:ss year". By amazing coincidence, a
function exists called `ctime()` that returns a string in
exactly that format, given the current system time.

So the plan is to print a timestamp, then take our variable argument
list, run it through `va_start` to get a `va_list`
out of it, and pass that `va_list` into
`vprintf()` and let it work its already-written
`printf()` magic. And...GO!

``` {.c}
#include <stdio.h>
#include <stdarg.h>
#include <time.h>  // for time() and ctime();

int timestamp_printf(char *format, ...)
{
    va_list args;
    time_t system_time;
    char *timestr;
    int return_value;

    system_time = time(NULL); // system time in seconds since epoch
    timestr = ctime(&system_time); // ready-to-print timestamp

    // print the timestamp:
    printf("%s", timestr); // timestr has a newline on the end already

    // get our va_list:
    va_start(args, format);

    // call vprintf() with our arg list:
    return_value = vprintf(format, args);

    // done with list, so we have to call va_end():
    va_end(args);

    // since we want to be *exactly* like printf(), we have saved its
    // return value, and we'll pass it on right here:
    return return_value;
}

int main(void)
{
    // example call:
    timestamp_printf("Brought to you by the number %d\n", 3490);

    return 0;
}
```

And there you have it! Your own little `printf()`-like
functionality!

Now, not every function has a "v" in front of the name for processing
variable argument lists, but most notably all the variants of
`printf()` and `scanf()` do, so feel free to use
them as you see fit!



