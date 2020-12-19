<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Unicode and All That

It was hard to know what to title this chapter because this is a
multifaceted part of C rife with historical cruft and messiness^[Most
modern languages show incredible bias toward UTF-8, and so does C11 to
some extent.].

Let's get an outline first of what we're going to look at:

* Unicode background
* Character encoding background
* Source and Execution character Sets
* Using Unicode and UTF-8
* Using other character types like `wchar_t` and `char16_t`

Before we start, let me go to the punchline: the general consensus is
that you should use UTF-8 encoding everywhere internally. Then convert
it to something else at the last minute if that's what you need. If you
don't know what UTF-8 is, read on---we'll do a crash course.

Also, let me add that C's Unicode handling is, in my opinion, subpar. If
you're going to need to do any heavy lifting, I recommend a third-party
library to get that done. More on that later.

## What is Unicode?

Back in the day, it was popular in the US and much of the world to use a
7-bit or 8-bit encoding for characters in memory. This meant we could
have 128 or 256 characters (including non-printable characters) total.
That was fine for a US-centric world, but it turns out there are
actually other alphabets out there---who knew? Chinese has over 50,000
characters, and that's not fitting in a byte.

So people came up with all kinds of alternate ways to represent their
own custom character sets. And that was fine, but turned into a
compatibility nightmare.

To escape it, Unicode was invented. One character set to rule them all.
It extends off into infinity (effectively) so we'll never run out of
space for new characters. It has Chinese, Latin, Greek, cuniform, chess
symbols, emojis... just about everything, really! And more is being
added all the time!

## Code Points

I want to talk about two concepts here. It's confusing because they're
both numbers... different numbers for the same thing. But bear with me.

Let's loosely define _code point_ to mean a numeric value representing a
character. (Code points can also represent unprintable control
characters, but just assume I mean something like the letter "B" or the
character "π".)

Each code point represents a unique character. And each character has a
unique numeric code point associated with it.

For example, in Unicode, the numeric value 66 represents "B", and 960
represents "π". Other character mappings use different values,
potentially, but let's forget them and concentrate on Unicode, the
future!

So that's one thing: there's a number that represents each character. In
Unicode, these numbers run from 0 to over 1 million.

Got it?

Because we're about to flip the table a little.

## Encoding

If you recall, an 8-bit byte can hold values from 0-255, inclusive.
That's great for "B" which is 66---that fits in a byte. But "π" is 960,
and that doesn't fit in a byte! We need another byte. How do we
represent that in memory? Or what about bigger numbers, like 195,024?
That's going to need a number of bytes to hold.

How are these numbers represented in memory? This is what we call the
_encoding_ of the characters.

There are plenty of encodings. You can make up your own right now, if
you want. But we're going to look at some really common encodings that
are in use with Unicode.

|Encoding|Description|
|:-:|:-|
|UTF-8|A byte-oriented encoding that uses a variable number of bytes per character. This is the one to use.|
|UTF-16|A 16-bit per character^[Ish. Technically, it's variable width---there's a way to represent code points higher than $2^{16}$ by putting two UTF-16 characters together.] encoding.|
|UTF-32|A 32-bit per character encoding.|

With UTF-16 and UTF-32, the byte order matters, so you might see
UTF-16BE for big-endian and UTF-16LE for little-endian. Same for UTF-32.
Technically, if unspecified, you should assume big-endian. But since
Windows uses UTF-16 extensively and is little-endian, sometimes that is
assumed^[There's a special character called the _Byte Order Mark_ (BOM)
that can optionally precede the data stream and indicate the endianess.
It is not required, however.].

Let's look at some examples. I'm going to write the values in hex
because that's exactly two digits per 8-bit byte, and it makes it easier
to see how things are arranged in memory.

|Character|Code Point|UTF-16BE|UTF-32BE|UTF-16LE|UTF-32LE|UTF-8|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|`A`|41|0041|00000041|4100|41000000|41|
|`B`|42|0042|00000042|4200|42000000|42|
|`~`|7E|007E|0000007E|7E00|7E000000|7E|
|`π`|3C0|03C0|000003C0|C003|C0030000|CF80|
|`€`|20AC|20AC|000020AC|AC20|AC200000|E282AC|

Look in there for the patterns. Note that UTF-16BE and UTF-32BE are
simply the code point represented directly as 16- and 32-bit
values^[Again, this is only true in UTF-16 for characters that bit in
two bytes.].

Little-endian is the same, except the bytes are in little-endian order.

Then we have UTF-8 at the end. First you might notice that the
single-byte code points are represented as a single byte. That's nice.
You might also notice that different code points take different number
of bytes. This is a variable-width encoding.

So as soon as we get above a certain value, UTF-8 starts using
additional bytes to store the values. And they don't appear to correlate
with the code point value, either.

[flw[The details of UTF-8 encoding|UTF-8]] are beyond the scope of this
guide, but it's enough to know that it has a variable number of bytes
per code point, and those byte values don't match up with the code point
_except for the first 128 code points_.

That last bit is a neat thing about Unicode and UTF-8 from a North
American perspective: it's backward compatible with 7-bit ASCII
encoding! So if you're used to ASCII, UTF-8 is the same! Every
ASCII-encoded document is also UTF-8 encoded! (But not the other way
around, obviously.)

It's probably that last point more than any other that is driving UTF-8
to take over the world.

## Source and Execution Character Sets

When programming, there are two character sets that are in play: the one
that your source code is written in, and the one that's in use where the
program is running. They might be the same, but they might not.

