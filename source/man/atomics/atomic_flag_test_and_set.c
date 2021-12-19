#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

// Shared non-atomic struct
struct {
    int x, y, z;
} s = {1, 2, 3};

atomic_flag f = ATOMIC_FLAG_INIT;

int run(void *arg)
{
    int tid = *(int*)arg;

    printf("Thread %d: waiting for lock...\n", tid);

    while (atomic_flag_test_and_set(&f));

    printf("Thread %d: got lock, s is {%d, %d, %d}\n", tid,
                                                       s.x, s.y, s.z);
    s.x = (tid + 1) * 5 + 0;
    s.y = (tid + 1) * 5 + 1;
    s.z = (tid + 1) * 5 + 2;
    printf("Thread %d: set s to {%d, %d, %d}\n", tid, s.x, s.y, s.z);

    printf("Thread %d: releasing lock...\n", tid);
    atomic_flag_clear(&f);

    return 0;
}

int main(void)
{
    thrd_t t1, t2;
    int tid[] = {0, 1};

    thrd_create(&t1, run, tid+0);
    thrd_create(&t2, run, tid+1);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
}
