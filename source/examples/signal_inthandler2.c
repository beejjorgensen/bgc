#include <stdio.h>
#include <signal.h>

void sigint_handler(int signum)
{
    (void)signum;                      // Unused variable warning
    signal(SIGINT, SIG_DFL);           // Reset signal handler
}

int main(void)
{
    signal(SIGINT, sigint_handler);

    printf("Hit ^C twice to exit.\n");

    while(1);
}
