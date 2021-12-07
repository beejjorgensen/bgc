<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Characters and Strings II

We've talked about how `char` types are actually just small integer
types... but it's the same for a character in single quotes.

But a string in double quotes is type `const char *`.

Turns out there are few more types of strings and characters, and it
leads down one of the most infamous rabbit holes in the language: the
whole multibyte/wide/Unicode/localization thingy.

We're going to peer into that rabbit hole, but not go in. ...Yet!

## Escape Sequences

We're used to strings and characters with regular letters, punctuation,
and numbers:

``` {.c}
char *s = "Hello!";
char t = 'c';
```

But what if we want some special characters in there that we can't type
on the keyboard because they don't exist (e.g. "€"), or even if we want
a character that's a single quote? We clearly can't do this:

``` {.c}
char t = ''';
```

To do these things, we use something called _escape sequences_. These
are the backslash character (`\`) followed by another character. The two
(or more) characters together have special meaning.

For our single quote character example, we can put an escape (that is,
`\`) in front of the central single quote to solve it:

``` {.c}
char t = '\'';
```

Now C knows that `\'` means just a regular quote we want to print, not
the end of the character sequence.

You can say either "backslash" or "escape" in this context ("escape that
quote") and C devs will know what you're talking about. Also, "escape"
in this context is different than your `Esc` key or the ASCII `ESC`
code.

### Frequently-used Escapes

In my humble opinion, these escape characters make up 99.2%^[I just made
up that number, but it's probably not far off] of all escapes.

|Code|Description|
|--|------------|
|`\n`|Newline character---when printing, continue subsequent output on the next line|
|`\'`|Single quote---used for a single quote character constant|
|`\"`|Double quote---used for a double quote in a string literal|
|`\\`|Backslash---used for a literal `\` in a string or character|

Here are some examples of the escapes and what they output when printed.

``` {.c}
printf("Use \\n for newline\n");  // Use \n for newline
printf("Say \"hello\"!\n");       // Say "hello"!
printf("%c\n", '\'');             // '
```

### Rarely-used Escapes

But there are more escapes! You just don't see these as often.

|Code|Description|
|--|------------|
|`\a`|Alert. This makes the terminal make a sound or flash, or both!|
|`\b`|Backspace. Moves the cursor back a character. Doesn't delete the character.|
|`\f`|Formfeed. This moves to the next "page", but that doesn't have much modern meaning. On my system, this behaves like `\v`.|
|`\r`|Return. Move to the beginning of the same line.|
|`\t`|Horizontal tab. Moves to the next horizontal tab stop. On my machine, this lines up on columns that are multiples of 8, but YMMV.|
|`\v`|Vertical tab. Moves to the next vertical tab stop. On my machine, this moves to the same column on the next line.|
|`\?`|Literal question mark. Sometimes you need this to avoid trigraphs, as shown below.|

#### Single Line Status Updates

A use case for `\b` or `\r` is to show status updates that appear on the
same line on the screen and don't cause the display to scroll. Here's an
example that does a countdown from 10. (Note this makes use of the
non-standard POSIX function `sleep()` from `<unistd.h>`---if you're not
on a Unix-like, search for your platform and `sleep` for the
equivalent.)

``` {.c .numberLines}
#include <stdio.h>
#include <threads.h>

int main(void)
{
    for (int i = 10; i >= 0; i--) {
        printf("\rT minus %d second%s... \b", i, i != 1? "s": "");

        fflush(stdout);  // Force output to update

        // Sleep for 1 second
        thrd_sleep(&(struct timespec){.tv_sec=1}, NULL);
    }

    printf("\rLiftoff!             \n");
}
```

Quite a few things are happening on line 7. First of all, we lead with a
`\r` to get us to the beginning of the current line, then we overwrite
whatever's there with the current countdown. (There's ternary operator
out there to make sure we print `1 second` instead of `1 seconds`.)

Also, there's a space after the `...` That's so that we properly
overwrite the last `.` when `i` drops from `10` to `9` and we get a
column narrower. Try it without the space to see what I mean.

And we wrap it up with a `\b` to back up over that space so the cursor
sits at the exact end of the line in an aesthetically-pleasing way.

Note that line 14 also has a lot of spaces at the end to overwrite the
characters that were already there from the countdown.

Finally, we have a weird `fflush(stdout)` in there, whatever that means.
Short answer is that most terminals are _line buffered_ by default,
meaning they don't actually display anything until a newline character
is encountered. Since we don't have a newline (we just have `\r`),
without this line, the program would just sit there until `Liftoff!`
and then print everything all in one instant. `fflush()` overrides this
behavior and forces output to happen _right now_.

#### The Question Mark Escape

Why bother with this? After all, this works just fine:

``` {.c}
printf("Doesn't it?\n");
```

And it works fine with the escape, too:

``` {.c}
printf("Doesn't it\?\n");   // Note \?
```

So what's the point??!

Let's get more emphatic with another question mark and an exclamation
point:

``` {.c}
printf("Doesn't it??!\n");
```

When I compile this, I get this warning:

``` {.zsh}
foo.c: In function ‘main’:
foo.c:5:23: warning: trigraph ??! converted to | [-Wtrigraphs]
    5 |     printf("Doesn't it??!\n");
      |    
```

And running it gives this unlikely result:

``` {.default}
Doesn't it|
```

So _trigraphs_? What the heck is this??!

I'm sure we'll revisit this dusty corner of the language later, but the
short of it is the compiler looks for certain triplets of characters
starting with `??` and it substitutes other characters in their place.
So if you're on some ancient terminal without a pipe symbol (`|`) on the
keyboard, you can type `??!` instead.

You can fix this by escaping the second question mark, like so:

``` {.c}
printf("Doesn't it?\?!\n");
```

And then it compiles and works as-expected.

These days, of course, no one ever uses trigraphs. But that whole `??!`
does sometimes appear if you decide to use it in a string for emphasis.

### Numeric Escapes

In addition, there are ways to specify numeric constants or other
character values inside strings or character constants.

If you know an octal or hexadecimal representation of a byte, you can
include that in a string or character constant.

The following table has example numbers, but any hex or octal numbers
may be used. Pad with leading zeros if necessary to read the proper
digit count.

|Code|Description|
|--|------------|
|`\123`|Embed the byte with octal value `123`, 3 digits exactly.|
|`\x4D`|Embed the byte with hex value `4D`, 2 digits.|
|`\u2620`|Embed the Unicode character at code point with hex value `2620`, 4 digits.|
|`\U0001243F`|Embed the Unicode character at code point with hex value `1243F`, 8 digits.|

Here's an example of the less-commonly used octal notation to represent
the letter `B` in between `A` and `C`. Normally this would be used for
some kind of special unprintable character, but we have other ways to do
that, below, and this is just an octal demo:

``` {.c}
printf("A\102C\n");  // 102 is `B` in ASCII/UTF-8
```

Note there's no leading zero on the octal number when you include it
this way. But it does need to be three characters, so pad with leading
zeros if you need to.

But far more common is to use hex constants these days. Here's a demo
that you shouldn't use, but it demos embedding the UTF-8 bytes 0xE2,
0x80, and 0xA2 in a string, which corresponds to the Unicode "bullet"
character (•).

``` {.c}
printf("\xE2\x80\xA2 Bullet 1\n");
printf("\xE2\x80\xA2 Bullet 2\n");
printf("\xE2\x80\xA2 Bullet 3\n");
```

Produces the following output if you're on a UTF-8 console (or probably
garbage if you're not):

``` {.default}
• Bullet 1
• Bullet 2
• Bullet 3
```

But that's a crummy way to do Unicode. You can use the escapes `\u`
(16-bit) or `\U` (32-bit) to just refer to Unicode by code point number.
The bullet is `2022` (hex) in Unicode, so you can do this and get more
portable results:

``` {.c}
printf("\u2022 Bullet 1\n");
printf("\u2022 Bullet 2\n");
printf("\u2022 Bullet 3\n");
```

Be sure to pad `\u` with enough leading zeros to get to four characters,
and `\U` with enough to get to eight.

For example, that bullet could be done with `\U` and four leading zeros:

``` {.c}
printf("\U00002022 Bullet 1\n");
```

But who has time to be that verbose?
