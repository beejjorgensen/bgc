#include <stdio.h>
#ifdef __APPLE__
#include <unistd.h>
#else
#include <threads.h>
#endif

void mysleep(int seconds)
{
#ifdef __APPLE__
    sleep(seconds);
#else
    thrd_sleep(&(struct timespec){.tv_sec=seconds}, NULL);
#endif
}

int main(void)
{
    for (int i = 10; i >= 0; i--) {
        printf("\rT minus %d second%s... \b", i, i != 1? "s": "");

        fflush(stdout);  // Force output to update

        // Sleep for 1 second
        mysleep(1);
    }

    printf("\rLiftoff!             \n");
}
