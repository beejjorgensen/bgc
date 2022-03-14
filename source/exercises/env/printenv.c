#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 1) {
        printf("usage: printenv var [var ...]\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        char *v = getenv(argv[i]);

        if (v == NULL) {
            printf("%s: no such variable\n", argv[i]);
            continue;
        }

        printf("%s=%s\n", argv[i], v);
    }
}
