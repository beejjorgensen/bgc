<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Atomics

> _"They tried and failed, all of them?"_ \
> _"Oh, no." She shook her head. "They tried and died."_
>
> ---Paul Atreides and The Reverend Mother Gaius Helen Mohiam, _Dune_

This is one of the more challenging aspects of multithreading with C.
But we'll try to take it easy.

Basically, I'll talk about the more straightforward uses of atomic
variables, what they are, and how they work, etc.  And I'll mention some
of the more insanely-complex paths that are available to you.

But I won't go down those paths. Not only am I barely qualified to even
write about them, but I figure if you know you need them, you already
know more than I do.

Another thing this chapter is not: a discussion of lock-free data
structures. For the same reason.

But there are some weird things out here even in the basics. So hang on,
everyone, 'cause Kansas is goin' bye-bye.

## Atomic Variables

Here's _part_ of how atomic variables work. If you have a shared atomic
variable and you write to it from one thread, that write will be _all or
nothing_ in a different thread.

That is, the other thread will see the entire write of, say, a 32-bit
value. Not half of it. There's no way for one thread to interrupt
another that is in the _middle_ of an atomic multi-byte write.

It's almost like there's a little lock around the getting and setting of
that one variable. (And there _might_ be! See [Lock-Free Atomic
Variables](#lock-free-atomic), below.)

That's only part of the story.

Before we go further, how do you declare a variable to be atomic?

First, include `<stdatomic.h>`.

This gives us types such as `atomic_int`. (See the [`<stdatomic.h>` types
reference page](#stdatomic-types) for the complete list.)

And then we can simply declare variables to be of that type.

But let's do a demo where we have two threads. The first runs for a
while and then sets a variable to a specific value, then exits. The
other runs until it sees that value get set, and then it exits.

<!-- demo here -->

The second thread spins in place, looking at the flag and waiting for it
to get set to the value `3490`.

## Sequential Consistency

## Atomic operators

## Library Functions that automatically synchronize

* thrd_create()
* etc

## Lock-Free Atomic Variables {#lock-free-atomic}

## Signal Handlers

## Fences

## Memory Access Order

* seq cst
* acq rel
* acq consume
  * dependencies, kill_dependency()
* relaxed

