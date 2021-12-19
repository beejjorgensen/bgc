#include <stdio.h>
#include <signal.h>
#include <stdatomic.h>

int global;

void handler(int sig)
{
    (void)sig;

    // If this runs before the release, the handler will
    // potentially see global == 0.
    //
    // Otherwise, it will definitely see global == 10.

    atomic_signal_fence(memory_order_acquire);

    printf("%d\n", global);
}

int main(void)
{
    signal(SIGINT, handler);

    global = 10;

    atomic_signal_fence(memory_order_release);

    // If the signal handler runs after the release
    // it will definitely see the value 10 in global.
}
