<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Exercises

These are the exercises for the various chapters.

**Warning:** if you look at the solution before you try to solve it
yourself, you're only gaining 10%^[In my non-scientific estimation. But
for realsies, do the work before looking at the answer. It's the best
way to learn.] of the potential learning. Work on it for a while before
giving up and looking at the answer.

## Intro {#ex-intro}

1. Modify the "Hello, world!" example to print your name instead.
   [fls[intro/hello_name.c]]

1. Modify the "Hello, world!" example to print your name one one line,
   and your favorite fruit on the next. [fls[intro/hello_fruit.c]]

## Variables and Statements {#ex-var-stat}

1. An approximation of $\pi$ is $208341 / 66317$. Write a program that
   computes this, stores the result in a `float` variable, and prints it
   out. Hint: put a `.0` after at least one of the numbers to force the
   computation to be `float` and not integer. [fls[varstat/pi_approx.c]]

1. Choose a random integer in your head between 50 and 97. Call it $x$.
   Compute $(x+10\times x-5)/2$ as an integer and print it.
   [fls[varstat/formula1.c]]

1. Do the same thing as the previous problem, except divide it by `2.0`
   instead of `2` and store the result in a `float`. And print it.
   [fls[varstat/formula2.c]]

1. Let's say you have a variable `i = 2`. Write a program that assigns
   `j = i + 7` (to get `9`) _and_ then increments `i` on the same line
   of code. [fls[varstat/postinc.c]]

1. Let's say you have a variable `i = 2`. Write a program that
   decrements `i` _and_ then assigns `j = i + 7` (to get `8`) on the
   same line of code. [fls[varstat/predec.c]]

1. Choose a value for an integer variable `i`. Write a program that
   prints `"foo"` if `i` is greater than 17, otherwise it prints
   `"bar"`. [fls[varstat/foo17.c]]

1. If you used `if`-`else` for the previous problem, do the same thing
   using the ternary operator. Otherwise, do the reverse.
   [fls[varstat/foo17b.c]]

1. Use a while loop to determine how many times you can do an integer
   divide by $7$ on the number $20754371$ until it reaches zero.
   [fls[varstat/whilediv7.c]]

1. Use a single `for` loop with two variables to print out:
   ```
   0 0
   1 1
   2 3
   3 6
   4 10
   5 15
   ```
   where the second number is its previous value plus the first number.
   Print up to 99 in the left column. The last value for `j` will be
   `4950`. [fls[varstat/twofor.c]]

## Functions

1. Write a function that has two integer parameters `x` and `y`, and
   computes

   $x\times20+x\times y$

   and returns the result. [fls[functions/funcxy.c]]

1. Write a function to compute $\pi$ through Gregory and Leibniz
   infinite series to a specified number of terms (where the first term
   is $1/1$ (AKA just $1$), below):

   $\displaystyle\pi=4(\frac{1}{1}-\frac{1}{3}+\frac{1}{5}-\frac{1}{7}+\frac{1}{9}-\cdots)$

   Print out the results of the series for terms 1 to 5000. The 5000th
   iteration should produce $3.141397$. [fls[functions/]]

## Pointers

1. Write a function that takes a pointer to an `int` and a pointer to a
   `float` as arguments. It should add the `int` to the `float` and
   subtract the `float` from the `int`. It should appear as if both
   these happened simultaneously, for example input of `5` and `3.2`
   would give `1` and `8.2`.
   
   Since you're manipulating the value through the pointers, the result
   should be visible in the caller.


<!--
1. Write a function that returns the $+$ answer from the quadratic
   formula for a given `a`, `b`, and `c`.

   $\displaystyle\frac{-b+\sqrt{4ac}}{2a}$

   You can get a square root by including `<math.h>` and calling
   `sqrt(4*a*c)`. If you're on a Unix-like system, you might have to add
   `-lm` to the end of your command line to link to the math library.
   [fls[functions/]]
-->
