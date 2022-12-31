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
   [flsol[intro/hello_name.c]]

1. Modify the "Hello, world!" example to print your name one one line,
   and your favorite fruit on the next. [flsol[intro/hello_fruit.c]]

## Variables and Statements {#ex-var-stat}

1. An approximation of $\pi$ is $208341 / 66317$. Write a program that
   computes this, stores the result in a `float` variable, and prints it
   out. Hint: put a `.0` after at least one of the numbers to force the
   computation to be `float` and not integer. [flsol[varstat/pi_approx.c]]

1. Choose a random integer in your head between 50 and 97. Call it $x$.
   Compute $(x+10\times x-5)/2$ as an integer and print it.
   [flsol[varstat/formula1.c]]

1. Do the same thing as the previous problem, except divide it by `2.0`
   instead of `2` and store the result in a `float`. And print it.
   [flsol[varstat/formula2.c]]

1. Let's say you have a variable `i = 2`. Write a program that assigns
   `j = i + 7` (to get `9`) _and_ then increments `i` on the same line
   of code. [flsol[varstat/postinc.c]]

1. Let's say you have a variable `i = 2`. Write a program that
   decrements `i` _and_ then assigns `j = i + 7` (to get `8`) on the
   same line of code. [flsol[varstat/predec.c]]

1. Choose a value for an integer variable `i`. Write a program that
   prints `"foo"` if `i` is greater than 17, otherwise it prints
   `"bar"`. [flsol[varstat/foo17.c]]

1. If you used `if`-`else` for the previous problem, do the same thing
   using the ternary operator. Otherwise, do the reverse.
   [flsol[varstat/foo17b.c]]

1. Use a while loop to determine how many times you can do an integer
   divide by $7$ on the number $20754371$ until it reaches zero.
   [flsol[varstat/whilediv7.c]]

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
   `4950`. [flsol[varstat/twofor.c]]

## Functions

1. Write a function that has two integer parameters `x` and `y`, and
   computes

   $x\times20+x\times y$

   and returns the result. [flsol[functions/funcxy.c]]

1. Write a function to compute $\pi$ through Gregory and Leibniz
   infinite series to a specified number of terms (where the first term
   is $1/1$ (AKA just $1$), below):

   $\displaystyle\pi=4(\frac{1}{1}-\frac{1}{3}+\frac{1}{5}-\frac{1}{7}+\frac{1}{9}-\cdots)$

   Print out the results of the series for terms 1 to 5000. The 5000th
   iteration should produce $3.141397$. [flsol[functions/pi.c]]

## Pointers

1. Write a function that takes a pointer to an `int` and a pointer to a
   `float` as arguments. It should add the `int` to the `float` and
   subtract the `float` from the `int`. It should appear as if both
   these happened simultaneously, for example input of `5` and `3.2`
   would give `1` and `8.2`.
   
   Since you're manipulating the value through the pointers, the result
   should be visible in the caller. [flsol[pointers/pointers.c]]

1. Write a function that takes two `int*` arguments and swaps them.

   ``` {.c}
   int a = 10, b = 20;

   swap(&a, &b);

   printf("%d, %d\n", a, b); // Should print 20, 10
   ```

   [flsol[pointers/swap.c]]

## Arrays

1. Declare and initialize an `int` array containing the following
   values:

   ``` {.default}
   10, 5, 2, 30, 97, 64
   ```

   Write a function `sum()` that accepts the array and its length as
   arguments. Have the function return the sum of the elements in the
   array (which is `208`). [flsol[arrays/sum.c]]

1. Declare an array of 2048 elements with an initializer that sets the
   element at index `312` to `3490`. All other elements should be
   initialized to zero. [flsol[arrays/init312.c]]

1. Write a function that checks a [flw[tie-tac-toe|Tic-tac-toe]] board
   for a winner. A `0` on the $3\times3$ board means no one has moved
   there yet. A `1` means "X" has moved there, and a `2` means "O" has.
   Return `0`, `1`, or `2` for no winners, "X" won, or "O" won,
   respectively. [flsol[arrays/tictactoe.c]]

## Strings

1. Write a function that reverses a string in-place. Have it return the
   same pointer that was passed in. [flsol[strings/reverse.c]]

1. Write a function that returns true if a string is entirely uppercase.
   Hint: see the `isupper()` function. [flsol[strings/str_isupper.c]]

1. Write a function that returns a pointer to the first occurrence of a
   letter in a string. If the character doesn't appear in the string,
   return `NULL`. [flsol[strings/my_strchr.c]]

## Structs

1. Write a `struct point` that has two `double` members called `x` and `y`,
   useful for storing a 2D point. Write a function that adds two `struct
   point`s together by adding their individual components. It should
   return a `struct point` as the result. [flsol[structs/point.c]]

1. Do the same thing, except pass pointers to the `struct point`s into
   the function. Have the function modify the first point so that it is
   the sum of the first and second points. [flsol[structs/point_ptr.c]]

## I/O

1. Write a program that reads in up to 50 lines of no more than 80
   characters each from `stdin`, then prints them out in reverse order.
   Hint: 2D array of characters might help. [flsol[io/reverselines.c]]

1. Write a program that reads in the 3D coordinates from
   [fls[`input.txt`|io/input.txt]] and computes the average 3D point,
   printing it out. The average point is the average of the Xs, then the
   average of the Ys, then Zs from the input. In the case of
   `input.txt`:

   ```
   Average point <46.296997,48.968998,49.845001>
   ```

   [flsol[io/point_avg.c]]

## Typedef

1. Write a `typedef` that creates a new type called `vector3d` for this
   `struct`:

   ``` {.c}
   struct {
      float x, y, z;
   };
   ```

   Write a function called `dot3d` that computes and returns the vector
   dot product of two `vector3d`s. This function should accept two
   pointers to `vector3d`s as arguments, so it could be called with:

   ``` {.c}
   float result = dot3d(&v1, &v2);
   ```

   The vector dot product of two vectors is the sum of the products of
   the `x`, `y`, and `z` components:

   $v_1=(x_1,y_1,z_1)$\
   $v_2=(x_2,y_2,z_2)$

   $v_1\cdot v_2 = x_1\times x_2 + y_1\times y_2 + z_1\times z_2$

   [flsol[typedef/vector3d.c]]

## Pointers II

1. Write `my_strchr()`, a function that returns a pointer to the first
   instance of a character in a string, like `strchr()`. Use pointer
   arithmetic to implement this.
   [flsol[pointers2/my_strchr.c]]

1. Write `my_strrchr()`, a function that returns a pointer to the last
   instance of a character in a string, like `strrchr()`. Use pointer
   arithmetic to implement this.
   [flsol[pointers2/my_strrchr.c]]

1. Write a function `object_sum()` that computes the sum of the bytes of
   the object pointed to by a `void*` parameter.

   ``` {.c}
   int object_sum(void *p, size_t count)
   ```

   ``` {.c}
   int t;
   float x = 3.14159;
   int y = 3490;

   t = object_sum(&x, sizeof x);
   t = object_sum(&y, sizeof y);
   ```

   [flsol[pointers2/object_sum.c]]

1. Write a comparison function that sorts an `int` array with `qsort()`
   in ascending order. Write another comparison function that sorts it
   in descending order.  [flsol[pointers2/int_qsort.c]]

## Manual Memory Allocation

1. Write a program that allocates space for an array of 10 `int`s, then
   fills that array for each index with the index number times 10.
   [flsol[manmem/tenints.c]]

1. Write a program that reads a file of arbitrary size into memory and
   returns a pointer to the data without knowing the file size in
   advance. The `fread()` function might help here.
   [flsol[manmem/readfile.c]]

## Scope

1. What will this code print?

   ``` {.c .numberLines}
   #include <stdio.h>

   int total = 0;

   void add(int x)
   {
       total += x;
       printf("Total is now %d\n", total);
   }

   int main(void)
   {
       for (int i = 0; i < 3; i++)
           add(i);
   }
   ```

   Answer^[`Total is now` with values `1`, `3`, `5`. `total` is at file
   scope so each call to `add()` affects it.].

1. What will this code print?

   ``` {.c .numberLines}
   #include <stdio.h>

   int main(void)
   {
       int i = 0;

       if (1 < 2) {  // always true
           int i = 0;
        
           i += 5;
       }

       i += 5;

       printf("%d\n", i);
   }
   ```

   Answer^[`5`--the first increase in `i` is on a variable that hides
   the one at outer scope.].

1. What will this code print?

   ``` {.c .numberLines}
   #include <stdio.h>

   int main(void)
   {
       for (int i = 0; i < 3; i++) {
           for (int i = 0; i < 2; i++) {
               printf("Hi there!\n");
           }
       }
   }
   ```

   Answer^[Prints `Hi there` six times. The inner declaration of `i`
   hides the outer one.].

## Types II

1. Write a program that initializes an `unsigned char` to `0`. Then, in
   a loop, increment that value a total of 260 times, printing it out
   each time.

   What happens as you pass 255^[Assuming your machine has 8-bit
   bytes.]?

   Answer^[Wraps around like an odometer from 255 to 0. This happens
   because 255 is binary `11111111`---all 8 bits are `1`, so adding one
   more gets us to `100000000`. But that's 9 bits, too big to fit in
   your byte. The highest bit is thrown out, leaving us with binary
   `00000000`. All unsigned types will do this when they are incremented
   from their highest value.].  [flsol[types2/overflow.c]]

## Types III

1. Write a function that prints out the whole number portion of a
   floating pointer number, followed by a `+` sign, followed by the
   fractional part of the number. For example, given the `float` value
   `3.14159`, it should print:

   ``` {.default}
   3+0.14159
   ```

   Try to do this without any string manipulation, just numerically.

   [flsol[types3/cutfrac.c]]

1. Write your own function that converts a string to an integer without
   using any of the built-in conversion functions (like `atoi()` or
   `strtol()`).

   ``` {.c}
   int my_atoi(char *s)
   {
        // TODO
   }

   printf("%d\n", my_atoi("3490"));  // prints 3490
   ````

   For bonus points:

   * Make it skip leading whitespace---see the `isspace()` function
   * Make it stop on the first non-numeric character---see the
     `isdigit()` function
   * Make it handle negative numbers

   [flsol[types3/my_atoi.c]]

## Types IV

1. This is going to be a two-file project. One file contains
   functionality to add numbers to a running total. The other calls that
   functionality.
   
   The main file `runner.c` will contain the following:

   ``` {.c .numberLines}
   // runner.c

   #include <stdio.h>

   int main(void)
   {
      for (int i = 0; i < 10; i++)
         add(i);

      printf("Total is %d over %d calls\n", total(), count);
   }
   ```

   The file `total.c` will have three things that are callable from
   other source files:

   1. A function called `void add(int x)`. This will add on to the
      running total that starts at `0`.
   2. A function called `int total(void)` that returns the total so far.
   3. A shared global variable called `count` that returns the number of
      times `add()` has been called.

   Implement the functionality in `total.c` and then modify `runner.c`
   so it can call the functions `add()` and `total()` and can see the
   variable `count`.

   Make sure that `total()` is the only way `runner.c` can get the
   total.
   
   You'll have to make use of `static` and `extern` to solve this one.

   Normally this is a case where we'd use a [header
   file](#includes-func-protos), but we haven't talked about those yet.

   You can compile two files together by specifying them both on the
   command line:

   ``` {.default}
   gcc -Wall -Wextra  -o runner runner.c total.c
   ```

   [flsol[types4/runner.c]] [flsol[types4/total.c]]

## Multifile Projects

1. Modify the exercise for Types IV, above, to use a header file
   `total.h`.

   Be sure to use the `#ifdef` wrapper on the header file.

   Bonus points for compiling the C files to object files before linking
   them all together.

   [flsol[multifile/runner.c]] [flsol[multifile/total.c]]
   [flsol[multifile/total.h]]

## The Outside Environment

1. Write a program that prints out the values of the environment
   variables that are specified on the command line.

   If the variable isn't set, it should print out the message `no such
   variable`.
   
   For example:

   ``` {.default}
   $ ./printenv HOME LANG FOOBAR PATH
   ```
   ``` {.default}
   HOME=/Users/beej
   LANG=en_US.UTF-8
   FOOBAR: no such variable
   PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
   ```

   (`HOME`, `LANG`, and `PATH` are variables commonly-set by the shell,
   so it's likely they're already there for you.)

   You'll want to use `getenv()` for this one.

   [flsol[env/printenv.c]]

## The C Preprocessor

1. Write a macro that takes two arguments, multiplies them together,
   then adds the current source code line number to the product. The
   result of the macro should be integer type.

   [flsol[cpp/macro2.c]]

1. Write code that prints out `C89`, `C99`, or `C11 or higher`
   corresponding to your current C version.

   [flsol[cpp/cversion.c]]

1. Have a program `#define` a numeric value for `X`. Use conditional
   compilation to either print `X is greater than 30` if that is true.
   Otherwise, print out the values from `0` up to `X-1`.

   [flsol[cpp/condcomp.c]]

<!--
1. Write a function that returns the $+$ answer from the quadratic
   formula for a given `a`, `b`, and `c`.

   $\displaystyle\frac{-b+\sqrt{4ac}}{2a}$

   You can get a square root by including `<math.h>` and calling
   `sqrt(4*a*c)`. If you're on a Unix-like system, you might have to add
   `-lm` to the end of your command line to link to the math library.
   [flsol[functions/]]
-->

