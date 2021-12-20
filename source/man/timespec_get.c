#include <stdio.h>
#include <time.h>

int main(void)
{
    struct timespec ts;

    timespec_get(&ts, TIME_UTC);

    printf("%ld s, %ld ns\n", ts.tv_sec, ts.tv_nsec);

    double float_time = ts.tv_sec + ts.tv_nsec/1000000000.0;
    printf("%f seconds since epoch\n", float_time);
}

