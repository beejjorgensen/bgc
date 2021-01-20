<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Fixed Width Integer Types

intN_t
uintN_t
8, 16, 32, 64, should be present if no padding, two's complement

int_leastN_t
uint_leastN_t
8, 16, 32, 64 must be present

int_fastN_t
uint_fastN_t
8, 16, 32, 64 must be present

intmax_t
uintmax_t

INTn_C()
UINTn_C()

INTn_MAX
INTn_MIN
UINTn_MAX

INT_LEASTn_MAX
INT_LEASTn_MIN
UINT_LEASTn_MAX

INT_FASTn_MAX
INT_FASTn_MIN
UINT_FASTn_MAX

INTPTR_MIN
INTPTR_MAX
UINTPTR_MAX

INTMAX_MIN
INTMAX_MAX
UINTMAX_MAX


inttypes.h


PRI
SCN

imaxdiv_t