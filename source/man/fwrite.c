#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i;
    int n[10];
    FILE *fp;

    // populate the array with random numbers:
    for(i = 0; i < 10; i++) {
        n[i] = rand();
        printf("n[%d] = %d\n", i, n[i]);
    }

    // save the random numbers (10 ints) to the file
    fp = fopen("numbers.dat", "wb");
    fwrite(n, sizeof(int), 10, fp); // write 10 ints
    fclose(fp);
}

