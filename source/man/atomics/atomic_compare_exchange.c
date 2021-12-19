#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#define LOOP_COUNT 10000

atomic_int value;

int run(void *arg)
{
    (void)arg;

    for(int i = 0; i < LOOP_COUNT; i++) {

        int cur = value;
        int next;

        do {
            next = cur + 2;
        } while (!atomic_compare_exchange_strong(&value, &cur, next));
    }

    return 0;
}

int main(void)
{
    thrd_t t1, t2;

    thrd_create(&t1, run, NULL);
    thrd_create(&t2, run, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);
    
    printf("%d should equal %d\n", value, LOOP_COUNT * 4);
}
