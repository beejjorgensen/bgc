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
`typedef` it, you can pass them to functions, and
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
}
```

And there you have it! Your own little `printf()`-like
functionality!

Now, not every function has a "v" in front of the name for processing
variable argument lists, but most notably all the variants of
`printf()` and `scanf()` do, so feel free to use
them as you see fit!



# Wide Char Notes

Source character set
    Basic character set  | Extended character set
    Extended characters  |

Execution character set
    Basic character set  | Extended character set
    Extended characters  |

character, char: fits in a byte

multibyte character: sequence of one or more bytes representing a member
of the extended character set (S or E)

    'x'

wide character, wchar_t: value can represent any character in locale.
These are 32-bit that represent the code point for the character.

    L'x'  wchar_t
    u'x'  char16_t
    U'x'  char32_t

char16_t
char32_t

locale

\unnnn
\Unnnnnnnn

String encoding prefixes:

    UTF-8 string literal
        u8"foo" char

    Wide string literals
        L"foo"  wchar_t
        u"foo"  char16_t
        U"foo"  char32_t

mblen
mbtowc
mbstowcs

wctomb
wcctombs

The wide output only works if the stream is wide-oriented fwide()
    Orientation gets set on first write
    Other orientations won't work

## Types

wchar_t

mbstate_t
* Used for mb<->wide conversion
* Shift state

wint_t

* which is an integer type unchanged by default argument promotions that
  can hold any value corresponding to members of the extended character
  set, as well as at least one value that does not correspond to any
  member of the extended character set
    
WEOF

    wide EOF

## Function groups:

* Functions that perform input and output of wide characters, or
  multibyte characters, or both;
* Functions that provide wide string numeric conversion;
* Functions that perform general wide string manipulation;
* Functions for wide string date and time conversion; and
* Functions that provide extended capabilities for conversion between
  multibyte and wide character sequences.

## I/O:

* int fwprintf(FILE * restrict stream, const wchar_t * restrict format, ...);
  * Characters from the array are converted as if by repeated calls to
    the mbrtowc function, with the conversion state described by an
    mbstate_t object initialized to zero before the first multibyte
    character is converted
* int fwscanf(FILE * restrict stream, const wchar_t * restrict format,
  ...);
* int swprintf(wchar_t * restrict s, size_t n, const wchar_t * restrict
  format, ...);
* int swscanf(const wchar_t * restrict s, const wchar_t * restrict
  format, ...);
* int vfwprintf(FILE * restrict stream, const wchar_t * restrict format,
  va_list arg);
* int vfwscanf(FILE * restrict stream, const wchar_t * restrict format,
  va_list arg);
* int vswprintf(wchar_t * restrict s, size_t n, const wchar_t * restrict
  format, va_list arg);
* int vswscanf(const wchar_t * restrict s, const wchar_t * restrict
  format, va_list arg);
* int vwprintf(const wchar_t * restrict format, va_list arg);
* int vwscanf(const wchar_t * restrict format, va_list arg);
* int wprintf(const wchar_t * restrict format, ...);
* int wscanf(const wchar_t * restrict format, ...);
* wint_t fgetwc(FILE *stream);
* wchar_t *fgetws(wchar_t * restrict s, int n, FILE * restrict stream);
* wint_t fputwc(wchar_t c, FILE *stream);
* int fputws(const wchar_t * restrict s, FILE * restrict stream);
* int fwide(FILE *stream, int mode);
* wint_t getwc(FILE *stream);
* wint_t getwchar(void);
* wint_t putwc(wchar_t c, FILE *stream);
* wint_t putwchar(wchar_t c);
* wint_t ungetwc(wint_t c, FILE *stream);

## Conversion Functions

* double wcstod(const wchar_t * restrict nptr, wchar_t ** restrict endptr);
* float wcstof(const wchar_t * restrict nptr, wchar_t ** restrict endptr);
* long double wcstold(const wchar_t * restrict nptr, wchar_t ** restrict endptr);

* long int wcstol(const wchar_t * restrict nptr, wchar_t ** restrict endptr, int base);
* long long int wcstoll(const wchar_t * restrict nptr, wchar_t ** restrict endptr, int base);
* unsigned long int wcstoul(const wchar_t * restrict nptr, wchar_t ** restrict endptr, int base);
* unsigned long long int wcstoull(const wchar_t * restrict nptr, wchar_t ** restrict endptr, int base);

## Copying

* wchar_t *wcscpy(wchar_t * restrict s1, const wchar_t * restrict s2);

* wchar_t *wcsncpy(wchar_t * restrict s1, const wchar_t * restrict s2, size_t n);
* wchar_t *wmemcpy(wchar_t * restrict s1, const wchar_t * restrict s2, size_t n);

* wchar_t *wmemmove(wchar_t *s1, const wchar_t *s2, size_t n);
* wchar_t *wcscat(wchar_t * restrict s1, const wchar_t * restrict s2);
* wchar_t *wcsncat(wchar_t * restrict s1, const wchar_t * restrict s2,
  size_t n);

## Comparing

* int wcscmp(const wchar_t *s1, const wchar_t *s2);
* int wcscoll(const wchar_t *s1, const wchar_t *s2);

* int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
* size_t wcsxfrm(wchar_t * restrict s1, const wchar_t * restrict s2, size_t n);
* int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n);

## Searching

* wchar_t *wcschr(const wchar_t *s, wchar_t c);
* size_t wcscspn(const wchar_t *s1, const wchar_t *s2);
* wchar_t *wcspbrk(const wchar_t *s1, const wchar_t *s2);
* wchar_t *wcsrchr(const wchar_t *s, wchar_t c);
* size_t wcsspn(const wchar_t *s1, const wchar_t *s2);
* wchar_t *wcsstr(const wchar_t *s1, const wchar_t *s2);
* wchar_t *wcstok(wchar_t * restrict s1, const wchar_t * restrict s2, wchar_t ** restrict ptr);
* wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n);

## Length and Misc

* size_t wcslen(const wchar_t *s);
* wchar_t *wmemset(wchar_t *s, wchar_t c, size_t n);
* size_t wcsftime(wchar_t * restrict s, size_t maxsize, const wchar_t * restrict format, const struct tm * restrict timeptr);

## String Conversions

* wint_t btowc(int c);
* int wctob(wint_t c);
* int mbsinit(const mbstate_t *ps);
* size_t mbrlen(const char * restrict s, size_t n, mbstate_t * restrict ps);
* size_t mbrtowc(wchar_t * restrict pwc, const char * restrict s, size_t n, mbstate_t * restrict ps);
* size_t wcrtomb(char * restrict s, wchar_t wc, mbstate_t * restrict ps);
* size_t mbsrtowcs(wchar_t * restrict dst, const char ** restrict src, size_t len, mbstate_t * restrict ps);
* size_t wcsrtombs(char * restrict dst, const wchar_t ** restrict src, size_t len, mbstate_t * restrict ps);

## Character Classification

`<wctype.h>`

* int iswalnum(wint_t wc);
* int iswalpha(wint_t wc);
* int iswblank(wint_t wc);
* int iswcntrl(wint_t wc);
* int iswdigit(wint_t wc);
* int iswgraph(wint_t wc);
* int iswlower(wint_t wc);
* int iswprint(wint_t wc);
* int iswpunct(wint_t wc);
* int iswspace(wint_t wc);
* int iswupper(wint_t wc);
* int iswxdigit(wint_t wc);
* int iswctype(wint_t wc, wctype_t desc);
  * iswctype(wc, wctype("alnum")) // iswalnum(wc)
  * iswctype(wc, wctype("alpha")) // iswalpha(wc)
  * iswctype(wc, wctype("blank")) // iswblank(wc)
  * iswctype(wc, wctype("cntrl")) // iswcntrl(wc)
  * iswctype(wc, wctype("digit")) // iswdigit(wc)
  * iswctype(wc, wctype("graph")) // iswgraph(wc)
  * iswctype(wc, wctype("lower")) // iswlower(wc)
  * iswctype(wc, wctype("print")) // iswprint(wc)
  * iswctype(wc, wctype("punct")) // iswpunct(wc)
  * iswctype(wc, wctype("space")) // iswspace(wc)
  * iswctype(wc, wctype("upper")) // iswupper(wc)
  * iswctype(wc, wctype("xdigit")) // iswxdigit(wc)
* wctype_t wctype(const char *property);
* wint_t towlower(wint_t wc);
* wint_t towupper(wint_t wc);
* wint_t towctrans(wint_t wc, wctrans_t desc);
  * towctrans(wc, wctrans("tolower")) // towlower(wc)
  * towctrans(wc, wctrans("toupper")) // towupper(wc)
* wctrans_t wctrans(const char *property);

## Unicode Utils

`<uchar.h>`

* size_t mbrtoc16(char16_t * restrict pc16, const char * restrict s, size_t n, mbstate_t * restrict ps);
* size_t c16rtomb(char * restrict s, char16_t c16, mbstate_t * restrict ps);
* size_t mbrtoc32(char32_t * restrict pc32, const char * restrict s, size_t n, mbstate_t * restrict ps);
* size_t c32rtomb(char * restrict s, char32_t c32, mbstate_t * restrict ps);




## Unnamed Objects, Compound Literals

This is an interesting feature of C11 that allows you to make an object
without a name. A couple places this might be useful is if you need to
pass a one-off `struct` to a function, but want it to just _exist_ as an
argument, but not as a regular variable elsewhere.

First, let's look at some contrived and useless syntax.

``` {.c}
int a = (int){2};
```

Right of the assignment, we have an unnamed `int` that's initializd to
`2`. The we immediately assign it into a variable `a`.

``` {.c}
int a = (int){2};
int a = 2;         // Same thing
```

If those are the same, why even bother this this syntax? It's harder to
read and more verbose.

Well, let's try it with a `struct` and see what happens.


Incomplete types

6.2.5p22

An array type of unknown size is an incomplete type. It is completed,
for an identifier of that type, by specifying the size in a later
declaration (with internal or external linkage).

A structure or union type of unknown content (as described in 6.7.2.3)
is an incomplete type. It is completed, for all declarations of that
type, by declaring the same structure or union tag with its defining
content later in the same scope.

6.3.2.1p1

A modifiable lvalue is an lvalue that does not have array type, does not
have an incomplete type, does not have a constqualified type, and if it
is a structure or union, does not have any member (including,
recursively, any member or element of all contained aggregates or
unions) with a constqualified type.

6.3.2.1p2

If the lvalue has an incomplete type and does not have array type, the
behavior is undefined.

6.5.3.4p1

The sizeof operator shall not be applied to an expression that has
function type or an incomplete type, to the parenthesized name of such a
type, or to an expression that designates a bit-field member. The
_Alignof operator shall not be applied to a function type or an
incomplete type.

fn129

An incomplete type may only by used when the size of an object of that
type is not needed. It is not needed, for example, when a typedef name
is declared to be a specifier for a structure or union, or when a
pointer to or a function returning a structure or union is being
declared. (See incomplete types in 6.2.5.) The specification has to be
complete before such a function is called or defined.

6.7.6.2p4

If the size is not present, the array type is an incomplete type.

6.7.6.2p8

EXAMPLE 2 Note the distinction between the declarations 

    extern int *x;
    extern int y[];

The first declares x to be a pointer to int; the second declares y to be
an array of int of unspecified size (an incomplete type), the storage
for which is defined elsewhere.

6.7.9p31

EXAMPLE 7 One form of initialization that completes array types involves
typedef names. Given the declaration

    typedef int A[]; // OK - declared with block scope

the declaration

    A a = { 1, 2 }, b = { 3, 4, 5 };

is identical to

    int a[] = { 1, 2 }, b[] = { 3, 4, 5 };

due to the rules for incomplete types.

6.9.2p3

If the declaration of an identifier for an object is a tentative
definition and has internal linkage, the declared type shall not be an
incomplete type.

6.9.2p5

EXAMPLE 2 If at the end of the translation unit containing

    int i[];

the array i still has incomplete type, the implicit initializer causes
it to have one element, which is set to zero on program startup.

6.2.5p1

At various points within a translation unit an object type may be
incomplete (lacking sufficient information to determine the size of
objects of that type) or complete (having sufficient information).

6.2.5p19

The void type comprises an empty set of values; it is an incomplete
object type that cannot be completed.

6.7.2.1p3

A structure or union shall not contain a member with incomplete or
function type (hence, a structure shall not contain an instance of
itself, but may contain a pointer to an instance of itself), except that
the last member of a structure with more than one named member may have
incomplete array type; such a structure (and any union containing,
possibly recursively, a member that is such a structure) shall not be a
member of a structure or an element of an array.

6.7.2.1p8

The [struct/union] type is incomplete until immediately after the } that
terminates the list, and complete thereafter.

6.7.2.1p18

As a special case, the last element of a structure with more than one
named member may have an incomplete array type; this is called a
flexible array member. In most situations, the flexible array member is
ignored. In particular, the size of the structure is as if the flexible
array member were omitted except that it may have more trailing padding
than the omission would imply. Howev er, when a . (or ->) operator has a
left operand that is (a pointer to) a structure with a flexible array
member and the right operand names that member, it behaves as if that
member were replaced with the longest array (with the same element type)
that would not make the structure larger than the object being accessed;
the offset of the array shall remain that of the flexible array member,
even if this would differ from that of the replacement array. If this
array would have no elements, it behaves as if it had one element but
the behavior is undefined if any attempt is made to access that element
or to generate a pointer one past it.

6.7.2.3p4

Each enumerated type shall be compatible with char, a signed integer
type, or an unsigned integer type. The choice of type is
implementation-defined,128) but shall be capable of representing the
values of all the members of the enumeration. The enumerated type is
incomplete until immediately after the } that terminates the list of
enumerator declarations, and complete thereafter.

6.7.2.3p8

If a type specifier of the form struct-or-union identifier occurs other
than as part of one of the above forms, and no other declaration of the
identifier as a tag is visible, then it declares an incomplete structure
or union type, and declares the identifier as the tag of that type.

## Complex

FN24

The evaluation method determines evaluation formats of expressions
involving all floating types, not just real types. For example, if
FLT_EVAL_METHOD is 1, then the product of two float _Complex operands is
represented in the double _Complex format, and its parts are evaluated
to double.

6.2.5p11-13

There are three complex types, designated as float _Complex, double
_Complex, and long double _Complex.43) (Complex types are a conditional
feature that implementations need not support; see 6.10.8.3.) The real
floating and complex types are collectively called the floating types.

For each floating type there is a corresponding real type, which is
always a real floating type. For real floating types, it is the same
type. For complex types, it is the type given by deleting the keyword
_Complex from the type name.

Each complex type has the same representation and alignment requirements
as an array type containing exactly two elements of the corresponding
real type; the first element is equal to the real part, and the second
element to the imaginary part, of the complex number.


STDC_NO_COMPLEX
_ _STDC_IEC_559_COMPLEX_ _

complex == _Complex


float complex
double complex
long double complex

_Complex_I

_Imaginary
_Imaginary_I

I expands to _Imaginary_I if there, else _Complex_I

https://en.cppreference.com/w/c/numeric/complex/imaginary

pragma STDC CX_LIMITED_RANGE

All the math function

CMPLX



