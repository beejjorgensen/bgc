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

### Commonly-used Escapes

In my humble opinion, these escape characters make up 99.2%^[I just made
up that number, but it's probably not far off] of all escapes.

|Code|Description|
|-|-|
|`\n`|Newline character---when printing, continue subsequent output on the next line|
|`\'`|Single quote---used for a single quote character constant|
|`\"`|Double quote---used for a double quote in a string literal|
|`\\`|Backslash---used for a literal `\` in a string or character|

Here are some examples of the escapes and what they output when printed.



### Rarely-used Escapes

|Code|Description|
|-|-|
|`\a`|Alarm. Make|
|`\b`||
|`\f`||
|`\r`||
|`\t`||
|`\v`||
|`\?`||

### Numeric Escapes

In addition, there are ways to specify numeric constants or other
character values inside strings or character constants.

abfnrtv
'"?\
\octal
\xhex
\uxxxx
\Uxxxxxxxx


## String and Character Types
