#include <stdio.h>

int main(void)
{
    int i;
    //float f;
    //char c;

    char *s = _Generic(i,
                    int: "that variable is an int",
                    float: "that variable is a float",
                    default: "that variable is some type"
                );

    printf("%s\n", s);
}
