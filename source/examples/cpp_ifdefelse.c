#include <stdio.h>

#define EXTRA_HAPPY

int main(void)
{
#ifdef EXTRA_HAPPY
    printf("I'm extra happy!\n");
#else
    printf("I'm just regular\n");
#endif

    printf("OK!\n");
}
