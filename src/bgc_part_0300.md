<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

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


