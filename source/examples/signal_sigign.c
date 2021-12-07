#include <stdio.h>
#include <signal.h>

int main(void)
{
    char s[1024];

    signal(SIGINT, SIG_IGN);    // Ignore SIGINT, caused by ^C

    printf("Try hitting ^C... (hit RETURN to exit)\n");

    // Wait for a line of input so the program doesn't just exit
    fgets(s, sizeof s, stdin);
}
