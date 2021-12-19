#include <stdio.h>
#if __APPLE__
#include <unistd.h> // for prototype for sleep()
#else
#include <threads.h>
#endif

void sleep_seconds(int s)
{
#ifdef __APPLE__
    sleep(s);
#else
    thrd_sleep(&(struct timespec){.tv_sec=s}, NULL);
#endif
}

int main(void)
{
    int count;

    for(count = 10; count >= 0; count--) {
        printf("\rSeconds until launch: ");  // lead with a CR
        if (count > 0)
            printf("%2d", count);
        else
            printf("blastoff!\n");

        // force output now!!
        fflush(stdout);

        sleep_seconds(1);
    }
}

