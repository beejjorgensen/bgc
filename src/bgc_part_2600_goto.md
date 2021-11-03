<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# `goto`

The `goto` statement is universally revered and can be here presented
without contest.

Just kidding! Over the years, there has been a lot of back-and-forth
over whether or not (often not) `goto` is [flw[considered
harmful|Goto#Criticism]].

In this programmer's opinion, you should use whichever constructs leads
to the _best_ code, factoring in maintainability and speed. And
sometimes this might be `goto`!

In this chapter, we'll see how `goto` works in C, and then check out
some of the common cases where it is used^[I'd like to point out that
using `goto` in all these cases is avoidable. You can use variables and
loops intead. It's just that some people think `goto` produces the
_best_ code in those circumstances.].

## A Simple Example

In this example, we're going to use `goto` to skip a line of code and
jump to a _label_. The label is the identifier that can be a `goto`
target---it ends with a colon (`:`).

``` {.c .numberLines}
#include <stdio.h>

int main(void)
{
    printf("One\n");
    printf("Two\n");

    goto skip_3;

    printf("Three\n");

skip_3:

    printf("Five!\n");
}
```

The output is:

``` {.default}
One
Two
Five!
```

`goto` sends execution jumping to the specified label, skipping
everything in between.

You can jump forward or backward with `goto`.

``` {.c}
infinite_loop:
    print("Hello, world!\n");
    goto infinite_loop;
```

Labels are skipped over during execution. The following will print all
three numbers in order just as if the labels weren't there:

``` {.c}
    printf("Zero\n");
label_1:
label_2:
    printf("One\n");
label_3:
    printf("Two\n");
label_4:
    printf("Three\n");
```

As you've noticed, it's common convention to justify the labels all the
way on the left. This increases readability because a reader can quickly
scan to find the destination.

Labels have _function scope_. That is, no matter how many levels deep in
blocks they appear, you can still `goto` them from anywhere in the
function.

It also means you can only `goto` labels that are in the same function
as the `goto` itself. Labels in other functions are out of scope from
`goto`'s perspective. And it means you can use the same label name in
two functions---just not the same label name in the same function.

## Labeled `continue`

In some languages, you can actually specify a label for a `continue`
statement. C doesn't allow it, but you can easily use `goto` instead.

To show the issue, check out `continue` in this nested loop:

``` {.c}
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        printf("%d, %d\n", i, j);
        continue;   // Always goes to next j
    }
}
```

As we see, that `continue`, like all `continues`, goes to the next
iteration of the nearest enclosing loop. What if we want to `continue`
in the next loop out, the loop with `i`?

Well, we can `break` to get back to the outer loop, right?

``` {.c}
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        printf("%d, %d\n", i, j);
        break;     // Gets us to the next iteration of i
    }
}
```

That gets us two levels of nested loop. But then if we nest another
loop, we're out of options. What about this, where we don't have any
statement that will get us out to the next iteration of `i`?

``` {.c}
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
            printf("%d, %d, %d\n", i, j, k);

            continue;  // Gets us to the next iteration of k
            break;     // Gets us to the next iteration of j
            ????;      // Gets us to the next iteration of i???

        }
    }
}
```

The `goto` statement offers us a way!

``` {.c}
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                printf("%d, %d, %d\n", i, j, k);

                goto continue_i;   // Now continuing the i loop!!
            }
        }
continue_i: ;
    }
```

We have a `;` at the end there---that's because you can't have a label
pointing to the plain end of a compound statement (or before a variable
declaration).

## Bailing Out

When you're super nested in the middle of some code, you can use `goto`
to get out of it in a manner that's often cleaner than nesting more
`if`s and using flag variables.

``` {.c}
    // Pseudocode

    for(...) {
        for (...) {
            while (...) {
                do {
                    if (some_error_condition)
                        goto bail;

                } while(...);
            }
        }
    }

bail:
    // Cleanup here
```

Without `goto`, you'd have to check an error condition flag in all of
the loops to get all the way out.

## Labeled `break`

This is a very similar situation to how `continue` only continues the
innermost loop. `break` also only breaks out of the innermost loop.

``` {.c}
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d, %d\n", i, j);
            break;   // Only breaks out of the j loop
        }
    }

    printf("Done!\n");
```

But we can use `goto` to break farther:

``` {.c}
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d, %d\n", i, j);
            goto break_i;   // Now breaking out of the i loop!
        }
    }

break_i:

    printf("Done!\n");
```

## Multi-level Cleanup

If you're calling multiple functions to initialize multiple systems and
one of them fails, you should only de-initialize the ones that you've
gotten to so far.

Let's do a fake example where we start initializing systems and checking
to see if any returns an error (we'll use `-1` to indicate an error). If
one of them does, we have to shutdown only the systems we've initialized
so far.

``` {.c}
    if (init_system_1() == -1)
        goto shutdown;

    if (init_system_2() == -1)
        goto shutdown_1;

    if (init_system_3() == -1)
        goto shutdown_2;

    if (init_system_4() == -1)
        goto shutdown_3;

    do_main_thing();   // Run our program

    shutdown_system4();

shutdown_3:
    shutdown_system3();

shutdown_2:
    shutdown_system2();

shutdown_1:
    shutdown_system1();

shutdown:
    print("All subsystems shut down.\n");
```

Note that we're shutting down in the reverse order that we initialized
the subsystems. So if subsystem 4 fails to start up, it will shut down
3, 2, then 1 in that order.

## Tail Call Optimization

Kinda. For recursive functions only.

If you're unfamiliar, [flw[Tail Call Optimization (TCO)|Tail_call]] is a
way to not waste stack space when calling other functions under very
specific circumstances. Unfortunately the details are beyond the scope
of this guide.

But if you have a recursive function you know can be optimized in this
way, you can make use of this technique. (Note that you can't tail call
other functions due to the function scope of labels.)

Let's do a straightforward example, factorial.

Here's a recursive version that's not TCO, but it can be!

``` {.c .numberLines}
#include <stdio.h>
#include <complex.h>

int factorial(int n, int a)
{
    if (n == 0)
        return a;

    return factorial(n - 1, a * n);
}

int main(void)
{
    for (int i = 0; i < 8; i++)
        printf("%d! == %ld\n", i, factorial(i, 1));
}
```

To make it happen, you can replace the call with two steps:

1. Set the values of the parameters to what they'd be on the next call.
2. `goto` a label on the first line of the function.

Let's try it:

``` {.c .numberLines}
#include <stdio.h>

int factorial(int n, int a)
{
tco:  // add this

    if (n == 0)
        return a;

    // replace return by setting new parameter values and
    // goto-ing the beginning of the function

    //return factorial(n - 1, a * n);

    int next_n = n - 1;  // See how these match up with
    int next_a = a * n;  // the recursive arguments, above?

    n = next_n;   // Set the parameters to the new values
    a = next_a;

    goto tco;   // And repeat!
}

int main(void)
{
    for (int i = 0; i < 8; i++)
        printf("%d! == %d\n", i, factorial(i, 1));
}
```

I used temporary variables up there to set the next values of the
parameters before jumping to the start of the function. See how they
correspond to the recursive arguments that were in the recursive call?

Now, why use temp variables? I could have done this instead:

``` {.c}
    a *= n;
    n -= 1;

    goto tco;
```

and that actually works just fine. But if I carelessly reverse those two
lines of code:

``` {.c}
    n -= 1;  // BAD NEWS
    a *= n;
```

---now we're in trouble. We modified `n` before using it to modify `a`.
That's Bad because that's not how it works when you call recursively.
Using the temporary variables avoids this problem even if you're not
looking out for it. And the compiler likely optimizes them out, anyway.


## Restarting Interrupted System Calls

This is outside the spec, but commonly seen in Unix-like systems.

Certain long-lived system calls might return an error if they're
interrupted by a signal, and `errno` will be set to `EINTR` to indicate
the syscall was doing fine; it was just interrupted.

In those cases, it's really common for the programmer to want to restart
the call and try it again.

``` {.c}
retry:
    byte_count = read(0, buf, sizeof(buf) - 1);  // Unix read() syscall

    if (byte_count == -1) {            // An error occurred...
        if (errno == EINTR) {          // But it was just interrupted
            printf("Restarting...\n");
            goto retry;
        }
```

Many Unix-likes have an `SA_RESTART` flag you can pass to `sigaction()`
to request the OS automatically restart any slow syscalls instead of
failing with `EINTR`.

Again, this is Unix-specific and is outside the C standard.

That said, it's possible to use a similar technique any time any
function should be restarted.

## `goto` and Variable Scope 

We've already seen that labels have function scope, but weird things can
happen if we jump past some variable initialization.

Look at this example where we jump from a place where the variable `x`
is out of scope into the middle of its scope (in the block).

``` {.c}
    goto label;

    {
        int x = 12345;

label:
        printf("%d\n", x);
    }
```

This will compile and run, but gives me a warning:

``` {.default}
warning: ‘x’ is used uninitialized in this function
```

And then it prints out `0` when I run it (your mileage may vary).

Basically what has happened is that we jumped into `x`'s scope (so it
was OK to reference it in the `printf()`) but we jumped over the line
that actually initialized it to `12345`. So the value was indeterminate.

The fix is, of course, to get the initialization _after_ the label one
way or another.

``` {.c}
    goto label;

    {
        int x;

label:
        x = 12345;
        printf("%d\n", x);
    }
```

Let's look at one more example.

``` {.c}
    {
        int x = 10;

label:

        printf("%d\n", x);
    }

    goto label;
```

What happens here?

The first time through the block, we're good. `x` is `10` and that's
what prints.

But after the `goto`, we're jumping into the scope of `x`, but past its
initialization. Which means we can still print it, but the value is
indeterminate (since it hasn't been reinitialized).

On my machine, it prints `10` again (to infinity), but that's just luck.
It could print any value after the `goto` since `x` is uninitialized.

## `goto` and Variable-Length Arrays

When it comes to VLAs and `goto`, there's one rule: you can't jump from
outside the scope of a VLA into the scope of that VLA.

If I try to do this:

``` {.c}
    int x = 10;

    goto label;

    {
        int v[x];

label:

        printf("Hi!\n");
    }
```

I get an error:

``` {.default}
error: jump into scope of identifier with variably modified type
```

You can jump in ahead of the VLA declaration, like this:

``` {.c}
    int x = 10;

    goto label;

    {
label:  ;
        int v[x];

        printf("Hi!\n");
    }
```

Because that way the VLA gets allocated properly before its inevitable
deallocation once it falls out of scope.
