Currently pass 1: BGC and man pages
Pass 2: bgclr_part_0150

# C23 Stuff

## Standard Library (new functions)

* add `memset_explicit()` function in `<string.h>` to erase sensitive
  data, where memory store must always be performed regardless of
  optimizations.

* add `memccpy()` function in `<string.h>` to efficiently concatenate
  strings - similar to POSIX and SVID C extensions.[7]

* add strdup() and strndup() functions in <string.h> to allocate a copy
  of a string – similar to POSIX and SVID C extensions.[8]

* add memalignment() function in <stdlib.h> to determine the byte
  alignment of a pointer.[9]

* add bit utility functions / macros / types in new header <stdbit.h> to
  examine many integer types. All start with stdc_ to minimize conflict
  with legacy code and 3rd party libraries.[10]
  * In the following, replace * with uc, us, ui, ul, ull for five
    function names, or blank for a type-generic macro.[10]
  * add stdc_count_ones*() and stdc_count_zeros*() to count number of 1
    or 0 bits in value.[10]
  * add stdc_leading_ones*() and stdc_leading_zeros*() to count leading
    1 or 0 bits in value.[10]
  * add stdc_trailing_ones*() and stdc_trailing_zeros*() to count
    trailing 1 or 0 bits in value.[10]
  * add stdc_first_leading_one*() and stdc_first_leading_zero*() to find
    first leading bit with 1 or 0 in value.[10]
  * add stdc_first_trailing_one*() and stdc_first_trailing_zero*() to
    find first trailing bit with 1 or 0 in value.[10]
  * add stdc_has_single_bit*() to determine if value an exact power of 2
    (return true if and only if there is a single 1 bit).[10]
  * add stdc_bit_floor*() to determine the largest integral power of 2
    that is not greater than value.[10]
  * add stdc_bit_ceil*() to determine the smallest integral power of 2
    that is not less than value.[10]
  * add stdc_bit_width*() to determine number of bits to represent a
    value.[10]

* add timegm() function in <time.h> to convert time structure into
  calendar time value - similar to function in glibc and musl
  libraries.[11]

## Standard Library (existing functions)

* add %b binary conversion specifier to printf() function family,
  prepending non-zero values with 0b, similar to how %x works.
  Implementations that previously didn't use %B as their own extension
  are encouraged to implement and prepend non-zero values with 0B,
  similar to how %X works.[12]

* add %b binary conversion specifier to scanf() function family.[12]

* add 0b and 0B binary conversion support to strtol() and wcstol()
  function families.[12]

* make the functions bsearch(), bsearch_s(), memchr(), strchr(),
  strpbrk(), strrchr(), strstr(), and their wide counterparts wmemchr(),
  wcschr(), wcspbrk(), wcsrchr(), wcsstr() return a const qualified
  object if one was passed to them.[13]

## Preprocessor

* add #elifdef and #elifndef directives[14] are essentially equivalent
  to #elif defined and #elif !defined. Both directives were added to
  C++23 standard and GCC 12 compiler too.[15]
* add `#embed` directive for binary resource inclusion.[16]

BOOKMARK

* add `#warning` directive for diagnostics.[17]
* add `__has_include` allowing the availability of a header to be checked
  by preprocessor directives.[18]
* add `__has_c_attribute` allowing the availability of an attribute to be
  checked by preprocessor directives.[19] (see "C++ compatibility" group
  for new attribute feature)
* add `__VA_OPT__` functional macro for variadic macros which expands to
  its argument only if a variadic argument has been passed to the
  containing macro.[20]

## Types

* add nullptr_t type.[21]
* add _BitInt(N) and unsigned _BitInt(N) types for bit-precise integers.
  Add BITINT_MAXWIDTH macro for maximum bit width.[22][23] Add
  ckd_add(), ckd_sub(), ckd_mul() macros for checked integer
  operations.[24]
* Variably-modified types (but not VLAs which are automatic variables
  allocated on the stack) become a mandatory feature.[25]
* Standardization of the typeof(...) operator.[26]
* the meaning of the auto keyword was changed to cause type inference
  while also retaining its old meaning of a storage class specifier if
  used alongside a type.[27]

## Constants

* add nullptr constant for nullptr_t type.[21]
* add wb and uwb integer literal suffixes for _BitInt(N) and unsigned
  _BitInt(N) types,[28] such as 6uwb yields an unsigned _BitInt(3), and
  -6wb yields a signed _BitInt(4) which has three value bits and one
  sign bit.
* add 0b and 0B binary literal constant prefixes,[29] such as 0b10101010
  (equating to 0xAA).
* add ' digit separator to literal constants,[30] such as 0xFE'DC'BA'98
  (equating to 0xFEDCBA98), 299'792'458 (equating to 299792458),
  1.414'213'562 (equating to 1.414213562).
* add the ability to specify the underlying type of an enum.[31]
* allow enums with no fixed underlying type to store values that aren't
  representable by int.[32]

## Keywords

* add true and false keywords.[33]
* add alignas, alignof, bool, static_assert, thread_local keywords.
  Previously defined keywords become alternative spellings: _Alignas,
  _Alignof, _Bool, _Static_assert, _Thread_local.[34]
* add _BitInt keyword (see "types" group)
* add typeof and typeof_unqual keywords (see "types" group)
* add nullptr keyword (see "constants" group)
* add constexpr keyword (see "other" group)
* add _Decimal32, _Decimal64, _Decimal128 keywords for (optional)
  decimal floating-point arithmetic (see "other" group)

## C++ compatibility

* add C++11 style attribute syntax[35] using double square brackets
  [[]]. Add attributes [36] [[deprecated]],[37] [[fallthrough]],[38]
  [[maybe_unused]],[39] [[nodiscard]],[40] and [[noreturn]] attribute
  for compatibility with C++11, then deprecate _Noreturn, noreturn,
  header <stdnoreturn.h> features introduced in C11.[41] Duplicate
  attributes are allowed for compatibility with C++23.[42] All standard
  attributes can also be surrounded by double underscores (e.g
  [[__deprecated__]] is equivalent to [[deprecated]]).
* add u8 prefix for character literals to represent UTF-8 encoding for
  compatibility with C++17.[43][44]
* add #elifdef and #elifndef preprocessing directives for compatibility
  with C++23.[14] (see "preprocessor" group)
* add single-argument _Static_assert for compatibility with C++17.[45]

## Other

* Support for the ISO/IEC 60559:2020, the current version of the IEEE
  754 standard for floating-point arithmetic, with extended binary
  floating-point arithmetic and (optional) decimal floating-point
  arithmetic.[46][47]
* Labels can appear before declarations and at the end of compound
  statements.[48]
* Unnamed parameters in function definitions.[49]
* Better support for using const with arrays.[50]
* Zero initialization with {} (including initialization of VLAs).[51]
* The constexpr specifier for objects but not functions, unlike C++'s
  equivalent.[52]
* variadic functions no longer need a named argument before the ellipsis
  and the va_start macro no longer needs a second argument nor does it
  evaluate any argument after the first one if present.[53]
* add the char8_t type for storing UTF-8 encoded data and change the
  type of u8 character constants and string literals to char8_t. Also,
  the functions mbrtoc8() and c8rtomb() to convert a narrow multibyte
  character to UTF-8 encoding and a single code point from UTF-8 to a
  narrow multibyte character representation respectively.[54]
* allow storage class specifiers to appear in compound literal
  definition.[55]

## Misc

* Update C version references
* strfrom functions in stdlib.h
* Verify other library functions
