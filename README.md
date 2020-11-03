# Beej's Guide to C

This is the source for Beej's Guide to C.

If you merely wish to read the guide, please visit the [Beej's Guide to
C](https://beej.us/guide/bgc/) website.

This is here so that Beej has everything in a repo and so translators
can easily clone it.

## Build Instructions

### Dependencies

* [Gnu make](https://www.gnu.org/software/make/) (XCode make works, too)
* [Python 3+](https://www.python.org/)
* [Pandoc 2.7.3+](https://pandoc.org/)
* XeLaTeX (can be found in [TeX Live](https://www.tug.org/texlive/))
* [Liberation fonts](https://en.wikipedia.org/wiki/Liberation_fonts) (sans, serif, mono)

Mac dependencies install (reopen terminal after doing this):

```
xcode-select --install                  # installs make
brew install python                     # installs Python3
brew install pandoc
brew cask install mactex                # installs XeLaTeX
brew tap homebrew/cask-fonts
brew cask install font-liberation-sans  # installs sans, serif, and mono
```

### Build

1. Type `make` from the top-level directory.

   If you have Gnu Make, it should work fine.  Other makes might work as
   well.  Windows users might want to check out Cygwin.

2. Type `make stage` to copy all the build products and website to the
   `stage` directory.

3. There is no step three.

You can also `cd` to the `src` directory and `make`.

`make clean` cleans, and `make pristine` cleans to "original" state.

To embed your own fonts in the PDFs, see the `src/Makefile` for examples.

The `upload` target in the root `Makefile` demonstrates the build steps
for a complete release.  You'll need to change the `UPLOADDIR` macro in
the top-level `Makefile` to point to your host if you want to use that.
You're free to upload whatever versions you desire individually, as
well.

## Pull Requests

Please keep these on the scale of typo and bug fixes. That way I don't
have to consider any copyright issues when merging changes.

## TODO

* Change audience to more advanced
* Exercises
* Outline
  * Hello, world
  * Variables and types
    * Just int and float types
  * Statements
    * if
    * while
    * for
    * do-while
    * switch  TODO
  * Functions
    * Function scope vs global scope
  * Pointers
    * Basics
    * Pass by reference
  * Arrays
    * Multi-dimensional arrays
    * Pointer-array equivalence heads-up
  * Strings
    * Declaring
    * Initializers
    * Arrays vs pointers
    * Length
    * Copying
  * Structs
    * Basic usage
    * Initializers
    * Pointers to structs, arrow operator
    * Passing structs to functions
  * Pointers II
    * void pointers
    * Pointer-array equivalence
    * Pointer arithmetic
  * Scope
    * Block scope
    * Static and file scope
  * Types II
    * Signed/Unsigned
      * char/unsigned char
    * long/double
    * Casting
    * Standard promotions
  * Types III
    * Storage classes
      * Static
      * Register
      * Auto
      * Extern
    * Type qualifiers
      * const
      * restrict
      * volatile
  * Structs II
    * Anonymous Structs
    * Typedef
    * Flexible array members
    * Bitfields
    * Padding bytes
    * Unions
  * Preprocessor
    * Macros
    * Include
    * Conditional Compilation
    * Advanced macros
    * Built-in macros
  * Enum
  * Typedef
  * Pointers III
    * const pointers
    * Pointers to pointers
    * Pointers to functions
    * Pointer integer types
    * NULL is 0
    * ptrdiff_t
    * unnamed objects
    * Iterating through bytes of an object
  * Arrays II
    * static const
    * Equivalent initializers 6.7.9p29
    * Array of struct initializers 6.7.9p35
  * Variadic functions
  * Bitwise operations
  * Preprocessor II
  * Goto
    * Labels
  * Incomplete types
  * lvalues
  * Incomplete types
  * The outside environment
    * argc, argv
    * exit values
  * More man pages
  * Optional features
    * Threads
    * Complex numbers
    * Atomic
    * VLAs



### Bug fixes

* When pandoc 2.8 comes up, switch all man page subheaders to h3 and supress
  them from the table of contents.

