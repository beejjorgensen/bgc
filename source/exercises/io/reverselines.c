#include <stdio.h>

int main(void)
{
    char lines[50][80];
    int line_count = 0;

    while (fgets(lines[line_count++], 80, stdin) != NULL)
        ;  // empty loop--increment to line_count happens above

    for (int i = line_count - 2; i >= 0; i--)
        printf("%s", lines[i]);
}
