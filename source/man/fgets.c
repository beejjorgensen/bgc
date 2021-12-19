#include <stdio.h>

int main(void)
{
    FILE *fp;
    char s[100];

    gets(s); // read from standard input (don't use this--use fgets()!)

    fgets(s, sizeof s, stdin); // read 100 bytes from standard input

    fp = fopen("spoon.txt", "r"); // (you should error-check this)
    fgets(s, 100, fp); // read 100 bytes from the file datafile.dat
    fclose(fp);

    fgets(s, 20, stdin); // read a maximum of 20 bytes from stdin
}
