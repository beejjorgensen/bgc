#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int shared_1 = 1;
atomic_int shared_2 = 2;

int thread_1(void *arg)
{
    (void)arg;

    atomic_store_explicit(&shared_1, 10, memory_order_relaxed);

    atomic_thread_fence(memory_order_release);

    atomic_store_explicit(&shared_2, 20, memory_order_relaxed);

    return 0;
}

int thread_2(void *arg)
{
    (void)arg;

    // If this fence runs after the release fence, we're
    // guaranteed to see thread_1's changes to the shared
    // varaibles.

    atomic_thread_fence(memory_order_acquire);

    if (shared_2 == 20) {
        printf("Shared_1 better be 10 and it's %d\n", shared_1);
    } else {
        printf("Anything's possible: %d %d\n", shared_1, shared_2);
    }

    return 0;
}

int main(void)
{
    thrd_t t1, t2;

    thrd_create(&t2, thread_2, NULL);
    thrd_create(&t1, thread_1, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
}
