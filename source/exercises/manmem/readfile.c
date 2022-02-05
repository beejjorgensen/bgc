#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 4096

char *readfile(FILE *fp, int *size)
{
    int data_size = BUFSIZE;
    int offset = 0;
    int b;
    char *data = malloc(data_size);

    if (data == NULL) {
        return NULL;
    }

    while ((b = fread(data + offset, 1, BUFSIZE, fp)) > 0) {
        offset += b;
        data_size += b;

        char *new_data = realloc(data, data_size);

        if (new_data == NULL) {
            free(data);
            return NULL;
        }

        data = new_data;
    }

    char *new_data = realloc(data, offset);

    if (new_data == NULL) {
        free(data);
        return NULL;
    }

    data = new_data;
    *size = offset;

    return data;
}


int main(void)
{
    FILE *fp;

    if ((fp = fopen("testfile.dat", "rb")) == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    int size;
    char *data = readfile(fp, &size);

    printf("Read %d bytes:\n", size);

    for (int i = 0; i < size; i++) {
        putchar(data[i]);
    }

    fclose(fp);
}