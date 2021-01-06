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
target---it ends with a colon (`:`). Labels have function scope.

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
label 3:
    printf("Two\n");
label 4:
    printf("Three\n");
```

As you've noticed, it's common convention to justify the labels all the
way on the left. This increases readability because a reader can quickly
scan to find the destination.

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
iteration of the nearest enclosing loop.

But what if we wanted it to continue in the outer loop instead? We
could rewrite like this to accomplish it with `goto`.

``` {.c}
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d, %d\n", i, j);
            goto continue_i;   // Now continuing the i loop!
        }
continue_i: ;
    }
```

We have a `;` at the end there---that's because you can't have a label
pointing to the plain end of a compound statement.

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

```
warning: ‘a’ is used uninitialized in this function
```

See
