<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Foreward

No point in wasting words here, folks, let's jump straight into the C
code:

``` {.c}
E((ck?main((z?(stat(M,&t)?P+=a+'{'?0:3:
execv(M,k),a=G,i=P,y=G&255,
sprintf(Q,y/'@'-3?A(*L(V(%d+%d)+%d,0)
```

And they lived happily ever after. The End.

What's this? You say something's still not clear about this whole C
programming language thing?

Well, to be quite honest, I'm not even sure what the above code does.
It's a snippet from one of the entires in the 2001 [fl[International
Obfuscated C Code Contest|http://www.ioccc.org/]], a wonderful
competition wherein the entrants attempt to write the most unreadable C
code possible, with often surprising results.

The bad news is that if you're a beginner in this whole thing, all C
code you see probably looks obfuscated! The good news is, it's not
going to be that way for long.

What we'll try to do over the course of this guide is lead you from
complete and utter sheer lost confusion on to the sort of enlightened
bliss that can only be obtained though pure C programming. Right on.

## Audience

This guide assumes that you've already got some programming knowledge
under your belt from another language, such as
[flw[Python|Python_(programming_language)]],
[flw[JavaScript|JavaScript]], [flw[Java|Java_(programming_language)]],
[flw[Rust|Rust_(programming_language)]],
[flw[Go|Go_(programming_language)]],
[flw[Swift|Swift_(programming_language)]], etc.
([flw[Objective-C|Objective-C]] devs will have a particularly easy time
of it!)

We're going to assume you know what variables are, what loops do, how
functions work, and so on.

If that's not you for whatever reason the best I can hope to provide is
some pastey entertainment for your reading pleasure. The only thing I
can reasonably promise is that this guide won't end on a
cliffhanger...or _will_ it?

## Platform and Compiler

I'll try to stick to Plain Ol'-Fashioned [flw[ISO-standard C|ANSI_C]].
Well, for the most part. Here and there I might go crazy and start
talking about [flw[POSIX|POSIX]] or something, but we'll see.

**Unix** users (e.g. Linux, BSD, etc.) try running `cc` or `gcc` from
the command line--you might already have a compiler installed. If you
don't, search your distribution for installing `gcc` or `clang`.

**Windows** users should check out [fl[Visual Studio
Community|https://visualstudio.microsoft.com/vs/community/]]. Or, if
you're looking for a more Unix-like experience (recommended!), install
[fl[WSL|https://docs.microsoft.com/en-us/windows/wsl/install-win10]] and
`gcc`.

**Mac** users will want to install
[fl[XCode|https://developer.apple.com/xcode/]], and in particular the
command line tools.

There are a lot of compilers out there, and virtually all of them will
work for this book. And for those not in the know, a C++ compiler will
compile C most code, so it'll work for the purposes of this guide. Some
of the compilers I am familiar with are the following:

## Official Homepage

This official location of this document is
[fl[http://beej.us/guide/bgc/|http://beej.us/guide/bgc/]]. Maybe
this'll change in the future, but it's more likely that all the other
guides are migrated off Chico State computers.

## Email Policy

I'm generally available to help out with email questions so feel free to
write in, but I can't guarantee a response. I lead a pretty busy life
and there are times when I just can't answer a question you have. When
that's the case, I usually just delete the message. It's nothing
personal; I just won't ever have the time to give the detailed answer
you require.

As a rule, the more complex the question, the less likely I am to
respond. If you can narrow down your question before mailing it and be
sure to include any pertinent information (like platform, compiler,
error messages you're getting, and anything else you think might help me
troubleshoot), you're much more likely to get a response.

If you don't get a response, hack on it some more, try to find the
answer, and if it's still elusive, then write me again with the
information you've found and hopefully it will be enough for me to help
out.

Now that I've badgered you about how to write and not write me, I'd just
like to let you know that I _fully_ appreciate all the praise the guide
has received over the years. It's a real morale boost, and it gladdens
me to hear that it is being used for good! `:-)` Thank you!

## Mirroring

You are more than welcome to mirror this site, whether publicly or
privately. If you publicly mirror the site and want me to link to it
from the main page, drop me a line at [`beej@beej.us`](beej@beej.us).

## Note for Translators

If you want to translate the guide into another language, write me at
[`beej@beej.us`](beej@beej.us) and I'll link to your translation from
the main page. Feel free to add your name and contact info to the
translation.

Please note the license restrictions in the Copyright and Distribution
section, below.

## Copyright and Distribution

Beej's Guide to Network Programming is Copyright © 2020 Brian "Beej
Jorgensen" Hall.

With specific exceptions for source code and translations, below, this
work is licensed under the Creative Commons Attribution-Noncommercial-No
Derivative Works 3.0 License. To view a copy of this license, visit
[`http://creativecommons.org/licenses/by-nc-nd/3.0/`](http://creativecommons.org/licenses/by-nc-nd/3.0/)
or send a letter to Creative Commons, 171 Second Street, Suite 300, San
Francisco, California, 94105, USA.

One specific exception to the "No Derivative Works" portion of the
license is as follows: this guide may be freely translated into any
language, provided the translation is accurate, and the guide is
reprinted in its entirety. The same license restrictions apply to the
translation as to the original guide. The translation may also include
the name and contact information for the translator.

The C source code presented in this document is hereby granted to the
public domain, and is completely free of any license restriction.

Educators are freely encouraged to recommend or supply copies of this
guide to their students.

Contact [`beej@beej.us`](beej@beej.us) for more information.

## What to Expect from C

> _"Where do these stairs go?"_<br>
> _"They go up."_
>
> ---Ray Stantz and Peter Venkman, Ghostbusters

C is a low-level language.

It didn't used to be. Back in the day when people carved punch cards out
of granite, C was an incredibly way to be free of the drudgery of
lower-level languages like [flw[assembly|Assembly_language]].

But now in these modern times, current-generation languages offer all
kinds of features that didn't exist in 1972 when C was invented. This
means C is a pretty basic language with not a lot of features. It can do
_anything_, but it can make you work for it.

So why would we even use it today?

* As a learning tool: not only is C a venerable piece of computing
  history, but it is connected to the [flw[bare metal|Bare_machine]] in
  a way that present-day languages are not. When you learn C, you learn
  about how software interfaces with computer memory at a low level.
  There are no seatbelts. You'll write software that crashes, I assure
  you. And that's all part of the fun!

* As a useful tool: C still is used for certain applications, such as
  building [flw[operating systems|Operating_system]] or in [flw[embedded
  systems|Embedded_system]]. (Though the
  [flw[Rust|Rust_(programming_language)]] programming language is eyeing
  both these fields!)

If you're familiar with another language, a lot of things about C are
easy. C inspired many other languages, and you'll see bits of it in Go,
Rust, Swift, Python, JavaScript, Java, and all kinds of other languages.
Those parts will be familiar.

The one thing about C that hangs people up is _pointers_. Virtually
everything else is familiar, but pointers are the weird one. The concept
behind pointers is likely one you already know, but C forces you to be
explicit about it, using operators you've likely never seen before.

It's especially insidious because once you [flw[_grok_|Grok]] pointers,
they're suddenly easy. But up until that moment, they're slippery eels.

Everything else in C is just memorizing another way (or sometimes the
same way!) of doing something you've done already. Pointers are the
weird bit.

So get ready for a rollicking adventure as close to the core of the
computer as you can get without assembly, in the most influential
computer language of all time^[I know someone will fight me on that,
but it's gotta be at least in the top three, right?]. Hang on!


## Hello, World!

This is the canonical example of a C program. Everyone uses it:

``` {.c}
/* Hello world program */

#include <stdio.h>

int main(void)
{
    printf("Hello, World!\n");  // Actually do the work here

    return 0;
}
```

We're going to don our long-sleeved heavy-duty rubber gloves, grab a
scalpel, and rip into this thing to see what makes it tick. So, scrub
up, because here we go. Cutting very gently...

Let's get the easy thing out of the way: anything between the digraphs
`/*` and `*/` is a comment and will be completely ignored by the
compiler. Same goes for anything on a line after a `//`. This allows you
to leave messages to yourself and others, so that when you come back and
read your code in the distant future, you'll know what the heck it was
you were trying to do. Believe me, you will forget; it happens.

Now, what is this `#include`? GROSS! Well, it tells the C Preprocessor
to pull the contents of another file and insert it into the code right
_there_.

Wait---what's a C Preprocessor? Good question. There are two stages
(well, technically there are more than two, but hey, let's pretend there
are two and have a good laugh) to compilation: the preprocessor and the
compiler. Anything that starts with pound sign, or "octothorpe", (`#`)
is something the preprocessor operates on before the compiler even gets
started. Common _preprocessor directives_, as they're called, are
`#include` and `#define`. More on that later.

Before we go on, why would I even begin to bother pointing out that a
pound sign is called an octothorpe? The answer is simple: I think the
word octothorpe is so excellently funny, I have to gratuitously spread
its name around whenever I get the opportunity. Octothorpe. Octothorpe,
octothorpe, octothorpe.

So _anyway_. After the C preprocessor has finished preprocessing
everything, the results are ready for the compiler to take them and
produce [flw[assembly code|Assembly_language]], [flw[machine
code|Machine_code]], or whatever it's about to do. Don't worry about the
technical details of compilation for now; just know that your source
runs through the preprocessor, then the output of that runs through the
compiler, then that produces an executable for you to run. Octothorpe.

What about the rest of the line? What's `<stdio.h>`? That is what is
known as a _header file_. It's the dot-h at the end that gives it away.
In fact it's the "Standard I/O" (stdio) header file that you will grow
to know and love. It contains preprocessor directives and function
prototypes (more on that later) for common input and output needs. For
our demo program, we're outputting the string "Hello, World!", so we in
particular need the function prototype for the `printf()` function from
this header file. Basically, if we tried to use `printf()` without
`#include <stdio.h>`, the compiler would have complained to us about it.

How did I know I needed to `#include <stdio.h>` for `printf()`? Answer:
it's in the documentation. If you're on a Unix system, `man printf` and
it'll tell you right at the top of the man page what header files are
required. Or see the reference section in this book. `:-)`

Holy moly. That was all to cover the first line! But, let's face it, it
has been completely dissected. No mystery shall remain!

So take a breather...look back over the sample code. Only a couple easy
lines to go.

Welcome back from your break! I know you didn't really take a break; I
was just humoring you.

The next line is `main()`. This is the definition of the function
`main()`; everything between the squirrelly braces (`{` and `}`) is part
of the function definition.

How do you call a different function, anyway? The answer lies in the
`printf()` line, but we'll get to that in a minute.

Now, the main function is a special one in many ways, but one way stands
above the rest: it is the function that will be called automatically
when your program starts executing. Nothing of yours gets called before
`main()`. In the case of our example, this works fine since all we want
to do is print a line and exit.

Oh, that's another thing: once the program executes past the end of
`main()`, down there at the closing squirrelly brace, the program will
exit, and you'll be back at your command prompt.

So now we know that that program has brought in a header file,
`stdio.h`, and declared a `main()` function that will execute when the
program is started. What are the goodies in `main()`?

I am so happy you asked. Really! We only have the one goodie: a call to
the function `printf()`. You can tell this is a function call and not a
function definition in a number of ways, but one indicator is the lack
of squirrelly braces after it. And you end the function call with a
semicolon so the compiler knows it's the end of the expression. You'll
be putting semicolons after most everything, as you'll see.

You're passing one parameter to the function `printf()`: a string to be
printed when you call it. Oh, yeah---we're calling a function! We rock!
Wait, wait---don't get cocky. What's that crazy `\n` at the end of the
string? Well, most characters in the string look just like they are
stored. But there are certain characters that you can't print on screen
well that are embedded as two-character backslash codes. One of the most
popular is `\n` (read "backslash-N") that corresponds to the _newline_
character. This is the character that causing further printing to
continue on the next line instead of the current. It's like hitting
return at the end of the line.

So copy that code into a file, build it, and run it---see what
happens:

```
Hello, World! 
```

It's done and tested! Ship it!

