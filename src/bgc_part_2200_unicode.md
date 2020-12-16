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
library to get that done.

## What is Unicode?

Back in the day, it was popular in the US and much of the world to use a
7-bit or 8-bit encoding for characters in memory. ASCII was super
popular. This meant we could have 128 or 256 characters (including
non-printable characters) total. That was fine for a US-centric world,
but it turns out there are actually other alphabets out there---who
knew? Chinese has over 50,000 characters, and that's not fitting in a
byte.

So people came up with all kinds of alternate ways to represent their
own custom character sets. And that was fine, but turned into a
compatibility nightmare.

To escape it, Unicode was invented. One character set to rule them all.
It is backward-compatible with ASCII, so that's a win. But it extends
off into infinity (effectively) so we'll never run out of space for new
characters. It has Chinese, Latin, Greek, cuniform, chess symbols,
emojis... just about everything, really! And more is being added all the
time!

So if you're used to ASCII, think of Unicode as ASCII on steroids.

## Code Points vs. Encoding

I want to talk about two concepts here. If you come from an ASCII
background, these are the same. But in Unicode they're different. If you
don't know what those are, keep reading.

Let's loosely define _code point_ to mean a numeric value representing a
character. (Code points can also represent unprintable control
characters, but just assume I mean something like the letter "B" or the
character "π".)

In both ASCII and Unicode, for example, the numeric value 66 represents
"B". And in Unicode, 960 represents "π". (ASCII has no "π" character
representation.)

The point is, each code point represents a unique character. And each
character has a numeric value associated with it.

But how are these numbers represented in memory? This is what we call
the _encoding_ of the characters.

Now with ASCII, the code points and encoding are the same. ASCII only
supports code points 0-127 (because it's 7-bit), and each of those code
points is simply represented in memory as one code point per byte.


