#include <stdio.h>
#include <ctype.h>

int main(void)
{
    //             changing this char
    //                      v
    printf("%c\n", toupper('B'));  // B (unchanged)
    printf("%c\n", toupper('e'));  // E (made uppercase!)
    printf("%c\n", toupper('!'));  // ! (unchanged)
}

