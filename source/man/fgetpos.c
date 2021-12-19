#include <stdio.h>

int main(void)
{
    char s[100];
    fpos_t pos;
    FILE *fp;

    fp = fopen("spoon.txt", "r");

    fgets(s, sizeof(s), fp); // read a line from the file
    printf("%s", s);

    fgetpos(fp, &pos);   // save the position after the read

    fgets(s, sizeof(s), fp); // read another line from the file
    printf("%s", s);

    fsetpos(fp, &pos);   // now restore the position to where we saved

    fgets(s, sizeof(s), fp); // read the earlier line again
    printf("%s", s);

    fclose(fp);
}

