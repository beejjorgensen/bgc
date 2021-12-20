#include <stdlib.h>

int main(void)
{
    int contrived_exit_type = 1;

    switch(contrived_exit_type) {
        case 1:
            exit(EXIT_SUCCESS);

#ifndef __APPLE__
        case 2:
            // Not supported in OS X
            quick_exit(EXIT_SUCCESS);
#endif

        case 3:
            _Exit(2);
    }
}

